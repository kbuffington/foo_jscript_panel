#pragma once
#include "script_interface.h"

class Plman : public IDispatchImpl3<IPlman>
{
protected:
	Plman();
	virtual ~Plman();

public:
	STDMETHODIMP AddItemToPlaybackQueue(IMetadbHandle* handle);
	STDMETHODIMP AddLocations(UINT playlistIndex, VARIANT locations, VARIANT_BOOL select);
	STDMETHODIMP AddPlaylistItemToPlaybackQueue(UINT playlistIndex, UINT playlistItemIndex);
	STDMETHODIMP ClearPlaylist(UINT playlistIndex);
	STDMETHODIMP ClearPlaylistSelection(UINT playlistIndex);
	STDMETHODIMP CreateAutoPlaylist(UINT playlistIndex, BSTR name, BSTR query, BSTR sort, UINT flags, int* p);
	STDMETHODIMP CreatePlaylist(UINT playlistIndex, BSTR name, UINT* p);
	STDMETHODIMP DuplicatePlaylist(UINT from, BSTR name, UINT* p);
	STDMETHODIMP EnsurePlaylistItemVisible(UINT playlistIndex, UINT playlistItemIndex);
	STDMETHODIMP ExecutePlaylistDefaultAction(UINT playlistIndex, UINT playlistItemIndex, VARIANT_BOOL* p);
	STDMETHODIMP FindOrCreatePlaylist(BSTR name, VARIANT_BOOL unlocked, UINT* p);
	STDMETHODIMP FindPlaybackQueueItemIndex(IMetadbHandle* handle, UINT playlistIndex, UINT playlistItemIndex, int* p);
	STDMETHODIMP FindPlaylist(BSTR name, int* p);
	STDMETHODIMP FlushPlaybackQueue();
	STDMETHODIMP GetPlaybackQueueContents(VARIANT* p);
	STDMETHODIMP GetPlaybackQueueHandles(IMetadbHandleList** pp);
	STDMETHODIMP GetPlayingItemLocation(IPlayingItemLocation** pp);
	STDMETHODIMP GetPlaylistFocusItemIndex(UINT playlistIndex, int* p);
	STDMETHODIMP GetPlaylistItems(UINT playlistIndex, IMetadbHandleList** pp);
	STDMETHODIMP GetPlaylistName(UINT playlistIndex, BSTR* p);
	STDMETHODIMP GetPlaylistSelectedItems(UINT playlistIndex, IMetadbHandleList** pp);
	STDMETHODIMP InsertPlaylistItems(UINT playlistIndex, UINT base, IMetadbHandleList* handles, VARIANT_BOOL select);
	STDMETHODIMP InsertPlaylistItemsFilter(UINT playlistIndex, UINT base, IMetadbHandleList* handles, VARIANT_BOOL select);
	STDMETHODIMP IsAutoPlaylist(UINT playlistIndex, VARIANT_BOOL* p);
	STDMETHODIMP IsPlaylistItemSelected(UINT playlistIndex, UINT playlistItemIndex, VARIANT_BOOL* p);
	STDMETHODIMP IsPlaylistLocked(UINT playlistIndex, VARIANT_BOOL* p);
	STDMETHODIMP MovePlaylist(UINT from, UINT to, VARIANT_BOOL* p);
	STDMETHODIMP MovePlaylistSelection(UINT playlistIndex, int delta, VARIANT_BOOL* p);
	STDMETHODIMP PlaylistItemCount(UINT playlistIndex, UINT* p);
	STDMETHODIMP RemoveItemFromPlaybackQueue(UINT index);
	STDMETHODIMP RemoveItemsFromPlaybackQueue(VARIANT affectedItems);
	STDMETHODIMP RemovePlaylist(UINT playlistIndex, VARIANT_BOOL* p);
	STDMETHODIMP RemovePlaylistSelection(UINT playlistIndex, VARIANT_BOOL crop);
	STDMETHODIMP RemovePlaylistSwitch(UINT playlistIndex, VARIANT_BOOL* p);
	STDMETHODIMP RenamePlaylist(UINT playlistIndex, BSTR name, VARIANT_BOOL* p);
	STDMETHODIMP SetActivePlaylistContext();
	STDMETHODIMP SetPlaylistFocusItem(UINT playlistIndex, UINT playlistItemIndex);
	STDMETHODIMP SetPlaylistFocusItemByHandle(UINT playlistIndex, IMetadbHandle* handle);
	STDMETHODIMP SetPlaylistSelection(UINT playlistIndex, VARIANT affectedItems, VARIANT_BOOL state);
	STDMETHODIMP SetPlaylistSelectionSingle(UINT playlistIndex, UINT playlistItemIndex, VARIANT_BOOL state);
	STDMETHODIMP ShowAutoPlaylistUI(UINT playlistIndex, VARIANT_BOOL* pp);
	STDMETHODIMP SortByFormat(UINT playlistIndex, BSTR pattern, VARIANT_BOOL selOnly, VARIANT_BOOL* p);
	STDMETHODIMP SortByFormatV2(UINT playlistIndex, BSTR pattern, int direction, VARIANT_BOOL* p);
	STDMETHODIMP SortPlaylistsByName(int direction);
	STDMETHODIMP UndoBackup(UINT playlistIndex);
	STDMETHODIMP get_ActivePlaylist(int* p);
	STDMETHODIMP get_PlaybackOrder(UINT* p);
	STDMETHODIMP get_PlayingPlaylist(int* p);
	STDMETHODIMP get_PlaylistCount(UINT* p);
	STDMETHODIMP get_PlaylistRecyclerManager(__interface IPlaylistRecyclerManager** pp);
	STDMETHODIMP put_ActivePlaylist(UINT playlistIndex);
	STDMETHODIMP put_PlaybackOrder(UINT order);
	STDMETHODIMP put_PlayingPlaylist(UINT playlistIndex);

private:
	IPlaylistRecyclerManagerPtr m_fbPlaylistRecyclerManager;
};
