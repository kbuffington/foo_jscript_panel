#pragma once
#include "script_interface.h"

class PlayingItemLocation : public IDispatchImpl3<IPlayingItemLocation>
{
protected:
	PlayingItemLocation(bool isValid, size_t playlistIndex, size_t playlistItemIndex);

	bool m_isValid = false;
	size_t m_playlistIndex = SIZE_MAX;
	size_t m_playlistItemIndex = SIZE_MAX;

public:
	STDMETHODIMP get_IsValid(VARIANT_BOOL* p) override;
	STDMETHODIMP get_PlaylistIndex(int* p) override;
	STDMETHODIMP get_PlaylistItemIndex(int* p) override;
};
