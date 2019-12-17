#pragma once
#include "callback_data.h"

class panel_manager
{
public:
	panel_manager();

	using api_item = std::pair<std::string, size_t>;

	static panel_manager& instance();

	std::vector<api_item> get_apis();
	void add_window(CWindow hwnd);
	void notify_others(CWindow hwnd_except, pfc::refcounted_object_root* param);
	void post_msg_to_all(callback_id id, WPARAM wp = 0, LPARAM lp = 0);
	void post_msg_to_all_pointer(callback_id id, pfc::refcounted_object_root* param);
	void remove_window(CWindow hwnd);
	void unload_all();

private:
	static panel_manager instance_;

	std::set<CWindow> m_hwnds;
	std::vector<api_item> m_apis;

	PFC_CLASS_NOT_COPYABLE_EX(panel_manager)
};
