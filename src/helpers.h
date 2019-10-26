#pragma once
#include "script_interface.h"
#include "thread_pool.h"

namespace helpers
{
	struct custom_sort_data
	{
		wchar_t* text;
		t_size index;
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

	using wrapped_item_list = std::vector<wrapped_item>;

	COLORREF convert_argb_to_colorref(DWORD argb);
	DWORD convert_colorref_to_argb(COLORREF color);
	IGdiBitmap* get_album_art(const metadb_handle_ptr& handle, t_size art_id, bool need_stub, bool no_load, pfc::string_base& image_path);
	IGdiBitmap* get_album_art_embedded(const pfc::string8_fast& rawpath, t_size art_id);
	IGdiBitmap* load_image(BSTR path);
	IGdiBitmap* read_album_art_into_bitmap(const album_art_data_ptr& data);
	bool execute_context_command_by_name(const char* p_command, metadb_handle_list_cref p_handles);
	bool execute_context_command_recur(const char* p_command, const pfc::string_base& p_path, contextmenu_node* p_parent);
	bool execute_mainmenu_command_by_name(const char* p_command);
	bool execute_mainmenu_command_recur(const char* p_command, pfc::string8_fast path, mainmenu_node::ptr node);
	bool is_wrap_char(wchar_t current, wchar_t next);
	bool read_file_wide(t_size codepage, const wchar_t* path, pfc::array_t<wchar_t>& content);
	bool supports_chakra();
	bool write_file(const char* path, const pfc::string8_fast& content);
	const GUID convert_artid_to_guid(t_size art_id);
	int get_encoder_clsid(const wchar_t* format, CLSID* pClsid);
	int get_text_height(HDC hdc, const wchar_t* text, int len);
	int get_text_width(HDC hdc, const wchar_t* text, int len);
	pfc::string8_fast get_fb2k_component_path();
	pfc::string8_fast get_fb2k_path();
	pfc::string8_fast get_profile_path();
	pfc::string8_fast read_file(const char* path);
	str_list split_string(const std::string& str, const std::string& delims);
	t_size detect_charset(const char* fileName);
	void estimate_line_wrap(HDC hdc, const wchar_t* text, int max_width, wrapped_item_list& out);
	void estimate_line_wrap_recur(HDC hdc, const wchar_t* text, int len, int max_width, wrapped_item_list& out);
	void list_files(const char* path, bool recur, pfc::string_list_impl& out);
	void list_folders(const char* path, pfc::string_list_impl& out);
	wchar_t* make_sort_string(const char* in);

	template <class T>
	bool ensure_gdiplus_object(T* obj)
	{
		return ((obj) && (obj->GetLastStatus() == Gdiplus::Ok));
	}

	template <int direction>
	static int custom_sort_compare(const custom_sort_data& elem1, const custom_sort_data& elem2)
	{
		int ret = direction * StrCmpLogicalW(elem1.text, elem2.text);
		if (ret == 0) ret = pfc::sgn_t((t_ssize)elem1.index - (t_ssize)elem2.index);
		return ret;
	}

	class embed_thread : public threaded_process_callback
	{
	public:
		enum class actions
		{
			attach,
			remove,
			remove_all
		};

		embed_thread(actions action, album_art_data_ptr data, metadb_handle_list_cref handles, t_size art_id) : m_action(action), m_data(data), m_handles(handles), m_art_id(art_id) {}

		void run(threaded_process_status& p_status, abort_callback& p_abort) override
		{
			const GUID what = convert_artid_to_guid(m_art_id);
			auto api = file_lock_manager::get();
			const t_size count = m_handles.get_count();
			for (t_size i = 0; i < count; ++i)
			{
				pfc::string8_fast path = m_handles.get_item(i)->get_path();
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

		static const t_size flags = threaded_process::flag_show_progress | threaded_process::flag_show_delayed | threaded_process::flag_show_item;

	private:
		actions m_action;
		album_art_data_ptr m_data;
		metadb_handle_list m_handles;
		t_size m_art_id;
	};

	class album_art_async : public simple_thread_task
	{
	public:
		album_art_async(HWND p_wnd, metadb_handle* handle, t_size art_id, bool need_stub, bool only_embed, bool no_load) : m_hwnd(p_wnd), m_handle(handle), m_art_id(art_id), m_need_stub(need_stub), m_only_embed(only_embed), m_no_load(no_load) {}

		struct t_param
		{
			t_param(IMetadbHandle* p_handle, t_size p_art_id, IGdiBitmap* p_bitmap, BSTR p_path) : handle(p_handle), art_id(p_art_id), bitmap(p_bitmap), path(p_path) {}

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
			t_size art_id;
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
			SendMessage(m_hwnd, callback_id::on_get_album_art_done, reinterpret_cast<WPARAM>(&param), 0);
		}

	private:
		HWND m_hwnd;
		bool m_need_stub;
		bool m_no_load;
		bool m_only_embed;
		metadb_handle_ptr m_handle;
		t_size m_art_id;
	};

	class load_image_async : public simple_thread_task
	{
	public:
		load_image_async(HWND p_wnd, BSTR path) : m_hwnd(p_wnd), m_path(path) {}

		struct t_param
		{
			t_param(t_size p_cookie, IGdiBitmap* p_bitmap, BSTR p_path) : cookie(p_cookie), bitmap(p_bitmap), path(p_path) {}

			~t_param()
			{
				if (bitmap)
				{
					bitmap->Release();
				}
			}

			IGdiBitmap* bitmap;
			_bstr_t path;
			t_size cookie;
		};

		void run() override
		{
			IGdiBitmap* bitmap = load_image(m_path);
			t_param param(reinterpret_cast<t_size>(this), bitmap, m_path);
			SendMessage(m_hwnd, callback_id::on_load_image_done, reinterpret_cast<WPARAM>(&param), 0);
		}

	private:
		HWND m_hwnd;
		_bstr_t m_path;
	};

	class js_process_locations : public process_locations_notify
	{
	public:
		js_process_locations(bool to_select, t_size base, t_size playlist) : m_to_select(to_select), m_base(base), m_playlist(playlist) {}

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
		bool m_to_select;
		t_size m_base;
		t_size m_playlist;
	};

	class js_file_info_filter : public file_info_filter {
	public:
		using tag = std::pair<std::string, str_list>;

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

		static str_list get_values(json j)
		{
			str_list values;
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

	class com_array
	{
	public:
		com_array() : m_psa(nullptr), m_reader(true), m_count(0) {}

		~com_array()
		{
			if (m_reader)
			{
				reset();
			}
		}

		LONG get_count()
		{
			return m_count;
		}

		SAFEARRAY* get_ptr()
		{
			return m_psa;
		}

		bool convert(const VARIANT& v)
		{
			if (v.vt != VT_DISPATCH || !v.pdispVal) return false;

			IDispatch* pdisp = v.pdispVal;
			DISPID id_length;
			LPOLESTR slength = (LPOLESTR)L"length";
			DISPPARAMS params = { 0 };
			_variant_t ret;

			HRESULT hr = pdisp->GetIDsOfNames(IID_NULL, &slength, 1, LOCALE_USER_DEFAULT, &id_length);
			if (SUCCEEDED(hr)) hr = pdisp->Invoke(id_length, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &params, &ret, nullptr, nullptr);
			if (SUCCEEDED(hr)) hr = VariantChangeType(&ret, &ret, 0, VT_I4);
			if (FAILED(hr))
			{
				return false;
			}

			m_count = ret.lVal;
			SAFEARRAY* psa = SafeArrayCreateVector(VT_VARIANT, 0, m_count);

			for (LONG i = 0; i < m_count; ++i)
			{
				DISPID dispid = 0;
				params = { 0 };
				wchar_t buf[33];
				LPOLESTR name = buf;
				_variant_t element;
				_itow_s(i, buf, 10);

				hr = pdisp->GetIDsOfNames(IID_NULL, &name, 1, LOCALE_USER_DEFAULT, &dispid);
				if (SUCCEEDED(hr)) hr = pdisp->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &params, &element, nullptr, nullptr);
				if (SUCCEEDED(hr)) hr = SafeArrayPutElement(psa, &i, &element);
				if (FAILED(hr))
				{
					SafeArrayDestroy(psa);
					return false;
				}
			}
			m_psa = psa;
			return true;
		}

		bool convert(const VARIANT& v, pfc::bit_array_bittable& out)
		{
			if (!convert(v)) return false;
			if (m_count == 0) out.resize(0);

			for (LONG i = 0; i < m_count; ++i)
			{
				_variant_t var;
				if (!get_item(i, var, VT_I4)) return false;
				out.set(var.lVal, true);
			}
			return true;
		}

		bool convert(const VARIANT& v, pfc::string_list_impl& out)
		{
			if (!convert(v)) return false;
			for (LONG i = 0; i < m_count; ++i)
			{
				_variant_t var;
				if (!get_item(i, var, VT_BSTR)) return false;
				out.add_item(string_utf8_from_wide(var.bstrVal));
			}
			return true;
		}

		bool convert(const VARIANT& v, std::vector<Gdiplus::PointF>& out)
		{
			if (!convert(v)) return false;
			if (m_count % 2 != 0) return false;

			out.resize(m_count >> 1);

			for (LONG i = 0; i < m_count; i += 2)
			{
				_variant_t varX, varY;
				if (!get_item(i, varX, VT_R4)) return false;
				if (!get_item(i + 1, varY, VT_R4)) return false;
				out[i >> 1] = { varX.fltVal, varY.fltVal };
			}
			return true;
		}

		bool create(LONG count)
		{
			m_count = count;
			m_psa = SafeArrayCreateVector(VT_VARIANT, 0, count);
			m_reader = false;
			return m_psa != nullptr;
		}

		bool get_item(LONG idx, VARIANT& var, VARTYPE vt)
		{
			if (SUCCEEDED(SafeArrayGetElement(m_psa, &idx, &var)))
			{
				return SUCCEEDED(VariantChangeType(&var, &var, 0, vt));
			}
			return false;
		}

		bool put_item(LONG idx, VARIANT& var)
		{
			if (SUCCEEDED(SafeArrayPutElement(m_psa, &idx, &var)))
			{
				return true;
			}
			reset();
			return false;
		}

		void reset()
		{
			m_count = 0;
			if (m_psa)
			{
				SafeArrayDestroy(m_psa);
				m_psa = nullptr;
			}
		}

	private:
		LONG m_count;
		SAFEARRAY* m_psa;
		bool m_reader;
	};
}
