#pragma once
#include "script_interface.h"

class Gdi : public IDispatchImpl3<IGdi>
{
protected:
	Gdi();
	virtual ~Gdi();

public:
	STDMETHODIMP CreateImage(int w, int h, IGdiBitmap** pp);
	STDMETHODIMP Font(BSTR name, float pxSize, int style, IGdiFont** pp);
	STDMETHODIMP Image(BSTR path, IGdiBitmap** pp);
	STDMETHODIMP LoadImageAsync(UINT window_id, BSTR path, UINT* p);
};
