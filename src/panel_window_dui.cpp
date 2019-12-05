#include "stdafx.h"
#include "panel_window.h"

class panel_window_dui : public panel_window, public ui_element_instance, public CWindowImpl<panel_window_dui>
{
public:
	DECLARE_WND_CLASS_EX(PFC_WIDESTRING(COMPONENT_NAME), CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS, -1);

	panel_window_dui(ui_element_config::ptr cfg, ui_element_instance_callback::ptr callback) : m_callback(callback)
	{
		m_instance_type = instance_type::dui;
		set_configuration(cfg);
	}

	static GUID g_get_guid()
	{
		return jsp_guids::window_dui;
	}

	static GUID g_get_subclass()
	{
		return ui_element_subclass_utility;
	}

	static pfc::string8_fast g_get_description()
	{
		return "Customisable panel with JScript scripting support.";
	}

	static ui_element_config::ptr g_get_default_configuration()
	{
		ui_element_config_builder builder;
		return builder.finish(g_get_guid());
	}

	static void g_get_name(pfc::string_base& out)
	{
		out = COMPONENT_NAME;
	}

	BOOL ProcessWindowMessage(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp, LRESULT& res, DWORD) override
	{
		res = 0;

		switch (msg)
		{
		case WM_SETCURSOR:
		case WM_ERASEBKGND:
			res = 1;
			return TRUE;
		case WM_CONTEXTMENU:
			res = 1;
			if (m_callback->is_edit_mode_enabled()) return FALSE;
			on_context_menu(lp);
			return TRUE;
		case WM_RBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONDBLCLK:
			if (m_callback->is_edit_mode_enabled()) return FALSE;
			break;
		case WM_GETDLGCODE:
			res = DLGC_WANTALLKEYS;
			return TRUE;
		}

		if (handle_message(hwnd, msg, wp, lp)) return TRUE;
		return FALSE;
	}

	COLORREF get_colour_ui(size_t type) override
	{
		return type < jsp_guids::colours.size() ? m_callback->query_std_color(*jsp_guids::colours[type]) : 0;
	}

	GUID get_guid() override
	{
		return g_get_guid();
	}

	GUID get_subclass() override
	{
		return g_get_subclass();
	}

	HWND get_wnd() override
	{
		return m_hwnd;
	}

	IGdiFont* get_font_ui(size_t type) override
	{
		IGdiFont* ret = nullptr;

		if (type < jsp_guids::fonts.size())
		{
			HFONT hFont = m_callback->query_font_ex(*jsp_guids::fonts[type]);
			if (hFont)
			{
				auto font = new Gdiplus::Font(m_hdc, hFont);
				if (helpers::ensure_gdiplus_object(font))
				{
					ret = new com_object_impl_t<GdiFont>(font, hFont, false);
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

	bool edit_mode_context_menu_test(const POINT& p_point, bool p_fromkeyboard) override
	{
		return true;
	}

	ui_element_config::ptr get_configuration() override
	{
		ui_element_config_builder builder;
		save_config(&builder.m_stream, fb2k::noAbort);
		return builder.finish(g_get_guid());
	}

	void edit_mode_context_menu_build(const POINT& p_point, bool p_fromkeyboard, HMENU p_menu, size_t p_id_base) override
	{
		build_context_menu(p_menu, p_id_base);
	}

	void edit_mode_context_menu_command(const POINT& p_point, bool p_fromkeyboard, size_t p_id, size_t p_id_base) override
	{
		execute_context_menu_command(p_id, p_id_base);
	}

	void initialize_window(HWND parent)
	{
		Create(parent);
	}

	void notify(const GUID& p_what, size_t p_param1, const void* p_param2, size_t p_param2size) override
	{
		if (p_what == ui_element_notify_colors_changed)
		{
			script_invoke(callback_id::on_colours_changed);
		}
		else if (p_what == ui_element_notify_font_changed)
		{
			script_invoke(callback_id::on_font_changed);
		}
	}

	void notify_size_limit_changed() override
	{
		m_callback->on_min_max_info_change();
	}

	void set_configuration(ui_element_config::ptr data) override
	{
		ui_element_config_parser parser(data);

		load_config(&parser.m_stream, parser.get_remaining(), fb2k::noAbort);

		// FIX: If window already created, DUI won't destroy it and create it again.
		if (m_hwnd)
		{
			update_script();
		}
	}

private:
	ui_element_instance_callback::ptr m_callback;
};

class my_ui_element_impl : public ui_element_impl<panel_window_dui> {};

FB2K_SERVICE_FACTORY(my_ui_element_impl);
