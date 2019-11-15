#include "stdafx.h"
#include "PlayingItemLocation.h"

PlayingItemLocation::PlayingItemLocation(bool isValid, t_size playlistIndex, t_size playlistItemIndex) : m_isValid(isValid), m_playlistIndex(playlistIndex), m_playlistItemIndex(playlistItemIndex) {}

STDMETHODIMP PlayingItemLocation::get_IsValid(VARIANT_BOOL* p)
{
	if (!p) return E_POINTER;

	*p = TO_VARIANT_BOOL(m_isValid);
	return S_OK;
}

STDMETHODIMP PlayingItemLocation::get_PlaylistIndex(int* p)
{
	if (!p) return E_POINTER;

	*p = m_isValid ? m_playlistIndex : -1;
	return S_OK;
}

STDMETHODIMP PlayingItemLocation::get_PlaylistItemIndex(int* p)
{
	if (!p) return E_POINTER;

	*p = m_isValid ? m_playlistItemIndex : -1;
	return S_OK;
}

Profiler::Profiler(const char* p_name) : m_name(p_name)
{
	m_timer.start();
}
