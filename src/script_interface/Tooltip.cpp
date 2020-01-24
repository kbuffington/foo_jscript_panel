#include "stdafx.h"
#include "helpers.h"
#include "Tooltip.h"

Tooltip::Tooltip(CWindow wndparent, const panel_tooltip_param_ptr& param_ptr) : m_wndparent(wndparent), m_panel_tooltip_param_ptr(param_ptr), m_tip_buffer(TO_BSTR(jsp::component_name))
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
		m_wndparent,
		nullptr,
		core_api::get_my_instance(),
		nullptr);

	m_wndtooltip.SetWindowPos(HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	memset(&m_ti, 0, sizeof(m_ti));
	m_ti.cbSize = sizeof(m_ti);
	m_ti.uFlags = TTF_IDISHWND | TTF_SUBCLASS | TTF_TRANSPARENT;
	m_ti.hinst = core_api::get_my_instance();
	m_ti.hwnd = m_wndparent;
	m_ti.uId = reinterpret_cast<UINT_PTR>(m_wndparent.m_hWnd);
	m_ti.lpszText = m_tip_buffer;

	const HFONT hFont = helpers::create_font(m_panel_tooltip_param_ptr->font_name, m_panel_tooltip_param_ptr->font_size, m_panel_tooltip_param_ptr->font_style);

	m_wndtooltip.SendMessage(TTM_ADDTOOL, 0, reinterpret_cast<LPARAM>(&m_ti));
	m_wndtooltip.SendMessage(TTM_ACTIVATE, FALSE, 0);
	m_wndtooltip.SendMessage(WM_SETFONT, reinterpret_cast<WPARAM>(hFont), MAKELPARAM(FALSE, 0));

	m_panel_tooltip_param_ptr->tooltip_hwnd = m_wndtooltip;
}

Tooltip::~Tooltip() {}

void Tooltip::FinalRelease()
{
	if (m_wndtooltip && m_wndtooltip.IsWindow())
	{
		m_wndtooltip.DestroyWindow();
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
	m_wndtooltip.SendMessage(TTM_ACTIVATE, TRUE, 0);
	return S_OK;
}

STDMETHODIMP Tooltip::Deactivate()
{
	m_wndtooltip.SendMessage(TTM_ACTIVATE, FALSE, 0);
	return S_OK;
}

STDMETHODIMP Tooltip::GetDelayTime(int type, int* p)
{
	if (!p) return E_POINTER;
	if (type < TTDT_AUTOMATIC || type > TTDT_INITIAL) return E_INVALIDARG;

	*p = m_wndtooltip.SendMessage(TTM_GETDELAYTIME, type, 0);
	return S_OK;
}

STDMETHODIMP Tooltip::SetDelayTime(int type, int time)
{
	if (type < TTDT_AUTOMATIC || type > TTDT_INITIAL) return E_INVALIDARG;

	m_wndtooltip.SendMessage(TTM_SETDELAYTIME, type, time);
	return S_OK;
}

STDMETHODIMP Tooltip::SetMaxWidth(int width)
{
	m_wndtooltip.SendMessage(TTM_SETMAXTIPWIDTH, 0, width);
	return S_OK;
}

STDMETHODIMP Tooltip::TrackPosition(int x, int y)
{
	CPoint pt(x, y);
	m_wndparent.ClientToScreen(&pt);
	m_wndtooltip.SendMessage(TTM_TRACKPOSITION, 0, MAKELONG(pt.x, pt.y));
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
	m_wndtooltip.SendMessage(TTM_SETTOOLINFO, 0, reinterpret_cast<LPARAM>(&m_ti));
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

	m_wndtooltip.SendMessage(TTM_TRACKACTIVATE, activate != VARIANT_FALSE ? TRUE : FALSE, reinterpret_cast<LPARAM>(&m_ti));
	return S_OK;
}
