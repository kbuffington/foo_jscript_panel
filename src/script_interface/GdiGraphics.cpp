#include "stdafx.h"
#include "GdiGraphics.h"
#include "helpers.h"

GdiGraphics::GdiGraphics() : GdiObj<IGdiGraphics, Gdiplus::Graphics>(nullptr) {}

void GdiGraphics::GetRoundRectPath(Gdiplus::GraphicsPath& gp, const Gdiplus::RectF& rect, float arc_width, float arc_height)
{
	const float arc_dia_w = arc_width * 2;
	const float arc_dia_h = arc_height * 2;
	Gdiplus::RectF corner(rect.X, rect.Y, arc_dia_w, arc_dia_h);

	gp.Reset();

	gp.AddArc(corner, 180, 90);

	corner.X += (rect.Width - arc_dia_w);
	gp.AddArc(corner, 270, 90);

	corner.Y += (rect.Height - arc_dia_h);
	gp.AddArc(corner, 0, 90);

	corner.X -= (rect.Width - arc_dia_w);
	gp.AddArc(corner, 90, 90);

	gp.CloseFigure();
}

void GdiGraphics::SetFormat(int flags, Gdiplus::StringFormat& fmt)
{
	if (flags != 0)
	{
		fmt.SetAlignment(static_cast<Gdiplus::StringAlignment>((flags >> 28) & 0x3));
		fmt.SetLineAlignment(static_cast<Gdiplus::StringAlignment>((flags >> 24) & 0x3));
		fmt.SetTrimming(static_cast<Gdiplus::StringTrimming>((flags >> 20) & 0x7));
		fmt.SetFormatFlags(static_cast<Gdiplus::StringFormatFlags>(flags & 0x7FFF));
	}
}

STDMETHODIMP GdiGraphics::put__ptr(void* p)
{
	m_ptr = reinterpret_cast<Gdiplus::Graphics*>(p);
	return S_OK;
}

STDMETHODIMP GdiGraphics::CalcTextHeight(BSTR str, IGdiFont* font, UINT* p)
{
	if (!m_ptr || !p) return E_POINTER;

	HFONT hFont = nullptr;
	font->get__HFont(reinterpret_cast<UINT*>(&hFont));
	HDC dc = m_ptr->GetHDC();

	{
		SelectObjectScope scope(dc, hFont);
		*p = helpers::get_text_height(dc, str, SysStringLen(str));
	}

	m_ptr->ReleaseHDC(dc);
	return S_OK;
}

STDMETHODIMP GdiGraphics::CalcTextWidth(BSTR str, IGdiFont* font, UINT* p)
{
	if (!m_ptr || !p) return E_POINTER;

	HFONT hFont = nullptr;
	font->get__HFont(reinterpret_cast<UINT*>(&hFont));
	HDC dc = m_ptr->GetHDC();

	{
		SelectObjectScope scope(dc, hFont);
		*p = helpers::get_text_width(dc, str, SysStringLen(str));
	}

	m_ptr->ReleaseHDC(dc);
	return S_OK;
}

STDMETHODIMP GdiGraphics::DrawEllipse(float x, float y, float w, float h, float line_width, __int64 colour)
{
	if (!m_ptr) return E_POINTER;

	Gdiplus::Pen pen(TO_UINT(colour), line_width);
	m_ptr->DrawEllipse(&pen, x, y, w, h);
	return S_OK;
}

STDMETHODIMP GdiGraphics::DrawImage(IGdiBitmap* image, float dstX, float dstY, float dstW, float dstH, float srcX, float srcY, float srcW, float srcH, float angle, BYTE alpha)
{
	if (!m_ptr) return E_POINTER;

	Gdiplus::Bitmap* img = nullptr;
	GET_PTR(image, img)

	const Gdiplus::RectF rect(dstX, dstY, dstW, dstH);
	Gdiplus::Matrix old_m;

	if (angle != 0.f)
	{
		const Gdiplus::PointF pt(dstX + dstW / 2, dstY + dstH / 2);
		Gdiplus::Matrix m;
		m.RotateAt(angle, pt);
		m_ptr->GetTransform(&old_m);
		m_ptr->SetTransform(&m);
	}

	if (alpha < UCHAR_MAX)
	{
		Gdiplus::ImageAttributes ia;
		Gdiplus::ColorMatrix cm = { 0.f };
		cm.m[0][0] = cm.m[1][1] = cm.m[2][2] = cm.m[4][4] = 1.f;
		cm.m[3][3] = static_cast<float>(alpha) / UCHAR_MAX;
		ia.SetColorMatrix(&cm);

		m_ptr->DrawImage(img, rect, srcX, srcY, srcW, srcH, Gdiplus::UnitPixel, &ia);
	}
	else
	{
		m_ptr->DrawImage(img, rect, srcX, srcY, srcW, srcH, Gdiplus::UnitPixel);
	}

	if (angle != 0.f)
	{
		m_ptr->SetTransform(&old_m);
	}

	return S_OK;
}

STDMETHODIMP GdiGraphics::DrawLine(float x1, float y1, float x2, float y2, float line_width, __int64 colour)
{
	if (!m_ptr) return E_POINTER;

	Gdiplus::Pen pen(TO_UINT(colour), line_width);
	m_ptr->DrawLine(&pen, x1, y1, x2, y2);
	return S_OK;
}

STDMETHODIMP GdiGraphics::DrawPolygon(__int64 colour, float line_width, VARIANT points)
{
	if (!m_ptr) return E_POINTER;

	com_array helper;
	std::vector<Gdiplus::PointF> point_array;
	if (!helper.convert(points, point_array)) return E_INVALIDARG;

	Gdiplus::Pen pen(TO_UINT(colour), line_width);
	m_ptr->DrawPolygon(&pen, point_array.data(), point_array.size());
	return S_OK;
}

STDMETHODIMP GdiGraphics::DrawRect(float x, float y, float w, float h, float line_width, __int64 colour)
{
	if (!m_ptr) return E_POINTER;

	Gdiplus::Pen pen(TO_UINT(colour), line_width);
	m_ptr->DrawRectangle(&pen, x, y, w, h);
	return S_OK;
}

STDMETHODIMP GdiGraphics::DrawRoundRect(float x, float y, float w, float h, float arc_width, float arc_height, float line_width, __int64 colour)
{
	if (!m_ptr) return E_POINTER;

	if (2 * arc_width > w || 2 * arc_height > h) return E_INVALIDARG;

	Gdiplus::Pen pen(TO_UINT(colour), line_width);
	Gdiplus::GraphicsPath gp;
	const Gdiplus::RectF rect(x, y, w, h);
	GetRoundRectPath(gp, rect, arc_width, arc_height);
	pen.SetStartCap(Gdiplus::LineCapRound);
	pen.SetEndCap(Gdiplus::LineCapRound);
	m_ptr->DrawPath(&pen, &gp);
	return S_OK;
}

STDMETHODIMP GdiGraphics::DrawString(BSTR str, IGdiFont* font, __int64 colour, float x, float y, float w, float h, int flags)
{
	if (!m_ptr) return E_POINTER;

	Gdiplus::Font* fn = nullptr;
	GET_PTR(font, fn)

	Gdiplus::StringFormat fmt(Gdiplus::StringFormat::GenericTypographic());
	SetFormat(flags, fmt);

	Gdiplus::SolidBrush br(TO_UINT(colour));
	m_ptr->DrawString(str, -1, fn, Gdiplus::RectF(x, y, w, h), &fmt, &br);
	return S_OK;
}

STDMETHODIMP GdiGraphics::EstimateLineWrap(BSTR str, IGdiFont* font, int max_width, VARIANT* p)
{
	if (!m_ptr || !p) return E_POINTER;

	helpers::wrapped_item_vec result;

	HFONT hFont = nullptr;
	font->get__HFont(reinterpret_cast<UINT*>(&hFont));
	HDC dc = m_ptr->GetHDC();

	{
		SelectObjectScope scope(dc, hFont);
		helpers::estimate_line_wrap(dc, str, max_width, result);
	}

	m_ptr->ReleaseHDC(dc);

	const size_t count = result.size();
	com_array helper;
	if (!helper.create(count * 2)) return E_OUTOFMEMORY;

	for (size_t i = 0; i < count; ++i)
	{
		_variant_t var1, var2;
		var1.vt = VT_BSTR;
		var1.bstrVal = result[i].text;
		var2.vt = VT_I4;
		var2.lVal = result[i].width;
		if (!helper.put_item(i * 2, var1)) return E_OUTOFMEMORY;
		if (!helper.put_item((i * 2) + 1, var2)) return E_OUTOFMEMORY;
	}
	p->vt = VT_ARRAY | VT_VARIANT;
	p->parray = helper.get_ptr();
	return S_OK;
}

STDMETHODIMP GdiGraphics::FillEllipse(float x, float y, float w, float h, __int64 colour)
{
	if (!m_ptr) return E_POINTER;

	Gdiplus::SolidBrush br(TO_UINT(colour));
	m_ptr->FillEllipse(&br, x, y, w, h);
	return S_OK;
}

STDMETHODIMP GdiGraphics::FillGradRect(float x, float y, float w, float h, float angle, __int64 colour1, __int64 colour2, float focus)
{
	if (!m_ptr) return E_POINTER;

	const Gdiplus::RectF rect(x, y, w, h);
	Gdiplus::LinearGradientBrush brush(rect, TO_UINT(colour1), TO_UINT(colour2), angle, TRUE);
	brush.SetBlendTriangularShape(focus);
	m_ptr->FillRectangle(&brush, rect);
	return S_OK;
}

STDMETHODIMP GdiGraphics::FillPolygon(__int64 colour, int fillmode, VARIANT points)
{
	if (!m_ptr) return E_POINTER;

	com_array helper;
	std::vector<Gdiplus::PointF> point_array;
	if (!helper.convert(points, point_array)) return E_INVALIDARG;

	Gdiplus::SolidBrush br(TO_UINT(colour));
	m_ptr->FillPolygon(&br, point_array.data(), point_array.size(), (Gdiplus::FillMode)fillmode);
	return S_OK;
}

STDMETHODIMP GdiGraphics::FillRoundRect(float x, float y, float w, float h, float arc_width, float arc_height, __int64 colour)
{
	if (!m_ptr) return E_POINTER;

	if (2 * arc_width > w || 2 * arc_height > h) return E_INVALIDARG;

	Gdiplus::SolidBrush br(TO_UINT(colour));
	Gdiplus::GraphicsPath gp;
	const Gdiplus::RectF rect(x, y, w, h);
	GetRoundRectPath(gp, rect, arc_width, arc_height);
	m_ptr->FillPath(&br, &gp);
	return S_OK;
}

STDMETHODIMP GdiGraphics::FillSolidRect(float x, float y, float w, float h, __int64 colour)
{
	if (!m_ptr) return E_POINTER;

	Gdiplus::SolidBrush brush(TO_UINT(colour));
	m_ptr->FillRectangle(&brush, x, y, w, h);
	return S_OK;
}

STDMETHODIMP GdiGraphics::GdiAlphaBlend(IGdiRawBitmap* bitmap, int dstX, int dstY, int dstW, int dstH, int srcX, int srcY, int srcW, int srcH, BYTE alpha)
{
	if (!m_ptr) return E_POINTER;

	HDC src_dc = nullptr;
	bitmap->get__Handle(&src_dc);
	HDC dc = m_ptr->GetHDC();
	BLENDFUNCTION bf = { AC_SRC_OVER, 0, alpha, AC_SRC_ALPHA };

	::GdiAlphaBlend(dc, dstX, dstY, dstW, dstH, src_dc, srcX, srcY, srcW, srcH, bf);
	m_ptr->ReleaseHDC(dc);
	return S_OK;
}

STDMETHODIMP GdiGraphics::GdiDrawBitmap(IGdiRawBitmap* bitmap, int dstX, int dstY, int dstW, int dstH, int srcX, int srcY, int srcW, int srcH)
{
	if (!m_ptr) return E_POINTER;

	HDC src_dc = nullptr;
	bitmap->get__Handle(&src_dc);
	HDC dc = m_ptr->GetHDC();

	if (dstW == srcW && dstH == srcH)
	{
		BitBlt(dc, dstX, dstY, dstW, dstH, src_dc, srcX, srcY, SRCCOPY);
	}
	else
	{
		SetStretchBltMode(dc, HALFTONE);
		SetBrushOrgEx(dc, 0, 0, nullptr);
		StretchBlt(dc, dstX, dstY, dstW, dstH, src_dc, srcX, srcY, srcW, srcH, SRCCOPY);
	}

	m_ptr->ReleaseHDC(dc);
	return S_OK;
}

STDMETHODIMP GdiGraphics::GdiDrawText(BSTR str, IGdiFont* font, __int64 colour, int x, int y, int w, int h, int format)
{
	if (!m_ptr) return E_POINTER;
	if (format & DT_MODIFYSTRING) return E_INVALIDARG;

	HFONT hFont = nullptr;
	font->get__HFont(reinterpret_cast<UINT*>(&hFont));
	HDC dc = m_ptr->GetHDC();

	{
		RECT rc = { x, y, x + w, y + h };
		SelectObjectScope scope(dc, hFont);

		SetTextColor(dc, helpers::convert_argb_to_colorref(TO_UINT(colour)));
		SetBkMode(dc, TRANSPARENT);
		SetTextAlign(dc, TA_LEFT | TA_TOP | TA_NOUPDATECP);

		if (format & DT_CALCRECT)
		{
			RECT rc_calc = { 0 }, rc_old = { 0 };

			memcpy(&rc_calc, &rc, sizeof(RECT));
			memcpy(&rc_old, &rc, sizeof(RECT));

			DrawText(dc, str, -1, &rc_calc, format);

			format &= ~DT_CALCRECT;

			if (format & DT_VCENTER)
			{
				rc.top = rc_old.top + ((RECT_CY(rc_old) - RECT_CY(rc_calc)) >> 1);
				rc.bottom = rc.top + RECT_CY(rc_calc);
			}
			else if (format & DT_BOTTOM)
			{
				rc.top = rc_old.bottom - RECT_CY(rc_calc);
			}
		}

		DRAWTEXTPARAMS dpt = { sizeof(DRAWTEXTPARAMS), 4, 0, 0, 0 };
		DrawTextEx(dc, str, -1, &rc, format, &dpt);
	}

	m_ptr->ReleaseHDC(dc);
	return S_OK;
}

STDMETHODIMP GdiGraphics::MeasureString(BSTR str, IGdiFont* font, float x, float y, float w, float h, int flags, IMeasureStringInfo** pp)
{
	if (!m_ptr || !pp) return E_POINTER;

	Gdiplus::Font* fn = nullptr;
	GET_PTR(font, fn)

	Gdiplus::StringFormat fmt(Gdiplus::StringFormat::GenericTypographic());
	SetFormat(flags, fmt);

	Gdiplus::RectF bound;
	int chars, lines;

	m_ptr->MeasureString(str, -1, fn, Gdiplus::RectF(x, y, w, h), &fmt, &bound, &chars, &lines);

	*pp = new com_object_impl_t<MeasureStringInfo>(bound.X, bound.Y, bound.Width, bound.Height, lines, chars);
	return S_OK;
}

STDMETHODIMP GdiGraphics::SetInterpolationMode(int mode)
{
	if (!m_ptr) return E_POINTER;

	m_ptr->SetInterpolationMode(static_cast<Gdiplus::InterpolationMode>(mode));
	return S_OK;
}

STDMETHODIMP GdiGraphics::SetSmoothingMode(int mode)
{
	if (!m_ptr) return E_POINTER;

	m_ptr->SetSmoothingMode(static_cast<Gdiplus::SmoothingMode>(mode));
	return S_OK;
}

STDMETHODIMP GdiGraphics::SetTextRenderingHint(UINT mode)
{
	if (!m_ptr) return E_POINTER;

	m_ptr->SetTextRenderingHint(static_cast<Gdiplus::TextRenderingHint>(mode));
	return S_OK;
}
