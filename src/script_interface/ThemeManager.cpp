#include "stdafx.h"
#include "ThemeManager.h"

ThemeManager::ThemeManager(HWND hwnd, BSTR classlist)
{
	m_theme = OpenThemeData(hwnd, classlist);

	if (!m_theme) throw pfc::exception_invalid_params();
}

ThemeManager::~ThemeManager() {}

void ThemeManager::FinalRelease()
{
	if (m_theme)
	{
		CloseThemeData(m_theme);
		m_theme = nullptr;
	}
}

STDMETHODIMP ThemeManager::DrawThemeBackground(IGdiGraphics* gr, int x, int y, int w, int h, int clip_x, int clip_y, int clip_w, int clip_h)
{
	if (!m_theme) return E_POINTER;

	Gdiplus::Graphics* graphics = nullptr;
	GET_PTR(gr, graphics);

	RECT rc = { x, y, x + w, y + h };
	RECT rc_clip = { clip_x, clip_y, clip_x + clip_w, clip_y + clip_h };
	LPCRECT rc_pclip = &rc_clip;
	HDC dc = graphics->GetHDC();

	if (clip_x == 0 && clip_y == 0 && clip_w == 0 && clip_h == 0)
	{
		rc_pclip = nullptr;
	}

	const HRESULT hr = ::DrawThemeBackground(m_theme, dc, m_partid, m_stateid, &rc, rc_pclip);

	graphics->ReleaseHDC(dc);
	return hr;
}

STDMETHODIMP ThemeManager::IsThemePartDefined(int partid, int stateid, VARIANT_BOOL* p)
{
	if (!m_theme || !p) return E_POINTER;

	*p = TO_VARIANT_BOOL(::IsThemePartDefined(m_theme, partid, stateid));
	return S_OK;
}

STDMETHODIMP ThemeManager::SetPartAndStateID(int partid, int stateid)
{
	if (!m_theme) return E_POINTER;

	m_partid = partid;
	m_stateid = stateid;
	return S_OK;
}
