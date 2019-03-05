#pragma once
#include "helpers.h"
#include "host_comm.h"
#include "host_drop_target.h"
#include "panel_manager.h"
#include "script_host.h"

class js_panel_window : public host_comm, public ui_helpers::container_window
{
public:
	js_panel_window();
	~js_panel_window();

	HRESULT script_invoke_v(t_size callbackId, VARIANTARG* argv = nullptr, t_size argc = 0, VARIANT* ret = nullptr);
	LRESULT on_message(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) override;
	class_data& get_class_data() const override;
	void update_script();

protected:
	bool show_configure_popup(HWND parent);
	bool show_property_popup(HWND parent);
	static void build_context_menu(HMENU menu, int x, int y, int id_base);
	void execute_context_menu_command(int id, int id_base);

private:
	bool on_mouse_button_up(UINT msg, WPARAM wp, LPARAM lp);
	void create_context();
	void delete_context();
	void on_always_on_top_changed(WPARAM wp);
	void on_colours_changed();
	void on_context_menu(int x, int y);
	void on_cursor_follow_playback_changed(WPARAM wp);
	void on_dsp_preset_changed();
	void on_font_changed();
	void on_get_album_art_done(WPARAM wp);
	void on_item_focus_change(WPARAM wp);
	void on_item_played(WPARAM wp);
	void on_library_items_added(WPARAM wp);
	void on_library_items_changed(WPARAM wp);
	void on_library_items_removed(WPARAM wp);
	void on_load_image_done(WPARAM wp);
	void on_main_menu(WPARAM wp);
	void on_metadb_changed(WPARAM wp);
	void on_mouse_button_dblclk(UINT msg, WPARAM wp, LPARAM lp);
	void on_mouse_button_down(UINT msg, WPARAM wp, LPARAM lp);
	void on_mouse_leave();
	void on_mouse_move(WPARAM wp, LPARAM lp);
	void on_mouse_wheel(WPARAM wp);
	void on_mouse_wheel_h(WPARAM wp);
	void on_notify_data(WPARAM wp);
	void on_output_device_changed();
	void on_paint(HDC dc, LPRECT lpUpdateRect);
	void on_paint_error(HDC memdc);
	void on_paint_user(HDC memdc, LPRECT lpUpdateRect);
	void on_playback_dynamic_info();
	void on_playback_dynamic_info_track();
	void on_playback_edited(WPARAM wp);
	void on_playback_follow_cursor_changed(WPARAM wp);
	void on_playback_new_track(WPARAM wp);
	void on_playback_order_changed(WPARAM wp);
	void on_playback_pause(WPARAM wp);
	void on_playback_queue_changed(WPARAM wp);
	void on_playback_seek(WPARAM wp);
	void on_playback_starting(WPARAM wp, LPARAM lp);
	void on_playback_stop(WPARAM wp);
	void on_playback_time(WPARAM wp);
	void on_playlist_item_ensure_visible(WPARAM wp, LPARAM lp);
	void on_playlist_items_added(WPARAM wp);
	void on_playlist_items_removed(WPARAM wp, LPARAM lp);
	void on_playlist_items_reordered(WPARAM wp);
	void on_playlist_items_selection_change();
	void on_playlist_stop_after_current_changed(WPARAM wp);
	void on_playlist_switch();
	void on_playlists_changed();
	void on_replaygain_mode_changed(WPARAM wp);
	void on_selection_changed();
	void on_size(int w, int h);
	void on_volume_change(WPARAM wp);
	void script_load();
	void script_unload();

	CComPtr<IDropTargetImpl> m_drop_target;
	IGdiGraphicsPtr m_gr_wrap;
	bool m_is_droptarget_registered;
	bool m_is_mouse_tracked;
	script_host* m_script_host;
};
