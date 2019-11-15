#include "stdafx.h"
#include "PlaybackQueueItem.h"

PlaybackQueueItem::PlaybackQueueItem() {}

PlaybackQueueItem::PlaybackQueueItem(const t_playback_queue_item& playbackQueueItem)
{
	m_playback_queue_item.m_handle = playbackQueueItem.m_handle;
	m_playback_queue_item.m_playlist = playbackQueueItem.m_playlist;
	m_playback_queue_item.m_item = playbackQueueItem.m_item;
}

PlaybackQueueItem::~PlaybackQueueItem() {}

void PlaybackQueueItem::FinalRelease()
{
	m_playback_queue_item.m_handle.release();
	m_playback_queue_item.m_playlist = 0;
	m_playback_queue_item.m_item = 0;
}

STDMETHODIMP PlaybackQueueItem::get__ptr(void** pp)
{
	if (!pp) return E_POINTER;

	*pp = &m_playback_queue_item;
	return S_OK;
}

STDMETHODIMP PlaybackQueueItem::get_Handle(IMetadbHandle** pp)
{
	if (!pp) return E_POINTER;

	*pp = new com_object_impl_t<MetadbHandle>(m_playback_queue_item.m_handle);
	return S_OK;
}

STDMETHODIMP PlaybackQueueItem::get_PlaylistIndex(int* p)
{
	if (!p) return E_POINTER;

	*p = m_playback_queue_item.m_playlist;
	return S_OK;
}

STDMETHODIMP PlaybackQueueItem::get_PlaylistItemIndex(int* p)
{
	if (!p) return E_POINTER;

	*p = m_playback_queue_item.m_item;
	return S_OK;
}
