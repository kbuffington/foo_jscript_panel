#include "stdafx.h"
#include "GdiBitmap.h"
#include "helpers.h"
#include "kmeans.h"
#include "stackblur.h"

GdiBitmap::GdiBitmap(Gdiplus::Bitmap* p) : GdiObj<IGdiBitmap, Gdiplus::Bitmap>(p) {}

STDMETHODIMP GdiBitmap::ApplyAlpha(BYTE alpha, IGdiBitmap** pp)
{
	if (!m_ptr || !pp) return E_POINTER;

	const auto width = m_ptr->GetWidth();
	const auto height = m_ptr->GetHeight();
	auto out = new Gdiplus::Bitmap(width, height, PixelFormat32bppPARGB);
	Gdiplus::Graphics g(out);
	const Gdiplus::Rect rect(0, 0, width, height);

	Gdiplus::ImageAttributes ia;
	Gdiplus::ColorMatrix cm = { 0.f };
	cm.m[0][0] = cm.m[1][1] = cm.m[2][2] = cm.m[4][4] = 1.f;
	cm.m[3][3] = static_cast<float>(alpha) / UCHAR_MAX;
	ia.SetColorMatrix(&cm);

	g.DrawImage(m_ptr, rect, 0, 0, width, height, Gdiplus::UnitPixel, &ia);

	*pp = new com_object_impl_t<GdiBitmap>(out);
	return S_OK;
}

STDMETHODIMP GdiBitmap::ApplyMask(IGdiBitmap* image, VARIANT_BOOL* p)
{
	if (!m_ptr || !p) return E_POINTER;

	*p = VARIANT_FALSE;
	Gdiplus::Bitmap* bitmap_mask = nullptr;
	GET_PTR(image, bitmap_mask);

	if (bitmap_mask->GetHeight() != m_ptr->GetHeight() || bitmap_mask->GetWidth() != m_ptr->GetWidth())
	{
		return E_INVALIDARG;
	}

	const Gdiplus::Rect rect(0, 0, m_ptr->GetWidth(), m_ptr->GetHeight());
	Gdiplus::BitmapData bmpdata_mask = { 0 };
	Gdiplus::BitmapData bmpdata_dst = { 0 };

	auto status = bitmap_mask->LockBits(&rect, Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, &bmpdata_mask);
	if (status == Gdiplus::Ok)
	{
		status = m_ptr->LockBits(&rect, Gdiplus::ImageLockModeRead | Gdiplus::ImageLockModeWrite, PixelFormat32bppARGB, &bmpdata_dst);
		if (status == Gdiplus::Ok)
		{
			size_t* mask = reinterpret_cast<size_t*>(bmpdata_mask.Scan0);
			size_t* dst = reinterpret_cast<size_t*>(bmpdata_dst.Scan0);
			const size_t* mask_end = mask + (rect.Width * rect.Height);
			size_t alpha;

			while (mask < mask_end)
			{
				alpha = (((~*mask & UCHAR_MAX) * (*dst >> 24)) << 16) & 0xff000000;
				*dst = alpha | (*dst & 0xffffff);
				++mask;
				++dst;
			}

			m_ptr->UnlockBits(&bmpdata_dst);
			*p = VARIANT_TRUE;
		}
		bitmap_mask->UnlockBits(&bmpdata_mask);
	}
	return S_OK;
}

STDMETHODIMP GdiBitmap::Clone(float x, float y, float w, float h, IGdiBitmap** pp)
{
	if (!m_ptr || !pp) return E_POINTER;

	*pp = nullptr;
	Gdiplus::Bitmap* img = m_ptr->Clone(x, y, w, h, PixelFormat32bppPARGB);
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

STDMETHODIMP GdiBitmap::CreateRawBitmap(IGdiRawBitmap** pp)
{
	if (!m_ptr || !pp) return E_POINTER;

	*pp = new com_object_impl_t<GdiRawBitmap>(m_ptr);
	return S_OK;
}

STDMETHODIMP GdiBitmap::GetColourSchemeJSON(UINT count, BSTR* p)
{
	if (!m_ptr || !p) return E_POINTER;

	const int w = std::min(static_cast<int>(m_ptr->GetWidth()), 220);
	const int h = std::min(static_cast<int>(m_ptr->GetHeight()), 220);
	pfc::ptrholder_t<Gdiplus::Bitmap> bitmap = new Gdiplus::Bitmap(w, h, PixelFormat32bppPARGB);
	Gdiplus::Graphics gb(bitmap.get_ptr());
	gb.SetInterpolationMode(Gdiplus::InterpolationMode::InterpolationModeHighQualityBilinear);
	gb.DrawImage(m_ptr, 0, 0, w, h);

	const Gdiplus::Rect rect(0, 0, w, h);
	Gdiplus::BitmapData bmpdata;
	if (bitmap->LockBits(&rect, Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, &bmpdata) != Gdiplus::Ok) return E_POINTER;

	const size_t colours_length = bmpdata.Width * bmpdata.Height;
	const size_t* colours = static_cast<const size_t*>(bmpdata.Scan0);
	std::map<size_t, int> colour_counters;

	for (size_t i = 0; i < colours_length; ++i)
	{
		BYTE r = (colours[i] >> 16) & UCHAR_MAX;
		BYTE g = (colours[i] >> 8) & UCHAR_MAX;
		BYTE b = (colours[i] & UCHAR_MAX);

		// We're reducing total colors from 2^24 to 2^15 by rounding each colour component value to multiples of 8.
		// First we need to check if the byte will overflow, and if so pin to 0xff, otherwise add 4 and round down.
		r = (r > 251) ? UCHAR_MAX : (r + 4) & 0xf8;
		g = (g > 251) ? UCHAR_MAX : (g + 4) & 0xf8;
		b = (b > 251) ? UCHAR_MAX : (b + 4) & 0xf8;

		++colour_counters[r << 16 | g << 8 | b];
	}

	bitmap->UnlockBits(&bmpdata);

	int id = 0;
	std::vector<KPoint> points;
	for (const auto& elem : colour_counters)
	{
		const BYTE r = (elem.first >> 16) & UCHAR_MAX;
		const BYTE g = (elem.first >> 8) & UCHAR_MAX;
		const BYTE b = elem.first & UCHAR_MAX;

		std::vector<size_t> values = { r, g, b };
		KPoint pt(id, values, elem.second);
		points.emplace_back(pt);
		id++;
	}

	KMeans kmeans(count, points.size());
	std::vector<Cluster> clusters = kmeans.run(points);

	std::sort(clusters.begin(), clusters.end(), [](const Cluster& a, const Cluster& b)
		{
			return a.get_total_points() > b.get_total_points();
		});

	json j = json::array();

	if (count > clusters.size()) count = clusters.size();
	for (size_t i = 0; i < count; ++i)
	{
		double frequency = clusters[i].get_total_points() / static_cast<double>(colours_length);

		j.push_back(
		{
			{ "col", clusters[i].get_colour() },
			{ "freq", frequency }
		});
	}
	*p = TO_BSTR((j.dump()).c_str());
	return S_OK;
}

STDMETHODIMP GdiBitmap::GetGraphics(IGdiGraphics** pp)
{
	if (!m_ptr || !pp) return E_POINTER;

	auto g = new Gdiplus::Graphics(m_ptr);
	*pp = new com_object_impl_t<GdiGraphics>();
	(*pp)->put__ptr(g);
	return S_OK;
}

STDMETHODIMP GdiBitmap::ReleaseGraphics(IGdiGraphics* p)
{
	if (p)
	{
		Gdiplus::Graphics* g = nullptr;
		GET_PTR(p, g);
		p->put__ptr(nullptr);
		if (g) delete g;
	}

	return S_OK;
}

STDMETHODIMP GdiBitmap::Resize(UINT w, UINT h, int interpolationMode, IGdiBitmap** pp)
{
	if (!m_ptr || !pp) return E_POINTER;

	auto bitmap = new Gdiplus::Bitmap(w, h, PixelFormat32bppPARGB);
	Gdiplus::Graphics g(bitmap);
	g.SetInterpolationMode(static_cast<Gdiplus::InterpolationMode>(interpolationMode));
	g.DrawImage(m_ptr, 0, 0, w, h);
	*pp = new com_object_impl_t<GdiBitmap>(bitmap);
	return S_OK;
}

STDMETHODIMP GdiBitmap::RotateFlip(UINT mode)
{
	if (!m_ptr) return E_POINTER;

	m_ptr->RotateFlip(static_cast<Gdiplus::RotateFlipType>(mode));
	return S_OK;
}

STDMETHODIMP GdiBitmap::SaveAs(BSTR path, BSTR format, VARIANT_BOOL* p)
{
	if (!m_ptr || !p) return E_POINTER;

	*p = VARIANT_FALSE;
	CLSID clsid_encoder;
	if (helpers::get_encoder_clsid(format, &clsid_encoder))
	{
		m_ptr->Save(path, &clsid_encoder);
		*p = TO_VARIANT_BOOL(m_ptr->GetLastStatus() == Gdiplus::Ok);
	}
	return S_OK;
}

STDMETHODIMP GdiBitmap::StackBlur(BYTE radius)
{
	if (!m_ptr) return E_POINTER;

	stack_blur_filter(*m_ptr, radius);
	return S_OK;
}

STDMETHODIMP GdiBitmap::get_Height(UINT* p)
{
	if (!m_ptr || !p) return E_POINTER;

	*p = m_ptr->GetHeight();
	return S_OK;
}

STDMETHODIMP GdiBitmap::get_Width(UINT* p)
{
	if (!m_ptr || !p) return E_POINTER;

	*p = m_ptr->GetWidth();
	return S_OK;
}
