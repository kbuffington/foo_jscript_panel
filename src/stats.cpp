#include "stdafx.h"
#include "stats.h"

namespace stats
{
	static constexpr std::array<const char*, 5> field_names =
	{
		"jsp_playcount",
		"jsp_loved",
		"jsp_first_played",
		"jsp_last_played",
		"jsp_rating"
	};

	metadb_index_manager::ptr g_cachedAPI;

	class jsp_metadb_index_client : public metadb_index_client
	{
	public:
		metadb_index_hash transform(const file_info& info, const playable_location& location) override
		{
			titleformat_object::ptr obj;
			titleformat_compiler::get()->compile_force(obj, "$lower(%artist% - %title%)");
			pfc::string8_fast str;
			obj->run_simple(location, &info, str);
			return hasher_md5::get()->process_single_string(str).xorHalve();
		}
	};
	static auto g_client = new service_impl_single_t<jsp_metadb_index_client>;

	class jsp_init_stage_callback : public init_stage_callback
	{
	public:
		void on_init_stage(size_t stage) override
		{
			if (stage == init_stages::before_config_read)
			{
				auto api = metadb_index_manager::get();
				g_cachedAPI = api;
				try
				{
					api->add(g_client, jsp::guids::metadb_index, system_time_periods::week * 4);
				}
				catch (const std::exception& e)
				{
					api->remove(jsp::guids::metadb_index);
					FB2K_console_formatter() << jsp::component_name << " stats: Critical initialisation failure: " << e;
					return;
				}
				api->dispatch_global_refresh();
			}
		}
	};

	class jsp_initquit : public initquit
	{
	public:
		void on_quit() override
		{
			g_cachedAPI.release();
		}
	};

	class jsp_metadb_display_field_provider : public metadb_display_field_provider
	{
	public:
		bool process_field(size_t index, metadb_handle* handle, titleformat_text_out* out) override
		{
			metadb_index_hash hash;
			if (!hashHandle(handle, hash)) return false;
			const fields tmp = get(hash);

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

		size_t get_field_count() override
		{
			return field_names.size();
		}

		void get_field_name(size_t index, pfc::string_base& out) override
		{
			out.set_string(field_names[index]);
		}
	};

	class jsp_track_property_provider_v4 : public track_property_provider_v4
	{
	public:
		bool is_our_tech_info(const char* name) override
		{
			return false;
		}

		void enumerate_properties_helper(metadb_handle_list_cref handles, track_property_provider_v3_info_source& info, track_property_callback_v2& callback, abort_callback& abort)
		{
			if (callback.is_group_wanted(jsp::component_name))
			{
				const size_t count = handles.get_count();
				if (count == 1)
				{
					metadb_index_hash hash;
					if (hashHandle(handles[0], hash))
					{
						const fields tmp = get(hash);
						callback.set_property(jsp::component_name, 0, "Playcount", std::to_string(tmp.playcount).c_str());
						callback.set_property(jsp::component_name, 1, "Loved", std::to_string(tmp.loved).c_str());
						callback.set_property(jsp::component_name, 2, "First Played", tmp.first_played);
						callback.set_property(jsp::component_name, 3, "Last Played", tmp.last_played);
						callback.set_property(jsp::component_name, 4, "Rating", std::to_string(tmp.rating).c_str());
					}
				}
				else
				{
					hash_set hashes;
					get_hashes(handles, hashes);

					size_t total = std::accumulate(hashes.begin(), hashes.end(), 0U, [](size_t t, const metadb_index_hash hash)
						{
							return t + get(hash).playcount;
						});

					if (total > 0)
					{
						callback.set_property(jsp::component_name, 0, "Playcount", std::to_string(total).c_str());
					}
				}
			}
		}

		void enumerate_properties_v4(metadb_handle_list_cref handles, track_property_provider_v3_info_source& info, track_property_callback_v2& callback, abort_callback& abort) override
		{
			enumerate_properties_helper(handles, info, callback, abort);
		}
	};

	FB2K_SERVICE_FACTORY(jsp_init_stage_callback);
	FB2K_SERVICE_FACTORY(jsp_initquit);
	FB2K_SERVICE_FACTORY(jsp_metadb_display_field_provider);
	FB2K_SERVICE_FACTORY(jsp_track_property_provider_v4);

	bool hashHandle(const metadb_handle_ptr& handle, metadb_index_hash& hash)
	{
		return g_client->hashHandle(handle, hash);
	}

	fields get(metadb_index_hash hash)
	{
		mem_block_container_impl temp;
		theAPI()->get_user_data(jsp::guids::metadb_index, hash, temp);
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

	void get_hashes(metadb_handle_list_cref handles, hash_set& hashes)
	{
		const auto count = handles.get_count();
		for (size_t i = 0; i < count; ++i)
		{
			metadb_index_hash hash;
			if (hashHandle(handles[i], hash))
			{
				hashes.emplace(hash);
			}
		}
	}

	void set(metadb_index_hash hash, fields f)
	{
		stream_writer_formatter_simple<false> writer;
		writer << f.playcount;
		writer << f.loved;
		writer << f.first_played;
		writer << f.last_played;
		writer << f.rating;
		theAPI()->set_user_data(jsp::guids::metadb_index, hash, writer.m_buffer.get_ptr(), writer.m_buffer.get_size());
	}
}
