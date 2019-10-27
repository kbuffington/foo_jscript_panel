#include "stdafx.h"
#include "gdi.h"
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
		const HFONT hFont = CreateFont(
			-static_cast<int>(pxSize),
			0,
			0,
			0,
			(style & Gdiplus::FontStyleBold) ? FW_BOLD : FW_NORMAL,
			(style & Gdiplus::FontStyleItalic) ? TRUE : FALSE,
			(style & Gdiplus::FontStyleUnderline) ? TRUE : FALSE,
			(style & Gdiplus::FontStyleStrikeout) ? TRUE : FALSE,
			DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE,
			name);
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

	*p = reinterpret_cast<t_size>(task);
	return S_OK;
}
