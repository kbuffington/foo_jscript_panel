#pragma once
#include "script_interface.h"

class GdiBitmap : public GdiObj<IGdiBitmap, Gdiplus::Bitmap>
{
protected:
	GdiBitmap(Gdiplus::Bitmap* p);

public:
	STDMETHODIMP ApplyAlpha(BYTE alpha, IGdiBitmap** pp) override;
	STDMETHODIMP ApplyMask(IGdiBitmap* mask, VARIANT_BOOL* p) override;
	STDMETHODIMP Clone(float x, float y, float w, float h, IGdiBitmap** pp) override;
	STDMETHODIMP CreateRawBitmap(IGdiRawBitmap** pp) override;
	STDMETHODIMP GetColourSchemeJSON(UINT count, BSTR* p) override;
	STDMETHODIMP GetGraphics(IGdiGraphics** pp) override;
	STDMETHODIMP ReleaseGraphics(IGdiGraphics* p) override;
	STDMETHODIMP Resize(UINT w, UINT h, int interpolationMode, IGdiBitmap** pp) override;
	STDMETHODIMP RotateFlip(UINT mode) override;
	STDMETHODIMP SaveAs(BSTR path, BSTR format, VARIANT_BOOL* p) override;
	STDMETHODIMP StackBlur(BYTE radius) override;
	STDMETHODIMP get_Height(UINT* p) override;
	STDMETHODIMP get_Width(UINT* p) override;
};
