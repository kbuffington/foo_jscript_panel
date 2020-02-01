#include "stdafx.h"
#include "DropSourceAction.h"

DropSourceAction::DropSourceAction() {}

DropSourceAction::~DropSourceAction() {}

void DropSourceAction::FinalRelease() {}

STDMETHODIMP DropSourceAction::get_Effect(UINT* p)
{
	if (!p) return E_POINTER;

	*p = m_effect;
	return S_OK;
}

STDMETHODIMP DropSourceAction::put_Base(UINT base)
{
	m_base = base;
	return S_OK;
}

STDMETHODIMP DropSourceAction::put_Effect(UINT effect)
{
	m_effect = effect;
	return S_OK;
}

STDMETHODIMP DropSourceAction::put_Playlist(UINT id)
{
	m_playlist_idx = id;
	return S_OK;
}

STDMETHODIMP DropSourceAction::put_ToSelect(VARIANT_BOOL select)
{
	m_to_select = select != VARIANT_FALSE;
	return S_OK;
}
