#include "stdafx.h"
#include "FileInfo.h"

FileInfo::FileInfo(file_info_impl* p_info_ptr) : m_info_ptr(p_info_ptr) {}
FileInfo::~FileInfo() {}

void FileInfo::FinalRelease()
{
	if (m_info_ptr)
	{
		delete m_info_ptr;
		m_info_ptr = nullptr;
	}
}

STDMETHODIMP FileInfo::get__ptr(void** pp)
{
	if (!pp) return E_POINTER;

	*pp = m_info_ptr;
	return S_OK;
}

STDMETHODIMP FileInfo::InfoFind(BSTR name, int* p)
{
	if (!m_info_ptr || !p) return E_POINTER;

	*p = TO_INT(m_info_ptr->info_find(string_utf8_from_wide(name)));
	return S_OK;
}

STDMETHODIMP FileInfo::InfoName(UINT idx, BSTR* p)
{
	if (!m_info_ptr || !p) return E_POINTER;

	if (idx < m_info_ptr->info_get_count())
	{
		*p = TO_BSTR(m_info_ptr->info_enum_name(idx));
		return S_OK;
	}
	return E_INVALIDARG;
}

STDMETHODIMP FileInfo::InfoValue(UINT idx, BSTR* p)
{
	if (!m_info_ptr || !p) return E_POINTER;

	if (idx < m_info_ptr->info_get_count())
	{
		*p = TO_BSTR(m_info_ptr->info_enum_value(idx));
		return S_OK;
	}
	return E_INVALIDARG;
}

STDMETHODIMP FileInfo::MetaFind(BSTR name, int* p)
{
	if (!m_info_ptr || !p) return E_POINTER;

	*p = TO_INT(m_info_ptr->meta_find(string_utf8_from_wide(name)));
	return S_OK;
}

STDMETHODIMP FileInfo::MetaName(UINT idx, BSTR* p)
{
	if (!m_info_ptr || !p) return E_POINTER;

	if (idx < m_info_ptr->meta_get_count())
	{
		*p = TO_BSTR(m_info_ptr->meta_enum_name(idx));
		return S_OK;
	}
	return E_INVALIDARG;
}

STDMETHODIMP FileInfo::MetaValue(UINT idx, UINT vidx, BSTR* p)
{
	if (!m_info_ptr || !p) return E_POINTER;

	if (idx < m_info_ptr->meta_get_count() && vidx < m_info_ptr->meta_enum_value_count(idx))
	{
		*p = TO_BSTR(m_info_ptr->meta_enum_value(idx, vidx));
		return S_OK;
	}
	return E_INVALIDARG;
}

STDMETHODIMP FileInfo::MetaValueCount(UINT idx, UINT* p)
{
	if (!m_info_ptr || !p) return E_POINTER;

	if (idx < m_info_ptr->meta_get_count())
	{
		*p = m_info_ptr->meta_enum_value_count(idx);
		return S_OK;
	}
	return E_INVALIDARG;
}

STDMETHODIMP FileInfo::get_InfoCount(UINT* p)
{
	if (!m_info_ptr || !p) return E_POINTER;

	*p = m_info_ptr->info_get_count();
	return S_OK;
}

STDMETHODIMP FileInfo::get_MetaCount(UINT* p)
{
	if (!m_info_ptr || !p) return E_POINTER;

	*p = m_info_ptr->meta_get_count();
	return S_OK;
}
