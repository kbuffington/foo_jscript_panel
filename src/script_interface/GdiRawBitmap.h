#pragma once
#include "script_interface.h"

class GdiRawBitmap : public IDisposableImpl4<IGdiRawBitmap>
{
protected:
	GdiRawBitmap(Gdiplus::Bitmap* p_bmp);
	~GdiRawBitmap();

	HBITMAP CreateHBITMAP(Gdiplus::Bitmap* bitmap_ptr);
	void FinalRelease() override;

	HBITMAP m_hbmp, m_hbmpold;
	HDC m_hdc;
	t_size m_height, m_width;

public:
	STDMETHODIMP get__Handle(HDC* p) override;
	STDMETHODIMP get_Height(UINT* p) override;
	STDMETHODIMP get_Width(UINT* p) override;
};
