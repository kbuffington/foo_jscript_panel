#include "stdafx.h"
#include "com_tools.h"

bool name_to_id_cache::lookup(ULONG hash, DISPID* p_dispid) const
{
	DISPID dispId;
	if (!m_map.query(hash, dispId)) return false;
	*p_dispid = dispId;
	return true;
}

void name_to_id_cache::add(ULONG hash, DISPID dispid)
{
	m_map[hash] = dispid;
}

type_info_cache_holder::type_info_cache_holder() : m_type_info(NULL) {}

HRESULT type_info_cache_holder::GetIDsOfNames(LPOLESTR* rgszNames, UINT cNames, MEMBERID* pMemId)
{
	PFC_ASSERT(m_type_info != NULL);
	for (t_size i = 0; i < cNames; ++i)
	{
		ULONG hash = LHashValOfName(LANG_NEUTRAL, rgszNames[i]);
		if (!m_cache.lookup(hash, &pMemId[i]))
		{
			HRESULT hr = m_type_info->GetIDsOfNames(&rgszNames[i], 1, &pMemId[i]);
			if (FAILED(hr)) return hr;
			m_cache.add(hash, pMemId[i]);
		}
	}
	return S_OK;
}

HRESULT type_info_cache_holder::GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo** ppTInfo)
{
	if (empty()) return E_UNEXPECTED;
	if (!ppTInfo) return E_POINTER;
	if (iTInfo != 0) return DISP_E_BADINDEX;
	m_type_info->AddRef();
	*ppTInfo = m_type_info.GetInterfacePtr();
	return S_OK;
}

HRESULT type_info_cache_holder::Invoke(PVOID pvInstance, MEMBERID memid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr)
{
	PFC_ASSERT(m_type_info != NULL);
	HRESULT hr = m_type_info->Invoke(pvInstance, memid, wFlags, pDispParams, pVarResult, pExcepInfo, puArgErr);
	PFC_ASSERT(hr != RPC_E_WRONG_THREAD);
	return hr;
}

ITypeInfo* type_info_cache_holder::get_ptr() throw()
{
	return m_type_info;
}

bool type_info_cache_holder::empty() throw()
{
	return m_type_info == NULL;
}

bool type_info_cache_holder::valid() throw()
{
	return m_type_info != NULL;
}

void type_info_cache_holder::init_from_typelib(ITypeLib* p_typeLib, const GUID& guid)
{
	p_typeLib->GetTypeInfoOfGuid(guid, &m_type_info);
}

void type_info_cache_holder::set_type_info(ITypeInfo* type_info)
{
	m_type_info = type_info;
}
