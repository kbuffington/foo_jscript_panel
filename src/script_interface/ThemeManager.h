#pragma once
#include "script_interface.h"

class ThemeManager : public IDisposableImpl4<IThemeManager>
{
protected:
	ThemeManager(HWND hwnd, BSTR classlist);
	~ThemeManager();

	void FinalRelease() override;

	HTHEME m_theme = nullptr;
	int m_partid = 0;
	int m_stateid = 0;

public:
	STDMETHODIMP DrawThemeBackground(IGdiGraphics* gr, int x, int y, int w, int h, int clip_x, int clip_y, int clip_w, int clip_h) override;
	STDMETHODIMP IsThemePartDefined(int partid, int stateid, VARIANT_BOOL* p) override;
	STDMETHODIMP SetPartAndStateID(int partid, int stateid) override;
};
