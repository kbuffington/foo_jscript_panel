#pragma once
#include "script_interface.h"

_COM_SMARTPTR_TYPEDEF(IPlaylistRecyclerManager, __uuidof(IPlaylistRecyclerManager));

class Plman : public IDispatchImpl3<IPlman>
{
protected:
	Plman();
	~Plman();

public:
	STDMETHODIMP AddItemToPlaybackQueue(IMetadbHandle* handle) override;
	STDMETHODIMP AddLocations(UINT playlistIndex, VARIANT locations, VARIANT_BOOL select) override;
	STDMETHODIMP AddPlaylistItemToPlaybackQueue(UINT playlistIndex, UINT playlistItemIndex) override;
	STDMETHODIMP ClearPlaylist(UINT playlistIndex) override;
	STDMETHODIMP ClearPlaylistSelection(UINT playlistIndex) override;
	STDMETHODIMP CreateAutoPlaylist(UINT playlistIndex, BSTR name, BSTR query, BSTR sort, UINT flags, int* p) override;
	STDMETHODIMP CreatePlaylist(UINT playlistIndex, BSTR name, UINT* p) override;
	STDMETHODIMP DuplicatePlaylist(UINT from, BSTR name, UINT* p) override;
	STDMETHODIMP EnsurePlaylistItemVisible(UINT playlistIndex, UINT playlistItemIndex) override;
	STDMETHODIMP ExecutePlaylistDefaultAction(UINT playlistIndex, UINT playlistItemIndex, VARIANT_BOOL* p) override;
	STDMETHODIMP FindOrCreatePlaylist(BSTR name, VARIANT_BOOL unlocked, UINT* p) override;
	STDMETHODIMP FindPlaybackQueueItemIndex(IMetadbHandle* handle, UINT playlistIndex, UINT playlistItemIndex, int* p) override;
	STDMETHODIMP FindPlaylist(BSTR name, int* p) override;
	STDMETHODIMP FlushPlaybackQueue() override;
	STDMETHODIMP GetPlaybackQueueContents(VARIANT* p) override;
	STDMETHODIMP GetPlaybackQueueHandles(IMetadbHandleList** pp) override;
	STDMETHODIMP GetPlayingItemLocation(IPlayingItemLocation** pp) override;
	STDMETHODIMP GetPlaylistFocusItemIndex(UINT playlistIndex, int* p) override;
	STDMETHODIMP GetPlaylistItems(UINT playlistIndex, IMetadbHandleList** pp) override;
	STDMETHODIMP GetPlaylistName(UINT playlistIndex, BSTR* p) override;
	STDMETHODIMP GetPlaylistSelectedItems(UINT playlistIndex, IMetadbHandleList** pp) override;
	STDMETHODIMP InsertPlaylistItems(UINT playlistIndex, UINT base, IMetadbHandleList* handles, VARIANT_BOOL select) override;
	STDMETHODIMP InsertPlaylistItemsFilter(UINT playlistIndex, UINT base, IMetadbHandleList* handles, VARIANT_BOOL select) override;
	STDMETHODIMP IsAutoPlaylist(UINT playlistIndex, VARIANT_BOOL* p) override;
	STDMETHODIMP IsPlaylistItemSelected(UINT playlistIndex, UINT playlistItemIndex, VARIANT_BOOL* p) override;
	STDMETHODIMP IsPlaylistLocked(UINT playlistIndex, VARIANT_BOOL* p) override;
	STDMETHODIMP MovePlaylist(UINT from, UINT to, VARIANT_BOOL* p) override;
	STDMETHODIMP MovePlaylistSelection(UINT playlistIndex, int delta, VARIANT_BOOL* p) override;
	STDMETHODIMP PlaylistItemCount(UINT playlistIndex, UINT* p) override;
	STDMETHODIMP RemoveItemFromPlaybackQueue(UINT index) override;
	STDMETHODIMP RemoveItemsFromPlaybackQueue(VARIANT affectedItems) override;
	STDMETHODIMP RemovePlaylist(UINT playlistIndex, VARIANT_BOOL* p) override;
	STDMETHODIMP RemovePlaylistSelection(UINT playlistIndex, VARIANT_BOOL crop) override;
	STDMETHODIMP RemovePlaylistSwitch(UINT playlistIndex, VARIANT_BOOL* p) override;
	STDMETHODIMP RenamePlaylist(UINT playlistIndex, BSTR name, VARIANT_BOOL* p) override;
	STDMETHODIMP SetActivePlaylistContext() override;
	STDMETHODIMP SetPlaylistFocusItem(UINT playlistIndex, UINT playlistItemIndex) override;
	STDMETHODIMP SetPlaylistFocusItemByHandle(UINT playlistIndex, IMetadbHandle* handle) override;
	STDMETHODIMP SetPlaylistSelection(UINT playlistIndex, VARIANT affectedItems, VARIANT_BOOL state) override;
	STDMETHODIMP SetPlaylistSelectionSingle(UINT playlistIndex, UINT playlistItemIndex, VARIANT_BOOL state) override;
	STDMETHODIMP ShowAutoPlaylistUI(UINT playlistIndex, VARIANT_BOOL* pp) override;
	STDMETHODIMP SortByFormat(UINT playlistIndex, BSTR pattern, VARIANT_BOOL selOnly, VARIANT_BOOL* p) override;
	STDMETHODIMP SortByFormatV2(UINT playlistIndex, BSTR pattern, int direction, VARIANT_BOOL* p) override;
	STDMETHODIMP SortPlaylistsByName(int direction) override;
	STDMETHODIMP UndoBackup(UINT playlistIndex) override;
	STDMETHODIMP get_ActivePlaylist(int* p) override;
	STDMETHODIMP get_PlaybackOrder(UINT* p) override;
	STDMETHODIMP get_PlayingPlaylist(int* p) override;
	STDMETHODIMP get_PlaylistCount(UINT* p) override;
	STDMETHODIMP get_PlaylistRecyclerManager(__interface IPlaylistRecyclerManager** pp) override;
	STDMETHODIMP put_ActivePlaylist(UINT playlistIndex) override;
	STDMETHODIMP put_PlaybackOrder(UINT order) override;
	STDMETHODIMP put_PlayingPlaylist(UINT playlistIndex) override;

private:
	IPlaylistRecyclerManagerPtr m_fbPlaylistRecyclerManager;
};
