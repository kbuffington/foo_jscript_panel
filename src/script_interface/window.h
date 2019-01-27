#pragma once
#include "script_interface.h"

class host_comm;

class Window : public IDispatchImpl3<IWindow>
{
protected:
	Window(host_comm* p);
	virtual ~Window();

public:
	STDMETHODIMP ClearInterval(UINT intervalID);
	STDMETHODIMP ClearTimeout(UINT timeoutID);
	STDMETHODIMP CreatePopupMenu(IMenuObj** pp);
	STDMETHODIMP CreateThemeManager(BSTR classid, IThemeManager** pp);
	STDMETHODIMP CreateTooltip(BSTR name, float pxSize, int style, ITooltip** pp);
	STDMETHODIMP GetColourCUI(UINT type, int* p);
	STDMETHODIMP GetColourDUI(UINT type, int* p);
	STDMETHODIMP GetFontCUI(UINT type, IGdiFont** pp);
	STDMETHODIMP GetFontDUI(UINT type, IGdiFont** pp);
	STDMETHODIMP GetProperty(BSTR name, VARIANT defaultval, VARIANT* p);
	STDMETHODIMP NotifyOthers(BSTR name, VARIANT info);
	STDMETHODIMP Reload();
	STDMETHODIMP Repaint(VARIANT_BOOL force);
	STDMETHODIMP RepaintRect(LONG x, LONG y, LONG w, LONG h, VARIANT_BOOL force);
	STDMETHODIMP SetCursor(UINT id);
	STDMETHODIMP SetInterval(IDispatch* func, int delay, UINT* p);
	STDMETHODIMP SetProperty(BSTR name, VARIANT val);
	STDMETHODIMP SetTimeout(IDispatch* func, int delay, UINT* p);
	STDMETHODIMP ShowConfigure();
	STDMETHODIMP ShowProperties();
	STDMETHODIMP get_Height(int* p);
	STDMETHODIMP get_ID(UINT* p);
	STDMETHODIMP get_InstanceType(UINT* p);
	STDMETHODIMP get_IsTransparent(VARIANT_BOOL* p);
	STDMETHODIMP get_IsVisible(VARIANT_BOOL* p);
	STDMETHODIMP get_MaxHeight(UINT* p);
	STDMETHODIMP get_MaxWidth(UINT* p);
	STDMETHODIMP get_MinHeight(UINT* p);
	STDMETHODIMP get_MinWidth(UINT* p);
	STDMETHODIMP get_Name(BSTR* p);
	STDMETHODIMP get_Width(int* p);
	STDMETHODIMP put_MaxHeight(UINT height);
	STDMETHODIMP put_MaxWidth(UINT width);
	STDMETHODIMP put_MinHeight(UINT height);
	STDMETHODIMP put_MinWidth(UINT width);

private:
	host_comm* m_host;
};
