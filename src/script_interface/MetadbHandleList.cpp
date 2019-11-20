#include "stdafx.h"
#include "MetadbHandleList.h"
#include "helpers.h"
#include "stats.h"

MetadbHandleList::MetadbHandleList(metadb_handle_list_cref handles) : m_handles(handles) {}
MetadbHandleList::~MetadbHandleList() {}

void MetadbHandleList::FinalRelease()
{
	m_handles.remove_all();
}

STDMETHODIMP MetadbHandleList::get__ptr(void** pp)
{
	if (!pp) return E_POINTER;

	*pp = &m_handles;
	return S_OK;
}

STDMETHODIMP MetadbHandleList::Add(IMetadbHandle* handle)
{
	metadb_handle* ptr = nullptr;
	GET_PTR(handle, ptr)

	m_handles.add_item(ptr);
	return S_OK;
}

STDMETHODIMP MetadbHandleList::AddRange(IMetadbHandleList* handles)
{
	metadb_handle_list* handles_ptr = nullptr;
	GET_PTR(handles, handles_ptr)

	m_handles.add_items(*handles_ptr);
	return S_OK;
}

STDMETHODIMP MetadbHandleList::AttachImage(BSTR path, UINT art_id)
{
	if (m_handles.get_count() == 0) return E_POINTER;

	album_art_data_ptr data;

	try
	{
		auto upath = string_utf8_from_wide(path);
		if (!filesystem::g_is_remote_or_unrecognized(upath))
		{
			file::ptr file;
			filesystem::g_open(file, upath, filesystem::open_mode_read, fb2k::noAbort);
			if (file.is_valid())
			{
				auto tmp = fb2k::service_new<album_art_data_impl>();
				tmp->from_stream(file.get_ptr(), t_size(file->get_size_ex(fb2k::noAbort)), fb2k::noAbort);
				data = tmp;
			}
		}
	}
	catch (...) {}

	if (data.is_valid())
	{
		auto cb = fb2k::service_new<helpers::embed>(helpers::embed::actions::attach, data, m_handles, art_id);
		threaded_process::get()->run_modeless(cb, helpers::threaded_process_flags, core_api::get_main_window(), "Embedding image...");
	}
	return S_OK;
}

STDMETHODIMP MetadbHandleList::BSearch(IMetadbHandle* handle, int* p)
{
	if (!p) return E_POINTER;

	metadb_handle* ptr = nullptr;
	GET_PTR(handle, ptr)

	*p = m_handles.bsearch_by_pointer(ptr);
	return S_OK;
}

STDMETHODIMP MetadbHandleList::CalcTotalDuration(double* p)
{
	if (!p) return E_POINTER;

	*p = m_handles.calc_total_duration();
	return S_OK;
}

STDMETHODIMP MetadbHandleList::CalcTotalSize(__int64* p)
{
	if (!p) return E_POINTER;

	*p = metadb_handle_list_helper::calc_total_size(m_handles, true);
	return S_OK;
}

STDMETHODIMP MetadbHandleList::Clone(IMetadbHandleList** pp)
{
	if (!pp) return E_POINTER;

	*pp = new com_object_impl_t<MetadbHandleList>(m_handles);
	return S_OK;
}

STDMETHODIMP MetadbHandleList::Convert(VARIANT* p)
{
	if (!p) return E_POINTER;

	const t_size count = m_handles.get_count();
	com_array helper;
	if (!helper.create(count)) return E_OUTOFMEMORY;

	for (t_size i = 0; i < count; ++i)
	{
		_variant_t var;
		var.vt = VT_DISPATCH;
		var.pdispVal = new com_object_impl_t<MetadbHandle>(m_handles[i]);
		if (!helper.put_item(i, var)) return E_OUTOFMEMORY;
	}
	p->vt = VT_ARRAY | VT_VARIANT;
	p->parray = helper.get_ptr();
	return S_OK;
}

STDMETHODIMP MetadbHandleList::Find(IMetadbHandle* handle, int* p)
{
	if (!p) return E_POINTER;

	metadb_handle* ptr = nullptr;
	GET_PTR(handle, ptr)

	*p = m_handles.find_item(ptr);
	return S_OK;
}

STDMETHODIMP MetadbHandleList::GetLibraryRelativePaths(VARIANT* p)
{
	if (!p) return E_POINTER;

	const t_size count = m_handles.get_count();
	com_array helper;
	if (!helper.create(count)) return E_OUTOFMEMORY;

	pfc::string8_fastalloc str;
	str.prealloc(512);

	auto api = library_manager::get();

	for (t_size i = 0; i < count; ++i)
	{
		metadb_handle_ptr item = m_handles[i];
		if (!api->get_relative_path(item, str)) str = "";
		_variant_t var;
		var.vt = VT_BSTR;
		var.bstrVal = TO_BSTR(str);
		if (!helper.put_item(i, var)) return E_OUTOFMEMORY;
	}
	p->vt = VT_ARRAY | VT_VARIANT;
	p->parray = helper.get_ptr();
	return S_OK;
}

STDMETHODIMP MetadbHandleList::Insert(UINT index, IMetadbHandle* handle)
{
	metadb_handle* ptr = nullptr;
	GET_PTR(handle, ptr)

	m_handles.insert_item(ptr, index);
	return S_OK;
}

STDMETHODIMP MetadbHandleList::InsertRange(UINT index, IMetadbHandleList* handles)
{
	metadb_handle_list* handles_ptr = nullptr;
	GET_PTR(handles, handles_ptr)

	m_handles.insert_items(*handles_ptr, index);
	return S_OK;
}

STDMETHODIMP MetadbHandleList::MakeDifference(IMetadbHandleList* handles)
{
	metadb_handle_list* handles_ptr = nullptr;
	GET_PTR(handles, handles_ptr)

	metadb_handle_list r1, r2;
	metadb_handle_list_helper::sorted_by_pointer_extract_difference(m_handles, *handles_ptr, r1, r2);
	m_handles = r1;
	return S_OK;
}

STDMETHODIMP MetadbHandleList::MakeIntersection(IMetadbHandleList* handles)
{
	metadb_handle_list* handles_ptr = nullptr;
	GET_PTR(handles, handles_ptr)

	const metadb_handle_list_ref handles_ref = *handles_ptr;
	metadb_handle_list result;
	t_size walk1 = 0;
	t_size walk2 = 0;
	const t_size last1 = m_handles.get_count();
	const t_size last2 = handles_ptr->get_count();

	while (walk1 != last1 && walk2 != last2)
	{
		if (m_handles[walk1] < handles_ref[walk2])
			++walk1;
		else if (handles_ref[walk2] < m_handles[walk1])
			++walk2;
		else
		{
			result.add_item(m_handles[walk1]);
			++walk1;
			++walk2;
		}
	}

	m_handles = result;
	return S_OK;
}

STDMETHODIMP MetadbHandleList::MakeUnion(IMetadbHandleList* handles)
{
	metadb_handle_list* handles_ptr = nullptr;
	GET_PTR(handles, handles_ptr)

	m_handles.add_items(*handles_ptr);
	m_handles.sort_by_pointer_remove_duplicates();
	return S_OK;
}

STDMETHODIMP MetadbHandleList::OptimiseFileLayout(VARIANT_BOOL minimise)
{
	if (m_handles.get_count() == 0) return E_POINTER;

	auto flags = helpers::threaded_process_flags | threaded_process::flag_show_abort;
	auto cb = fb2k::service_new<helpers::optimise_layout>(m_handles, minimise != VARIANT_FALSE);
	threaded_process::get()->run_modeless(cb, flags, core_api::get_main_window(), "Optimising...");
	return S_OK;
}

STDMETHODIMP MetadbHandleList::OrderByFormat(__interface ITitleFormat* script, int direction)
{
	titleformat_object* obj = nullptr;
	GET_PTR(script, obj)

	m_handles.sort_by_format(obj, nullptr, direction);
	return S_OK;
}

STDMETHODIMP MetadbHandleList::OrderByPath()
{
	m_handles.sort_by_path();
	return S_OK;
}

STDMETHODIMP MetadbHandleList::OrderByRelativePath()
{
	// lifted from metadb_handle_list.cpp - adds subsong index for better sorting. github issue #16
	auto api = library_manager::get();
	const t_size count = m_handles.get_count();
	t_size i;

	std::vector<helpers::custom_sort_data> data(count);

	pfc::string8_fastalloc str;
	str.prealloc(512);

	for (i = 0; i < count; ++i)
	{
		metadb_handle_ptr item = m_handles[i];
		if (!api->get_relative_path(item, str)) str = "";
		str << item->get_subsong_index();
		data[i].index = i;
		data[i].text = helpers::make_sort_string(str);
	}

	pfc::sort_t(data, helpers::custom_sort_compare<1>, count);
	order_helper order(count);

	for (i = 0; i < count; ++i)
	{
		order[i] = data[i].index;
		delete[] data[i].text;
	}

	m_handles.reorder(order.get_ptr());
	return S_OK;
}

STDMETHODIMP MetadbHandleList::RefreshStats()
{
	stats::hash_set hashes;
	stats::get_hashes(m_handles, hashes);

	pfc::list_t<metadb_index_hash> to_refresh;
	for (const auto& hash : hashes)
	{
		to_refresh += hash;
	}
	stats::theAPI()->dispatch_refresh(jsp_guids::metadb_index, to_refresh);
	return S_OK;
}

STDMETHODIMP MetadbHandleList::Remove(IMetadbHandle* handle)
{
	metadb_handle* ptr = nullptr;
	GET_PTR(handle, ptr)

	m_handles.remove_item(ptr);
	return S_OK;
}

STDMETHODIMP MetadbHandleList::RemoveAll()
{
	m_handles.remove_all();
	return S_OK;
}

STDMETHODIMP MetadbHandleList::RemoveAttachedImage(UINT art_id)
{
	if (m_handles.get_count() == 0) return E_POINTER;

	auto cb = fb2k::service_new<helpers::embed>(helpers::embed::actions::remove, album_art_data_ptr(), m_handles, art_id);
	threaded_process::get()->run_modeless(cb, helpers::threaded_process_flags, core_api::get_main_window(), "Removing images...");
	return S_OK;
}

STDMETHODIMP MetadbHandleList::RemoveAttachedImages()
{
	if (m_handles.get_count() == 0) return E_POINTER;

	auto cb = fb2k::service_new<helpers::embed>(helpers::embed::actions::remove_all, album_art_data_ptr(), m_handles, 0);
	threaded_process::get()->run_modeless(cb, helpers::threaded_process_flags, core_api::get_main_window(), "Removing images...");
	return S_OK;
}

STDMETHODIMP MetadbHandleList::RemoveById(UINT index)
{
	if (index < m_handles.get_count())
	{
		m_handles.remove_by_idx(index);
		return S_OK;
	}
	return E_INVALIDARG;
}

STDMETHODIMP MetadbHandleList::RemoveRange(UINT from, UINT count)
{
	m_handles.remove_from_idx(from, count);
	return S_OK;
}

STDMETHODIMP MetadbHandleList::Sort()
{
	m_handles.sort_by_pointer_remove_duplicates();
	return S_OK;
}

STDMETHODIMP MetadbHandleList::UpdateFileInfoFromJSON(BSTR str)
{
	const t_size count = m_handles.get_count();
	if (count == 0) return E_POINTER;

	json j = json::parse(string_utf8_from_wide(str).get_ptr(), nullptr, false);

	if (j.is_array() && j.size() == count)
	{
		std::vector<file_info_impl> info(count);

		for (t_size i = 0; i < count; ++i)
		{
			if (!j[i].is_object() || j[i].size() == 0) return E_INVALIDARG;

			info[i] = m_handles[i]->get_info_ref()->info();

			for (auto& [name, value] : j[i].items())
			{
				if (name.empty()) return E_INVALIDARG;

				info[i].meta_remove_field(name.c_str());

				for (auto& v : helpers::js_file_info_filter::get_values(value))
				{
					info[i].meta_add(name.c_str(), v.c_str());
				}
			}
		}

		metadb_io_v2::get()->update_info_async_simple(
			m_handles,
			pfc::ptr_list_const_array_t<const file_info, file_info_impl*>(info.data(), info.size()),
			core_api::get_main_window(),
			metadb_io_v2::op_flag_delay_ui,
			nullptr
		);
		return S_OK;
	}
	else if (j.is_object() && j.size() > 0)
	{
		std::vector<helpers::js_file_info_filter::tag> tags;

		for (auto& [name, value] : j.items())
		{
			if (name.empty()) return E_INVALIDARG;

			helpers::js_file_info_filter::tag t;
			t.first = name;
			t.second = helpers::js_file_info_filter::get_values(value);
			tags.emplace_back(t);
		}

		metadb_io_v2::get()->update_info_async(
			m_handles,
			fb2k::service_new<helpers::js_file_info_filter>(tags),
			core_api::get_main_window(),
			metadb_io_v2::op_flag_delay_ui,
			nullptr
		);
		return S_OK;
	}
	else
	{
		return E_INVALIDARG;
	}
}

STDMETHODIMP MetadbHandleList::get_Count(UINT* p)
{
	if (!p) return E_POINTER;

	*p = m_handles.get_count();
	return S_OK;
}

STDMETHODIMP MetadbHandleList::get_Item(UINT index, IMetadbHandle** pp)
{
	if (!pp) return E_POINTER;

	if (index < m_handles.get_count())
	{
		*pp = new com_object_impl_t<MetadbHandle>(m_handles.get_item_ref(index));
		return S_OK;
	}
	return E_INVALIDARG;
}

STDMETHODIMP MetadbHandleList::put_Item(UINT index, IMetadbHandle* handle)
{
	if (index < m_handles.get_count())
	{
		metadb_handle* ptr = nullptr;
		GET_PTR(handle, ptr)

		m_handles.replace_item(index, ptr);
		return S_OK;
	}
	return E_INVALIDARG;
}
