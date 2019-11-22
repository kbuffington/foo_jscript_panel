#include "stdafx.h"
#include "TitleFormat.h"
#include "helpers.h"

TitleFormat::TitleFormat(BSTR pattern)
{
	titleformat_compiler::get()->compile_safe(m_obj, string_utf8_from_wide(pattern));
}

TitleFormat::~TitleFormat() {}

void TitleFormat::FinalRelease()
{
	m_obj.release();
}

STDMETHODIMP TitleFormat::get__ptr(void** pp)
{
	if (!pp) return E_POINTER;

	*pp = m_obj.get_ptr();
	return S_OK;
}

STDMETHODIMP TitleFormat::Eval(BSTR* p)
{
	if (m_obj.is_empty() || !p) return E_POINTER;

	pfc::string8_fast str;
	playback_control::get()->playback_format_title(nullptr, str, m_obj, nullptr, playback_control::display_level_all);
	*p = TO_BSTR(str);
	return S_OK;
}

STDMETHODIMP TitleFormat::EvalWithMetadb(IMetadbHandle* handle, BSTR* p)
{
	if (m_obj.is_empty() || !p) return E_POINTER;

	metadb_handle* ptr = nullptr;
	GET_PTR(handle, ptr)

	pfc::string8_fast str;
	ptr->format_title(nullptr, str, m_obj, nullptr);
	*p = TO_BSTR(str);
	return S_OK;
}

STDMETHODIMP TitleFormat::EvalWithMetadbs(IMetadbHandleList* handles, VARIANT* p)
{
	if (m_obj.is_empty() || !p) return E_POINTER;

	metadb_handle_list* handles_ptr = nullptr;
	GET_PTR(handles, handles_ptr)

	const t_size count = handles_ptr->get_count();

	str_vec strings;
	for (t_size i = 0; i < count; ++i)
	{
		pfc::string8_fast str;
		handles_ptr->get_item(i)->format_title(nullptr, str, m_obj, nullptr);
		strings.emplace_back(str.get_ptr());
	}

	com_array helper;
	if (!helper.create(strings)) return E_OUTOFMEMORY;

	p->vt = VT_ARRAY | VT_VARIANT;
	p->parray = helper.get_ptr();
	return S_OK;
}
