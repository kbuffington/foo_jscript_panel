#include "stdafx.h"
#include "panel_manager.h"

panel_manager::panel_manager() {}

panel_manager panel_manager::instance_;

panel_manager& panel_manager::instance()
{
	return instance_;
}

void panel_manager::add_window(HWND p_wnd)
{
	m_hwnds.insert(p_wnd);
}

void panel_manager::notify_others(HWND p_wnd_except, UINT p_msg, pfc::refcounted_object_root* p_param)
{
	t_size count = m_hwnds.size();

	if (count < 2 || !p_param) return;

	for (t_size i = 0; i < count - 1; ++i)
	{
		p_param->refcount_add_ref();
	}

	for (const auto& hwnd : m_hwnds)
	{
		if (hwnd != p_wnd_except)
		{
			SendMessage(hwnd, p_msg, reinterpret_cast<WPARAM>(p_param), 0);
		}
	}
}

void panel_manager::post_msg_to_all(UINT p_msg, WPARAM p_wp, LPARAM p_lp)
{
	for (const auto& hwnd : m_hwnds)
	{
		PostMessage(hwnd, p_msg, p_wp, p_lp);
	}
}

void panel_manager::post_msg_to_all_pointer(UINT p_msg, pfc::refcounted_object_root* p_param)
{
	t_size count = m_hwnds.size();

	if (count == 0 || !p_param) return;

	for (t_size i = 0; i < count; ++i)
	{
		p_param->refcount_add_ref();
	}

	for (const auto& hwnd : m_hwnds)
	{
		PostMessage(hwnd, p_msg, reinterpret_cast<WPARAM>(p_param), 0);
	}
}

void panel_manager::remove_window(HWND p_wnd)
{
	m_hwnds.erase(p_wnd);
}

void panel_manager::unload_all()
{
	for (const auto& hwnd : m_hwnds)
	{
		SendMessage(hwnd, UWM_UNLOAD, 0, 0);
	}
}
