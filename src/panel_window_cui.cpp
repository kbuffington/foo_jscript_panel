#include "stdafx.h"
#include "panel_window.h"
#include "panel_window_cui.h"

// CUI panel instance
static uie::window_factory<panel_window_cui> g_js_panel_wndow_cui;

DWORD panel_window_cui::get_colour_ui(t_size type)
{
	if (type <= cui::colours::colour_active_item_frame)
	{
		cui::colours::helper helper(pfc::guid_null);
		return helpers::convert_colorref_to_argb(helper.get_colour((cui::colours::colour_identifier_t)type));
	}
	return 0;
}

HFONT panel_window_cui::get_font_ui(t_size type)
{
	if (type <= cui::fonts::font_type_labels)
	{
		return static_api_ptr_t<cui::fonts::manager>()->get_font((cui::fonts::font_type_t)type);
	}
	return nullptr;
}

HWND panel_window_cui::create_or_transfer_window(HWND parent, const uie::window_host_ptr& host, const ui_helpers::window_position_t& p_position)
{
	if (m_host.is_valid())
	{
		ShowWindow(m_hwnd, SW_HIDE);
		SetParent(m_hwnd, parent);
		m_host->relinquish_ownership(m_hwnd);
		m_host = host;

		SetWindowPos(m_hwnd, nullptr, p_position.x, p_position.y, p_position.cx, p_position.cy, SWP_NOZORDER);
	}
	else
	{
		m_host = host;
		create(parent, this, p_position);
	}
	return get_wnd();
}

HWND panel_window_cui::get_wnd() const
{
	return panel_window::get_wnd();
}

LRESULT panel_window_cui::on_message(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:
		if (uie::window::g_process_keydown_keyboard_shortcuts(wp))
			return 0;
		break;

	case WM_CREATE:
		try
		{
			static_api_ptr_t<cui::fonts::manager>()->register_common_callback(this);
			static_api_ptr_t<cui::colours::manager>()->register_common_callback(this);
		}
		catch (...) {}
		break;

	case WM_DESTROY:
		try
		{
			static_api_ptr_t<cui::fonts::manager>()->deregister_common_callback(this);
			static_api_ptr_t<cui::colours::manager>()->deregister_common_callback(this);
		}
		catch (...) {}
		break;
	}
	return panel_window::on_message(hwnd, msg, wp, lp);
}

bool panel_window_cui::is_available(const uie::window_host_ptr& p) const
{
	return true;
}

const GUID& panel_window_cui::get_extension_guid() const
{
	return g_guid_jsp_window_cui;
}

t_size panel_window_cui::get_type() const
{
	return uie::type_toolbar | uie::type_panel;
}

void panel_window_cui::destroy_window()
{
	destroy();
	m_host.release();
}

void panel_window_cui::get_category(pfc::string_base& out) const
{
	out = "Panels";
}

void panel_window_cui::get_config(stream_writer* writer, abort_callback& abort) const
{
	save_config(writer, abort);
}

void panel_window_cui::get_name(pfc::string_base& out) const
{
	out = JSP_NAME;
}

void panel_window_cui::on_bool_changed(t_size mask) const
{
	// TODO: may be implemented one day
}

void panel_window_cui::on_colour_changed(t_size mask) const
{
	PostMessage(m_hwnd, callback_id::on_colours_changed, 0, 0);
}

void panel_window_cui::on_font_changed(t_size mask) const
{
	PostMessage(m_hwnd, callback_id::on_font_changed, 0, 0);
}

void panel_window_cui::set_config(stream_reader* reader, t_size size, abort_callback& abort)
{
	load_config(reader, size, abort);
}

void panel_window_cui::notify_size_limit_changed()
{
	if (m_host.is_valid())
	{
		m_host->on_size_limit_change(m_hwnd, uie::size_limit_all);
	}
}
