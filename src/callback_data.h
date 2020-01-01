#pragma once

template <typename T1, typename T2 = char, typename T3 = char>
struct callback_data : public pfc::refcounted_object_root
{
	callback_data(const T1& item1) : m_item1(item1) {}
	callback_data(const T1& item1, const T2& item2) : m_item1(item1), m_item2(item2) {}
	callback_data(const T1& item1, const T2& item2, const T3& item3) : m_item1(item1), m_item2(item2), m_item3(item3) {}

	T1 m_item1;
	T2 m_item2;
	T3 m_item3;
};

struct metadb_callback_data : public pfc::refcounted_object_root
{
	metadb_callback_data(metadb_handle_list_cref handles) : m_handles(handles) {}

	metadb_handle_list m_handles;
};

template <class T>
class callback_data_scope_releaser
{
public:
	template <class TParam>
	callback_data_scope_releaser(TParam data)
	{
		m_data = reinterpret_cast<T*>(data);
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
