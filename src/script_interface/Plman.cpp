#include "stdafx.h"
#include "helpers.h"
#include "plman.h"

Plman::Plman() {}
Plman::~Plman() {}

STDMETHODIMP Plman::AddItemToPlaybackQueue(IMetadbHandle* handle)
{
	metadb_handle* ptr = nullptr;
	GET_PTR(handle, ptr)

	playlist_manager::get()->queue_add_item(ptr);
	return S_OK;
}

STDMETHODIMP Plman::AddLocations(UINT playlistIndex, VARIANT locations, VARIANT_BOOL select)
{
	auto api = playlist_manager::get();
	if (playlistIndex < api->get_playlist_count() && !api->playlist_lock_is_present(playlistIndex))
	{
		helpers::com_array helper;
		pfc::string_list_impl list;
		if (!helper.convert(locations, list)) return E_INVALIDARG;

		playlist_incoming_item_filter_v2::get()->process_locations_async(
			list,
			playlist_incoming_item_filter_v2::op_flag_no_filter | playlist_incoming_item_filter_v2::op_flag_delay_ui,
			nullptr,
			nullptr,
			nullptr,
			fb2k::service_new<helpers::js_process_locations>(select != VARIANT_FALSE, api->playlist_get_item_count(playlistIndex), playlistIndex));

		return S_OK;
	}
	return E_INVALIDARG;
}

STDMETHODIMP Plman::AddPlaylistItemToPlaybackQueue(UINT playlistIndex, UINT playlistItemIndex)
{
	playlist_manager::get()->queue_add_item_playlist(playlistIndex, playlistItemIndex);
	return S_OK;
}

STDMETHODIMP Plman::ClearPlaylist(UINT playlistIndex)
{
	playlist_manager::get()->playlist_clear(playlistIndex);
	return S_OK;
}

STDMETHODIMP Plman::ClearPlaylistSelection(UINT playlistIndex)
{
	playlist_manager::get()->playlist_clear_selection(playlistIndex);
	return S_OK;
}

STDMETHODIMP Plman::CreateAutoPlaylist(UINT playlistIndex, BSTR name, BSTR query, BSTR sort, UINT flags, int* p)
{
	if (!p) return E_POINTER;

	search_filter_v2::ptr filter;
	auto uquery = string_utf8_from_wide(query);

	try
	{
		filter = search_filter_manager_v2::get()->create_ex(uquery, fb2k::service_new<completion_notify_dummy>(), search_filter_manager_v2::KFlagSuppressNotify);
	}
	catch (...)
	{
		*p = -1;
		return S_OK;
	}

	t_size pos;
	CreatePlaylist(playlistIndex, name, &pos);
	autoplaylist_manager::get()->add_client_simple(uquery, string_utf8_from_wide(sort), pos, flags);
	*p = pos;
	return S_OK;
}

STDMETHODIMP Plman::CreatePlaylist(UINT playlistIndex, BSTR name, UINT* p)
{
	if (!p) return E_POINTER;

	auto api = playlist_manager::get();
	auto uname = string_utf8_from_wide(name);

	if (uname.length())
	{
		*p = api->create_playlist(uname, uname.length(), playlistIndex);
	}
	else
	{
		*p = api->create_playlist_autoname(playlistIndex);
	}
	return S_OK;
}

STDMETHODIMP Plman::DuplicatePlaylist(UINT from, BSTR name, UINT* p)
{
	if (!p) return E_POINTER;

	auto api = playlist_manager_v4::get();

	if (from < api->get_playlist_count())
	{
		metadb_handle_list contents;
		api->playlist_get_all_items(from, contents);

		pfc::string8_fast uname = string_utf8_from_wide(name).get_ptr();
		if (uname.is_empty())
		{
			api->playlist_get_name(from, uname);
		}

		stream_reader_dummy dummy_reader;
		*p = api->create_playlist_ex(uname.get_ptr(), uname.get_length(), from + 1, contents, &dummy_reader, fb2k::noAbort);
		return S_OK;
	}
	return E_INVALIDARG;
}

STDMETHODIMP Plman::EnsurePlaylistItemVisible(UINT playlistIndex, UINT playlistItemIndex)
{
	playlist_manager::get()->playlist_ensure_visible(playlistIndex, playlistItemIndex);
	return S_OK;
}

STDMETHODIMP Plman::ExecutePlaylistDefaultAction(UINT playlistIndex, UINT playlistItemIndex, VARIANT_BOOL* p)
{
	if (!p) return E_POINTER;

	auto api = playlist_manager::get();
	if (playlistIndex < api->get_playlist_count() && playlistItemIndex < api->playlist_get_item_count(playlistIndex))
	{
		*p = TO_VARIANT_BOOL(api->playlist_execute_default_action(playlistIndex, playlistItemIndex));
		return S_OK;
	}
	return E_INVALIDARG;
}

STDMETHODIMP Plman::FindOrCreatePlaylist(BSTR name, VARIANT_BOOL unlocked, UINT* p)
{
	if (!p) return E_POINTER;

	auto api = playlist_manager::get();
	auto uname = string_utf8_from_wide(name);

	if (unlocked != VARIANT_FALSE)
	{
		*p = api->find_or_create_playlist_unlocked(uname);
	}
	else
	{
		*p = api->find_or_create_playlist(uname);
	}
	return S_OK;
}

STDMETHODIMP Plman::FindPlaybackQueueItemIndex(IMetadbHandle* handle, UINT playlistIndex, UINT playlistItemIndex, int* p)
{
	if (!p) return E_POINTER;

	metadb_handle* ptr = nullptr;
	GET_PTR(handle, ptr)

	t_playback_queue_item item;
	item.m_handle = ptr;
	item.m_playlist = playlistIndex;
	item.m_item = playlistItemIndex;
	*p = playlist_manager::get()->queue_find_index(item);
	return S_OK;
}

STDMETHODIMP Plman::FindPlaylist(BSTR name, int* p)
{
	if (!p) return E_POINTER;

	*p = playlist_manager::get()->find_playlist(string_utf8_from_wide(name));
	return S_OK;
}

STDMETHODIMP Plman::FlushPlaybackQueue()
{
	playlist_manager::get()->queue_flush();
	return S_OK;
}

STDMETHODIMP Plman::GetPlaybackQueueContents(VARIANT* p)
{
	if (!p) return E_POINTER;

	pfc::list_t<t_playback_queue_item> contents;
	playlist_manager::get()->queue_get_contents(contents);
	const LONG count = contents.get_count();
	helpers::com_array helper;
	if (!helper.create(count)) return E_OUTOFMEMORY;

	for (LONG i = 0; i < count; ++i)
	{
		_variant_t var;
		var.vt = VT_DISPATCH;
		var.pdispVal = new com_object_impl_t<PlaybackQueueItem>(contents[i]);

		if (!helper.put_item(i, var)) return E_OUTOFMEMORY;
	}

	p->vt = VT_ARRAY | VT_VARIANT;
	p->parray = helper.get_ptr();
	return S_OK;
}

STDMETHODIMP Plman::GetPlaybackQueueHandles(IMetadbHandleList** pp)
{
	if (!pp) return E_POINTER;

	pfc::list_t<t_playback_queue_item> contents;
	playlist_manager::get()->queue_get_contents(contents);
	const t_size count = contents.get_count();
	metadb_handle_list items;
	for (t_size i = 0; i < count; ++i)
	{
		items.add_item(contents[i].m_handle);
	}
	*pp = new com_object_impl_t<MetadbHandleList>(items);
	return S_OK;
}

STDMETHODIMP Plman::GetPlayingItemLocation(IPlayingItemLocation** pp)
{
	if (!pp) return E_POINTER;

	t_size playlistIndex;
	t_size playlistItemIndex;
	bool isValid = playlist_manager::get()->get_playing_item_location(&playlistIndex, &playlistItemIndex);
	*pp = new com_object_impl_t<PlayingItemLocation>(isValid, playlistIndex, playlistItemIndex);
	return S_OK;
}

STDMETHODIMP Plman::GetPlaylistFocusItemIndex(UINT playlistIndex, int* p)
{
	if (!p) return E_POINTER;

	*p = playlist_manager::get()->playlist_get_focus_item(playlistIndex);
	return S_OK;
}

STDMETHODIMP Plman::GetPlaylistItems(UINT playlistIndex, IMetadbHandleList** pp)
{
	if (!pp) return E_POINTER;

	metadb_handle_list items;
	playlist_manager::get()->playlist_get_all_items(playlistIndex, items);
	*pp = new com_object_impl_t<MetadbHandleList>(items);
	return S_OK;
}

STDMETHODIMP Plman::GetPlaylistName(UINT playlistIndex, BSTR* p)
{
	if (!p) return E_POINTER;

	pfc::string8_fast str;
	playlist_manager::get()->playlist_get_name(playlistIndex, str);
	*p = TO_BSTR(str);
	return S_OK;
}

STDMETHODIMP Plman::GetPlaylistSelectedItems(UINT playlistIndex, IMetadbHandleList** pp)
{
	if (!pp) return E_POINTER;

	metadb_handle_list items;
	playlist_manager::get()->playlist_get_selected_items(playlistIndex, items);
	*pp = new com_object_impl_t<MetadbHandleList>(items);
	return S_OK;
}

STDMETHODIMP Plman::GetRecyclerItems(UINT index, IMetadbHandleList** pp)
{
	if (!pp) return E_POINTER;

	auto api = playlist_manager_v3::get();
	if (index < api->recycler_get_count())
	{
		metadb_handle_list handles;
		api->recycler_get_content(index, handles);
		*pp = new com_object_impl_t<MetadbHandleList>(handles);
		return S_OK;
	}
	return E_INVALIDARG;
}

STDMETHODIMP Plman::GetRecyclerName(UINT index, BSTR* p)
{
	if (!p) return E_POINTER;

	auto api = playlist_manager_v3::get();
	if (index < api->recycler_get_count())
	{
		pfc::string8_fast str;
		api->recycler_get_name(index, str);
		*p = TO_BSTR(str);
		return S_OK;
	}
	return E_INVALIDARG;
}

STDMETHODIMP Plman::InsertPlaylistItems(UINT playlistIndex, UINT base, IMetadbHandleList* handles, VARIANT_BOOL select)
{
	metadb_handle_list* handles_ptr = nullptr;
	GET_PTR(handles, handles_ptr)

	pfc::bit_array_val selection(select != VARIANT_FALSE);
	playlist_manager::get()->playlist_insert_items(playlistIndex, base, *handles_ptr, selection);
	return S_OK;
}

STDMETHODIMP Plman::InsertPlaylistItemsFilter(UINT playlistIndex, UINT base, IMetadbHandleList* handles, VARIANT_BOOL select)
{
	metadb_handle_list* handles_ptr = nullptr;
	GET_PTR(handles, handles_ptr)

	playlist_manager::get()->playlist_insert_items_filter(playlistIndex, base, *handles_ptr, select != VARIANT_FALSE);
	return S_OK;
}

STDMETHODIMP Plman::IsAutoPlaylist(UINT playlistIndex, VARIANT_BOOL* p)
{
	if (!p) return E_POINTER;

	if (playlistIndex < playlist_manager::get()->get_playlist_count())
	{
		*p = TO_VARIANT_BOOL(autoplaylist_manager::get()->is_client_present(playlistIndex));
		return S_OK;
	}
	return E_INVALIDARG;
}

STDMETHODIMP Plman::IsPlaylistItemSelected(UINT playlistIndex, UINT playlistItemIndex, VARIANT_BOOL* p)
{
	if (!p) return E_POINTER;

	*p = TO_VARIANT_BOOL(playlist_manager::get()->playlist_is_item_selected(playlistIndex, playlistItemIndex));
	return S_OK;
}

STDMETHODIMP Plman::IsPlaylistLocked(UINT playlistIndex, VARIANT_BOOL* p)
{
	if (!p) return E_POINTER;

	auto api = playlist_manager::get();
	if (playlistIndex < api->get_playlist_count())
	{
		*p = TO_VARIANT_BOOL(api->playlist_lock_is_present(playlistIndex));
		return S_OK;
	}
	return E_INVALIDARG;
}

STDMETHODIMP Plman::MovePlaylist(UINT from, UINT to, VARIANT_BOOL* p)
{
	if (!p) return E_POINTER;

	auto api = playlist_manager::get();
	const t_size count = api->get_playlist_count();

	if (from < count && to < count)
	{
		order_helper order(count);
		const int inc = (from < to) ? 1 : -1;

		for (t_size i = from; i != to; i += inc)
		{
			order[i] = order[i + inc];
		}

		order[to] = from;

		*p = TO_VARIANT_BOOL(api->reorder(order.get_ptr(), order.get_count()));
		return S_OK;
	}
	return E_INVALIDARG;
}

STDMETHODIMP Plman::MovePlaylistSelection(UINT playlistIndex, int delta, VARIANT_BOOL* p)
{
	if (!p) return E_POINTER;

	*p = TO_VARIANT_BOOL(playlist_manager::get()->playlist_move_selection(playlistIndex, delta));
	return S_OK;
}

STDMETHODIMP Plman::PlaylistItemCount(UINT playlistIndex, UINT* p)
{
	if (!p) return E_POINTER;

	*p = playlist_manager::get()->playlist_get_item_count(playlistIndex);
	return S_OK;
}

STDMETHODIMP Plman::RecyclerPurge(VARIANT affectedItems)
{
	helpers::com_array helper;
	auto api = playlist_manager_v3::get();
	pfc::bit_array_bittable affected(api->recycler_get_count());
	if (!helper.convert(affectedItems, affected)) return E_INVALIDARG;
	if (helper.get_count())
	{
		api->recycler_purge(affected);
	}
	return S_OK;
}

STDMETHODIMP Plman::RecyclerRestore(UINT index)
{
	auto api = playlist_manager_v3::get();
	if (index < api->recycler_get_count())
	{
		api->recycler_restore(index);
		return S_OK;
	}
	return E_INVALIDARG;
}

STDMETHODIMP Plman::RemoveItemFromPlaybackQueue(UINT index)
{
	playlist_manager::get()->queue_remove_mask(pfc::bit_array_one(index));
	return S_OK;
}

STDMETHODIMP Plman::RemoveItemsFromPlaybackQueue(VARIANT affectedItems)
{
	helpers::com_array helper;
	auto api = playlist_manager::get();
	pfc::bit_array_bittable affected(api->queue_get_count());
	if (!helper.convert(affectedItems, affected)) return E_INVALIDARG;
	if (helper.get_count())
	{
		api->queue_remove_mask(affected);
	}
	return S_OK;
}

STDMETHODIMP Plman::RemovePlaylist(UINT playlistIndex, VARIANT_BOOL* p)
{
	if (!p) return E_POINTER;

	*p = TO_VARIANT_BOOL(playlist_manager::get()->remove_playlist(playlistIndex));
	return S_OK;
}

STDMETHODIMP Plman::RemovePlaylistSelection(UINT playlistIndex, VARIANT_BOOL crop)
{
	playlist_manager::get()->playlist_remove_selection(playlistIndex, crop != VARIANT_FALSE);
	return S_OK;
}

STDMETHODIMP Plman::RemovePlaylistSwitch(UINT playlistIndex, VARIANT_BOOL* p)
{
	if (!p) return E_POINTER;

	*p = TO_VARIANT_BOOL(playlist_manager::get()->remove_playlist_switch(playlistIndex));
	return S_OK;
}

STDMETHODIMP Plman::RenamePlaylist(UINT playlistIndex, BSTR name, VARIANT_BOOL* p)
{
	if (!p) return E_POINTER;

	auto uname = string_utf8_from_wide(name);
	*p = TO_VARIANT_BOOL(playlist_manager::get()->playlist_rename(playlistIndex, uname, uname.length()));
	return S_OK;
}

STDMETHODIMP Plman::SetActivePlaylistContext()
{
	ui_edit_context_manager::get()->set_context_active_playlist();
	return S_OK;
}

STDMETHODIMP Plman::SetPlaylistFocusItem(UINT playlistIndex, UINT playlistItemIndex)
{
	playlist_manager::get()->playlist_set_focus_item(playlistIndex, playlistItemIndex);
	return S_OK;
}

STDMETHODIMP Plman::SetPlaylistFocusItemByHandle(UINT playlistIndex, IMetadbHandle* handle)
{
	metadb_handle* ptr = nullptr;
	GET_PTR(handle, ptr)

	playlist_manager::get()->playlist_set_focus_by_handle(playlistIndex, ptr);
	return S_OK;
}

STDMETHODIMP Plman::SetPlaylistSelection(UINT playlistIndex, VARIANT affectedItems, VARIANT_BOOL state)
{
	helpers::com_array helper;
	auto api = playlist_manager::get();
	pfc::bit_array_bittable affected(api->playlist_get_item_count(playlistIndex));
	if (!helper.convert(affectedItems, affected)) return E_INVALIDARG;
	if (helper.get_count())
	{
		pfc::bit_array_val status(state != VARIANT_FALSE);
		api->playlist_set_selection(playlistIndex, affected, status);
	}
	return S_OK;
}

STDMETHODIMP Plman::SetPlaylistSelectionSingle(UINT playlistIndex, UINT playlistItemIndex, VARIANT_BOOL state)
{
	playlist_manager::get()->playlist_set_selection_single(playlistIndex, playlistItemIndex, state != VARIANT_FALSE);
	return S_OK;
}

STDMETHODIMP Plman::ShowAutoPlaylistUI(UINT playlistIndex, VARIANT_BOOL* p)
{
	if (!p) return E_POINTER;

	if (playlistIndex < playlist_manager::get()->get_playlist_count())
	{
		*p = VARIANT_FALSE;
		auto api = autoplaylist_manager::get();
		if (api->is_client_present(playlistIndex))
		{
			autoplaylist_client_ptr client = api->query_client(playlistIndex);
			client->show_ui(playlistIndex);
			*p = VARIANT_TRUE;
		}
		return S_OK;
	}
	return E_INVALIDARG;
}

STDMETHODIMP Plman::SortByFormat(UINT playlistIndex, BSTR pattern, VARIANT_BOOL selOnly, VARIANT_BOOL* p)
{
	if (!p) return E_POINTER;

	auto upattern = string_utf8_from_wide(pattern);
	*p = TO_VARIANT_BOOL(playlist_manager::get()->playlist_sort_by_format(playlistIndex, upattern.length() ? upattern.get_ptr() : nullptr, selOnly != VARIANT_FALSE));
	return S_OK;
}

STDMETHODIMP Plman::SortByFormatV2(UINT playlistIndex, BSTR pattern, int direction, VARIANT_BOOL* p)
{
	if (!p) return E_POINTER;

	auto api = playlist_manager::get();

	metadb_handle_list handles;
	api->playlist_get_all_items(playlistIndex, handles);
	const t_size count = handles.get_count();

	std::vector<t_size> order(count);

	titleformat_object::ptr obj;
	titleformat_compiler::get()->compile_safe(obj, string_utf8_from_wide(pattern));

	metadb_handle_list_helper::sort_by_format_get_order_v2(handles, order.data(), obj, nullptr, direction, fb2k::noAbort);

	*p = TO_VARIANT_BOOL(api->playlist_reorder_items(playlistIndex, order.data(), count));
	return S_OK;
}

STDMETHODIMP Plman::SortPlaylistsByName(int direction)
{
	auto api = playlist_manager::get();
	const t_size count = api->get_playlist_count();
	t_size i;

	std::vector<helpers::custom_sort_data> data(count);

	pfc::string8_fastalloc str;
	str.prealloc(512);

	for (i = 0; i < count; ++i)
	{
		api->playlist_get_name(i, str);
		data[i].index = i;
		data[i].text = helpers::make_sort_string(str);
	}

	pfc::sort_t(data, direction > 0 ? helpers::custom_sort_compare<1> : helpers::custom_sort_compare<-1>, count);
	order_helper order(count);

	for (i = 0; i < count; ++i)
	{
		order[i] = data[i].index;
		delete[] data[i].text;
	}

	api->reorder(order.get_ptr(), order.get_count());
	return S_OK;
}

STDMETHODIMP Plman::UndoBackup(UINT playlistIndex)
{
	playlist_manager::get()->playlist_undo_backup(playlistIndex);
	return S_OK;
}

STDMETHODIMP Plman::get_ActivePlaylist(int* p)
{
	if (!p) return E_POINTER;

	*p = playlist_manager::get()->get_active_playlist();
	return S_OK;
}

STDMETHODIMP Plman::get_PlaybackOrder(UINT* p)
{
	if (!p) return E_POINTER;

	*p = playlist_manager::get()->playback_order_get_active();
	return S_OK;
}

STDMETHODIMP Plman::get_PlayingPlaylist(int* p)
{
	if (!p) return E_POINTER;

	*p = playlist_manager::get()->get_playing_playlist();
	return S_OK;
}

STDMETHODIMP Plman::get_PlaylistCount(UINT* p)
{
	if (!p) return E_POINTER;

	*p = playlist_manager::get()->get_playlist_count();
	return S_OK;
}

STDMETHODIMP Plman::get_RecyclerCount(UINT* p)
{
	if (!p) return E_POINTER;

	*p = playlist_manager_v3::get()->recycler_get_count();
	return S_OK;
}

STDMETHODIMP Plman::put_ActivePlaylist(UINT playlistIndex)
{
	auto api = playlist_manager::get();
	if (playlistIndex < api->get_playlist_count())
	{
		api->set_active_playlist(playlistIndex);
		return S_OK;
	}
	return E_INVALIDARG;
}

STDMETHODIMP Plman::put_PlaybackOrder(UINT p)
{
	auto api = playlist_manager::get();
	if (p < api->playback_order_get_count())
	{
		api->playback_order_set_active(p);
		return S_OK;
	}
	return E_INVALIDARG;
}
