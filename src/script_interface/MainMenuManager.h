#pragma once
#include "script_interface.h"

class MainMenuManager : public IDisposableImpl4<IMainMenuManager>
{
protected:
	MainMenuManager();
	~MainMenuManager();

	void FinalRelease() override;

	mainmenu_manager::ptr m_mm;

public:
	STDMETHODIMP BuildMenu(IMenuObj* p, UINT base_id) override;
	STDMETHODIMP ExecuteByID(UINT id, VARIANT_BOOL* p) override;
	STDMETHODIMP Init(BSTR root_name) override;
};
