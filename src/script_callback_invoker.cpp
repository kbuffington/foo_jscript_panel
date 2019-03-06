#include "stdafx.h"
#include "script_callback_invoker.h"

#define DEFINE_ID_NAME_MAP_ENTRY(x) { CallbackIds::##x, PFC_WIDESTRING(#x) }

struct IDToNameEntry
{
	t_size id;
	const wchar_t* name;
};

static const IDToNameEntry g_idToNames[] =
{
	DEFINE_ID_NAME_MAP_ENTRY(on_always_on_top_changed),
	DEFINE_ID_NAME_MAP_ENTRY(on_char),
	DEFINE_ID_NAME_MAP_ENTRY(on_colours_changed),
	DEFINE_ID_NAME_MAP_ENTRY(on_cursor_follow_playback_changed),
	DEFINE_ID_NAME_MAP_ENTRY(on_drag_drop),
	DEFINE_ID_NAME_MAP_ENTRY(on_drag_enter),
	DEFINE_ID_NAME_MAP_ENTRY(on_drag_leave),
	DEFINE_ID_NAME_MAP_ENTRY(on_drag_over),
	DEFINE_ID_NAME_MAP_ENTRY(on_dsp_preset_changed),
	DEFINE_ID_NAME_MAP_ENTRY(on_focus),
	DEFINE_ID_NAME_MAP_ENTRY(on_font_changed),
	DEFINE_ID_NAME_MAP_ENTRY(on_get_album_art_done),
	DEFINE_ID_NAME_MAP_ENTRY(on_item_focus_change),
	DEFINE_ID_NAME_MAP_ENTRY(on_item_played),
	DEFINE_ID_NAME_MAP_ENTRY(on_key_down),
	DEFINE_ID_NAME_MAP_ENTRY(on_key_up),
	DEFINE_ID_NAME_MAP_ENTRY(on_library_items_added),
	DEFINE_ID_NAME_MAP_ENTRY(on_library_items_changed),
	DEFINE_ID_NAME_MAP_ENTRY(on_library_items_removed),
	DEFINE_ID_NAME_MAP_ENTRY(on_load_image_done),
	DEFINE_ID_NAME_MAP_ENTRY(on_main_menu),
	DEFINE_ID_NAME_MAP_ENTRY(on_metadb_changed),
	DEFINE_ID_NAME_MAP_ENTRY(on_mouse_lbtn_dblclk),
	DEFINE_ID_NAME_MAP_ENTRY(on_mouse_lbtn_down),
	DEFINE_ID_NAME_MAP_ENTRY(on_mouse_lbtn_up),
	DEFINE_ID_NAME_MAP_ENTRY(on_mouse_leave),
	DEFINE_ID_NAME_MAP_ENTRY(on_mouse_mbtn_dblclk),
	DEFINE_ID_NAME_MAP_ENTRY(on_mouse_mbtn_down),
	DEFINE_ID_NAME_MAP_ENTRY(on_mouse_mbtn_up),
	DEFINE_ID_NAME_MAP_ENTRY(on_mouse_move),
	DEFINE_ID_NAME_MAP_ENTRY(on_mouse_rbtn_dblclk),
	DEFINE_ID_NAME_MAP_ENTRY(on_mouse_rbtn_down),
	DEFINE_ID_NAME_MAP_ENTRY(on_mouse_rbtn_up),
	DEFINE_ID_NAME_MAP_ENTRY(on_mouse_wheel),
	DEFINE_ID_NAME_MAP_ENTRY(on_mouse_wheel_h),
	DEFINE_ID_NAME_MAP_ENTRY(on_notify_data),
	DEFINE_ID_NAME_MAP_ENTRY(on_output_device_changed),
	DEFINE_ID_NAME_MAP_ENTRY(on_paint),
	DEFINE_ID_NAME_MAP_ENTRY(on_playback_dynamic_info),
	DEFINE_ID_NAME_MAP_ENTRY(on_playback_dynamic_info_track),
	DEFINE_ID_NAME_MAP_ENTRY(on_playback_edited),
	DEFINE_ID_NAME_MAP_ENTRY(on_playback_follow_cursor_changed),
	DEFINE_ID_NAME_MAP_ENTRY(on_playback_new_track),
	DEFINE_ID_NAME_MAP_ENTRY(on_playback_order_changed),
	DEFINE_ID_NAME_MAP_ENTRY(on_playback_pause),
	DEFINE_ID_NAME_MAP_ENTRY(on_playback_queue_changed),
	DEFINE_ID_NAME_MAP_ENTRY(on_playback_seek),
	DEFINE_ID_NAME_MAP_ENTRY(on_playback_starting),
	DEFINE_ID_NAME_MAP_ENTRY(on_playback_stop),
	DEFINE_ID_NAME_MAP_ENTRY(on_playback_time),
	DEFINE_ID_NAME_MAP_ENTRY(on_playlist_item_ensure_visible),
	DEFINE_ID_NAME_MAP_ENTRY(on_playlist_items_added),
	DEFINE_ID_NAME_MAP_ENTRY(on_playlist_items_removed),
	DEFINE_ID_NAME_MAP_ENTRY(on_playlist_items_reordered),
	DEFINE_ID_NAME_MAP_ENTRY(on_playlist_items_selection_change),
	DEFINE_ID_NAME_MAP_ENTRY(on_playlist_stop_after_current_changed),
	DEFINE_ID_NAME_MAP_ENTRY(on_playlist_switch),
	DEFINE_ID_NAME_MAP_ENTRY(on_playlists_changed),
	DEFINE_ID_NAME_MAP_ENTRY(on_replaygain_mode_changed),
	DEFINE_ID_NAME_MAP_ENTRY(on_script_unload),
	DEFINE_ID_NAME_MAP_ENTRY(on_selection_changed),
	DEFINE_ID_NAME_MAP_ENTRY(on_size),
	DEFINE_ID_NAME_MAP_ENTRY(on_volume_change)
};

script_callback_invoker::script_callback_invoker() {}

script_callback_invoker::~script_callback_invoker()
{
	reset();
}

HRESULT script_callback_invoker::invoke(t_size callbackId, VARIANTARG* argv, t_size argc, VARIANT* ret)
{
	if (!m_active_script_root) return E_POINTER;
	int dispId;
	if (!m_callback_invoker_map.query(callbackId, dispId)) return DISP_E_MEMBERNOTFOUND;
	DISPPARAMS param = { argv, nullptr, argc, 0 };
	return m_active_script_root->Invoke(dispId, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &param, ret, nullptr, nullptr);
}

void script_callback_invoker::init(IDispatch* pActiveScriptRoot)
{
	reset();
	if (!pActiveScriptRoot) return;
	m_active_script_root = pActiveScriptRoot;
	for (const auto& i : g_idToNames)
	{
		LPOLESTR name = const_cast<LPOLESTR>(i.name);
		DISPID dispId;
		if (SUCCEEDED(m_active_script_root->GetIDsOfNames(IID_NULL, &name, 1, LOCALE_USER_DEFAULT, &dispId)))
		{
			m_callback_invoker_map[i.id] = dispId;
		}
	}
}

void script_callback_invoker::reset()
{
	m_callback_invoker_map.remove_all();
}
