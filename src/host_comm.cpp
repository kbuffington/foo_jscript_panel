#include "stdafx.h"
#include "host_comm.h"

host_comm::host_comm()
	: m_hwnd(nullptr)
	, m_hdc(nullptr)
	, m_width(0)
	, m_height(0)
	, m_gr_bmp(nullptr)
	, m_suppress_drawing(false)
	, m_paint_pending(false)
	, m_instance_type(KInstanceTypeCUI)
	, m_script_info()
	, m_panel_tooltip_param_ptr(new panel_tooltip_param)
{
	m_max_size = { INT_MAX, INT_MAX };
	m_min_size = { 0, 0 };
	reset_config();
}

host_comm::~host_comm() {}

DWORD host_comm::get_edge_style() const
{
	switch (m_edge_style)
	{
	case SUNKEN_EDGE:
		return WS_EX_CLIENTEDGE;

	case GREY_EDGE:
		return WS_EX_STATICEDGE;

	default:
		return 0;
	}
}

HDC host_comm::get_hdc()
{
	return m_hdc;
}

HWND host_comm::get_hwnd()
{
	return m_hwnd;
}

int host_comm::get_height()
{
	return m_height;
}

int host_comm::get_width()
{
	return m_width;
}

panel_tooltip_param_ptr& host_comm::panel_tooltip()
{
	return m_panel_tooltip_param_ptr;
}

pfc::string8_fast host_comm::get_default_script_code()
{
	puResource pures = uLoadResource(core_api::get_my_instance(), uMAKEINTRESOURCE(IDR_SCRIPT), "TEXT");
	pfc::string8_fast content(static_cast<const char*>(pures->GetPointer()), pures->GetSize());
	return content;
}

pfc::string8_fast host_comm::get_default_script_engine_str()
{
	return helpers::supports_chakra() ? "Chakra" : "JScript";
}

t_size host_comm::get_instance_type()
{
	return m_instance_type;
}

void host_comm::load_config(stream_reader* reader, t_size size, abort_callback& abort)
{
	reset_config();

	if (size > sizeof(t_size))
	{
		t_size ver = 0;
		try
		{
			reader->read_object_t(ver, abort);
			reader->skip_object(sizeof(false), abort); // HACK: skip over "delay load"
			reader->skip_object(sizeof(GUID), abort); // HACK: skip over "GUID"
			reader->read_object(&m_edge_style, sizeof(m_edge_style), abort);
			m_config_prop.load(reader, abort);
			reader->skip_object(sizeof(false), abort); // HACK: skip over "disable before"
			reader->read_object_t(m_grab_focus, abort);
			reader->read_object(&m_wndpl, sizeof(m_wndpl), abort);
			reader->read_string(m_script_engine_str, abort);
			reader->read_string(m_script_code, abort);
			reader->read_object_t(m_pseudo_transparent, abort);
		}
		catch (...)
		{
			reset_config();
			FB2K_console_formatter() << JSP_NAME_VERSION ": Configuration has been corrupted. All settings have been reset.";
		}
	}
}

void host_comm::redraw()
{
	m_paint_pending = false;
	RedrawWindow(m_hwnd, nullptr, nullptr, RDW_INVALIDATE | RDW_UPDATENOW);
}

void host_comm::refresh_background(LPRECT lprcUpdate)
{
	HWND wnd_parent = GetAncestor(m_hwnd, GA_PARENT);

	if (!wnd_parent || IsIconic(core_api::get_main_window()) || !IsWindowVisible(m_hwnd))
		return;

	// HACK: for Tab control
	HWND hwnd = FindWindowEx(wnd_parent, nullptr, nullptr, nullptr);
	while (hwnd != nullptr)
	{
		pfc::string8_fast name;
		uGetClassName(hwnd, name);
		if (name.equals("SysTabControl32"))
		{
			wnd_parent = hwnd;
			break;
		}
		hwnd = FindWindowEx(wnd_parent, hwnd, nullptr, nullptr);
	}

	HDC dc_parent = GetDC(wnd_parent);
	HDC hdc_bk = CreateCompatibleDC(dc_parent);
	POINT pt = { 0, 0 };
	RECT rect_child = { 0, 0, m_width, m_height };
	RECT rect_parent;
	HRGN rgn_child = nullptr;

	if (lprcUpdate)
	{
		HRGN rgn = CreateRectRgnIndirect(lprcUpdate);
		rgn_child = CreateRectRgnIndirect(&rect_child);
		CombineRgn(rgn_child, rgn_child, rgn, RGN_DIFF);
		DeleteRgn(rgn);
	}
	else
	{
		rgn_child = CreateRectRgn(0, 0, 0, 0);
	}

	ClientToScreen(m_hwnd, &pt);
	ScreenToClient(wnd_parent, &pt);

	CopyRect(&rect_parent, &rect_child);
	ClientToScreen(m_hwnd, (LPPOINT)&rect_parent);
	ClientToScreen(m_hwnd, (LPPOINT)&rect_parent + 1);
	ScreenToClient(wnd_parent, (LPPOINT)&rect_parent);
	ScreenToClient(wnd_parent, (LPPOINT)&rect_parent + 1);

	// Force Repaint
	m_suppress_drawing = true;
	SetWindowRgn(m_hwnd, rgn_child, FALSE);
	RedrawWindow(wnd_parent, &rect_parent, nullptr, RDW_INVALIDATE | RDW_ERASE | RDW_ERASENOW | RDW_UPDATENOW);

	// Background bitmap
	HBITMAP old_bmp = SelectBitmap(hdc_bk, m_gr_bmp_bk);

	BitBlt(hdc_bk, rect_child.left, rect_child.top, rect_child.right - rect_child.left, rect_child.bottom - rect_child.top, dc_parent, pt.x, pt.y, SRCCOPY);

	SelectBitmap(hdc_bk, old_bmp);
	DeleteDC(hdc_bk);
	ReleaseDC(wnd_parent, dc_parent);
	DeleteRgn(rgn_child);
	SetWindowRgn(m_hwnd, nullptr, FALSE);
	m_suppress_drawing = false;
	if (m_edge_style) SendMessage(m_hwnd, WM_NCPAINT, 1, 0);
	repaint(true);
}

void host_comm::repaint(bool force)
{
	m_paint_pending = true;

	if (force)
	{
		RedrawWindow(m_hwnd, nullptr, nullptr, RDW_INVALIDATE | RDW_UPDATENOW);
	}
	else
	{
		InvalidateRect(m_hwnd, nullptr, FALSE);
	}
}

void host_comm::repaint_rect(int x, int y, int w, int h, bool force)
{
	RECT rc = { x, y, x + w, y + h };
	m_paint_pending = true;

	if (force)
	{
		RedrawWindow(m_hwnd, &rc, nullptr, RDW_INVALIDATE | RDW_UPDATENOW);
	}
	else
	{
		InvalidateRect(m_hwnd, &rc, FALSE);
	}
}

void host_comm::reset_config()
{
	m_script_engine_str = get_default_script_engine_str();
	m_script_code = get_default_script_code();
	m_pseudo_transparent = false;
	m_wndpl.length = 0;
	m_grab_focus = true;
	m_edge_style = NO_EDGE;
}

void host_comm::save_config(stream_writer* writer, abort_callback& abort) const
{
	const t_size ver = 123;

	try
	{
		writer->write_object_t(ver, abort);
		writer->write_object_t(false, abort); // HACK: write this in place of "delay load"
		writer->write_object_t(pfc::guid_null, abort); // HACK: write this in place of "GUID"
		writer->write_object(&m_edge_style, sizeof(m_edge_style), abort);
		m_config_prop.save(writer, abort);
		writer->write_object_t(false, abort); // HACK: write this in place of "disable before"
		writer->write_object_t(m_grab_focus, abort);
		writer->write_object(&m_wndpl, sizeof(m_wndpl), abort);
		writer->write_string(m_script_engine_str, abort);
		writer->write_string(m_script_code, abort);
		writer->write_object_t(m_pseudo_transparent, abort);
	}
	catch (...) {}
}
