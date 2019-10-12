#pragma once

template <typename T1, typename T2 = char, typename T3 = char>
struct callback_data : public pfc::refcounted_object_root
{
	callback_data(const T1& p_item1) : m_item1(p_item1) {}
	callback_data(const T1& p_item1, const T2& p_item2) : m_item1(p_item1), m_item2(p_item2) {}
	callback_data(const T1& p_item1, const T2& p_item2, const T3& p_item3) : m_item1(p_item1), m_item2(p_item2), m_item3(p_item3) {}

	T1 m_item1;
	T2 m_item2;
	T3 m_item3;
};

struct metadb_callback_data : public pfc::refcounted_object_root
{
	metadb_callback_data(metadb_handle_list_cref p_items) : m_items(p_items) {}

	metadb_handle_list m_items;
};

template <class T>
class callback_data_scope_releaser
{
public:
	template <class TParam>
	callback_data_scope_releaser(TParam p_data)
	{
		m_data = reinterpret_cast<T*>(p_data);
	}

	template <class TParam>
	callback_data_scope_releaser(TParam* p_data)
	{
		m_data = reinterpret_cast<T*>(p_data);
	}

	~callback_data_scope_releaser()
	{
		m_data->refcount_release();
	}

	T* operator->()
	{
		return m_data;
	}

private:
	T* m_data;
};

class panel_manager
{
public:
	panel_manager();

	using api_item = std::pair<std::string, t_size>;

	static panel_manager& instance();

	std::vector<api_item> get_apis();
	void add_window(HWND p_wnd);
	void notify_others(HWND p_wnd_except, UINT p_msg, pfc::refcounted_object_root* p_param);
	void post_msg_to_all(UINT p_msg, WPARAM p_wp = 0, LPARAM p_lp = 0);
	void post_msg_to_all_pointer(UINT p_msg, pfc::refcounted_object_root* p_param);
	void remove_window(HWND p_wnd);
	void unload_all();

private:
	static panel_manager instance_;

	std::set<HWND> m_hwnds;
	std::vector<api_item> m_apis;

	PFC_CLASS_NOT_COPYABLE_EX(panel_manager)
};
