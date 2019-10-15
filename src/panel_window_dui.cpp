#include "stdafx.h"
#include "panel_window.h"

class panel_window_dui : public panel_window, public ui_element_instance
{
public:
	panel_window_dui(ui_element_config::ptr cfg, ui_element_instance_callback::ptr callback) : m_callback(callback)
	{
		m_instance_type = KInstanceTypeDUI;
		set_configuration(cfg);
	}

	~panel_window_dui()
	{
		panel_window::destroy();
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
		out = JSP_NAME;
	}

	DWORD get_colour_ui(t_size type) override
	{
		static const std::array<const GUID*, 4> guids =
		{
			&ui_color_text,
			&ui_color_background,
			&ui_color_highlight,
			&ui_color_selection,
		};
		return type < guids.size() ? helpers::convert_colorref_to_argb(m_callback->query_std_color(*guids[type])) : 0;
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

	IGdiFont* get_font_ui(t_size type) override
	{
		IGdiFont* ret = nullptr;

		static const std::array<const GUID*, 6> guids =
		{
			&ui_font_default,
			&ui_font_tabs,
			&ui_font_lists,
			&ui_font_playlists,
			&ui_font_statusbar,
			&ui_font_console,
		};

		if (type < guids.size())
		{
			HFONT hFont = m_callback->query_font_ex(*guids[type]);
			if (hFont)
			{
				Gdiplus::Font* font = new Gdiplus::Font(m_hdc, hFont);
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

	LRESULT on_message(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) override
	{
		switch (msg)
		{
		case WM_RBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONDBLCLK:
		case WM_CONTEXTMENU:
			if (m_callback->is_edit_mode_enabled())
				return DefWindowProc(hwnd, msg, wp, lp);
			break;
		case WM_GETDLGCODE:
			return DLGC_WANTALLKEYS;
		}
		return panel_window::on_message(hwnd, msg, wp, lp);
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

	void edit_mode_context_menu_build(const POINT& p_point, bool p_fromkeyboard, HMENU p_menu, t_size p_id_base) override
	{
		build_context_menu(p_menu, p_id_base);
	}

	void edit_mode_context_menu_command(const POINT& p_point, bool p_fromkeyboard, t_size p_id, t_size p_id_base) override
	{
		execute_context_menu_command(p_id, p_id_base);
	}

	void notify(const GUID& p_what, t_size p_param1, const void* p_param2, t_size p_param2size) override
	{
		if (p_what == ui_element_notify_colors_changed)
		{
			PostMessage(m_hwnd, callback_id::on_colours_changed, 0, 0);
		}
		else if (p_what == ui_element_notify_font_changed)
		{
			PostMessage(m_hwnd, callback_id::on_font_changed, 0, 0);
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

template <class T>
class my_ui_element : public ui_element
{
public:
	GUID get_guid() override
	{
		return T::g_get_guid();
	}

	GUID get_subclass() override
	{
		return T::g_get_subclass();
	}

	bool get_description(pfc::string_base& out) override
	{
		out = T::g_get_description();
		return true;
	}

	ui_element_children_enumerator_ptr enumerate_children(ui_element_config::ptr cfg) override
	{
		return nullptr;
	}

	ui_element_config::ptr get_default_configuration() override
	{
		return T::g_get_default_configuration();
	}

	ui_element_instance::ptr instantiate(HWND parent, ui_element_config::ptr cfg, ui_element_instance_callback::ptr callback) override
	{
		PFC_ASSERT(cfg->get_guid() == get_guid());
		auto item = fb2k::service_new<T>(cfg, callback);
		item->create(parent);
		return item;
	}

	void get_name(pfc::string_base& out) override
	{
		T::g_get_name(out);
	}
};

// DUI panel instance
static service_factory_t<my_ui_element<panel_window_dui>> g_my_ui_element;
