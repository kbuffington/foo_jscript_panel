#include "stdafx.h"
#include "GdiRawBitmap.h"

GdiRawBitmap::GdiRawBitmap(Gdiplus::Bitmap* bmp)
{
	PFC_ASSERT(bmp != nullptr);
	m_width = bmp->GetWidth();
	m_height = bmp->GetHeight();

	m_hdc = CreateCompatibleDC(nullptr);
	m_hbmp = CreateHBITMAP(bmp);
	m_hbmpold = static_cast<HBITMAP>(SelectObject(m_hdc, m_hbmp));
}

GdiRawBitmap::~GdiRawBitmap() {}

HBITMAP GdiRawBitmap::CreateHBITMAP(Gdiplus::Bitmap* bitmap_ptr)
{
	const Gdiplus::Rect rect(0, 0, bitmap_ptr->GetWidth(), bitmap_ptr->GetHeight());
	Gdiplus::BitmapData bmpdata;

	if (bitmap_ptr->LockBits(&rect, Gdiplus::ImageLockModeRead, PixelFormat32bppPARGB, &bmpdata) != Gdiplus::Ok)
	{
		return nullptr;
	}

	BITMAP bm;
	bm.bmType = 0;
	bm.bmWidth = bmpdata.Width;
	bm.bmHeight = bmpdata.Height;
	bm.bmWidthBytes = bmpdata.Stride;
	bm.bmPlanes = 1;
	bm.bmBitsPixel = 32;
	bm.bmBits = bmpdata.Scan0;

	HBITMAP hBitmap = CreateBitmapIndirect(&bm);
	bitmap_ptr->UnlockBits(&bmpdata);
	return hBitmap;
}

void GdiRawBitmap::FinalRelease()
{
	if (m_hdc)
	{
		SelectObject(m_hdc, m_hbmpold);
		DeleteDC(m_hdc);
		m_hdc = nullptr;
	}

	if (m_hbmp)
	{
		DeleteBitmap(m_hbmp);
		m_hbmp = nullptr;
	}
}

STDMETHODIMP GdiRawBitmap::get__Handle(HDC* p)
{
	if (!m_hdc || !p) return E_POINTER;

	*p = m_hdc;
	return S_OK;
}

STDMETHODIMP GdiRawBitmap::get_Height(UINT* p)
{
	if (!m_hdc || !p) return E_POINTER;

	*p = m_height;
	return S_OK;
}

STDMETHODIMP GdiRawBitmap::get_Width(UINT* p)
{
	if (!m_hdc || !p) return E_POINTER;

	*p = m_width;
	return S_OK;
}
