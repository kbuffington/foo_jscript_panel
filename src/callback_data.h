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
