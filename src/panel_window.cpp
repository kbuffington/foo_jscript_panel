#include "stdafx.h"
#include "host_timer_dispatcher.h"
#include "panel_window.h"
#include "ui_conf.h"
#include "ui_property.h"

panel_window::panel_window() : m_script_host(new script_host(this)), m_is_mouse_tracked(false), m_is_droptarget_registered(false) {}

panel_window::~panel_window()
{
	m_script_host->Release();
}

HRESULT panel_window::script_invoke(t_size callbackId, VARIANTARG* argv, t_size argc, VARIANT* ret)
{
	return m_script_host->InvokeCallback(callbackId, argv, argc, ret);
}

LRESULT panel_window::on_message(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_CREATE:
		{
			m_hwnd = hwnd;
			m_hdc = GetDC(m_hwnd);
			create_context();
			m_gr_wrap.Attach(new com_object_impl_t<GdiGraphics>(), false);
			panel_manager::instance().add_window(m_hwnd);
			load_script();
		}
		return 0;
	case WM_DESTROY:
		unload_script();
		panel_manager::instance().remove_window(m_hwnd);
		if (m_gr_wrap)
		{
			m_gr_wrap.Release();
		}
		delete_context();
		ReleaseDC(m_hwnd, m_hdc);
		return 0;
	case WM_DISPLAYCHANGE:
	case WM_THEMECHANGED:
		update_script();
		return 0;
	case WM_ERASEBKGND:
		if (m_pseudo_transparent)
		{
			redraw();
		}
		return 1;
	case WM_PAINT:
		{
			if (m_suppress_drawing)
				break;

			if (m_pseudo_transparent && !m_paint_pending)
			{
				RECT rc;
				GetUpdateRect(m_hwnd, &rc, FALSE);
				refresh_background(&rc);
				return 0;
			}

			PAINTSTRUCT ps;
			HDC dc = BeginPaint(m_hwnd, &ps);
			on_paint(dc, &ps.rcPaint);
			EndPaint(m_hwnd, &ps);
			m_paint_pending = false;
		}
		return 0;
	case WM_SIZE:
		{
			RECT rc;
			GetClientRect(m_hwnd, &rc);
			on_size(rc.right - rc.left, rc.bottom - rc.top);
			if (m_pseudo_transparent)
				redraw();
			else
				repaint();
		}
		return 0;
	case WM_GETMINMAXINFO:
		{
			LPMINMAXINFO pmmi = reinterpret_cast<LPMINMAXINFO>(lp);
			memcpy(&pmmi->ptMaxTrackSize, &m_max_size, sizeof(POINT));
			memcpy(&pmmi->ptMinTrackSize, &m_min_size, sizeof(POINT));
		}
		return 0;
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		on_mouse_button_down(msg, wp, lp);
		break;
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
		if (on_mouse_button_up(msg, wp, lp))
			return 0;
		break;
	case WM_LBUTTONDBLCLK:
	case WM_MBUTTONDBLCLK:
	case WM_RBUTTONDBLCLK:
		on_mouse_button_dblclk(msg, wp, lp);
		break;
	case WM_CONTEXTMENU:
		on_context_menu(GET_X_LPARAM(lp), GET_Y_LPARAM(lp));
		return 1;
	case WM_MOUSEMOVE:
		on_mouse_move(wp, lp);
		break;
	case WM_MOUSELEAVE:
		on_mouse_leave();
		break;
	case WM_MOUSEWHEEL:
		on_mouse_wheel(wp);
		break;
	case WM_MOUSEHWHEEL:
		on_mouse_wheel_h(wp);
		break;
	case WM_SETCURSOR:
		return 1;
	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:
		{
			VARIANTARG args[1];
			args[0].vt = VT_UI4;
			args[0].ulVal = (ULONG)wp;
			script_invoke(callback_id::on_key_down, args, _countof(args));
		}
		return 0;
	case WM_KEYUP:
		{
			VARIANTARG args[1];
			args[0].vt = VT_UI4;
			args[0].ulVal = (ULONG)wp;
			script_invoke(callback_id::on_key_up, args, _countof(args));
		}
		return 0;
	case WM_CHAR:
		{
			VARIANTARG args[1];
			args[0].vt = VT_UI4;
			args[0].ulVal = (ULONG)wp;
			script_invoke(callback_id::on_char, args, _countof(args));
		}
		return 0;
	case WM_SETFOCUS:
		{
			m_selection_holder = ui_selection_manager::get()->acquire();

			VARIANTARG args[1];
			args[0].vt = VT_BOOL;
			args[0].boolVal = VARIANT_TRUE;
			script_invoke(callback_id::on_focus, args, _countof(args));
		}
		break;
	case WM_KILLFOCUS:
		{
			m_selection_holder.release();

			VARIANTARG args[1];
			args[0].vt = VT_BOOL;
			args[0].boolVal = VARIANT_FALSE;
			script_invoke(callback_id::on_focus, args, _countof(args));
		}
		break;
	case callback_id::on_always_on_top_changed:
		on_always_on_top_changed(wp);
		return 0;
	case callback_id::on_colours_changed:
		on_colours_changed();
		return 0;
	case callback_id::on_cursor_follow_playback_changed:
		on_cursor_follow_playback_changed(wp);
		return 0;
	case callback_id::on_dsp_preset_changed:
		on_dsp_preset_changed();
		return 0;
	case callback_id::on_font_changed:
		on_font_changed();
		return 0;
	case callback_id::on_get_album_art_done:
		on_get_album_art_done(wp);
		return 0;
	case callback_id::on_item_focus_change:
		on_item_focus_change(wp);
		return 0;
	case callback_id::on_item_played:
		on_item_played(wp);
		return 0;
	case callback_id::on_library_items_added:
		on_library_items_added(wp);
		return 0;
	case callback_id::on_library_items_changed:
		on_library_items_changed(wp);
		return 0;
	case callback_id::on_library_items_removed:
		on_library_items_removed(wp);
		return 0;
	case callback_id::on_load_image_done:
		on_load_image_done(wp);
		return 0;
	case callback_id::on_main_menu:
		on_main_menu(wp);
		return 0;
	case callback_id::on_metadb_changed:
		on_metadb_changed(wp);
		return 0;
	case callback_id::on_notify_data:
		on_notify_data(wp);
		return 0;
	case callback_id::on_output_device_changed:
		on_output_device_changed();
		return 0;
	case callback_id::on_playback_dynamic_info:
		on_playback_dynamic_info();
		return 0;
	case callback_id::on_playback_dynamic_info_track:
		on_playback_dynamic_info_track();
		return 0;
	case callback_id::on_playback_edited:
		on_playback_edited(wp);
		return 0;
	case callback_id::on_playback_follow_cursor_changed:
		on_playback_follow_cursor_changed(wp);
		return 0;
	case callback_id::on_playback_new_track:
		on_playback_new_track(wp);
		return 0;
	case callback_id::on_playback_order_changed:
		on_playback_order_changed(wp);
		return 0;
	case callback_id::on_playback_pause:
		on_playback_pause(wp);
		return 0;
	case callback_id::on_playback_queue_changed:
		on_playback_queue_changed(wp);
		return 0;
	case callback_id::on_playback_seek:
		on_playback_seek(wp);
		return 0;
	case callback_id::on_playback_starting:
		on_playback_starting(wp, lp);
		return 0;
	case callback_id::on_playback_stop:
		on_playback_stop(wp);
		return 0;
	case callback_id::on_playback_time:
		on_playback_time(wp);
		return 0;
	case callback_id::on_playlist_item_ensure_visible:
		on_playlist_item_ensure_visible(wp, lp);
		return 0;
	case callback_id::on_playlist_items_added:
		on_playlist_items_added(wp);
		return 0;
	case callback_id::on_playlist_items_removed:
		on_playlist_items_removed(wp, lp);
		return 0;
	case callback_id::on_playlist_items_reordered:
		on_playlist_items_reordered(wp);
		return 0;
	case callback_id::on_playlist_items_selection_change:
		on_playlist_items_selection_change();
		return 0;
	case callback_id::on_playlist_stop_after_current_changed:
		on_playlist_stop_after_current_changed(wp);
		return 0;
	case callback_id::on_playlist_switch:
		on_playlist_switch();
		return 0;
	case callback_id::on_playlists_changed:
		on_playlists_changed();
		return 0;
	case callback_id::on_replaygain_mode_changed:
		on_replaygain_mode_changed(wp);
		return 0;
	case callback_id::on_selection_changed:
		on_selection_changed();
		return 0;
	case callback_id::on_volume_change:
		on_volume_change(wp);
		return 0;
	case UWM_TIMER:
		host_timer_dispatcher::instance().invoke_message(wp);
		return 0;
	case UWM_UNLOAD:
		unload_script();
		return 0;
	}
	return uDefWindowProc(hwnd, msg, wp, lp);
}

bool panel_window::on_mouse_button_up(UINT msg, WPARAM wp, LPARAM lp)
{
	bool ret = false;

	VARIANTARG args[3];
	args[0].vt = VT_I4;
	args[0].lVal = wp;
	args[1].vt = VT_I4;
	args[1].lVal = GET_Y_LPARAM(lp);
	args[2].vt = VT_I4;
	args[2].lVal = GET_X_LPARAM(lp);

	switch (msg)
	{
	case WM_LBUTTONUP:
		script_invoke(callback_id::on_mouse_lbtn_up, args, _countof(args));
		break;

	case WM_MBUTTONUP:
		script_invoke(callback_id::on_mouse_mbtn_up, args, _countof(args));
		break;

	case WM_RBUTTONUP:
		{
			// Bypass the user code?
			if (IsKeyPressed(VK_LSHIFT) && IsKeyPressed(VK_LWIN))
			{
				break;
			}

			_variant_t result;
			if (SUCCEEDED(script_invoke(callback_id::on_mouse_rbtn_up, args, _countof(args), &result)))
			{
				if (SUCCEEDED(VariantChangeType(&result, &result, 0, VT_BOOL)))
				{
					ret = !!result.boolVal;
				}
			}
		}
		break;
	}

	ReleaseCapture();
	return ret;
}

bool panel_window::show_configure_popup(HWND parent)
{
	modal_dialog_scope scope;
	if (scope.can_create())
	{
		scope.initialize(parent);
		CDialogConf dlg(this);
		return dlg.DoModal(parent) == IDOK;
	}
	return false;
}

ui_helpers::container_window::class_data& panel_window::get_class_data() const
{
	__implement_get_class_data_ex(JSP_NAME L" Class", L"", false, 0, WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, get_edge_style(), CS_DBLCLKS);
}

void panel_window::build_context_menu(HMENU menu, int x, int y, int id_base)
{
	uAppendMenu(menu, MF_STRING, id_base + 1, "&Reload");
	uAppendMenu(menu, MF_SEPARATOR, 0, 0);
	uAppendMenu(menu, MF_STRING, id_base + 2, "&Properties");
	uAppendMenu(menu, MF_STRING, id_base + 3, "&Configure...");
}

void panel_window::execute_context_menu_command(int id, int id_base)
{
	switch (id - id_base)
	{
	case 1:
		update_script();
		break;
	case 2:
		show_property_popup(m_hwnd);
		break;
	case 3:
		show_configure_popup(m_hwnd);
		break;
	}
}

void panel_window::load_script()
{
	pfc::hires_timer timer;
	timer.start();

	DWORD extstyle = GetWindowLongPtr(m_hwnd, GWL_EXSTYLE);
	extstyle &= ~WS_EX_CLIENTEDGE & ~WS_EX_STATICEDGE;
	extstyle |= get_edge_style();
	SetWindowLongPtr(m_hwnd, GWL_EXSTYLE, extstyle);
	SetWindowPos(m_hwnd, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

	m_max_size = { INT_MAX, INT_MAX };
	m_min_size = { 0, 0 };
	notify_size_limit_changed();
	m_dragdrop = false;
	m_grabfocus = false;

	if (FAILED(m_script_host->Initialise()))
	{
		return;
	}

	if (m_dragdrop)
	{
		m_drop_target.Attach(new com_object_impl_t<host_drop_target>(this));
		m_drop_target->RegisterDragDrop();
		m_is_droptarget_registered = true;
	}

	// HACK: Script update will not call on_size, so invoke it explicitly
	on_size(m_width, m_height);
	if (m_pseudo_transparent)
	{
		redraw();
	}
	else
	{
		repaint();
	}

	FB2K_console_formatter() << m_script_info.build_info_string() << ": initialised in " << (int)(timer.query() * 1000) << " ms";
}

void panel_window::on_always_on_top_changed(WPARAM wp)
{
	VARIANTARG args[1];
	args[0].vt = VT_BOOL;
	args[0].boolVal = TO_VARIANT_BOOL(wp);
	script_invoke(callback_id::on_always_on_top_changed, args, _countof(args));
}

void panel_window::on_colours_changed()
{
	script_invoke(callback_id::on_colours_changed);
}

void panel_window::on_context_menu(int x, int y)
{
	HMENU menu = CreatePopupMenu();
	const int base_id = 0;
	build_context_menu(menu, x, y, base_id);
	const int idx = TrackPopupMenu(menu, TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD, x, y, 0, m_hwnd, 0);
	execute_context_menu_command(idx, base_id);
	DestroyMenu(menu);
}

void panel_window::on_cursor_follow_playback_changed(WPARAM wp)
{
	VARIANTARG args[1];
	args[0].vt = VT_BOOL;
	args[0].boolVal = TO_VARIANT_BOOL(wp);
	script_invoke(callback_id::on_cursor_follow_playback_changed, args, _countof(args));
}

void panel_window::on_dsp_preset_changed()
{
	script_invoke(callback_id::on_dsp_preset_changed);
}

void panel_window::on_font_changed()
{
	script_invoke(callback_id::on_font_changed);
}

void panel_window::on_get_album_art_done(WPARAM wp)
{
	auto param = reinterpret_cast<helpers::album_art_async::t_param*>(wp);

	VARIANTARG args[4];
	args[0].vt = VT_BSTR;
	args[0].bstrVal = param->path;
	args[1].vt = VT_DISPATCH;
	args[1].pdispVal = param->bitmap;
	args[2].vt = VT_UI4;
	args[2].ulVal = param->art_id;
	args[3].vt = VT_DISPATCH;
	args[3].pdispVal = param->handle;
	script_invoke(callback_id::on_get_album_art_done, args, _countof(args));
}

void panel_window::on_item_focus_change(WPARAM wp)
{
	callback_data_scope_releaser<callback_data<t_size, t_size, t_size>> data(wp);

	VARIANTARG args[3];
	args[0].vt = VT_UI4;
	args[0].ulVal = data->m_item3;
	args[1].vt = VT_UI4;
	args[1].ulVal = data->m_item2;
	args[2].vt = VT_UI4;
	args[2].ulVal = data->m_item1;
	script_invoke(callback_id::on_item_focus_change, args, _countof(args));
}

void panel_window::on_item_played(WPARAM wp)
{
	callback_data_scope_releaser<callback_data<metadb_handle_ptr>> data(wp);
	auto handle = new com_object_impl_t<MetadbHandle>(data->m_item1);

	VARIANTARG args[1];
	args[0].vt = VT_DISPATCH;
	args[0].pdispVal = handle;
	script_invoke(callback_id::on_item_played, args, _countof(args));

	if (handle)
		handle->Release();
}

void panel_window::on_load_image_done(WPARAM wp)
{
	auto param = reinterpret_cast<helpers::load_image_async::t_param*>(wp);

	VARIANTARG args[3];
	args[0].vt = VT_BSTR;
	args[0].bstrVal = param->path;
	args[1].vt = VT_DISPATCH;
	args[1].pdispVal = param->bitmap;
	args[2].vt = VT_UI4;
	args[2].ulVal = param->cookie;
	script_invoke(callback_id::on_load_image_done, args, _countof(args));
}

void panel_window::on_library_items_added(WPARAM wp)
{
	callback_data_scope_releaser<metadb_callback_data> data(wp);
	auto handles = new com_object_impl_t<MetadbHandleList>(data->m_items);

	VARIANTARG args[1];
	args[0].vt = VT_DISPATCH;
	args[0].pdispVal = handles;
	script_invoke(callback_id::on_library_items_added, args, _countof(args));

	if (handles)
		handles->Release();
}

void panel_window::on_library_items_changed(WPARAM wp)
{
	callback_data_scope_releaser<metadb_callback_data> data(wp);
	auto handles = new com_object_impl_t<MetadbHandleList>(data->m_items);

	VARIANTARG args[1];
	args[0].vt = VT_DISPATCH;
	args[0].pdispVal = handles;
	script_invoke(callback_id::on_library_items_changed, args, _countof(args));

	if (handles)
		handles->Release();
}

void panel_window::on_library_items_removed(WPARAM wp)
{
	callback_data_scope_releaser<metadb_callback_data> data(wp);
	auto handles = new com_object_impl_t<MetadbHandleList>(data->m_items);

	VARIANTARG args[1];
	args[0].vt = VT_DISPATCH;
	args[0].pdispVal = handles;
	script_invoke(callback_id::on_library_items_removed, args, _countof(args));

	if (handles)
		handles->Release();
}

void panel_window::on_main_menu(WPARAM wp)
{
	VARIANTARG args[1];
	args[0].vt = VT_UI4;
	args[0].ulVal = wp;
	script_invoke(callback_id::on_main_menu, args, _countof(args));
}

void panel_window::on_metadb_changed(WPARAM wp)
{
	callback_data_scope_releaser<metadb_callback_data> data(wp);
	auto handles = new com_object_impl_t<MetadbHandleList>(data->m_items);

	VARIANTARG args[1];
	args[0].vt = VT_DISPATCH;
	args[0].pdispVal = handles;
	script_invoke(callback_id::on_metadb_changed, args, _countof(args));

	if (handles)
		handles->Release();
}

void panel_window::on_mouse_button_dblclk(UINT msg, WPARAM wp, LPARAM lp)
{
	VARIANTARG args[3];
	args[0].vt = VT_I4;
	args[0].lVal = wp;
	args[1].vt = VT_I4;
	args[1].lVal = GET_Y_LPARAM(lp);
	args[2].vt = VT_I4;
	args[2].lVal = GET_X_LPARAM(lp);

	switch (msg)
	{
	case WM_LBUTTONDBLCLK:
		script_invoke(callback_id::on_mouse_lbtn_dblclk, args, _countof(args));
		break;

	case WM_MBUTTONDBLCLK:
		script_invoke(callback_id::on_mouse_mbtn_dblclk, args, _countof(args));
		break;

	case WM_RBUTTONDBLCLK:
		script_invoke(callback_id::on_mouse_rbtn_dblclk, args, _countof(args));
		break;
	}
}

void panel_window::on_mouse_button_down(UINT msg, WPARAM wp, LPARAM lp)
{
	if (m_grabfocus)
	{
		SetFocus(m_hwnd);
	}

	SetCapture(m_hwnd);

	VARIANTARG args[3];
	args[0].vt = VT_I4;
	args[0].lVal = wp;
	args[1].vt = VT_I4;
	args[1].lVal = GET_Y_LPARAM(lp);
	args[2].vt = VT_I4;
	args[2].lVal = GET_X_LPARAM(lp);

	switch (msg)
	{
	case WM_LBUTTONDOWN:
		script_invoke(callback_id::on_mouse_lbtn_down, args, _countof(args));
		break;

	case WM_MBUTTONDOWN:
		script_invoke(callback_id::on_mouse_mbtn_down, args, _countof(args));
		break;

	case WM_RBUTTONDOWN:
		script_invoke(callback_id::on_mouse_rbtn_down, args, _countof(args));
		break;
	}
}

void panel_window::on_mouse_leave()
{
	m_is_mouse_tracked = false;

	script_invoke(callback_id::on_mouse_leave);
	SetCursor(LoadCursor(nullptr, IDC_ARROW));
}

void panel_window::on_mouse_move(WPARAM wp, LPARAM lp)
{
	if (!m_is_mouse_tracked)
	{
		TRACKMOUSEEVENT tme;

		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hwnd;
		tme.dwFlags = TME_LEAVE;
		TrackMouseEvent(&tme);
		m_is_mouse_tracked = true;
		SetCursor(LoadCursor(nullptr, IDC_ARROW));
	}

	VARIANTARG args[3];
	args[0].vt = VT_I4;
	args[0].lVal = wp;
	args[1].vt = VT_I4;
	args[1].lVal = GET_Y_LPARAM(lp);
	args[2].vt = VT_I4;
	args[2].lVal = GET_X_LPARAM(lp);
	script_invoke(callback_id::on_mouse_move, args, _countof(args));
}

void panel_window::on_mouse_wheel(WPARAM wp)
{
	VARIANTARG args[1];
	args[0].vt = VT_I4;
	args[0].lVal = GET_WHEEL_DELTA_WPARAM(wp) > 0 ? 1 : -1;
	script_invoke(callback_id::on_mouse_wheel, args, _countof(args));
}

void panel_window::on_mouse_wheel_h(WPARAM wp)
{
	VARIANTARG args[1];
	args[0].vt = VT_I4;
	args[0].lVal = GET_WHEEL_DELTA_WPARAM(wp) > 0 ? 1 : -1;
	script_invoke(callback_id::on_mouse_wheel_h, args, _countof(args));
}

void panel_window::on_notify_data(WPARAM wp)
{
	callback_data_scope_releaser<callback_data<_bstr_t, _variant_t>> data(wp);

	VARIANTARG args[2];
	args[0] = data->m_item2;
	args[1].vt = VT_BSTR;
	args[1].bstrVal = data->m_item1;
	script_invoke(callback_id::on_notify_data, args, _countof(args));
}

void panel_window::on_output_device_changed()
{
	script_invoke(callback_id::on_output_device_changed);
}

void panel_window::on_paint(HDC dc, LPRECT lpUpdateRect)
{
	if (!dc || !lpUpdateRect || !m_gr_bmp || !m_gr_wrap) return;

	HDC memdc = CreateCompatibleDC(dc);
	HBITMAP oldbmp = SelectBitmap(memdc, m_gr_bmp);

	if (m_script_host->HasError())
	{
		on_paint_error(memdc);
	}
	else
	{
		if (m_pseudo_transparent)
		{
			HDC bkdc = CreateCompatibleDC(dc);
			HBITMAP bkoldbmp = SelectBitmap(bkdc, m_gr_bmp_bk);

			BitBlt(
				memdc,
				lpUpdateRect->left,
				lpUpdateRect->top,
				lpUpdateRect->right - lpUpdateRect->left,
				lpUpdateRect->bottom - lpUpdateRect->top,
				bkdc,
				lpUpdateRect->left,
				lpUpdateRect->top,
				SRCCOPY);

			SelectBitmap(bkdc, bkoldbmp);
			DeleteDC(bkdc);
		}
		else
		{
			RECT rc = { 0, 0, m_width, m_height };

			FillRect(memdc, &rc, (HBRUSH)(COLOR_WINDOW + 1));
		}

		on_paint_user(memdc, lpUpdateRect);
	}

	BitBlt(dc, 0, 0, m_width, m_height, memdc, 0, 0, SRCCOPY);
	SelectBitmap(memdc, oldbmp);
	DeleteDC(memdc);
}

void panel_window::on_paint_error(HDC memdc)
{
	const wchar_t errmsg[] = L"Aw, crashed :(";
	RECT rc = { 0, 0, m_width, m_height };
	SIZE sz = { 0 };

	HFONT newfont = CreateFont(
		20,
		0,
		0,
		0,
		FW_BOLD,
		FALSE,
		FALSE,
		FALSE,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		L"Tahoma");

	HFONT oldfont = (HFONT)SelectObject(memdc, newfont);

	{
		LOGBRUSH lbBack = { BS_SOLID, RGB(225, 60, 45), 0 };
		HBRUSH hBack = CreateBrushIndirect(&lbBack);

		FillRect(memdc, &rc, hBack);
		SetBkMode(memdc, TRANSPARENT);

		SetTextColor(memdc, RGB(255, 255, 255));
		DrawText(memdc, errmsg, -1, &rc, DT_CENTER | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);

		DeleteObject(hBack);
	}

	SelectObject(memdc, oldfont);
}

void panel_window::on_paint_user(HDC memdc, LPRECT lpUpdateRect)
{
	if (m_script_host->Ready())
	{
		Gdiplus::Graphics gr(memdc);
		Gdiplus::Rect rect(lpUpdateRect->left, lpUpdateRect->top, lpUpdateRect->right - lpUpdateRect->left, lpUpdateRect->bottom - lpUpdateRect->top);
		gr.SetClip(rect);
		m_gr_wrap->put__ptr(&gr);

		{
			VARIANTARG args[1];
			args[0].vt = VT_DISPATCH;
			args[0].pdispVal = m_gr_wrap;
			script_invoke(callback_id::on_paint, args, _countof(args));
		}

		m_gr_wrap->put__ptr(nullptr);
	}
}

void panel_window::on_playback_dynamic_info()
{
	script_invoke(callback_id::on_playback_dynamic_info);
}

void panel_window::on_playback_dynamic_info_track()
{
	script_invoke(callback_id::on_playback_dynamic_info_track);
}

void panel_window::on_playback_edited(WPARAM wp)
{
	callback_data_scope_releaser<callback_data<metadb_handle_ptr>> data(wp);
	auto handle = new com_object_impl_t<MetadbHandle>(data->m_item1);

	VARIANTARG args[1];
	args[0].vt = VT_DISPATCH;
	args[0].pdispVal = handle;
	script_invoke(callback_id::on_playback_edited, args, _countof(args));

	if (handle)
		handle->Release();
}

void panel_window::on_playback_follow_cursor_changed(WPARAM wp)
{
	VARIANTARG args[1];
	args[0].vt = VT_BOOL;
	args[0].boolVal = TO_VARIANT_BOOL(wp);
	script_invoke(callback_id::on_playback_follow_cursor_changed, args, _countof(args));
}

void panel_window::on_playback_new_track(WPARAM wp)
{
	callback_data_scope_releaser<callback_data<metadb_handle_ptr>> data(wp);
	auto handle = new com_object_impl_t<MetadbHandle>(data->m_item1);

	VARIANTARG args[1];
	args[0].vt = VT_DISPATCH;
	args[0].pdispVal = handle;
	script_invoke(callback_id::on_playback_new_track, args, _countof(args));

	if (handle)
		handle->Release();
}

void panel_window::on_playback_order_changed(WPARAM wp)
{
	VARIANTARG args[1];
	args[0].vt = VT_UI4;
	args[0].ulVal = wp;
	script_invoke(callback_id::on_playback_order_changed, args, _countof(args));
}

void panel_window::on_playback_pause(WPARAM wp)
{
	VARIANTARG args[1];
	args[0].vt = VT_BOOL;
	args[0].boolVal = TO_VARIANT_BOOL(wp);
	script_invoke(callback_id::on_playback_pause, args, _countof(args));
}

void panel_window::on_playback_queue_changed(WPARAM wp)
{
	VARIANTARG args[1];
	args[0].vt = VT_UI4;
	args[0].ulVal = wp;
	script_invoke(callback_id::on_playback_queue_changed, args, _countof(args));
}

void panel_window::on_playback_seek(WPARAM wp)
{
	callback_data_scope_releaser<callback_data<double>> data(wp);

	VARIANTARG args[1];
	args[0].vt = VT_R8;
	args[0].dblVal = data->m_item1;
	script_invoke(callback_id::on_playback_seek, args, _countof(args));
}

void panel_window::on_playback_starting(WPARAM wp, LPARAM lp)
{
	VARIANTARG args[2];
	args[0].vt = VT_BOOL;
	args[0].boolVal = TO_VARIANT_BOOL(lp);
	args[1].vt = VT_UI4;
	args[1].ulVal = wp;
	script_invoke(callback_id::on_playback_starting, args, _countof(args));
}

void panel_window::on_playback_stop(WPARAM wp)
{
	VARIANTARG args[1];
	args[0].vt = VT_UI4;
	args[0].ulVal = wp;
	script_invoke(callback_id::on_playback_stop, args, _countof(args));
}

void panel_window::on_playback_time(WPARAM wp)
{
	callback_data_scope_releaser<callback_data<double>> data(wp);

	VARIANTARG args[1];
	args[0].vt = VT_R8;
	args[0].dblVal = data->m_item1;
	script_invoke(callback_id::on_playback_time, args, _countof(args));
}

void panel_window::on_playlist_item_ensure_visible(WPARAM wp, LPARAM lp)
{
	VARIANTARG args[2];
	args[0].vt = VT_UI4;
	args[0].ulVal = lp;
	args[1].vt = VT_UI4;
	args[1].ulVal = wp;
	script_invoke(callback_id::on_playlist_item_ensure_visible, args, _countof(args));
}

void panel_window::on_playlist_items_added(WPARAM wp)
{
	VARIANTARG args[1];
	args[0].vt = VT_UI4;
	args[0].ulVal = wp;
	script_invoke(callback_id::on_playlist_items_added, args, _countof(args));
}

void panel_window::on_playlist_items_removed(WPARAM wp, LPARAM lp)
{
	VARIANTARG args[2];
	args[0].vt = VT_UI4;
	args[0].ulVal = lp;
	args[1].vt = VT_UI4;
	args[1].ulVal = wp;
	script_invoke(callback_id::on_playlist_items_removed, args, _countof(args));
}

void panel_window::on_playlist_items_reordered(WPARAM wp)
{
	VARIANTARG args[1];
	args[0].vt = VT_UI4;
	args[0].ulVal = wp;
	script_invoke(callback_id::on_playlist_items_reordered, args, _countof(args));
}

void panel_window::on_playlist_items_selection_change()
{
	script_invoke(callback_id::on_playlist_items_selection_change);
}

void panel_window::on_playlist_stop_after_current_changed(WPARAM wp)
{
	VARIANTARG args[1];
	args[0].vt = VT_BOOL;
	args[0].boolVal = TO_VARIANT_BOOL(wp);
	script_invoke(callback_id::on_playlist_stop_after_current_changed, args, _countof(args));
}

void panel_window::on_playlist_switch()
{
	script_invoke(callback_id::on_playlist_switch);
}

void panel_window::on_playlists_changed()
{
	script_invoke(callback_id::on_playlists_changed);
}

void panel_window::on_replaygain_mode_changed(WPARAM wp)
{
	VARIANTARG args[1];
	args[0].vt = VT_UI4;
	args[0].ulVal = wp;
	script_invoke(callback_id::on_replaygain_mode_changed, args, _countof(args));
}

void panel_window::on_selection_changed()
{
	script_invoke(callback_id::on_selection_changed);
}

void panel_window::on_size(int w, int h)
{
	m_width = w;
	m_height = h;

	delete_context();
	create_context();

	script_invoke(callback_id::on_size);
}

void panel_window::on_volume_change(WPARAM wp)
{
	callback_data_scope_releaser<callback_data<float>> data(wp);

	VARIANTARG args[1];
	args[0].vt = VT_R4;
	args[0].fltVal = data->m_item1;
	script_invoke(callback_id::on_volume_change, args, _countof(args));
}

void panel_window::show_property_popup(HWND parent)
{
	modal_dialog_scope scope;
	if (scope.can_create())
	{
		scope.initialize(parent);
		CDialogProperty dlg(this);
		dlg.DoModal(parent);
	}
}

void panel_window::unload_script()
{
	m_script_host->Finalise();

	if (m_is_droptarget_registered)
	{
		m_drop_target->RevokeDragDrop();
		m_is_droptarget_registered = false;
	}

	host_timer_dispatcher::instance().kill_timers(m_hwnd);
	m_selection_holder.release();
}

void panel_window::update_script()
{
	unload_script();
	load_script();
}
