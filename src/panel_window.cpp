#include "stdafx.h"
#include "host_drop_target.h"
#include "host_timer_dispatcher.h"
#include "panel_manager.h"
#include "panel_window.h"
#include "ui_conf.h"
#include "ui_property.h"

panel_window::panel_window() : m_script_host(new script_host(this)), m_panel_tooltip_param_ptr(new panel_tooltip_param)
{
	m_panel_config.reset();
	m_size.reset_min_max();
}

panel_window::~panel_window()
{
	m_script_host->Release();
}

bool panel_window::handle_message(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_CREATE:
		m_hwnd = hwnd;
		m_hdc = m_hwnd.GetDC();
		create_context();
		m_gr_wrap.Attach(new com_object_impl_t<GdiGraphics>(), false);
		panel_manager::instance().add_window(m_hwnd);
		load_script();
		return true;
	case WM_DESTROY:
		unload_script();
		panel_manager::instance().remove_window(m_hwnd);
		if (m_gr_wrap)
		{
			m_gr_wrap.Release();
		}
		delete_context();
		m_hwnd.ReleaseDC(m_hdc);
		return true;
	case WM_DISPLAYCHANGE:
	case WM_THEMECHANGED:
		update_script();
		return true;
	case WM_PAINT:
		{
			if (is_transparent() && m_refreshbk)
			{
				refresh_background();
			}
			else
			{
				on_paint();
			}
		}
		return true;
	case WM_SIZE:
		{
			CRect rc;
			m_hwnd.GetClientRect(&rc);
			m_size.width = rc.Width();
			m_size.height = rc.Height();
			on_size();
		}
		return true;
	case WM_GETMINMAXINFO:
		{
			auto info = reinterpret_cast<LPMINMAXINFO>(lp);
			info->ptMaxTrackSize = m_size.max;
			info->ptMinTrackSize = m_size.min;
		}
		return true;
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		{
			if (m_grabfocus)
			{
				m_hwnd.SetFocus();
			}

			m_hwnd.SetCapture();

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
		return false;
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
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
					script_invoke(callback_id::on_mouse_rbtn_up, args, _countof(args), &result);
					if (SUCCEEDED(VariantChangeType(&result, &result, 0, VT_BOOL)))
					{
						ret = !!result.boolVal;
					}
				}
				break;
			}

			ReleaseCapture();
			return ret;
		}
	case WM_LBUTTONDBLCLK:
	case WM_MBUTTONDBLCLK:
	case WM_RBUTTONDBLCLK:
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
		return false;
	case WM_MOUSEMOVE:
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
		return false;
	case WM_MOUSELEAVE:
		m_is_mouse_tracked = false;

		script_invoke(callback_id::on_mouse_leave);
		SetCursor(LoadCursor(nullptr, IDC_ARROW));
		return false;
	case WM_MOUSEWHEEL:
	case WM_MOUSEHWHEEL:
		{
			VARIANTARG args[1];
			args[0].vt = VT_I4;
			args[0].lVal = GET_WHEEL_DELTA_WPARAM(wp) > 0 ? 1 : -1;
			script_invoke(msg == WM_MOUSEWHEEL ? callback_id::on_mouse_wheel : callback_id::on_mouse_wheel_h, args, _countof(args));
		}
		return false;
	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:
		{
			VARIANTARG args[1];
			args[0].vt = VT_UI4;
			args[0].ulVal = wp;
			script_invoke(callback_id::on_key_down, args, _countof(args));
		}
		return true;
	case WM_KEYUP:
		{
			VARIANTARG args[1];
			args[0].vt = VT_UI4;
			args[0].ulVal = wp;
			script_invoke(callback_id::on_key_up, args, _countof(args));
		}
		return true;
	case WM_CHAR:
		{
			VARIANTARG args[1];
			args[0].vt = VT_UI4;
			args[0].ulVal = wp;
			script_invoke(callback_id::on_char, args, _countof(args));
		}
		return true;
	case WM_SETFOCUS:
		{
			m_selection_holder = ui_selection_manager::get()->acquire();

			VARIANTARG args[1];
			args[0].vt = VT_BOOL;
			args[0].boolVal = VARIANT_TRUE;
			script_invoke(callback_id::on_focus, args, _countof(args));
		}
		return false;
	case WM_KILLFOCUS:
		{
			m_selection_holder.release();

			VARIANTARG args[1];
			args[0].vt = VT_BOOL;
			args[0].boolVal = VARIANT_FALSE;
			script_invoke(callback_id::on_focus, args, _countof(args));
		}
		return false;
	case jsp::uwm_refreshbk:
		redraw(true);
		return true;
	case jsp::uwm_timer:
		host_timer_dispatcher::instance().invoke_message(wp);
		return true;
	case jsp::uwm_unload:
		unload_script();
		return true;
	}

	callback_id id = static_cast<callback_id>(msg);

	switch (id)
	{
	case callback_id::on_dsp_preset_changed:
	case callback_id::on_output_device_changed:
	case callback_id::on_playback_dynamic_info:
	case callback_id::on_playback_dynamic_info_track:
	case callback_id::on_playlist_items_selection_change:
	case callback_id::on_playlist_switch:
	case callback_id::on_playlists_changed:
	case callback_id::on_selection_changed:
		script_invoke(id);
		return true;
	case callback_id::on_always_on_top_changed:
	case callback_id::on_cursor_follow_playback_changed:
	case callback_id::on_playback_follow_cursor_changed:
	case callback_id::on_playback_pause:
	case callback_id::on_playlist_stop_after_current_changed:
		{
			VARIANTARG args[1];
			args[0].vt = VT_BOOL;
			args[0].boolVal = TO_VARIANT_BOOL(wp);
			script_invoke(id, args, _countof(args));
		}
		return true;
	case callback_id::on_item_played:
	case callback_id::on_playback_edited:
	case callback_id::on_playback_new_track:
		{
			callback_data_scope_releaser<callback_data<metadb_handle_ptr>> data(wp);
			auto handle = new com_object_impl_t<MetadbHandle>(data->m_item1);

			VARIANTARG args[1];
			args[0].vt = VT_DISPATCH;
			args[0].pdispVal = handle;
			script_invoke(id, args, _countof(args));

			if (handle)
				handle->Release();
		}
		return true;
	case callback_id::on_library_items_added:
	case callback_id::on_library_items_changed:
	case callback_id::on_library_items_removed:
	case callback_id::on_metadb_changed:
		{
			callback_data_scope_releaser<metadb_callback_data> data(wp);
			auto handles = new com_object_impl_t<MetadbHandleList>(data->m_handles);

			VARIANTARG args[1];
			args[0].vt = VT_DISPATCH;
			args[0].pdispVal = handles;
			script_invoke(id, args, _countof(args));

			if (handles)
				handles->Release();
		}
		return true;
	case callback_id::on_main_menu:
	case callback_id::on_playback_order_changed:
	case callback_id::on_playback_queue_changed:
	case callback_id::on_playback_stop:
	case callback_id::on_playlist_items_added:
	case callback_id::on_playlist_items_reordered:
	case callback_id::on_replaygain_mode_changed:
		{
			VARIANTARG args[1];
			args[0].vt = VT_UI4;
			args[0].ulVal = wp;
			script_invoke(id, args, _countof(args));
		}
		return true;
	case callback_id::on_volume_change:
		{
			callback_data_scope_releaser<callback_data<float>> data(wp);

			VARIANTARG args[1];
			args[0].vt = VT_R4;
			args[0].fltVal = data->m_item1;
			script_invoke(id, args, _countof(args));
		}
		return true;
	case callback_id::on_playback_seek:
	case callback_id::on_playback_time:
		{
			callback_data_scope_releaser<callback_data<double>> data(wp);

			VARIANTARG args[1];
			args[0].vt = VT_R8;
			args[0].dblVal = data->m_item1;
			script_invoke(id, args, _countof(args));
		}
		return true;
	case callback_id::on_get_album_art_done:
		{
			auto param = reinterpret_cast<helpers::album_art_async::t_param*>(wp);

			VARIANTARG args[4];
			args[0].vt = VT_BSTR;
			args[0].bstrVal = param->m_path;
			args[1].vt = VT_DISPATCH;
			args[1].pdispVal = param->m_bitmap;
			args[2].vt = VT_UI4;
			args[2].ulVal = param->m_art_id;
			args[3].vt = VT_DISPATCH;
			args[3].pdispVal = param->m_handle;
			script_invoke(id, args, _countof(args));
		}
		return true;
	case callback_id::on_item_focus_change:
		{
			callback_data_scope_releaser<callback_data<size_t, size_t, size_t>> data(wp);

			VARIANTARG args[3];
			args[0].vt = VT_UI4;
			args[0].ulVal = data->m_item3;
			args[1].vt = VT_UI4;
			args[1].ulVal = data->m_item2;
			args[2].vt = VT_UI4;
			args[2].ulVal = data->m_item1;
			script_invoke(id, args, _countof(args));
		}
		return true;
	case callback_id::on_load_image_done:
		{
			auto param = reinterpret_cast<helpers::load_image_async::t_param*>(wp);

			VARIANTARG args[3];
			args[0].vt = VT_BSTR;
			args[0].bstrVal = param->m_path;
			args[1].vt = VT_DISPATCH;
			args[1].pdispVal = param->m_bitmap;
			args[2].vt = VT_UI4;
			args[2].ulVal = param->m_cookie;
			script_invoke(id, args, _countof(args));
		}
		return true;
	case callback_id::on_notify_data:
		{
			callback_data_scope_releaser<callback_data<_bstr_t, _variant_t>> data(wp);

			VARIANTARG args[2];
			args[0] = data->m_item2;
			args[1].vt = VT_BSTR;
			args[1].bstrVal = data->m_item1;
			script_invoke(id, args, _countof(args));
		}
		return true;
	case callback_id::on_playback_starting:
		{
			VARIANTARG args[2];
			args[0].vt = VT_BOOL;
			args[0].boolVal = TO_VARIANT_BOOL(lp);
			args[1].vt = VT_UI4;
			args[1].ulVal = wp;
			script_invoke(id, args, _countof(args));
		}
		return true;
	case callback_id::on_playlist_item_ensure_visible:
	case callback_id::on_playlist_items_removed:
		{
			VARIANTARG args[2];
			args[0].vt = VT_UI4;
			args[0].ulVal = lp;
			args[1].vt = VT_UI4;
			args[1].ulVal = wp;
			script_invoke(id, args, _countof(args));
		}
		return true;
	}
	return false;
}

bool panel_window::is_dui()
{
	return m_instance_type == instance_type::dui;
}

bool panel_window::is_transparent()
{
	return m_supports_transparency && m_panel_config.transparent;
}

panel_tooltip_param_ptr& panel_window::panel_tooltip()
{
	return m_panel_tooltip_param_ptr;
}

void panel_window::build_context_menu(HMENU menu, int id_base)
{
	uAppendMenu(menu, MF_STRING, id_base + 1, "&Reload");
	uAppendMenu(menu, MF_SEPARATOR, 0, 0);
	uAppendMenu(menu, MF_STRING, id_base + 2, "&Properties");
	uAppendMenu(menu, MF_STRING, id_base + 3, "&Configure...");
}

void panel_window::create_context()
{
	delete_context();

	m_gr_bmp = CreateCompatibleBitmap(m_hdc, m_size.width, m_size.height);

	if (is_transparent())
	{
		m_gr_bmp_bk = CreateCompatibleBitmap(m_hdc, m_size.width, m_size.height);
	}
}

void panel_window::delete_context()
{
	if (m_gr_bmp)
	{
		DeleteBitmap(m_gr_bmp);
		m_gr_bmp = nullptr;
	}

	if (m_gr_bmp_bk)
	{
		DeleteBitmap(m_gr_bmp_bk);
		m_gr_bmp_bk = nullptr;
	}
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

	if (!is_dui())
	{
		DWORD extstyle = m_hwnd.GetWindowLongPtr(GWL_EXSTYLE);
		extstyle &= ~WS_EX_CLIENTEDGE & ~WS_EX_STATICEDGE;
		extstyle |= m_panel_config.get_edge_style();
		m_hwnd.SetWindowLongPtr(GWL_EXSTYLE, extstyle);
	}

	m_hwnd.SetWindowPos(nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

	m_size.reset_min_max();
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
	}

	// HACK: Script update will not call on_size, so invoke it explicitly
	on_size();

	FB2K_console_formatter() << m_script_host->m_info.build_info_string() << ": initialised in " << static_cast<int>(timer.query() * 1000) << " ms";
}

void panel_window::on_context_menu(LPARAM lp)
{
	CPoint pt(lp);
	if (pt.x == -1 && pt.y == -1)
	{
		GetMessagePos(&pt);

		CRect rc;
		m_hwnd.GetWindowRect(&rc);

		if (!rc.PtInRect(pt))
		{
			pt.SetPoint(0, 0);
			m_hwnd.MapWindowPoints(nullptr, &pt, 1);
			pt.Offset(m_size.width / 2, m_size.height / 2);
		}
	}

	CMenu menu = CreatePopupMenu();
	constexpr int base_id = 0;
	build_context_menu(menu, base_id);
	const int idx = menu.TrackPopupMenu(TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD, pt.x, pt.y, m_hwnd, nullptr);
	execute_context_menu_command(idx, base_id);
}

void panel_window::on_paint()
{
	if (!m_gr_bmp || !m_gr_wrap) return;

	CPaintDC paintdc(m_hwnd);
	CDC memdc = CreateCompatibleDC(paintdc);
	SelectObjectScope scope_bmp(memdc, m_gr_bmp);
	CRect rc(0, 0, m_size.width, m_size.height);

	if (m_script_host->HasError())
	{
		CFont font = CreateFont(
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

		LOGBRUSH lbBack = { BS_SOLID, RGB(225, 60, 45), 0 };
		CBrush hBack = CreateBrushIndirect(&lbBack);
		SelectObjectScope scope(memdc, font);

		memdc.FillRect(&rc, hBack);
		memdc.SetBkMode(TRANSPARENT);
		memdc.SetTextColor(RGB(255, 255, 255));
		memdc.DrawText(L"Aw, crashed :(", -1, &rc, DT_CENTER | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);
	}
	else
	{
		if (is_transparent())
		{
			CDC bkdc = CreateCompatibleDC(paintdc);
			SelectObjectScope scope_bk(bkdc, m_gr_bmp_bk);
			memdc.BitBlt(paintdc.m_ps.rcPaint.left, paintdc.m_ps.rcPaint.top, RECT_CX(paintdc.m_ps.rcPaint), RECT_CY(paintdc.m_ps.rcPaint), bkdc, paintdc.m_ps.rcPaint.left, paintdc.m_ps.rcPaint.top, SRCCOPY);
		}
		else
		{
			memdc.FillRect(&rc, reinterpret_cast<HBRUSH>(COLOR_WINDOWFRAME));
		}

		if (m_script_host->Ready())
		{
			Gdiplus::Graphics gr(memdc);
			const Gdiplus::Rect rect(paintdc.m_ps.rcPaint.left, paintdc.m_ps.rcPaint.top, RECT_CX(paintdc.m_ps.rcPaint), RECT_CY(paintdc.m_ps.rcPaint));
			gr.SetClip(rect);
			m_gr_wrap->put__ptr(&gr);

			VARIANTARG args[1];
			args[0].vt = VT_DISPATCH;
			args[0].pdispVal = m_gr_wrap;
			script_invoke(callback_id::on_paint, args, _countof(args));

			m_gr_wrap->put__ptr(nullptr);
		}
	}
	paintdc.BitBlt(0, 0, m_size.width, m_size.height, memdc, 0, 0, SRCCOPY);
	m_refreshbk = true;
}

void panel_window::on_size()
{
	if (m_size.width == 0 || m_size.height == 0) return;
	create_context();
	script_invoke(callback_id::on_size);
	if (is_transparent()) m_hwnd.PostMessage(jsp::uwm_refreshbk);
	else repaint();
}

void panel_window::redraw(bool refreshbk)
{
	m_refreshbk = refreshbk;
	m_hwnd.RedrawWindow(nullptr, nullptr, RDW_INVALIDATE | RDW_UPDATENOW);
}

void panel_window::refresh_background()
{
	CWindow wnd_parent = GetAncestor(m_hwnd, GA_PARENT);

	if (!wnd_parent || IsIconic(core_api::get_main_window()) || !IsWindowVisible(m_hwnd))
		return;

	CPoint pt(0, 0);
	m_hwnd.ClientToScreen(&pt);
	wnd_parent.ScreenToClient(&pt);

	CRect rc(0, 0, m_size.width, m_size.height);
	CRect rect_parent(pt, rc.Size());

	CRgn rgn = CreateRectRgn(0, 0, 0, 0);
	m_hwnd.SetWindowRgn(rgn);
	wnd_parent.RedrawWindow(&rect_parent, nullptr, RDW_INVALIDATE | RDW_ERASE | RDW_ERASENOW);

	// Background bitmap
	{
		CClientDC dc_parent(wnd_parent);
		CDC hdc_bk = CreateCompatibleDC(dc_parent);
		SelectObjectScope scope(hdc_bk, m_gr_bmp_bk);
		hdc_bk.BitBlt(rc.left, rc.top, rc.Width(), rc.Height(), dc_parent, pt.x, pt.y, SRCCOPY);
	}

	m_hwnd.SetWindowRgn(nullptr);
	if (m_panel_config.style != panel_config::edge_style::none) m_hwnd.SendMessage(WM_NCPAINT, 1, 0);
	redraw(false);
}

void panel_window::repaint()
{
	m_refreshbk = false;
	m_hwnd.InvalidateRect(nullptr, FALSE);
}

void panel_window::repaint_rect(int x, int y, int w, int h)
{
	m_refreshbk = false;
	CRect rc(x, y, x + w, y + h);
	m_hwnd.InvalidateRect(&rc, FALSE);
}

void panel_window::script_invoke(callback_id id, VARIANTARG* argv, size_t argc, VARIANT* ret) const
{
	m_script_host->InvokeCallback(id, argv, argc, ret);
}

void panel_window::show_configure_popup(HWND parent)
{
	modal_dialog_scope scope;
	if (scope.can_create())
	{
		scope.initialize(parent);
		CDialogConf dlg(this);
		dlg.DoModal(parent);
	}
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

	if (m_dragdrop)
	{
		m_drop_target->RevokeDragDrop();
	}

	host_timer_dispatcher::instance().kill_timers(m_hwnd);
	m_selection_holder.release();
}

void panel_window::update_script()
{
	unload_script();
	load_script();
}
