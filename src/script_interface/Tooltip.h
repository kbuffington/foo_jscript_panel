#pragma once
#include "script_interface.h"

struct panel_tooltip_param
{
	panel_tooltip_param() : font_name(nullptr), tooltip_hwnd(nullptr), font_size(0.f), font_style(0) {}

	BSTR font_name;
	CWindow tooltip_hwnd;
	float font_size;
	int font_style;
};

using panel_tooltip_param_ptr = std::shared_ptr<panel_tooltip_param>;

class Tooltip : public IDisposableImpl4<ITooltip>
{
protected:
	Tooltip(CWindow wndparent, const panel_tooltip_param_ptr& param_ptr);
	~Tooltip();

	void FinalRelease() override;

	BSTR m_tip_buffer;
	CWindow m_wndparent;
	CWindow m_wndtooltip;
	TOOLINFO m_ti;
	panel_tooltip_param_ptr m_panel_tooltip_param_ptr;

public:
	STDMETHODIMP Activate() override;
	STDMETHODIMP Deactivate() override;
	STDMETHODIMP GetDelayTime(int type, int* p) override;
	STDMETHODIMP SetDelayTime(int type, int time) override;
	STDMETHODIMP SetMaxWidth(int width) override;
	STDMETHODIMP TrackPosition(int x, int y) override;
	STDMETHODIMP get_Text(BSTR* p) override;
	STDMETHODIMP put_Text(BSTR text) override;
	STDMETHODIMP put_TrackActivate(VARIANT_BOOL activate) override;
};
