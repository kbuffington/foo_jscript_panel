#pragma once
#include "script_interface.h"
#include "thread_pool.h"

#include <json.hpp>

using json = nlohmann::json;

namespace helpers
{
	struct custom_sort_data
	{
		wchar_t* text;
		t_size index;
	};

	struct wrapped_item
	{
		BSTR text;
		int width;
	};

	COLORREF convert_argb_to_colorref(DWORD argb);
	DWORD convert_colorref_to_argb(COLORREF color);
	IGdiBitmap* get_album_art(const metadb_handle_ptr& handle, t_size art_id, bool need_stub, pfc::string_base& image_path, bool no_load = false);
	IGdiBitmap* get_album_art_embedded(const pfc::string8_fast& rawpath, t_size art_id);
	IGdiBitmap* load_image(BSTR path);
	bool execute_context_command_by_name(const char* p_command, metadb_handle_list_cref p_handles, t_size flags);
	bool execute_context_command_recur(const char* p_command, pfc::string_base& p_path, contextmenu_node* p_parent);
	bool execute_mainmenu_command_by_name(const char* p_command);
	bool execute_mainmenu_command_recur(const char* p_command, pfc::string8_fast path, mainmenu_node::ptr node);
	bool read_album_art_into_bitmap(const album_art_data_ptr& data, Gdiplus::Bitmap** bitmap);
	bool read_file_wide(t_size codepage, const wchar_t* path, pfc::array_t<wchar_t>& content);
	bool supports_chakra();
	bool write_file(const char* path, const pfc::string_base& content, bool write_bom = false);
	const GUID convert_artid_to_guid(t_size art_id);
	int get_encoder_clsid(const wchar_t* format, CLSID* pClsid);
	int get_text_height(HDC hdc, const wchar_t* text, int len);
	int get_text_width(HDC hdc, const wchar_t* text, int len);
	int is_wrap_char(wchar_t current, wchar_t next);
	pfc::string8_fast get_fb2k_component_path();
	pfc::string8_fast get_fb2k_path();
	pfc::string8_fast get_profile_path();
	pfc::string8_fast iterator_to_string(json::iterator j);
	pfc::string8_fast read_file(const char* path);
	t_size detect_charset(const char* fileName);
	t_size get_colour_from_variant(VARIANT v);
	void estimate_line_wrap(HDC hdc, const wchar_t* text, int len, int width, pfc::list_t<wrapped_item>& out);
	void estimate_line_wrap_recur(HDC hdc, const wchar_t* text, int len, int width, pfc::list_t<wrapped_item>& out);
	void list(const char* path, bool files, bool recur, pfc::string_list_impl& out);
	wchar_t* make_sort_string(const char* in);

	template <class T>
	bool ensure_gdiplus_object(T* obj)
	{
		return ((obj) && (obj->GetLastStatus() == Gdiplus::Ok));
	}

	template<int direction>
	static int custom_sort_compare(const custom_sort_data& elem1, const custom_sort_data& elem2)
	{
		int ret = direction * StrCmpLogicalW(elem1.text, elem2.text);
		if (ret == 0) ret = pfc::sgn_t((t_ssize)elem1.index - (t_ssize)elem2.index);
		return ret;
	}

	class embed_thread : public threaded_process_callback
	{
	public:
		embed_thread(t_size action, album_art_data_ptr data, metadb_handle_list_cref handles, t_size art_id) : m_action(action), m_data(data), m_handles(handles), m_art_id(art_id) {}
		void run(threaded_process_status& p_status, abort_callback& p_abort)
		{
			GUID what = convert_artid_to_guid(m_art_id);
			auto api = file_lock_manager::get();
			t_size count = m_handles.get_count();
			for (t_size i = 0; i < count; ++i)
			{
				pfc::string8_fast path = m_handles.get_item(i)->get_path();
				p_status.set_progress(i, count);
				p_status.set_item_path(path);
				album_art_editor::ptr ptr;
				if (album_art_editor::g_get_interface(ptr, path))
				{
					file_lock_ptr lock = api->acquire_write(path, p_abort);
					album_art_editor_instance_ptr aaep;
					try
					{
						aaep = ptr->open(NULL, path, p_abort);
						switch (m_action)
						{
						case attach:
							aaep->set(what, m_data, p_abort);
							break;
						case remove:
							aaep->remove(what);
							break;
						case remove_all:
							{
								album_art_editor_instance_v2::ptr v2;
								if (aaep->cast(v2))
								{
									// not all file formats support this
									v2->remove_all();
								}
								else
								{
									// m4a is one example that needs this fallback
									aaep->remove(album_art_ids::artist);
									aaep->remove(album_art_ids::cover_back);
									aaep->remove(album_art_ids::cover_front);
									aaep->remove(album_art_ids::disc);
									aaep->remove(album_art_ids::icon);
								}
							}
							break;
						}
						aaep->commit(p_abort);
					}
					catch (...) {}
					lock.release();
				}
			}
		}

		enum
		{
			attach,
			remove,
			remove_all
		};

	private:
		album_art_data_ptr m_data;
		metadb_handle_list m_handles;
		t_size m_action;
		t_size m_art_id;
	};

	class album_art_async : public simple_thread_task
	{
	public:
		album_art_async(HWND notify_hwnd, metadb_handle* handle, t_size art_id, bool need_stub, bool only_embed, bool no_load) : m_notify_hwnd(notify_hwnd), m_handle(handle), m_art_id(art_id), m_need_stub(need_stub), m_only_embed(only_embed), m_no_load(no_load) {}

		struct t_param
		{
			t_param(IMetadbHandle* p_handle, t_size p_art_id, IGdiBitmap* p_bitmap, const char* p_image_path) : handle(p_handle), art_id(p_art_id), bitmap(p_bitmap), image_path(p_image_path) {}

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
			string_wide_from_utf8_fast image_path;
			t_size art_id;
		};

	private:
		virtual void run()
		{
			IGdiBitmap* bitmap = NULL;
			MetadbHandle* handle = NULL;
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
					bitmap = get_album_art(m_handle, m_art_id, m_need_stub, image_path, m_no_load);
				}

				handle = new com_object_impl_t<MetadbHandle>(m_handle);
			}

			t_param param(handle, m_art_id, bitmap, image_path);
			SendMessage(m_notify_hwnd, CALLBACK_UWM_ON_GET_ALBUM_ART_DONE, (WPARAM)&param, 0);
		}

		HWND m_notify_hwnd;
		bool m_need_stub;
		bool m_no_load;
		bool m_only_embed;
		metadb_handle_ptr m_handle;
		t_size m_art_id;
	};

	class load_image_async : public simple_thread_task
	{
	public:
		load_image_async(HWND notify_wnd, BSTR path) : m_notify_hwnd(notify_wnd), m_path(path) {}

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

	private:
		virtual void run()
		{
			IGdiBitmap* bitmap = load_image(m_path);
			t_param param(reinterpret_cast<t_size>(this), bitmap, m_path);
			SendMessage(m_notify_hwnd, CALLBACK_UWM_ON_LOAD_IMAGE_DONE, (WPARAM)&param, 0);
		}

		HWND m_notify_hwnd;
		_bstr_t m_path;
	};

	class popup_msg : public main_thread_callback
	{
	public:
		popup_msg(pfc::string8_fast msg, pfc::string8_fast title) : m_msg(msg), m_title(title) {}

	private:
		virtual void callback_run()
		{
			popup_message::g_show(m_msg, m_title);
		}

		pfc::string8_fast m_msg, m_title;
	};

	class js_process_locations : public process_locations_notify
	{
	public:
		js_process_locations(bool to_select, t_size base, t_size playlist) : m_to_select(to_select), m_base(base), m_playlist(playlist) {}

		void on_aborted() {}

		void on_completion(metadb_handle_list_cref p_items)
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

	class com_array
	{
	public:
		com_array() : m_psa(NULL), m_reader(true), m_count(0) {}

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

		bool convert(VARIANT* v)
		{
			if (v->vt != VT_DISPATCH || !v->pdispVal) return false;

			IDispatch* pdisp = v->pdispVal;
			DISPID id_length;
			LPOLESTR slength = (LPOLESTR)L"length";
			DISPPARAMS params = { 0 };
			_variant_t ret;

			HRESULT hr = pdisp->GetIDsOfNames(IID_NULL, &slength, 1, LOCALE_USER_DEFAULT, &id_length);
			if (SUCCEEDED(hr)) hr = pdisp->Invoke(id_length, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &params, &ret, NULL, NULL);
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
				if (SUCCEEDED(hr)) hr = pdisp->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &params, &element, NULL, NULL);
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

		bool convert_to_bit_array(VARIANT v, pfc::bit_array_bittable& out)
		{
			if (!convert(&v)) return false;
			if (m_count == 0)
			{
				out.resize(0);
				return true;
			}
			for (LONG i = 0; i < m_count; ++i)
			{
				_variant_t var;
				if (!get_item(i, var, VT_I4)) return false;
				out.set(var.lVal, true);
			}
			return true;
		}

		bool create(LONG count)
		{
			m_count = count;
			m_psa = SafeArrayCreateVector(VT_VARIANT, 0, count);
			m_reader = false;
			return m_psa != NULL;
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
				m_psa = NULL;
			}
		}

	private:
		LONG m_count;
		SAFEARRAY* m_psa;
		bool m_reader;
	};
}
