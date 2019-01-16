#include "stdafx.h"
#include "host_comm.h"

host_comm::host_comm()
	: m_hwnd(NULL)
	, m_hdc(NULL)
	, m_width(0)
	, m_height(0)
	, m_gr_bmp(NULL)
	, m_suppress_drawing(false)
	, m_paint_pending(false)
	, m_instance_type(KInstanceTypeCUI)
	, m_script_info(get_config_guid())
	, m_panel_tooltip_param_ptr(new panel_tooltip_param)
{
	m_max_size = { INT_MAX, INT_MAX };
	m_min_size = { 0, 0 };
}

host_comm::~host_comm() {}

HDC host_comm::GetHDC()
{
	return m_hdc;
}

HWND host_comm::GetHWND()
{
	return m_hwnd;
}

UINT host_comm::GetInstanceType()
{
	return m_instance_type;
}

panel_tooltip_param_ptr& host_comm::PanelTooltipParam()
{
	return m_panel_tooltip_param_ptr;
}

int host_comm::GetHeight()
{
	return m_height;
}

int host_comm::GetWidth()
{
	return m_width;
}

void host_comm::Redraw()
{
	m_paint_pending = false;
	RedrawWindow(m_hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

void host_comm::RefreshBackground(LPRECT lprcUpdate)
{
	HWND wnd_parent = GetAncestor(m_hwnd, GA_PARENT);

	if (!wnd_parent || IsIconic(core_api::get_main_window()) || !IsWindowVisible(m_hwnd))
		return;

	// HACK: for Tab control
	HWND hwnd = FindWindowEx(wnd_parent, NULL, NULL, NULL);
	while (hwnd != NULL)
	{
		pfc::string8_fast name;
		uGetClassName(hwnd, name);
		if (name.equals("SysTabControl32"))
		{
			wnd_parent = hwnd;
			break;
		}
		hwnd = FindWindowEx(wnd_parent, hwnd, NULL, NULL);
	}

	HDC dc_parent = GetDC(wnd_parent);
	HDC hdc_bk = CreateCompatibleDC(dc_parent);
	POINT pt = { 0, 0 };
	RECT rect_child = { 0, 0, m_width, m_height };
	RECT rect_parent;
	HRGN rgn_child = NULL;

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
	RedrawWindow(wnd_parent, &rect_parent, NULL, RDW_INVALIDATE | RDW_ERASE | RDW_ERASENOW | RDW_UPDATENOW);

	// Background bitmap
	HBITMAP old_bmp = SelectBitmap(hdc_bk, m_gr_bmp_bk);

	// Paint BK
	BitBlt(hdc_bk, rect_child.left, rect_child.top, rect_child.right - rect_child.left, rect_child.bottom - rect_child.top, dc_parent, pt.x, pt.y, SRCCOPY);

	SelectBitmap(hdc_bk, old_bmp);
	DeleteDC(hdc_bk);
	ReleaseDC(wnd_parent, dc_parent);
	DeleteRgn(rgn_child);
	SetWindowRgn(m_hwnd, NULL, FALSE);
	m_suppress_drawing = false;
	if (get_edge_style()) SendMessage(m_hwnd, WM_NCPAINT, 1, 0);
	Repaint(true);
}

void host_comm::Repaint(bool force)
{
	m_paint_pending = true;

	if (force)
	{
		RedrawWindow(m_hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
	}
	else
	{
		InvalidateRect(m_hwnd, NULL, FALSE);
	}
}

void host_comm::RepaintRect(LONG x, LONG y, LONG w, LONG h, bool force)
{
	RECT rc = { x, y, x + w, y + h };
	m_paint_pending = true;

	if (force)
	{
		RedrawWindow(m_hwnd, &rc, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
	}
	else
	{
		InvalidateRect(m_hwnd, &rc, FALSE);
	}
}
