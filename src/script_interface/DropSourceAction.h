#pragma once
#include "script_interface.h"

class DropSourceAction : public IDisposableImpl4<IDropSourceAction>
{
protected:
	DropSourceAction();
	~DropSourceAction();

	void FinalRelease() override;

public:
	void Reset()
	{
		m_playlist_idx = 0;
		m_base = 0;
		m_to_select = true;
		m_effect = DROPEFFECT_NONE;
	}

	STDMETHODIMP get_Effect(UINT* p) override;
	STDMETHODIMP put_Base(UINT base) override;
	STDMETHODIMP put_Effect(UINT effect) override;
	STDMETHODIMP put_Playlist(UINT id) override;
	STDMETHODIMP put_ToSelect(VARIANT_BOOL to_select) override;

	DWORD m_effect;
	bool m_to_select;
	size_t m_base;
	size_t m_playlist_idx;
};
