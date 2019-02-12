#include "stdafx.h"
#include "stats.h"

namespace stats
{
	metadb_index_manager::ptr g_cachedAPI;

	class my_metadb_index_client : public metadb_index_client
	{
	public:
		metadb_index_hash transform(const file_info& info, const playable_location& location) override
		{
			titleformat_object::ptr obj;
			titleformat_compiler::get()->compile_force(obj, "$lower(%artist% - %title%)");
			pfc::string_formatter str;
			obj->run_simple(location, &info, str);
			return hasher_md5::get()->process_single_string(str).xorHalve();
		}
	};
	static my_metadb_index_client* g_client = new service_impl_single_t<my_metadb_index_client>;

	class my_init_stage_callback : public init_stage_callback
	{
	public:
		void on_init_stage(t_size stage) override
		{
			if (stage == init_stages::before_config_read)
			{
				auto api = metadb_index_manager::get();
				g_cachedAPI = api;
				try
				{
					api->add(g_client, g_guid_jsp_metadb_index, system_time_periods::week * 4);
				}
				catch (std::exception const& e)
				{
					api->remove(g_guid_jsp_metadb_index);
					FB2K_console_formatter() << JSP_NAME " stats: Critical initialisation failure: " << e;
					return;
				}
				api->dispatch_global_refresh();
			}
		}
	};

	class my_initquit : public initquit
	{
	public:
		void on_quit() override
		{
			g_cachedAPI.release();
		}
	};

	class my_metadb_display_field_provider : public metadb_display_field_provider
	{
	public:
		bool process_field(t_size index, metadb_handle* handle, titleformat_text_out* out) override
		{
			metadb_index_hash hash;
			if (!hashHandle(handle, hash)) return false;
			fields tmp = get(hash);

			switch (index)
			{
			case 0:
				if (tmp.playcount == 0) return false;
				out->write_int(titleformat_inputtypes::meta, tmp.playcount);
				return true;
			case 1:
				if (tmp.loved == 0) return false;
				out->write_int(titleformat_inputtypes::meta, tmp.loved);
				return true;
			case 2:
				if (tmp.first_played.is_empty()) return false;
				out->write(titleformat_inputtypes::meta, tmp.first_played);
				return true;
			case 3:
				if (tmp.last_played.is_empty()) return false;
				out->write(titleformat_inputtypes::meta, tmp.last_played);
				return true;
			case 4:
				if (tmp.rating == 0) return false;
				out->write_int(titleformat_inputtypes::meta, tmp.rating);
				return true;
			}
			return false;
		}

		t_size get_field_count() override
		{
			return 5;
		}

		void get_field_name(t_size index, pfc::string_base& out) override
		{
			switch (index)
			{
			case 0:
				out = JSP_STATS_PREFIX "playcount";
				break;
			case 1:
				out = JSP_STATS_PREFIX "loved";
				break;
			case 2:
				out = JSP_STATS_PREFIX "first_played";
				break;
			case 3:
				out = JSP_STATS_PREFIX "last_played";
				break;
			case 4:
				out = JSP_STATS_PREFIX "rating";
				break;
			}
		}
	};

	class my_track_property_provider_v2 : public track_property_provider_v2
	{
	public:
		bool is_our_tech_info(const char* p_name) override
		{
			return false;
		}

		void enumerate_properties(metadb_handle_list_cref p_tracks, track_property_callback& p_out)
		{
			const t_size count = p_tracks.get_count();
			if (count == 1)
			{
				metadb_index_hash hash;
				if (hashHandle(p_tracks[0], hash))
				{
					fields tmp = get(hash);
					p_out.set_property(JSP_NAME, 0, "Playcount", pfc::format_uint(tmp.playcount));
					p_out.set_property(JSP_NAME, 1, "Loved", pfc::format_uint(tmp.loved));
					p_out.set_property(JSP_NAME, 2, "First Played", tmp.first_played);
					p_out.set_property(JSP_NAME, 3, "Last Played", tmp.last_played);
					p_out.set_property(JSP_NAME, 4, "Rating", pfc::format_uint(tmp.rating));
				}
			}
			else
			{
				pfc::avltree_t<metadb_index_hash> hashes;

				for (t_size i = 0; i < count; ++i)
				{
					metadb_index_hash hash;
					if (hashHandle(p_tracks[i], hash))
					{
						hashes += hash;
					}
				}

				t_size total = 0;
				for (auto it = hashes.first(); it.is_valid(); ++it)
				{
					total += get(*it).playcount;
				}

				if (total > 0)
				{
					p_out.set_property(JSP_NAME, 0, "Playcount", pfc::format_uint(total));
				}
			}
		}

		void enumerate_properties_v2(metadb_handle_list_cref p_tracks, track_property_callback_v2& p_out) override
		{
			if (p_out.is_group_wanted(JSP_NAME))
			{
				enumerate_properties(p_tracks, p_out);
			}
		}
	};

	static service_factory_single_t<my_init_stage_callback> g_my_init_stage_callback;
	static service_factory_single_t<my_initquit> g_my_initquit;
	static service_factory_single_t<my_metadb_display_field_provider> g_my_metadb_display_field_provider;
	static service_factory_single_t<my_track_property_provider_v2> g_my_track_property_provider_v2;

	bool hashHandle(const metadb_handle_ptr& handle, metadb_index_hash& hash)
	{
		return g_client->hashHandle(handle, hash);
	}

	fields get(metadb_index_hash hash)
	{
		mem_block_container_impl temp;
		theAPI()->get_user_data(g_guid_jsp_metadb_index, hash, temp);
		if (temp.get_size() > 0)
		{
			try
			{
				stream_reader_formatter_simple_ref<false> reader(temp.get_ptr(), temp.get_size());
				fields ret;
				reader >> ret.playcount;
				reader >> ret.loved;
				reader >> ret.first_played;
				reader >> ret.last_played;
				reader >> ret.rating;
				return ret;
			}
			catch (exception_io_data) {}
		}
		return fields();
	}

	metadb_index_manager::ptr theAPI()
	{
		auto ret = g_cachedAPI;
		if (ret.is_empty()) ret = metadb_index_manager::get();
		return ret;
	}

	void set(metadb_index_hash hash, fields f)
	{
		stream_writer_formatter_simple<false> writer;
		writer << f.playcount;
		writer << f.loved;
		writer << f.first_played;
		writer << f.last_played;
		writer << f.rating;
		theAPI()->set_user_data(g_guid_jsp_metadb_index, hash, writer.m_buffer.get_ptr(), writer.m_buffer.get_size());
	}
}
