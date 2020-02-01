#pragma once
#include "script_interface.h"

class UiSelectionHolder : public IDisposableImpl4<IUiSelectionHolder>
{
protected:
	UiSelectionHolder(const ui_selection_holder::ptr& holder);
	~UiSelectionHolder();

	void FinalRelease() override;

	ui_selection_holder::ptr m_holder;

public:
	STDMETHODIMP SetPlaylistSelectionTracking() override;
	STDMETHODIMP SetPlaylistTracking() override;
	STDMETHODIMP SetSelection(IMetadbHandleList* handles) override;
};
