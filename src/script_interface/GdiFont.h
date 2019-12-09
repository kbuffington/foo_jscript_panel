#pragma once
#include "script_interface.h"

class GdiFont : public GdiObj<IGdiFont, Gdiplus::Font>
{
protected:
	GdiFont(Gdiplus::Font* p, HFONT hFont, bool managed = true);
	~GdiFont();

	void FinalRelease() override;

	HFONT m_hFont = nullptr;
	bool m_managed = true;

public:
	STDMETHODIMP get__HFont(UINT* p) override;
	STDMETHODIMP get_Height(UINT* p) override;
	STDMETHODIMP get_Name(BSTR* p) override;
	STDMETHODIMP get_Size(float* p) override;
	STDMETHODIMP get_Style(int* p) override;
};
