#pragma once
#include "script_interface.h"

class MenuObj : public IDisposableImpl4<IMenuObj>
{
protected:
	MenuObj(HWND wnd_parent);
	~MenuObj();

	void FinalRelease() override;

	HMENU m_hMenu = nullptr;
	HWND m_wnd_parent = nullptr;
	bool m_has_detached = false;

public:
	STDMETHODIMP get__ID(HMENU* p) override;
	STDMETHODIMP AppendMenuItem(UINT flags, UINT item_id, BSTR text) override;
	STDMETHODIMP AppendMenuSeparator() override;
	STDMETHODIMP AppendTo(IMenuObj* parent, UINT flags, BSTR text) override;
	STDMETHODIMP CheckMenuItem(UINT item_id, VARIANT_BOOL check) override;
	STDMETHODIMP CheckMenuRadioItem(UINT first, UINT last, UINT selected) override;
	STDMETHODIMP TrackPopupMenu(int x, int y, UINT flags, UINT* p) override;
};
