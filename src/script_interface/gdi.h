#pragma once
#include "script_interface.h"

class Gdi : public IDispatchImpl3<IGdi>
{
protected:
	Gdi();
	virtual ~Gdi();

public:
	STDMETHODIMP CreateImage(int w, int h, IGdiBitmap** pp) override;
	STDMETHODIMP Font(BSTR name, float pxSize, int style, IGdiFont** pp) override;
	STDMETHODIMP Image(BSTR path, IGdiBitmap** pp) override;
	STDMETHODIMP LoadImageAsync(UINT window_id, BSTR path, UINT* p) override;
};
