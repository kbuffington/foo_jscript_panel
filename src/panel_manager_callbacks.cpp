#include "stdafx.h"
#include "callback_data.h"
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

	class jsp_config_object_notify : public config_object_notify
	{
	public:
		GUID get_watched_object(size_t index) override
		{
			return *watched_objects[index].first;
		}

		size_t get_watched_object_count() override
		{
			return watched_objects.size();
		}

		void on_watched_object_changed(const config_object::ptr& object) override
		{
			const GUID g = object->get_guid();
			const auto it = FIND_IF(watched_objects, [=](const auto& item) { return g == *item.first; });
			if (it != watched_objects.end())
			{
				bool b;
				object->get_data_bool(b);
				panel_manager::instance().post_msg_to_all(it->second, b);
			}
		}
	};

	class jsp_dsp_config_callback : public dsp_config_callback
	{
	public:
		void on_core_settings_change(const dsp_chain_config&) override
		{
			panel_manager::instance().post_msg_to_all(callback_id::on_dsp_preset_changed);
		}
	};

	class jsp_initquit : public initquit, public ui_selection_callback, public replaygain_core_settings_notify, public output_config_change_callback
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

		void on_selection_changed(metadb_handle_list_cref) override
		{
			panel_manager::instance().post_msg_to_all(callback_id::on_selection_changed);
		}

		void outputConfigChanged() override
		{
			panel_manager::instance().post_msg_to_all(callback_id::on_output_device_changed);
		}
	};

	class jsp_library_callback : public library_callback
	{
	public:
		void on_items_added(metadb_handle_list_cref handles) override
		{
			auto data = new metadb_callback_data(handles);
			panel_manager::instance().post_msg_to_all_pointer(callback_id::on_library_items_added, data);
		}

		void on_items_modified(metadb_handle_list_cref handles) override
		{
			auto data = new metadb_callback_data(handles);
			panel_manager::instance().post_msg_to_all_pointer(callback_id::on_library_items_changed, data);
		}

		void on_items_removed(metadb_handle_list_cref handles) override
		{
			auto data = new metadb_callback_data(handles);
			panel_manager::instance().post_msg_to_all_pointer(callback_id::on_library_items_removed, data);
		}
	};

	class jsp_metadb_io_callback : public metadb_io_callback
	{
	public:
		void on_changed_sorted(metadb_handle_list_cref handles, bool fromhook) override
		{
			auto data = new metadb_callback_data(handles);
			panel_manager::instance().post_msg_to_all_pointer(callback_id::on_metadb_changed, data);
		}
	};

	class jsp_play_callback_static : public play_callback_static
	{
	public:
		uint32_t get_flags() override
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

	class jsp_playback_queue_callback : public playback_queue_callback
	{
	public:
		void on_changed(t_change_origin origin) override
		{
			panel_manager::instance().post_msg_to_all(callback_id::on_playback_queue_changed, origin);
		}
	};

	class jsp_playback_statistics_collector : public playback_statistics_collector
	{
	public:
		void on_item_played(metadb_handle_ptr handle) override
		{
			auto data = new callback_data<metadb_handle_ptr>(handle);
			panel_manager::instance().post_msg_to_all_pointer(callback_id::on_item_played, data);
		}
	};

	class jsp_playlist_callback_static : public playlist_callback_static
	{
	public:
		void on_default_format_changed() override {}
		void on_items_modified(size_t, const pfc::bit_array&) override {}
		void on_items_modified_fromplayback(size_t, const pfc::bit_array&, playback_control::t_display_level) override {}
		void on_items_removing(size_t, const pfc::bit_array&, size_t, size_t) override {}
		void on_items_replaced(size_t, const pfc::bit_array&, const pfc::list_base_const_t<t_on_items_replaced_entry>&) override {}
		void on_playlists_removing(const pfc::bit_array&, size_t, size_t) override {}

		uint32_t get_flags() override
		{
			return flag_on_item_ensure_visible | flag_on_item_focus_change | flag_on_items_added | flag_on_items_removed | flag_on_items_reordered |
				flag_on_items_selection_change | flag_on_playback_order_changed | flag_on_playlist_activate | flag_on_playlist_created |
				flag_on_playlist_locked | flag_on_playlist_renamed | flag_on_playlists_removed | flag_on_playlists_reorder;
		}

		void on_item_ensure_visible(size_t playlist, size_t idx) override
		{
			panel_manager::instance().post_msg_to_all(callback_id::on_playlist_item_ensure_visible, playlist, idx);
		}

		void on_item_focus_change(size_t playlist, size_t from, size_t to) override
		{
			auto data = new callback_data<size_t, size_t, size_t>(playlist, from, to);
			panel_manager::instance().post_msg_to_all_pointer(callback_id::on_item_focus_change, data);
		}

		void on_items_added(size_t playlist, size_t start, metadb_handle_list_cref handles, const pfc::bit_array& selection) override
		{
			panel_manager::instance().post_msg_to_all(callback_id::on_playlist_items_added, playlist);
		}

		void on_items_removed(size_t playlist, const pfc::bit_array& mask, size_t old_count, size_t new_count) override
		{
			panel_manager::instance().post_msg_to_all(callback_id::on_playlist_items_removed, playlist, new_count);
		}

		void on_items_reordered(size_t playlist, const size_t* order, size_t count) override
		{
			panel_manager::instance().post_msg_to_all(callback_id::on_playlist_items_reordered, playlist);
		}

		void on_items_selection_change(size_t playlist, const pfc::bit_array& affected, const pfc::bit_array& state) override
		{
			panel_manager::instance().post_msg_to_all(callback_id::on_playlist_items_selection_change);
		}

		void on_playback_order_changed(size_t new_index) override
		{
			panel_manager::instance().post_msg_to_all(callback_id::on_playback_order_changed, new_index);
		}

		void on_playlist_activate(size_t old_index, size_t new_index) override
		{
			if (old_index != new_index)
			{
				panel_manager::instance().post_msg_to_all(callback_id::on_playlist_switch);
			}
		}

		void on_playlist_created(size_t index, const char* name, size_t name_len) override
		{
			panel_manager::instance().post_msg_to_all(callback_id::on_playlists_changed);
		}

		void on_playlist_locked(size_t playlist, bool locked) override
		{
			panel_manager::instance().post_msg_to_all(callback_id::on_playlists_changed);
		}

		void on_playlist_renamed(size_t index, const char* new_name, size_t new_name_len) override
		{
			panel_manager::instance().post_msg_to_all(callback_id::on_playlists_changed);
		}

		void on_playlists_removed(const pfc::bit_array& mask, size_t old_count, size_t new_count) override
		{
			panel_manager::instance().post_msg_to_all(callback_id::on_playlists_changed);
		}

		void on_playlists_reorder(const size_t* order, size_t count) override
		{
			panel_manager::instance().post_msg_to_all(callback_id::on_playlists_changed);
		}
	};

	FB2K_SERVICE_FACTORY(jsp_config_object_notify);
	FB2K_SERVICE_FACTORY(jsp_dsp_config_callback);
	FB2K_SERVICE_FACTORY(jsp_initquit);
	FB2K_SERVICE_FACTORY(jsp_library_callback);
	FB2K_SERVICE_FACTORY(jsp_metadb_io_callback);
	FB2K_SERVICE_FACTORY(jsp_play_callback_static);
	FB2K_SERVICE_FACTORY(jsp_playback_queue_callback);
	FB2K_SERVICE_FACTORY(jsp_playback_statistics_collector);
	FB2K_SERVICE_FACTORY(jsp_playlist_callback_static);
}
