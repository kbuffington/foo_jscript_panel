#pragma once
#include "script_interface.h"

class PlayingItemLocation : public IDispatchImpl3<IPlayingItemLocation>
{
protected:
	PlayingItemLocation(bool isValid, size_t playlistIndex, size_t playlistItemIndex);

	bool m_isValid;
	size_t m_playlistIndex;
	size_t m_playlistItemIndex;

public:
	STDMETHODIMP get_IsValid(VARIANT_BOOL* p) override;
	STDMETHODIMP get_PlaylistIndex(int* p) override;
	STDMETHODIMP get_PlaylistItemIndex(int* p) override;
};
