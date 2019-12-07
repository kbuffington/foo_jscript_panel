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

		str_vec lines = helpers::split_string(content, "\r\n");
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

void panel_manager::add_window(HWND hwnd)
{
	m_hwnds.insert(hwnd);
}

void panel_manager::notify_others(HWND hwnd_except, pfc::refcounted_object_root* param)
{
	const size_t count = m_hwnds.size();

	if (count < 2 || !param) return;

	for (size_t i = 0; i < count - 1; ++i)
	{
		param->refcount_add_ref();
	}

	for (const auto& hwnd : m_hwnds)
	{
		if (hwnd != hwnd_except)
		{
			SendMessage(hwnd, TO_UINT(callback_id::on_notify_data), reinterpret_cast<WPARAM>(param), 0);
		}
	}
}

void panel_manager::post_msg_to_all(callback_id id, WPARAM wp, LPARAM lp)
{
	for (const auto& hwnd : m_hwnds)
	{
		PostMessage(hwnd, TO_UINT(id), wp, lp);
	}
}

void panel_manager::post_msg_to_all_pointer(callback_id id, pfc::refcounted_object_root* param)
{
	const size_t count = m_hwnds.size();

	if (count == 0 || !param) return;

	for (size_t i = 0; i < count; ++i)
	{
		param->refcount_add_ref();
	}

	for (const auto& hwnd : m_hwnds)
	{
		PostMessage(hwnd, TO_UINT(id), reinterpret_cast<WPARAM>(param), 0);
	}
}

void panel_manager::remove_window(HWND hwnd)
{
	m_hwnds.erase(hwnd);
}

void panel_manager::unload_all()
{
	for (const auto& hwnd : m_hwnds)
	{
		SendMessage(hwnd, jsp::uwm_unload, 0, 0);
	}
}
