#pragma once
#include "script_interface.h"

class PlaybackQueueItem : public IDisposableImpl4<IPlaybackQueueItem>
{
protected:
	PlaybackQueueItem();
	PlaybackQueueItem(const t_playback_queue_item& playbackQueueItem);
	~PlaybackQueueItem();

	void FinalRelease() override;

	t_playback_queue_item m_playback_queue_item;

public:
	STDMETHODIMP get__ptr(void** pp) override;
	STDMETHODIMP get_Handle(IMetadbHandle** pp) override;
	STDMETHODIMP get_PlaylistIndex(int* p) override;
	STDMETHODIMP get_PlaylistItemIndex(int* p) override;
};
