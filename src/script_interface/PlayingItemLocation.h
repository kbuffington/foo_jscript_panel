#pragma once
#include "script_interface.h"

class PlayingItemLocation : public IDispatchImpl3<IPlayingItemLocation>
{
protected:
	PlayingItemLocation(bool isValid, t_size playlistIndex, t_size playlistItemIndex);

	bool m_isValid;
	t_size m_playlistIndex;
	t_size m_playlistItemIndex;

public:
	STDMETHODIMP get_IsValid(VARIANT_BOOL* p) override;
	STDMETHODIMP get_PlaylistIndex(int* p) override;
	STDMETHODIMP get_PlaylistItemIndex(int* p) override;
};
