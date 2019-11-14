#include "stdafx.h"
#include "helpers.h"
#include "panel_manager.h"

panel_manager::panel_manager() {}

panel_manager panel_manager::instance_;

panel_manager& panel_manager::instance()
{
	return instance_;
}

std::vector<panel_manager::api_item> panel_manager::get_apis()
{
	if (m_apis.empty())
	{
		std::string content = helpers::get_resource_text(IDR_API).get_ptr();

		str_list lines = helpers::split_string(content, "\r\n");
		for (const auto& line : lines)
		{
			if (line.empty()) continue;
			api_item item = {line, std::min({line.find('('), line.find(' '), line.length()})};
			m_apis.emplace_back(item);
		}

		std::sort(m_apis.begin(), m_apis.end(), [](const auto& one, const auto& two) -> bool
			{
				return _stricmp(one.first.c_str(), two.first.c_str()) < 0;
			});
	}
	return m_apis;
}

void panel_manager::add_window(HWND p_wnd)
{
	m_hwnds.insert(p_wnd);
}

void panel_manager::notify_others(HWND p_wnd_except, pfc::refcounted_object_root* p_param)
{
	const t_size count = m_hwnds.size();

	if (count < 2 || !p_param) return;

	for (t_size i = 0; i < count - 1; ++i)
	{
		p_param->refcount_add_ref();
	}

	for (const auto& hwnd : m_hwnds)
	{
		if (hwnd != p_wnd_except)
		{
			SendMessage(hwnd, static_cast<unsigned int>(callback_id::on_notify_data), reinterpret_cast<WPARAM>(p_param), 0);
		}
	}
}

void panel_manager::post_msg_to_all(callback_id id, WPARAM p_wp, LPARAM p_lp)
{
	for (const auto& hwnd : m_hwnds)
	{
		PostMessage(hwnd, static_cast<unsigned int>(id), p_wp, p_lp);
	}
}

void panel_manager::post_msg_to_all_pointer(callback_id id, pfc::refcounted_object_root* p_param)
{
	const t_size count = m_hwnds.size();

	if (count == 0 || !p_param) return;

	for (t_size i = 0; i < count; ++i)
	{
		p_param->refcount_add_ref();
	}

	for (const auto& hwnd : m_hwnds)
	{
		PostMessage(hwnd, static_cast<unsigned int>(id), reinterpret_cast<WPARAM>(p_param), 0);
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
