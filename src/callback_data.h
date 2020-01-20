#pragma once
#include "script_interface.h"

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

struct async_art_data
{
	async_art_data(IMetadbHandle* handle, size_t art_id, IGdiBitmap* bitmap, BSTR path) : m_handle(handle), m_art_id(art_id), m_bitmap(bitmap), m_path(path) {}

	~async_art_data()
	{
		if (m_handle)
		{
			m_handle->Release();
		}

		if (m_bitmap)
		{
			m_bitmap->Release();
		}
	}

	IGdiBitmap* m_bitmap;
	IMetadbHandle* m_handle;
	_bstr_t m_path;
	size_t m_art_id;
};

struct async_image_data
{
	async_image_data(size_t cookie, IGdiBitmap* bitmap, BSTR path) : m_cookie(cookie), m_bitmap(bitmap), m_path(path) {}

	~async_image_data()
	{
		if (m_bitmap)
		{
			m_bitmap->Release();
		}
	}

	IGdiBitmap* m_bitmap;
	_bstr_t m_path;
	size_t m_cookie;
};
