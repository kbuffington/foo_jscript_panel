#include "stdafx.h"
#include "Gdi.h"
#include "helpers.h"

Gdi::Gdi() {}
Gdi::~Gdi() {}

STDMETHODIMP Gdi::CreateImage(int w, int h, IGdiBitmap** pp)
{
	if (!pp) return E_POINTER;

	*pp = nullptr;
	auto img = new Gdiplus::Bitmap(w, h, PixelFormat32bppPARGB);
	if (helpers::ensure_gdiplus_object(img))
	{
		*pp = new com_object_impl_t<GdiBitmap>(img);
	}
	else
	{
		if (img) delete img;
		img = nullptr;
	}

	return S_OK;
}

STDMETHODIMP Gdi::Font(BSTR name, float pxSize, int style, IGdiFont** pp)
{
	if (!pp) return E_POINTER;

	*pp = nullptr;
	auto font = new Gdiplus::Font(name, pxSize, style, Gdiplus::UnitPixel);
	if (helpers::ensure_gdiplus_object(font))
	{
		const HFONT hFont = helpers::create_font(name, pxSize, style);
		*pp = new com_object_impl_t<GdiFont>(font, hFont);
	}
	else
	{
		if (font) delete font;
		font = nullptr;
	}

	return S_OK;
}

STDMETHODIMP Gdi::Image(BSTR path, IGdiBitmap** pp)
{
	if (!pp) return E_POINTER;

	*pp = helpers::load_image(path);
	return S_OK;
}

STDMETHODIMP Gdi::LoadImageAsync(UINT window_id, BSTR path, UINT* p)
{
	if (!p) return E_POINTER;

	auto task = new helpers::load_image_async(reinterpret_cast<HWND>(window_id), path);
	simple_thread_pool::instance().enqueue(task);

	*p = reinterpret_cast<UINT_PTR>(task);
	return S_OK;
}
