#include "stdafx.h"
#include "Fb.h"
#include "drop_source_impl.h"
#include "helpers.h"

Fb::Fb() {}
Fb::~Fb() {}

STDMETHODIMP Fb::AcquireUiSelectionHolder(IUiSelectionHolder** pp)
{
	if (!pp) return E_POINTER;

	ui_selection_holder::ptr holder = ui_selection_manager::get()->acquire();
	*pp = new com_object_impl_t<UiSelectionHolder>(holder);
	return S_OK;
}

STDMETHODIMP Fb::AddDirectory()
{
	standard_commands::main_add_directory();
	return S_OK;
}

STDMETHODIMP Fb::AddFiles()
{
	standard_commands::main_add_files();
	return S_OK;
}

STDMETHODIMP Fb::CheckClipboardContents(UINT /* FFS */, VARIANT_BOOL* p)
{
	if (!p) return E_POINTER;

	*p = VARIANT_FALSE;
	pfc::com_ptr_t<IDataObject> pDO;
	if (SUCCEEDED(OleGetClipboard(pDO.receive_ptr())))
	{
		bool native;
		DWORD drop_effect = DROPEFFECT_COPY;
		*p = TO_VARIANT_BOOL(SUCCEEDED(ole_interaction::get()->check_dataobject(pDO, drop_effect, native)));
	}
	return S_OK;
}

STDMETHODIMP Fb::ClearPlaylist()
{
	standard_commands::main_clear_playlist();
	return S_OK;
}

STDMETHODIMP Fb::CopyHandleListToClipboard(IMetadbHandleList* handles, VARIANT_BOOL* p)
{
	if (!p) return E_POINTER;

	metadb_handle_list* handles_ptr = nullptr;
	GET_PTR(handles, handles_ptr);

	pfc::com_ptr_t<IDataObject> pDO = ole_interaction::get()->create_dataobject(*handles_ptr);
	*p = TO_VARIANT_BOOL(SUCCEEDED(OleSetClipboard(pDO.get_ptr())));
	return S_OK;
}

STDMETHODIMP Fb::CreateContextMenuManager(IContextMenuManager** pp)
{
	if (!pp) return E_POINTER;

	*pp = new com_object_impl_t<ContextMenuManager>();
	return S_OK;
}

STDMETHODIMP Fb::CreateHandleList(VARIANT handle, IMetadbHandleList** pp)
{
	if (!pp) return E_POINTER;

	metadb_handle_list items;
	IDispatch* temp = nullptr;

	if (handle.vt == VT_DISPATCH && handle.pdispVal && SUCCEEDED(handle.pdispVal->QueryInterface(__uuidof(IMetadbHandle), reinterpret_cast<void**>(&temp))))
	{
		IDispatchPtr handle_s = temp;
		void* ptr = nullptr;
		reinterpret_cast<IMetadbHandle*>(handle_s.GetInterfacePtr())->get__ptr(&ptr);
		if (!ptr) return E_INVALIDARG;

		items.add_item(reinterpret_cast<metadb_handle*>(ptr));
	}
	*pp = new com_object_impl_t<MetadbHandleList>(items);
	return S_OK;
}

STDMETHODIMP Fb::CreateMainMenuManager(IMainMenuManager** pp)
{
	if (!pp) return E_POINTER;

	*pp = new com_object_impl_t<MainMenuManager>();
	return S_OK;
}

STDMETHODIMP Fb::CreateProfiler(BSTR name, IProfiler** pp)
{
	if (!pp) return E_POINTER;

	*pp = new com_object_impl_t<Profiler>(string_utf8_from_wide(name));
	return S_OK;
}

STDMETHODIMP Fb::DoDragDrop(IMetadbHandleList* handles, UINT okEffects, UINT* p)
{
	if (!p) return E_POINTER;

	metadb_handle_list* handles_ptr = nullptr;
	GET_PTR(handles, handles_ptr);

	if (!handles_ptr->get_count() || okEffects == DROPEFFECT_NONE)
	{
		*p = DROPEFFECT_NONE;
		return S_OK;
	}

	pfc::com_ptr_t<IDataObject> pDO = ole_interaction::get()->create_dataobject(*handles_ptr);
	pfc::com_ptr_t<IDropSourceImpl> pIDropSource = new IDropSourceImpl();

	DWORD returnEffect;
	const HRESULT hr = ::DoDragDrop(pDO.get_ptr(), pIDropSource.get_ptr(), okEffects, &returnEffect);

	*p = hr == DRAGDROP_S_CANCEL ? DROPEFFECT_NONE : returnEffect;
	return S_OK;
}

STDMETHODIMP Fb::Exit()
{
	standard_commands::main_exit();
	return S_OK;
}

STDMETHODIMP Fb::GetClipboardContents(UINT /* FFS */, IMetadbHandleList** pp)
{
	if (!pp) return E_POINTER;

	auto api = ole_interaction::get();
	pfc::com_ptr_t<IDataObject> pDO;
	metadb_handle_list items;

	if (SUCCEEDED(OleGetClipboard(pDO.receive_ptr())))
	{
		DWORD drop_effect = DROPEFFECT_COPY;
		bool native;
		if (SUCCEEDED(api->check_dataobject(pDO, drop_effect, native)))
		{
			dropped_files_data_impl data;
			if (SUCCEEDED(api->parse_dataobject(pDO, data)))
			{
				data.to_handles(items, native, core_api::get_main_window());
			}
		}
	}

	*pp = new com_object_impl_t<MetadbHandleList>(items);
	return S_OK;
}

STDMETHODIMP Fb::GetDSPPresets(BSTR* p)
{
	if (!p) return E_POINTER;

	json j = json::array();
	auto api = dsp_config_manager_v2::get();
	const size_t count = api->get_preset_count();
	pfc::string8_fast name;

	for (size_t i = 0; i < count; ++i)
	{
		api->get_preset_name(i, name);

		j.push_back(
			{
				{ "active", api->get_selected_preset() == i },
				{ "name", name.get_ptr() }
			});
	}
	*p = TO_BSTR((j.dump()).c_str());
	return S_OK;
}

STDMETHODIMP Fb::GetFocusItem(IMetadbHandle** pp)
{
	if (!pp) return E_POINTER;

	metadb_handle_ptr metadb;
	*pp = playlist_manager::get()->activeplaylist_get_focus_item_handle(metadb) ? new com_object_impl_t<MetadbHandle>(metadb) : nullptr;
	return S_OK;
}

STDMETHODIMP Fb::GetLibraryItems(IMetadbHandleList** pp)
{
	if (!pp) return E_POINTER;

	metadb_handle_list items;
	library_manager::get()->get_all_items(items);
	*pp = new com_object_impl_t<MetadbHandleList>(items);
	return S_OK;
}

STDMETHODIMP Fb::GetNowPlaying(IMetadbHandle** pp)
{
	if (!pp) return E_POINTER;

	metadb_handle_ptr metadb;
	*pp = playback_control::get()->get_now_playing(metadb) ? new com_object_impl_t<MetadbHandle>(metadb) : nullptr;
	return S_OK;
}

STDMETHODIMP Fb::GetOutputDevices(BSTR* p)
{
	if (!p) return E_POINTER;

	json j = json::array();
	auto api = output_manager_v2::get();
	outputCoreConfig_t config;
	api->getCoreConfig(config);

	api->listDevices([&](pfc::string8_fast&& name, auto&& output_id, auto&& device_id)
		{
			pfc::string8_fast output_string, device_string;
			output_string << "{" << pfc::print_guid(output_id) << "}";
			device_string << "{" << pfc::print_guid(device_id) << "}";

			j.push_back(
				{
					{ "name", name.get_ptr() },
					{ "output_id", output_string.get_ptr() },
					{ "device_id", device_string.get_ptr() },
					{ "active", config.m_output == output_id && config.m_device == device_id }
				});
		});
	*p = TO_BSTR((j.dump()).c_str());
	return S_OK;
}

STDMETHODIMP Fb::GetQueryItems(IMetadbHandleList* handles, BSTR query, IMetadbHandleList** pp)
{
	if (!pp) return E_POINTER;

	metadb_handle_list* handles_ptr = nullptr;
	GET_PTR(handles, handles_ptr);

	search_filter_v2::ptr filter;

	try
	{
		filter = search_filter_manager_v2::get()->create_ex(string_utf8_from_wide(query), fb2k::service_new<completion_notify_dummy>(), search_filter_manager_v2::KFlagSuppressNotify);
	}
	catch (...)
	{
		return E_INVALIDARG;
	}

	metadb_handle_list copy(*handles_ptr);
	pfc::array_t<bool> mask;
	mask.set_size(copy.get_count());
	filter->test_multi(copy, mask.get_ptr());
	copy.filter_mask(mask.get_ptr());
	*pp = new com_object_impl_t<MetadbHandleList>(copy);
	return S_OK;
}

STDMETHODIMP Fb::GetSelection(IMetadbHandle** pp)
{
	if (!pp) return E_POINTER;

	metadb_handle_list items;
	ui_selection_manager::get()->get_selection(items);
	*pp = items.get_count() ? new com_object_impl_t<MetadbHandle>(items[0]) : nullptr;
	return S_OK;
}

STDMETHODIMP Fb::GetSelections(UINT flags, IMetadbHandleList** pp)
{
	if (!pp) return E_POINTER;

	metadb_handle_list items;
	ui_selection_manager_v2::get()->get_selection(items, flags);
	*pp = new com_object_impl_t<MetadbHandleList>(items);
	return S_OK;
}

STDMETHODIMP Fb::GetSelectionType(UINT* p)
{
	if (!p) return E_POINTER;

	const GUID type = ui_selection_manager_v2::get()->get_selection_type(0);
	const auto it = FIND_IF(jsp::guids::selections, [type](const auto& guid) { return *guid == type; });
	*p = it != jsp::guids::selections.end() ? std::distance(jsp::guids::selections.begin(), it) : 0;
	return S_OK;
}

STDMETHODIMP Fb::IsLibraryEnabled(VARIANT_BOOL* p)
{
	if (!p) return E_POINTER;

	*p = TO_VARIANT_BOOL(library_manager::get()->is_library_enabled());
	return S_OK;
}

STDMETHODIMP Fb::IsMetadbInMediaLibrary(IMetadbHandle* handle, VARIANT_BOOL* p)
{
	if (!p) return E_POINTER;

	metadb_handle* ptr = nullptr;
	GET_PTR(handle, ptr);

	*p = TO_VARIANT_BOOL(library_manager::get()->is_item_in_library(ptr));
	return S_OK;
}

STDMETHODIMP Fb::LoadPlaylist()
{
	standard_commands::main_load_playlist();
	return S_OK;
}

STDMETHODIMP Fb::Next()
{
	standard_commands::main_next();
	return S_OK;
}

STDMETHODIMP Fb::Pause()
{
	standard_commands::main_pause();
	return S_OK;
}

STDMETHODIMP Fb::Play()
{
	standard_commands::main_play();
	return S_OK;
}

STDMETHODIMP Fb::PlayOrPause()
{
	standard_commands::main_play_or_pause();
	return S_OK;
}

STDMETHODIMP Fb::Prev()
{
	standard_commands::main_previous();
	return S_OK;
}

STDMETHODIMP Fb::Random()
{
	standard_commands::main_random();
	return S_OK;
}

STDMETHODIMP Fb::RunContextCommand(BSTR command, VARIANT_BOOL* p)
{
	if (!p) return E_POINTER;

	*p = VARIANT_FALSE;
	if (playback_control::get()->is_playing())
	{
		contextmenu_manager::ptr cm;
		contextmenu_manager::g_create(cm);
		cm->init_context_now_playing(contextmenu_manager::flag_view_full);
		pfc::string8_fast path;
		*p = TO_VARIANT_BOOL(helpers::execute_context_command_recur(string_utf8_from_wide(command), path, cm->get_root()));
	}
	return S_OK;
}

STDMETHODIMP Fb::RunContextCommandWithMetadb(BSTR command, VARIANT handle, VARIANT_BOOL* p)
{
	if (!p) return E_POINTER;
	if (handle.vt != VT_DISPATCH || !handle.pdispVal) return E_INVALIDARG;

	metadb_handle_list handle_list;
	IDispatch* temp = nullptr;
	IDispatchPtr handle_s = nullptr;
	void* ptr = nullptr;

	if (SUCCEEDED(handle.pdispVal->QueryInterface(__uuidof(IMetadbHandle), reinterpret_cast<void**>(&temp))))
	{
		handle_s = temp;
		reinterpret_cast<IMetadbHandle*>(handle_s.GetInterfacePtr())->get__ptr(&ptr);
		if (!ptr) return E_INVALIDARG;
		handle_list = pfc::list_single_ref_t<metadb_handle_ptr>(reinterpret_cast<metadb_handle*>(ptr));
	}
	else if (SUCCEEDED(handle.pdispVal->QueryInterface(__uuidof(IMetadbHandleList), reinterpret_cast<void**>(&temp))))
	{
		handle_s = temp;
		reinterpret_cast<IMetadbHandleList*>(handle_s.GetInterfacePtr())->get__ptr(&ptr);
		if (!ptr) return E_INVALIDARG;
		handle_list = *reinterpret_cast<metadb_handle_list*>(ptr);
	}
	else
	{
		return E_INVALIDARG;
	}

	*p = handle_list.get_count() ? TO_VARIANT_BOOL(helpers::execute_context_command_by_name(string_utf8_from_wide(command), handle_list)) : VARIANT_FALSE;
	return S_OK;
}

STDMETHODIMP Fb::RunMainMenuCommand(BSTR command, VARIANT_BOOL* p)
{
	if (!p) return E_POINTER;

	*p = TO_VARIANT_BOOL(helpers::execute_mainmenu_command_by_name(string_utf8_from_wide(command)));
	return S_OK;
}

STDMETHODIMP Fb::SavePlaylist()
{
	standard_commands::main_save_playlist();
	return S_OK;
}

STDMETHODIMP Fb::SetDSPPreset(UINT idx)
{
	auto api = dsp_config_manager_v2::get();
	const size_t count = api->get_preset_count();
	if (idx < count)
	{
		api->select_preset(idx);
		return S_OK;
	}
	return E_INVALIDARG;
}

STDMETHODIMP Fb::SetOutputDevice(BSTR output, BSTR device)
{
	GUID output_id, device_id;
	if (CLSIDFromString(output, &output_id) == NOERROR && CLSIDFromString(device, &device_id) == NOERROR)
	{
		output_manager_v2::get()->setCoreConfigDevice(output_id, device_id);
	}
	return S_OK;
}

STDMETHODIMP Fb::ShowConsole()
{
	static constexpr GUID g = { 0x5b652d25, 0xce44, 0x4737,{ 0x99, 0xbb, 0xa3, 0xcf, 0x2a, 0xeb, 0x35, 0xcc } };
	standard_commands::run_main(g);
	return S_OK;
}

STDMETHODIMP Fb::ShowLibrarySearchUI(BSTR query)
{
	if (!query) return E_INVALIDARG;

	library_search_ui::get()->show(string_utf8_from_wide(query));
	return S_OK;
}

STDMETHODIMP Fb::ShowPopupMessage(BSTR msg, BSTR title)
{
	auto umsg = string_utf8_from_wide(msg);
	auto utitle = string_utf8_from_wide(title);
	fb2k::inMainThread([=]()
		{
			popup_message::g_show(umsg, utitle);
		});
	return S_OK;
}

STDMETHODIMP Fb::ShowPreferences()
{
	standard_commands::main_preferences();
	return S_OK;
}

STDMETHODIMP Fb::Stop()
{
	standard_commands::main_stop();
	return S_OK;
}

STDMETHODIMP Fb::TitleFormat(BSTR pattern, ITitleFormat** pp)
{
	if (!pp) return E_POINTER;

	*pp = new com_object_impl_t<::TitleFormat>(pattern);
	return S_OK;
}

STDMETHODIMP Fb::VolumeDown()
{
	standard_commands::main_volume_down();
	return S_OK;
}

STDMETHODIMP Fb::VolumeMute()
{
	standard_commands::main_volume_mute();
	return S_OK;
}

STDMETHODIMP Fb::VolumeUp()
{
	standard_commands::main_volume_up();
	return S_OK;
}

STDMETHODIMP Fb::get_AlwaysOnTop(VARIANT_BOOL* p)
{
	if (!p) return E_POINTER;

	*p = TO_VARIANT_BOOL(config_object::g_get_data_bool_simple(standard_config_objects::bool_ui_always_on_top, false));
	return S_OK;
}

STDMETHODIMP Fb::get_ComponentPath(BSTR* p)
{
	if (!p) return E_POINTER;

	*p = TO_BSTR(helpers::get_fb2k_component_path());
	return S_OK;
}

STDMETHODIMP Fb::get_CursorFollowPlayback(VARIANT_BOOL* p)
{
	if (!p) return E_POINTER;

	*p = TO_VARIANT_BOOL(config_object::g_get_data_bool_simple(standard_config_objects::bool_cursor_follows_playback, false));
	return S_OK;
}

STDMETHODIMP Fb::get_FoobarPath(BSTR* p)
{
	if (!p) return E_POINTER;

	*p = TO_BSTR(helpers::get_fb2k_path());
	return S_OK;
}

STDMETHODIMP Fb::get_IsPaused(VARIANT_BOOL* p)
{
	if (!p) return E_POINTER;

	*p = TO_VARIANT_BOOL(playback_control::get()->is_paused());
	return S_OK;
}

STDMETHODIMP Fb::get_IsPlaying(VARIANT_BOOL* p)
{
	if (!p) return E_POINTER;

	*p = TO_VARIANT_BOOL(playback_control::get()->is_playing());
	return S_OK;
}

STDMETHODIMP Fb::get_PlaybackFollowCursor(VARIANT_BOOL* p)
{
	if (!p) return E_POINTER;

	*p = TO_VARIANT_BOOL(config_object::g_get_data_bool_simple(standard_config_objects::bool_playback_follows_cursor, false));
	return S_OK;
}

STDMETHODIMP Fb::get_PlaybackLength(double* p)
{
	if (!p) return E_POINTER;

	*p = playback_control::get()->playback_get_length();
	return S_OK;
}

STDMETHODIMP Fb::get_PlaybackTime(double* p)
{
	if (!p) return E_POINTER;

	*p = playback_control::get()->playback_get_position();
	return S_OK;
}

STDMETHODIMP Fb::get_ProfilePath(BSTR* p)
{
	if (!p) return E_POINTER;

	*p = TO_BSTR(helpers::get_profile_path());
	return S_OK;
}

STDMETHODIMP Fb::get_ReplaygainMode(UINT* p)
{
	if (!p) return E_POINTER;

	t_replaygain_config rg;
	replaygain_manager::get()->get_core_settings(rg);
	*p = rg.m_source_mode;
	return S_OK;
}

STDMETHODIMP Fb::get_StopAfterCurrent(VARIANT_BOOL* p)
{
	if (!p) return E_POINTER;

	*p = TO_VARIANT_BOOL(playback_control::get()->get_stop_after_current());
	return S_OK;
}

STDMETHODIMP Fb::get_Volume(float* p)
{
	if (!p) return E_POINTER;

	*p = playback_control::get()->get_volume();
	return S_OK;
}

STDMETHODIMP Fb::put_AlwaysOnTop(VARIANT_BOOL p)
{
	config_object::g_set_data_bool(standard_config_objects::bool_ui_always_on_top, p != VARIANT_FALSE);
	return S_OK;
}

STDMETHODIMP Fb::put_CursorFollowPlayback(VARIANT_BOOL p)
{
	config_object::g_set_data_bool(standard_config_objects::bool_cursor_follows_playback, p != VARIANT_FALSE);
	return S_OK;
}

STDMETHODIMP Fb::put_PlaybackFollowCursor(VARIANT_BOOL p)
{
	config_object::g_set_data_bool(standard_config_objects::bool_playback_follows_cursor, p != VARIANT_FALSE);
	return S_OK;
}

STDMETHODIMP Fb::put_PlaybackTime(double time)
{
	playback_control::get()->playback_seek(time);
	return S_OK;
}

STDMETHODIMP Fb::put_ReplaygainMode(UINT p)
{
	switch (p)
	{
	case 0: standard_commands::main_rg_disable(); break;
	case 1: standard_commands::main_rg_set_track(); break;
	case 2: standard_commands::main_rg_set_album(); break;
	case 3: standard_commands::run_main(standard_commands::guid_main_rg_byorder); break;
	default: return E_INVALIDARG;
	}

	playback_control_v3::get()->restart();
	return S_OK;
}

STDMETHODIMP Fb::put_StopAfterCurrent(VARIANT_BOOL p)
{
	playback_control::get()->set_stop_after_current(p != VARIANT_FALSE);
	return S_OK;
}

STDMETHODIMP Fb::put_Volume(float value)
{
	playback_control::get()->set_volume(value);
	return S_OK;
}
