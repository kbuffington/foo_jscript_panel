#pragma once
#include "script_interface.h"
#include "thread_pool.h"

namespace helpers
{
	struct custom_sort_data
	{
		wchar_t* text;
		size_t index;
	};

	struct stricmp_ascii
	{
		bool operator()(const pfc::string_simple& a, const pfc::string_simple& b) const
		{
			return pfc::stricmp_ascii(a, b) < 0;
		}
	};

	struct wrapped_item
	{
		BSTR text;
		int width;
	};

	using wrapped_item_vec = std::vector<wrapped_item>;
	static constexpr size_t threaded_process_flags = threaded_process::flag_show_progress | threaded_process::flag_show_delayed | threaded_process::flag_show_item;
	namespace fs = std::filesystem;

	BSTR read_file_wide(const wchar_t* path, size_t codepage);
	COLORREF convert_argb_to_colorref(DWORD argb);
	DWORD convert_colorref_to_argb(COLORREF colour);
	GUID convert_artid_to_guid(size_t art_id);
	IGdiBitmap* get_album_art(const metadb_handle_ptr& handle, size_t art_id, bool need_stub, bool no_load, pfc::string_base& image_path);
	IGdiBitmap* get_album_art_embedded(const pfc::string8_fast& path, size_t art_id);
	IGdiBitmap* load_image(BSTR path);
	IGdiBitmap* read_album_art_into_bitmap(const album_art_data_ptr& data);
	bool execute_context_command_by_name(const char* p_command, metadb_handle_list_cref p_handles);
	bool execute_context_command_recur(const char* p_command, const pfc::string_base& p_path, contextmenu_node* p_parent);
	bool execute_mainmenu_command_by_name(const char* p_command);
	bool execute_mainmenu_command_recur(const char* p_command, pfc::string8_fast path, mainmenu_node::ptr node);
	bool get_encoder_clsid(const wchar_t* format, CLSID* pClsid);
	bool is_wrap_char(wchar_t current, wchar_t next);
	bool supports_chakra();
	bool write_file(const char* path, const pfc::string8_fast& content);
	int get_text_height(HDC hdc, const wchar_t* text, int len);
	int get_text_width(HDC hdc, const wchar_t* text, int len);
	pfc::string8_fast get_fb2k_component_path();
	pfc::string8_fast get_fb2k_path();
	pfc::string8_fast get_profile_path();
	pfc::string8_fast get_resource_text(int id);
	pfc::string8_fast read_file(const char* path);
	size_t guess_codepage(const pfc::string8_fast& content);
	str_vec split_string(const std::string& str, const std::string& delims);
	void estimate_line_wrap(HDC hdc, const wchar_t* text, int max_width, wrapped_item_vec& out);
	void estimate_line_wrap_recur(HDC hdc, const wchar_t* text, int len, int max_width, wrapped_item_vec& out);
	void list_files(const char* path, bool recur, pfc::string_list_impl& out);
	void list_folders(const char* path, pfc::string_list_impl& out);
	wchar_t* make_sort_string(const char* in);

	template <class T>
	bool ensure_gdiplus_object(T* obj)
	{
		return obj && obj->GetLastStatus() == Gdiplus::Ok;
	}

	template <int direction>
	static int custom_sort_compare(const custom_sort_data& elem1, const custom_sort_data& elem2)
	{
		int ret = direction * StrCmpLogicalW(elem1.text, elem2.text);
		if (ret == 0) ret = pfc::sgn_t(static_cast<t_ssize>(elem1.index) - static_cast<t_ssize>(elem2.index));
		return ret;
	}

	class embed : public threaded_process_callback
	{
	public:
		enum class actions
		{
			attach,
			remove,
			remove_all
		};

		embed(actions action, album_art_data_ptr data, metadb_handle_list_cref handles, size_t art_id) : m_action(action), m_data(data), m_handles(handles), m_art_id(art_id) {}

		void run(threaded_process_status& p_status, abort_callback& p_abort) override
		{
			const GUID what = convert_artid_to_guid(m_art_id);
			auto api = file_lock_manager::get();
			const size_t count = m_handles.get_count();

			for (size_t i = 0; i < count; ++i)
			{
				pfc::string8_fast path = m_handles[i]->get_path();
				p_status.set_progress(i, count);
				p_status.set_item_path(path);
				album_art_editor::ptr ptr;
				if (album_art_editor::g_get_interface(ptr, path))
				{
					auto lock = api->acquire_write(path, p_abort);
					try
					{
						album_art_editor_instance_ptr aaep = ptr->open(nullptr, path, p_abort);
						switch (m_action)
						{
						case actions::attach:
							aaep->set(what, m_data, p_abort);
							break;
						case actions::remove:
							aaep->remove(what);
							break;
						case actions::remove_all:
							aaep->remove_all_();
							break;
						}
						aaep->commit(p_abort);
					}
					catch (...) {}
				}
			}
		}

	private:
		actions m_action;
		album_art_data_ptr m_data;
		metadb_handle_list m_handles;
		size_t m_art_id = 0;
	};

	class optimise_layout : public threaded_process_callback
	{
	public:
		optimise_layout(metadb_handle_list_cref handles, bool minimise) : m_handles(handles), m_minimise(minimise) {}

		void run(threaded_process_status& p_status, abort_callback& p_abort) override
		{
			auto api = file_lock_manager::get();
			const size_t count = m_handles.get_count();

			for (size_t i = 0; i < count; ++i)
			{
				auto path = m_handles[i]->get_path();
				p_status.set_progress(i, count);
				p_status.set_item_path(path);

				try
				{
					auto lock = api->acquire_write(path, p_abort);
					for (auto e = service_enum_t<file_format_sanitizer>(); !e.finished(); ++e)
					{
						if (e.get()->sanitize_file(path, m_minimise, p_abort)) break;
					}
				}
				catch (...) {}
			}
		}

	private:
		bool m_minimise;
		metadb_handle_list m_handles;
	};

	class album_art_async : public simple_thread_task
	{
	public:
		album_art_async(CWindow p_wnd, const metadb_handle_ptr& handle, size_t art_id, bool need_stub, bool only_embed, bool no_load) : m_hwnd(p_wnd), m_handle(handle), m_art_id(art_id), m_need_stub(need_stub), m_only_embed(only_embed), m_no_load(no_load) {}

		struct t_param
		{
			t_param(IMetadbHandle* p_handle, size_t p_art_id, IGdiBitmap* p_bitmap, BSTR p_path) : handle(p_handle), art_id(p_art_id), bitmap(p_bitmap), path(p_path) {}

			~t_param()
			{
				if (handle)
				{
					handle->Release();
				}

				if (bitmap)
				{
					bitmap->Release();
				}
			}

			IGdiBitmap* bitmap;
			IMetadbHandle* handle;
			_bstr_t path;
			size_t art_id;
		};

		void run() override
		{
			IGdiBitmap* bitmap = nullptr;
			IMetadbHandle* handle = nullptr;
			pfc::string8_fast image_path;

			if (m_handle.is_valid())
			{
				if (m_only_embed)
				{
					pfc::string8_fast rawpath = m_handle->get_path();
					bitmap = get_album_art_embedded(rawpath, m_art_id);
					if (bitmap)
					{
						image_path = file_path_display(rawpath);
					}
				}
				else
				{
					bitmap = get_album_art(m_handle, m_art_id, m_need_stub, m_no_load, image_path);
				}

				handle = new com_object_impl_t<MetadbHandle>(m_handle);
			}

			t_param param(handle, m_art_id, bitmap, TO_BSTR(image_path));
			m_hwnd.SendMessage(TO_UINT(callback_id::on_get_album_art_done), reinterpret_cast<WPARAM>(&param), 0);
		}

	private:
		CWindow m_hwnd = nullptr;
		bool m_need_stub = true;
		bool m_no_load = false;
		bool m_only_embed = false;
		metadb_handle_ptr m_handle;
		size_t m_art_id = 0;
	};

	class load_image_async : public simple_thread_task
	{
	public:
		load_image_async(CWindow p_wnd, BSTR path) : m_hwnd(p_wnd), m_path(path) {}

		struct t_param
		{
			t_param(size_t p_cookie, IGdiBitmap* p_bitmap, BSTR p_path) : cookie(p_cookie), bitmap(p_bitmap), path(p_path) {}

			~t_param()
			{
				if (bitmap)
				{
					bitmap->Release();
				}
			}

			IGdiBitmap* bitmap;
			_bstr_t path;
			size_t cookie;
		};

		void run() override
		{
			IGdiBitmap* bitmap = load_image(m_path);
			t_param param(reinterpret_cast<UINT_PTR>(this), bitmap, m_path);
			m_hwnd.SendMessage(TO_UINT(callback_id::on_load_image_done), reinterpret_cast<WPARAM>(&param), 0);
		}

	private:
		CWindow m_hwnd = nullptr;
		_bstr_t m_path;
	};

	class js_process_locations : public process_locations_notify
	{
	public:
		js_process_locations(size_t playlist, size_t base, bool to_select) : m_playlist(playlist), m_base(base), m_to_select(to_select) {}

		void on_aborted() override {}

		void on_completion(metadb_handle_list_cref p_items) override
		{
			pfc::bit_array_val selection(m_to_select);
			auto api = playlist_manager::get();

			if (m_playlist < api->get_playlist_count() && !api->playlist_lock_is_present(m_playlist))
			{
				api->playlist_insert_items(m_playlist, m_base, p_items, selection);
				if (m_to_select)
				{
					api->set_active_playlist(m_playlist);
					api->playlist_set_focus_item(m_playlist, m_base);
				}
			}
		}

	private:
		bool m_to_select = true;
		size_t m_base = 0;
		size_t m_playlist = 0;
	};

	class js_file_info_filter : public file_info_filter
	{
	public:
		using tag = std::pair<std::string, str_vec>;

		js_file_info_filter(const std::vector<tag>& tags) : m_tags(tags) {}

		bool apply_filter(metadb_handle_ptr p_location, t_filestats p_stats, file_info& p_info) override
		{
			for (const auto& [name, value] : m_tags)
			{
				p_info.meta_remove_field(name.c_str());
				for (const auto& part : value)
				{
					p_info.meta_add(name.c_str(), part.c_str());
				}
			}
			return true;
		}

		static str_vec get_values(json j)
		{
			str_vec values;
			if (j.is_array())
			{
				for (auto& v : j)
				{
					std::string tmp = v.is_string() ? v.get<std::string>() : v.dump();
					if (tmp.length()) values.emplace_back(tmp);
				}
			}
			else
			{
				std::string tmp = j.is_string() ? j.get<std::string>() : j.dump();
				if (tmp.length()) values.emplace_back(tmp);
			}
			return values;
		}

	private:
		std::vector<tag> m_tags;
	};
}
