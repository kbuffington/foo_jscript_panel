#include "stdafx.h"
#include "GdiFont.h"

GdiFont::GdiFont(Gdiplus::Font* p, HFONT hFont, bool managed) : GdiObj<IGdiFont, Gdiplus::Font>(p), m_hFont(hFont), m_managed(managed) {}
GdiFont::~GdiFont() {}

void GdiFont::FinalRelease()
{
	if (m_hFont && m_managed)
	{
		DeleteFont(m_hFont);
		m_hFont = nullptr;
	}

	__super::FinalRelease();
}

STDMETHODIMP GdiFont::get__HFont(HFONT* p)
{
	if (!m_ptr || !p) return E_POINTER;

	*p = m_hFont;
	return S_OK;
}

STDMETHODIMP GdiFont::get_Height(UINT* p)
{
	if (!m_ptr || !p) return E_POINTER;

	Gdiplus::Bitmap img(1, 1, PixelFormat32bppPARGB);
	Gdiplus::Graphics g(&img);
	*p = TO_UINT(m_ptr->GetHeight(&g));
	return S_OK;
}

STDMETHODIMP GdiFont::get_Name(BSTR* p)
{
	if (!m_ptr || !p) return E_POINTER;

	wchar_t name[LF_FACESIZE] = { 0 };
	Gdiplus::FontFamily fontFamily;
	m_ptr->GetFamily(&fontFamily);
	fontFamily.GetFamilyName(name, LANG_NEUTRAL);
	*p = SysAllocString(name);
	return S_OK;
}

STDMETHODIMP GdiFont::get_Size(float* p)
{
	if (!m_ptr || !p) return E_POINTER;

	*p = m_ptr->GetSize();
	return S_OK;
}

STDMETHODIMP GdiFont::get_Style(int* p)
{
	if (!m_ptr || !p) return E_POINTER;

	*p = m_ptr->GetStyle();
	return S_OK;
}
