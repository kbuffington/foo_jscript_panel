#pragma once

#define JSP_NAME "JScript Panel"
#define JSP_DLL_NAME "foo_jscript_panel.dll"

// Remember to bump utils.version too
#define JSP_VERSION_NUMBER "2.3.0.1"
#define JSP_VERSION_TEST ""

#ifdef _DEBUG
#define JSP_VERSION_DEBUG_SUFFIX " (Debug)"
#else
#define JSP_VERSION_DEBUG_SUFFIX ""
#endif

#define JSP_VERSION JSP_VERSION_NUMBER JSP_VERSION_TEST JSP_VERSION_DEBUG_SUFFIX
#define JSP_NAME_VERSION JSP_NAME " v" JSP_VERSION

#define TO_VARIANT_BOOL(v) ((v) ? (VARIANT_TRUE) : (VARIANT_FALSE))
#define TO_BSTR(str) SysAllocString(string_wide_from_utf8_fast(str))

#define GET_PTR(X, Y) \
	X->get__ptr(reinterpret_cast<void**>(&Y)); \
	if (!Y) return E_INVALIDARG;

namespace jsp_guids
{
	static constexpr GUID scintilla_properties = { 0x7f6b71a1, 0x4f1a, 0x437d,{ 0x84, 0xcc, 0xd1, 0xad, 0xd6, 0x7a, 0xd9, 0x62 } };
	static constexpr GUID ui_pref = { 0x19681d48, 0xd90e, 0x4cb6,{ 0xbb, 0x6, 0x69, 0x5f, 0x42, 0x57, 0xbe, 0x23 } };
	static constexpr GUID window_cui = { 0x19646267, 0x86fc, 0x4676,{ 0xa9, 0x8a, 0x49, 0x27, 0x38, 0x52, 0xb0, 0x88 } };
	static constexpr GUID window_dui = { 0xdaa5f4e7, 0x177d, 0x4489,{ 0x9a, 0xd9, 0x9f, 0x3a, 0x8e, 0xfa, 0x96, 0xab } };
	static constexpr GUID metadb_index = { 0x835f0b63, 0xd96c, 0x447b,{ 0x9c, 0xcb, 0x71, 0x4f, 0xa8, 0x30, 0x49, 0x11 } };
	static constexpr GUID mainmenu_group_id = { 0x7b23ce27, 0x7d37, 0x4a1f,{ 0x80, 0x5b, 0xe5, 0x89, 0x19, 0x5b, 0xbd, 0xd0 } };
	static constexpr GUID menu_one = { 0xf56e5f2d, 0xf1a1, 0x4f54,{ 0x97, 0xf5, 0xe7, 0xc4, 0xbe, 0x47, 0x1a, 0xb3 } };
	static constexpr GUID menu_two = { 0xc3bda2f5, 0xf011, 0x4f54,{ 0x99, 0xa, 0x77, 0xf9, 0xef, 0x27, 0xf1, 0xb } };
	static constexpr GUID menu_three = { 0x9c610f78, 0x2eb7, 0x43b6,{ 0x89, 0x6d, 0x86, 0x9b, 0xd4, 0xec, 0xb9, 0xeb } };
	static constexpr GUID menu_four = { 0x6123f3ee, 0xeb4f, 0x4170,{ 0x80, 0x49, 0x15, 0x18, 0xe6, 0xad, 0x8a, 0x62 } };
	static constexpr GUID menu_five = { 0x59f48bd1, 0xa6a1, 0x4a8a,{ 0x93, 0x3c, 0x39, 0xcf, 0xef, 0x8, 0xee, 0x52 } };
	static constexpr GUID menu_six = { 0x365377e0, 0x7a4b, 0x430f,{ 0x88, 0x3, 0xfd, 0xaf, 0x22, 0x60, 0xf6, 0xe1 } };
	static constexpr GUID menu_seven = { 0x5e26ac8d, 0x38, 0x4743,{ 0x90, 0x72, 0x2c, 0x26, 0x56, 0x96, 0xcf, 0x14 } };
	static constexpr GUID menu_eight = { 0x6b00b1c4, 0xa55, 0x46d8,{ 0x83, 0x2f, 0xfd, 0xd5, 0xd9, 0xde, 0x69, 0x43 } };
	static constexpr GUID menu_nine = { 0xca840da4, 0xfc99, 0x44bc,{ 0x90, 0x71, 0xd2, 0xb0, 0x2b, 0x26, 0xd4, 0x35 } };
	static constexpr GUID menu_ten = { 0xab05eee8, 0xbadc, 0x49ba,{ 0x80, 0x27, 0x84, 0x72, 0xa8, 0xbd, 0x49, 0xdb } };
}

enum t_user_message
{
	UWM_APPLY = WM_USER + 1,
	UWM_PROPERTIES_CLEARED,
	UWM_TIMER,
	UWM_UNLOAD
};

enum callback_id
{
	on_always_on_top_changed = UWM_UNLOAD + 1,
	on_char,
	on_colours_changed,
	on_cursor_follow_playback_changed,
	on_drag_drop,
	on_drag_enter,
	on_drag_leave,
	on_drag_over,
	on_dsp_preset_changed,
	on_focus,
	on_font_changed,
	on_get_album_art_done,
	on_item_focus_change,
	on_item_played,
	on_key_down,
	on_key_up,
	on_library_items_added,
	on_library_items_changed,
	on_library_items_removed,
	on_load_image_done,
	on_main_menu,
	on_metadb_changed,
	on_mouse_lbtn_dblclk,
	on_mouse_lbtn_down,
	on_mouse_lbtn_up,
	on_mouse_leave,
	on_mouse_mbtn_dblclk,
	on_mouse_mbtn_down,
	on_mouse_mbtn_up,
	on_mouse_move,
	on_mouse_rbtn_dblclk,
	on_mouse_rbtn_down,
	on_mouse_rbtn_up,
	on_mouse_wheel,
	on_mouse_wheel_h,
	on_notify_data,
	on_output_device_changed,
	on_paint,
	on_playback_dynamic_info,
	on_playback_dynamic_info_track,
	on_playback_edited,
	on_playback_follow_cursor_changed,
	on_playback_new_track,
	on_playback_order_changed,
	on_playback_pause,
	on_playback_queue_changed,
	on_playback_seek,
	on_playback_starting,
	on_playback_stop,
	on_playback_time,
	on_playlist_item_ensure_visible,
	on_playlist_items_added,
	on_playlist_items_removed,
	on_playlist_items_reordered,
	on_playlist_items_selection_change,
	on_playlist_stop_after_current_changed,
	on_playlist_switch,
	on_playlists_changed,
	on_replaygain_mode_changed,
	on_script_unload,
	on_selection_changed,
	on_size,
	on_volume_change
};
