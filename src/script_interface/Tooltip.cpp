#include "stdafx.h"
#include "Tooltip.h"

Tooltip::Tooltip(HWND p_wndparent, const panel_tooltip_param_ptr& p_param_ptr) : m_wndparent(p_wndparent), m_panel_tooltip_param_ptr(p_param_ptr), m_tip_buffer(TO_BSTR(jsp::component_name))
{
	m_wndtooltip = CreateWindowEx(
		WS_EX_TOPMOST,
		TOOLTIPS_CLASS,
		nullptr,
		WS_POPUP | TTS_ALWAYSTIP | TTS_NOPREFIX,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		p_wndparent,
		nullptr,
		core_api::get_my_instance(),
		nullptr);

	SetWindowPos(m_wndtooltip, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	memset(&m_ti, 0, sizeof(m_ti));
	m_ti.cbSize = sizeof(m_ti);
	m_ti.uFlags = TTF_IDISHWND | TTF_SUBCLASS | TTF_TRANSPARENT;
	m_ti.hinst = core_api::get_my_instance();
	m_ti.hwnd = p_wndparent;
	m_ti.uId = reinterpret_cast<UINT_PTR>(p_wndparent);
	m_ti.lpszText = m_tip_buffer;

	const HFONT hFont = CreateFont(
		-static_cast<int>(m_panel_tooltip_param_ptr->font_size),
		0,
		0,
		0,
		(m_panel_tooltip_param_ptr->font_style & Gdiplus::FontStyleBold) ? FW_BOLD : FW_NORMAL,
		(m_panel_tooltip_param_ptr->font_style & Gdiplus::FontStyleItalic) ? TRUE : FALSE,
		(m_panel_tooltip_param_ptr->font_style & Gdiplus::FontStyleUnderline) ? TRUE : FALSE,
		(m_panel_tooltip_param_ptr->font_style & Gdiplus::FontStyleStrikeout) ? TRUE : FALSE,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		m_panel_tooltip_param_ptr->font_name);

	SendMessage(m_wndtooltip, TTM_ADDTOOL, 0, reinterpret_cast<LPARAM>(&m_ti));
	SendMessage(m_wndtooltip, TTM_ACTIVATE, FALSE, 0);
	SendMessage(m_wndtooltip, WM_SETFONT, reinterpret_cast<WPARAM>(hFont), MAKELPARAM(FALSE, 0));

	m_panel_tooltip_param_ptr->tooltip_hwnd = m_wndtooltip;
}

Tooltip::~Tooltip() {}

void Tooltip::FinalRelease()
{
	if (m_wndtooltip && IsWindow(m_wndtooltip))
	{
		DestroyWindow(m_wndtooltip);
		m_wndtooltip = nullptr;
	}

	if (m_tip_buffer)
	{
		SysFreeString(m_tip_buffer);
		m_tip_buffer = nullptr;
	}
}

STDMETHODIMP Tooltip::Activate()
{
	SendMessage(m_wndtooltip, TTM_ACTIVATE, TRUE, 0);
	return S_OK;
}

STDMETHODIMP Tooltip::Deactivate()
{
	SendMessage(m_wndtooltip, TTM_ACTIVATE, FALSE, 0);
	return S_OK;
}

STDMETHODIMP Tooltip::GetDelayTime(int type, int* p)
{
	if (!p) return E_POINTER;
	if (type < TTDT_AUTOMATIC || type > TTDT_INITIAL) return E_INVALIDARG;

	*p = SendMessage(m_wndtooltip, TTM_GETDELAYTIME, type, 0);
	return S_OK;
}

STDMETHODIMP Tooltip::SetDelayTime(int type, int time)
{
	if (type < TTDT_AUTOMATIC || type > TTDT_INITIAL) return E_INVALIDARG;

	SendMessage(m_wndtooltip, TTM_SETDELAYTIME, type, time);
	return S_OK;
}

STDMETHODIMP Tooltip::SetMaxWidth(int width)
{
	SendMessage(m_wndtooltip, TTM_SETMAXTIPWIDTH, 0, width);
	return S_OK;
}

STDMETHODIMP Tooltip::TrackPosition(int x, int y)
{
	POINT pt = { x, y };
	ClientToScreen(m_wndparent, &pt);
	SendMessage(m_wndtooltip, TTM_TRACKPOSITION, 0, MAKELONG(pt.x, pt.y));
	return S_OK;
}

STDMETHODIMP Tooltip::get_Text(BSTR* p)
{
	if (!p) return E_POINTER;

	*p = SysAllocString(m_tip_buffer);
	return S_OK;
}

STDMETHODIMP Tooltip::put_Text(BSTR text)
{
	SysReAllocString(&m_tip_buffer, text);
	m_ti.lpszText = m_tip_buffer;
	SendMessage(m_wndtooltip, TTM_SETTOOLINFO, 0, reinterpret_cast<LPARAM>(&m_ti));
	return S_OK;
}

STDMETHODIMP Tooltip::put_TrackActivate(VARIANT_BOOL activate)
{
	if (activate)
	{
		m_ti.uFlags |= TTF_TRACK | TTF_ABSOLUTE;
	}
	else
	{
		m_ti.uFlags &= ~(TTF_TRACK | TTF_ABSOLUTE);
	}

	SendMessage(m_wndtooltip, TTM_TRACKACTIVATE, activate != VARIANT_FALSE ? TRUE : FALSE, reinterpret_cast<LPARAM>(&m_ti));
	return S_OK;
}
