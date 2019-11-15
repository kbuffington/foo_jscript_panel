#pragma once
#include "script_interface.h"

class GdiGraphics : public GdiObj<IGdiGraphics, Gdiplus::Graphics>
{
protected:
	GdiGraphics();

	static void GetRoundRectPath(Gdiplus::GraphicsPath& gp, const Gdiplus::RectF& rect, float arc_width, float arc_height);
	static void SetFormat(int flags, Gdiplus::StringFormat& out);

public:
	STDMETHODIMP Dispose() override
	{
		return E_NOTIMPL;
	}

	STDMETHODIMP put__ptr(void* p) override;
	STDMETHODIMP CalcTextHeight(BSTR str, IGdiFont* font, UINT* p) override;
	STDMETHODIMP CalcTextWidth(BSTR str, IGdiFont* font, UINT* p) override;
	STDMETHODIMP DrawEllipse(float x, float y, float w, float h, float line_width, LONGLONG colour) override;
	STDMETHODIMP DrawImage(IGdiBitmap* image, float dstX, float dstY, float dstW, float dstH, float srcX, float srcY, float srcW, float srcH, float angle, BYTE alpha) override;
	STDMETHODIMP DrawLine(float x1, float y1, float x2, float y2, float line_width, LONGLONG colour) override;
	STDMETHODIMP DrawPolygon(LONGLONG colour, float line_width, VARIANT points) override;
	STDMETHODIMP DrawRect(float x, float y, float w, float h, float line_width, LONGLONG colour) override;
	STDMETHODIMP DrawRoundRect(float x, float y, float w, float h, float arc_width, float arc_height, float line_width, LONGLONG colour) override;
	STDMETHODIMP DrawString(BSTR str, IGdiFont* font, LONGLONG colour, float x, float y, float w, float h, int flags) override;
	STDMETHODIMP EstimateLineWrap(BSTR str, IGdiFont* font, int max_width, VARIANT* p) override;
	STDMETHODIMP FillEllipse(float x, float y, float w, float h, LONGLONG colour) override;
	STDMETHODIMP FillGradRect(float x, float y, float w, float h, float angle, LONGLONG colour1, LONGLONG colour2, float focus) override;
	STDMETHODIMP FillPolygon(LONGLONG colour, int fillmode, VARIANT points) override;
	STDMETHODIMP FillRoundRect(float x, float y, float w, float h, float arc_width, float arc_height, LONGLONG colour) override;
	STDMETHODIMP FillSolidRect(float x, float y, float w, float h, LONGLONG colour) override;
	STDMETHODIMP GdiAlphaBlend(IGdiRawBitmap* bitmap, int dstX, int dstY, int dstW, int dstH, int srcX, int srcY, int srcW, int srcH, BYTE alpha) override;
	STDMETHODIMP GdiDrawBitmap(IGdiRawBitmap* bitmap, int dstX, int dstY, int dstW, int dstH, int srcX, int srcY, int srcW, int srcH) override;
	STDMETHODIMP GdiDrawText(BSTR str, IGdiFont* font, LONGLONG colour, int x, int y, int w, int h, int format) override;
	STDMETHODIMP MeasureString(BSTR str, IGdiFont* font, float x, float y, float w, float h, int flags, IMeasureStringInfo** pp) override;
	STDMETHODIMP SetInterpolationMode(int mode) override;
	STDMETHODIMP SetSmoothingMode(int mode) override;
	STDMETHODIMP SetTextRenderingHint(UINT mode) override;
};
