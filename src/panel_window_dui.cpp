#include "stdafx.h"
#include "panel_window.h"
#include "panel_window_dui.h"

// Using this avoids having to include ATL helpers
template <typename TImpl>
class my_ui_element_impl : public ui_element
{
public:
	GUID get_guid() override
	{
		return TImpl::g_get_guid();
	}

	GUID get_subclass() override
	{
		return TImpl::g_get_subclass();
	}

	bool get_description(pfc::string_base& out) override
	{
		out = TImpl::g_get_description();
		return true;
	}

	ui_element_children_enumerator_ptr enumerate_children(ui_element_config::ptr cfg) override
	{
		return nullptr;
	}

	ui_element_config::ptr get_default_configuration() override
	{
		return TImpl::g_get_default_configuration();
	}

	ui_element_instance::ptr instantiate(HWND parent, ui_element_config::ptr cfg, ui_element_instance_callback::ptr callback) override
	{
		PFC_ASSERT(cfg->get_guid() == get_guid());
		auto item = fb2k::service_new<ui_element_instance_impl_helper>(cfg, callback);
		item->initialise_window(parent);
		return item;
	}

	void get_name(pfc::string_base& out) override
	{
		TImpl::g_get_name(out);
	}

private:
	class ui_element_instance_impl_helper : public TImpl
	{
	public:
		ui_element_instance_impl_helper(ui_element_config::ptr cfg, ui_element_instance_callback::ptr callback) : TImpl(cfg, callback) {}

		GUID get_guid() override
		{
			return TImpl::g_get_guid();
		}

		GUID get_subclass() override
		{
			return TImpl::g_get_subclass();
		}
	};
};

// DUI panel instance
static service_factory_t<my_ui_element_impl<panel_window_dui>> g_js_panel_wndow_dui;

panel_window_dui::panel_window_dui(ui_element_config::ptr cfg, ui_element_instance_callback::ptr callback) : m_callback(callback)
{
	m_instance_type = KInstanceTypeDUI;
	set_configuration(cfg);
}

panel_window_dui::~panel_window_dui()
{
	panel_window::destroy();
}

DWORD panel_window_dui::get_colour_ui(t_size type)
{
	const GUID* guids[] = {
		&ui_color_text,
		&ui_color_background,
		&ui_color_highlight,
		&ui_color_selection,
	};

	if (type < _countof(guids))
	{
		return helpers::convert_colorref_to_argb(m_callback->query_std_color(*guids[type]));
	}

	return 0;
}

GUID panel_window_dui::g_get_guid()
{
	return g_guid_jsp_window_dui;
}

GUID panel_window_dui::g_get_subclass()
{
	return ui_element_subclass_utility;
}

HFONT panel_window_dui::get_font_ui(t_size type)
{
	const GUID* guids[] = {
		&ui_font_default,
		&ui_font_tabs,
		&ui_font_lists,
		&ui_font_playlists,
		&ui_font_statusbar,
		&ui_font_console,
	};

	if (type < _countof(guids))
	{
		return m_callback->query_font_ex(*guids[type]);
	}

	return nullptr;
}

HWND panel_window_dui::get_wnd()
{
	return panel_window::get_wnd();
}

LRESULT panel_window_dui::on_message(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
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

bool panel_window_dui::edit_mode_context_menu_test(const POINT& p_point, bool p_fromkeyboard)
{
	return true;
}

pfc::string8_fast panel_window_dui::g_get_description()
{
	return "Customisable panel with JScript scripting support.";
}

ui_element_config::ptr panel_window_dui::g_get_default_configuration()
{
	ui_element_config_builder builder;
	abort_callback_dummy abort;
	js_panel_vars vars;

	vars.reset_config();
	vars.save_config(&builder.m_stream, abort);
	return builder.finish(g_get_guid());
}

ui_element_config::ptr panel_window_dui::get_configuration()
{
	ui_element_config_builder builder;
	abort_callback_dummy abort;
	save_config(&builder.m_stream, abort);
	return builder.finish(g_get_guid());
}

void panel_window_dui::edit_mode_context_menu_build(const POINT& p_point, bool p_fromkeyboard, HMENU p_menu, t_size p_id_base)
{
	build_context_menu(p_menu, p_point.x, p_point.y, p_id_base);
}

void panel_window_dui::edit_mode_context_menu_command(const POINT& p_point, bool p_fromkeyboard, t_size p_id, t_size p_id_base)
{
	execute_context_menu_command(p_id, p_id_base);
}

void panel_window_dui::g_get_name(pfc::string_base& out)
{
	out = JSP_NAME;
}

void panel_window_dui::initialise_window(HWND parent)
{
	create(parent);
}

void panel_window_dui::notify(const GUID& p_what, t_size p_param1, const void* p_param2, t_size p_param2size)
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

void panel_window_dui::notify_size_limit_changed(t_size limit)
{
	m_callback->on_min_max_info_change();
}

void panel_window_dui::set_configuration(ui_element_config::ptr data)
{
	ui_element_config_parser parser(data);
	abort_callback_dummy abort;

	load_config(&parser.m_stream, parser.get_remaining(), abort);

	// FIX: If window already created, DUI won't destroy it and create it again.
	if (m_hwnd)
	{
		update_script();
	}
}
