#include "stdafx.h"
#include "panel_window.h"

namespace
{
	class panel_window_cui : public panel_window, public uie::container_ui_extension, public cui::fonts::common_callback, public cui::colours::common_callback
	{
	public:
		panel_window_cui()
		{
			try
			{
				static_api_ptr_t<cui::fonts::manager>()->register_common_callback(this);
				static_api_ptr_t<cui::colours::manager>()->register_common_callback(this);
			}
			catch (...) {}
		}

		~panel_window_cui()
		{
			try
			{
				static_api_ptr_t<cui::fonts::manager>()->deregister_common_callback(this);
				static_api_ptr_t<cui::colours::manager>()->deregister_common_callback(this);
			}
			catch (...) {}
		}

		COLORREF get_colour_ui(size_t type) override
		{
			const auto t = static_cast<cui::colours::colour_identifier_t>(type);
			if (t <= cui::colours::colour_active_item_frame)
			{
				cui::colours::helper helper(pfc::guid_null);
				return helper.get_colour(t);
			}
			return 0;
		}

		HWND create_or_transfer_window(HWND parent, const uie::window_host_ptr& host, const ui_helpers::window_position_t& p_position) override
		{
			if (m_hwnd)
			{
				m_hwnd.ShowWindow(SW_HIDE);
				m_hwnd.SetParent(parent);
				m_host->relinquish_ownership(m_hwnd);
				m_host = host;

				m_hwnd.SetWindowPos(nullptr, p_position.x, p_position.y, p_position.cx, p_position.cy, SWP_NOZORDER);
			}
			else
			{
				m_host = host;
				create(parent, this, p_position);
			}

			pfc::string8_fast name;
			uGetClassName(parent, name);
			m_supports_transparency = name.equals("PSSWindowContainer") || name.equals("ReBarWindow32");

			return m_hwnd;
		}

		HWND get_wnd() const override
		{
			return m_hwnd;
		}

		IGdiFont* get_font_ui(size_t type) override
		{
			IGdiFont* ret = nullptr;

			const auto t = static_cast<cui::fonts::font_type_t>(type);
			if (t <= cui::fonts::font_type_labels)
			{
				HFONT hFont = static_api_ptr_t<cui::fonts::manager>()->get_font(t);
				if (hFont)
				{
					auto font = new Gdiplus::Font(m_hdc, hFont);
					if (helpers::ensure_gdiplus_object(font))
					{
						ret = new com_object_impl_t<GdiFont>(font, hFont);
					}
					else
					{
						if (font) delete font;
						font = nullptr;
					}
				}
			}
			return ret;
		}

		LRESULT on_message(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) override
		{
			switch (msg)
			{
			case WM_SETCURSOR:
				return 1;
			case WM_ERASEBKGND:
				if (is_transparent()) m_hwnd.PostMessage(jsp::uwm_refreshbk);
				return 1;
			case WM_CONTEXTMENU:
				on_context_menu(lp);
				return 1;
			case WM_SYSKEYDOWN:
			case WM_KEYDOWN:
				if (uie::window::g_process_keydown_keyboard_shortcuts(wp))
					return 0;
				break;
			}

			if (handle_message(hwnd, msg, wp, lp)) return 0;
			return uDefWindowProc(hwnd, msg, wp, lp);
		}

		bool have_config_popup() const override
		{
			return true;
		}

		bool is_available(const uie::window_host_ptr& p) const override
		{
			return true;
		}

		bool show_config_popup(HWND parent) override
		{
			show_configure_popup(parent);
			return true;
		}

		class_data& get_class_data() const override
		{
			__implement_get_class_data_ex(jsp::class_name_cui, L"", false, 0, WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0, CS_DBLCLKS);
		}

		const GUID& get_extension_guid() const override
		{
			return jsp::guids::window_cui;
		}

		size_t get_type() const override
		{
			return uie::type_toolbar | uie::type_panel;
		}

		void destroy_window() override
		{
			destroy();
			m_host.release();
		}

		void get_category(pfc::string_base& out) const override
		{
			out = "Panels";
		}

		void get_config(stream_writer* writer, abort_callback& abort) const override
		{
			m_panel_config.get(writer, abort);
		}

		void get_name(pfc::string_base& out) const override
		{
			out = jsp::component_name;
		}

		void on_bool_changed(size_t mask) const override {}

		void on_colour_changed(size_t mask) const override
		{
			script_invoke(callback_id::on_colours_changed);
		}

		void on_font_changed(size_t mask) const override
		{
			script_invoke(callback_id::on_font_changed);
		}

		void set_config(stream_reader* reader, size_t size, abort_callback& abort) override
		{
			m_panel_config.set(reader, size, abort);
		}

		void notify_size_limit_changed() override
		{
			if (m_host.is_valid())
			{
				m_host->on_size_limit_change(m_hwnd, uie::size_limit_all);
			}
		}

	private:
		uie::window_host_ptr m_host;
	};

	static uie::window_factory<panel_window_cui> g_panel_window_cui;
}
