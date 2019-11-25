#include "stdafx.h"
#include "panel_manager.h"
#include "thread_pool.h"

namespace
{
	static const std::vector<std::pair<const GUID*, callback_id>> watched_objects =
	{
		{ &standard_config_objects::bool_playlist_stop_after_current, callback_id::on_playlist_stop_after_current_changed },
		{ &standard_config_objects::bool_cursor_follows_playback, callback_id::on_cursor_follow_playback_changed },
		{ &standard_config_objects::bool_playback_follows_cursor, callback_id::on_playback_follow_cursor_changed },
		{ &standard_config_objects::bool_ui_always_on_top, callback_id::on_always_on_top_changed }
	};

	class my_config_object_notify : public config_object_notify
	{
	public:
		GUID get_watched_object(size_t p_index) override
		{
			return *watched_objects[p_index].first;
		}

		size_t get_watched_object_count() override
		{
			return watched_objects.size();
		}

		void on_watched_object_changed(const config_object::ptr& p_object) override
		{
			const GUID g = p_object->get_guid();
			const auto it = std::find_if(watched_objects.begin(), watched_objects.end(), [=](const auto& item)
				{
					return g == *item.first;
				});
			if (it != watched_objects.end())
			{
				bool b;
				p_object->get_data_bool(b);
				panel_manager::instance().post_msg_to_all(it->second, b);
			}
		}
	};

	class my_dsp_config_callback : public dsp_config_callback
	{
	public:
		void on_core_settings_change(const dsp_chain_config& p_newdata) override
		{
			panel_manager::instance().post_msg_to_all(callback_id::on_dsp_preset_changed);
		}
	};

	class my_initquit : public initquit, public ui_selection_callback, public replaygain_core_settings_notify, public output_config_change_callback
	{
	public:
		void on_changed(const t_replaygain_config& cfg) override
		{
			panel_manager::instance().post_msg_to_all(callback_id::on_replaygain_mode_changed, cfg.m_source_mode);
		}

		void on_init() override
		{
			replaygain_manager_v2::get()->add_notify(this);
			output_manager_v2::get()->addCallback(this);
			ui_selection_manager_v2::get()->register_callback(this, 0);
		}

		void on_quit() override
		{
			replaygain_manager_v2::get()->remove_notify(this);
			output_manager_v2::get()->removeCallback(this);
			ui_selection_manager_v2::get()->unregister_callback(this);
			panel_manager::instance().unload_all();
			simple_thread_pool::instance().exit();
		}

		void on_selection_changed(metadb_handle_list_cref p_selection) override
		{
			panel_manager::instance().post_msg_to_all(callback_id::on_selection_changed);
		}

		void outputConfigChanged() override
		{
			panel_manager::instance().post_msg_to_all(callback_id::on_output_device_changed);
		}
	};

	class my_library_callback : public library_callback
	{
	public:
		void on_items_added(metadb_handle_list_cref p_data) override
		{
			auto data = new metadb_callback_data(p_data);
			panel_manager::instance().post_msg_to_all_pointer(callback_id::on_library_items_added, data);
		}

		void on_items_modified(metadb_handle_list_cref p_data) override
		{
			auto data = new metadb_callback_data(p_data);
			panel_manager::instance().post_msg_to_all_pointer(callback_id::on_library_items_changed, data);
		}

		void on_items_removed(metadb_handle_list_cref p_data) override
		{
			auto data = new metadb_callback_data(p_data);
			panel_manager::instance().post_msg_to_all_pointer(callback_id::on_library_items_removed, data);
		}
	};

	class my_metadb_io_callback : public metadb_io_callback
	{
	public:
		void on_changed_sorted(metadb_handle_list_cref p_items_sorted, bool p_fromhook) override
		{
			auto data = new metadb_callback_data(p_items_sorted);
			panel_manager::instance().post_msg_to_all_pointer(callback_id::on_metadb_changed, data);
		}
	};

	class my_play_callback_static : public play_callback_static
	{
	public:
		size_t get_flags() override
		{
			return flag_on_playback_all | flag_on_volume_change;
		}

		void on_playback_dynamic_info(const file_info& info) override
		{
			panel_manager::instance().post_msg_to_all(callback_id::on_playback_dynamic_info);
		}

		void on_playback_dynamic_info_track(const file_info& info) override
		{
			panel_manager::instance().post_msg_to_all(callback_id::on_playback_dynamic_info_track);
		}

		void on_playback_edited(metadb_handle_ptr track) override
		{
			auto data = new callback_data<metadb_handle_ptr>(track);
			panel_manager::instance().post_msg_to_all_pointer(callback_id::on_playback_edited, data);
		}

		void on_playback_new_track(metadb_handle_ptr track) override
		{
			auto data = new callback_data<metadb_handle_ptr>(track);
			panel_manager::instance().post_msg_to_all_pointer(callback_id::on_playback_new_track, data);
		}

		void on_playback_pause(bool state) override
		{
			panel_manager::instance().post_msg_to_all(callback_id::on_playback_pause, state);
		}

		void on_playback_seek(double time) override
		{
			auto data = new callback_data<double>(time);
			panel_manager::instance().post_msg_to_all_pointer(callback_id::on_playback_seek, data);
		}

		void on_playback_starting(playback_control::t_track_command cmd, bool paused) override
		{
			panel_manager::instance().post_msg_to_all(callback_id::on_playback_starting, cmd, paused);
		}

		void on_playback_stop(playback_control::t_stop_reason reason) override
		{
			panel_manager::instance().post_msg_to_all(callback_id::on_playback_stop, reason);
		}

		void on_playback_time(double time) override
		{
			auto data = new callback_data<double>(time);
			panel_manager::instance().post_msg_to_all_pointer(callback_id::on_playback_time, data);
		}

		void on_volume_change(float newval) override
		{
			auto data = new callback_data<float>(newval);
			panel_manager::instance().post_msg_to_all_pointer(callback_id::on_volume_change, data);
		}
	};

	class my_playback_queue_callback : public playback_queue_callback
	{
	public:
		void on_changed(t_change_origin p_origin) override
		{
			panel_manager::instance().post_msg_to_all(callback_id::on_playback_queue_changed, p_origin);
		}
	};

	class my_playback_statistics_collector : public playback_statistics_collector
	{
	public:
		void on_item_played(metadb_handle_ptr p_item) override
		{
			auto data = new callback_data<metadb_handle_ptr>(p_item);
			panel_manager::instance().post_msg_to_all_pointer(callback_id::on_item_played, data);
		}
	};

	class my_playlist_callback_static : public playlist_callback_static
	{
	public:
		void on_default_format_changed() override {}
		void on_items_modified(size_t p_playlist, const pfc::bit_array& p_mask) override {}
		void on_items_modified_fromplayback(size_t p_playlist, const pfc::bit_array& p_mask, playback_control::t_display_level p_level) override {}
		void on_items_removing(size_t p_playlist, const pfc::bit_array& p_mask, size_t p_old_count, size_t p_new_count) override {}
		void on_items_replaced(size_t p_playlist, const pfc::bit_array& p_mask, const pfc::list_base_const_t<t_on_items_replaced_entry>& p_data) override {}
		void on_playlists_removing(const pfc::bit_array& p_mask, size_t p_old_count, size_t p_new_count) override {}

		size_t get_flags() override
		{
			return flag_on_item_ensure_visible | flag_on_item_focus_change | flag_on_items_added | flag_on_items_removed | flag_on_items_reordered |
				flag_on_items_selection_change | flag_on_playback_order_changed | flag_on_playlist_activate | flag_on_playlist_created |
				flag_on_playlist_locked | flag_on_playlists_removed | flag_on_playlist_renamed | flag_on_playlists_reorder;
		}

		void on_item_ensure_visible(size_t p_playlist, size_t p_idx) override
		{
			panel_manager::instance().post_msg_to_all(callback_id::on_playlist_item_ensure_visible, p_playlist, p_idx);
		}

		void on_item_focus_change(size_t p_playlist, size_t p_from, size_t p_to) override
		{
			auto data = new callback_data<size_t, size_t, size_t>(p_playlist, p_from, p_to);
			panel_manager::instance().post_msg_to_all_pointer(callback_id::on_item_focus_change, data);
		}

		void on_items_added(size_t p_playlist, size_t p_start, metadb_handle_list_cref p_data, const pfc::bit_array& p_selection) override
		{
			panel_manager::instance().post_msg_to_all(callback_id::on_playlist_items_added, p_playlist);
		}

		void on_items_removed(size_t p_playlist, const pfc::bit_array& p_mask, size_t p_old_count, size_t p_new_count) override
		{
			panel_manager::instance().post_msg_to_all(callback_id::on_playlist_items_removed, p_playlist, p_new_count);
		}

		void on_items_reordered(size_t p_playlist, const size_t* p_order, size_t p_count) override
		{
			panel_manager::instance().post_msg_to_all(callback_id::on_playlist_items_reordered, p_playlist);
		}

		void on_items_selection_change(size_t p_playlist, const pfc::bit_array& p_affected, const pfc::bit_array& p_state) override
		{
			panel_manager::instance().post_msg_to_all(callback_id::on_playlist_items_selection_change);
		}

		void on_playback_order_changed(size_t p_new_index) override
		{
			panel_manager::instance().post_msg_to_all(callback_id::on_playback_order_changed, p_new_index);
		}

		void on_playlist_activate(size_t p_old, size_t p_new) override
		{
			if (p_old != p_new)
			{
				panel_manager::instance().post_msg_to_all(callback_id::on_playlist_switch);
			}
		}

		void on_playlist_created(size_t p_index, const char* p_name, size_t p_name_len) override
		{
			panel_manager::instance().post_msg_to_all(callback_id::on_playlists_changed);
		}

		void on_playlist_locked(size_t p_playlist, bool p_locked) override
		{
			panel_manager::instance().post_msg_to_all(callback_id::on_playlists_changed);
		}

		void on_playlist_renamed(size_t p_index, const char* p_new_name, size_t p_new_name_len) override
		{
			panel_manager::instance().post_msg_to_all(callback_id::on_playlists_changed);
		}

		void on_playlists_removed(const pfc::bit_array& p_mask, size_t p_old_count, size_t p_new_count) override
		{
			panel_manager::instance().post_msg_to_all(callback_id::on_playlists_changed);
		}

		void on_playlists_reorder(const size_t* p_order, size_t p_count) override
		{
			panel_manager::instance().post_msg_to_all(callback_id::on_playlists_changed);
		}
	};

	FB2K_SERVICE_FACTORY(my_config_object_notify);
	FB2K_SERVICE_FACTORY(my_dsp_config_callback);
	FB2K_SERVICE_FACTORY(my_initquit);
	FB2K_SERVICE_FACTORY(my_library_callback);
	FB2K_SERVICE_FACTORY(my_metadb_io_callback);
	FB2K_SERVICE_FACTORY(my_play_callback_static);
	FB2K_SERVICE_FACTORY(my_playback_queue_callback);
	FB2K_SERVICE_FACTORY(my_playback_statistics_collector);
	FB2K_SERVICE_FACTORY(my_playlist_callback_static);
}
