#include "stdafx.h"
#include "panel_manager.h"

panel_manager::panel_manager() {}

panel_manager panel_manager::instance_;

panel_manager& panel_manager::instance()
{
	return instance_;
}

panel_manager::api_list panel_manager::get_apis()
{
	if (m_apis.empty())
	{
		puResource pures = uLoadResource(core_api::get_my_instance(), uMAKEINTRESOURCE(IDR_API), "TEXT");
		pfc::string8_fast content(static_cast<const char*>(pures->GetPointer()), pures->GetSize());
		pfc::string_list_impl list;
		pfc::splitStringByLines(list, content);

		for (t_size i = 0; i < list.get_count(); ++i)
		{
			pfc::string8_fast tmp = list[i];
			if (tmp.get_length())
			{
				m_apis.emplace_back(tmp);
			}
		}

		std::sort(m_apis.begin(), m_apis.end(), [](const pfc::string_simple& one, const pfc::string_simple& two) -> bool
		{
			return _stricmp(one, two) < 0;
		});
	}
	return m_apis;
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
