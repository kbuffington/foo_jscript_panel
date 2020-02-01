#pragma once
#include "script_interface.h"

class ContextMenuManager : public IDisposableImpl4<IContextMenuManager>
{
protected:
	ContextMenuManager();
	~ContextMenuManager();

	void FinalRelease() override;

	contextmenu_manager::ptr m_cm;

public:
	STDMETHODIMP BuildMenu(IMenuObj* p, int base_id) override;
	STDMETHODIMP ExecuteByID(UINT id, VARIANT_BOOL* p) override;
	STDMETHODIMP InitContext(IMetadbHandleList* handles) override;
	STDMETHODIMP InitContextPlaylist() override;
	STDMETHODIMP InitNowPlaying() override;
};
