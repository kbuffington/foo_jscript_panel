#include "stdafx.h"
#include "MetadbHandle.h"
#include "helpers.h"
#include "stats.h"

MetadbHandle::MetadbHandle(const metadb_handle_ptr& src) : m_handle(src) {}
MetadbHandle::~MetadbHandle() {}

void MetadbHandle::FinalRelease()
{
	m_handle.release();
}

STDMETHODIMP MetadbHandle::get__ptr(void** pp)
{
	if (!pp) return E_POINTER;

	*pp = m_handle.get_ptr();
	return S_OK;
}

STDMETHODIMP MetadbHandle::ClearStats()
{
	if (m_handle.is_empty()) return E_POINTER;

	metadb_index_hash hash;
	if (stats::hashHandle(m_handle, hash))
	{
		stats::set(hash, stats::fields());
	}
	return S_OK;
}

STDMETHODIMP MetadbHandle::Compare(IMetadbHandle* handle, VARIANT_BOOL* p)
{
	if (m_handle.is_empty() || !p) return E_POINTER;

	metadb_handle* ptr = nullptr;
	GET_PTR(handle, ptr)

	*p = TO_VARIANT_BOOL(ptr == m_handle.get_ptr());
	return S_OK;
}

STDMETHODIMP MetadbHandle::GetAlbumArt(UINT art_id, VARIANT_BOOL need_stub, VARIANT* p)
{
	if (m_handle.is_empty() || !p) return E_POINTER;

	pfc::string8_fast image_path;
	IGdiBitmap* bitmap = helpers::get_album_art(m_handle, art_id, need_stub != VARIANT_FALSE, false, image_path);

	_variant_t var1, var2;
	var1.vt = VT_DISPATCH;
	var1.pdispVal = bitmap;
	var2.vt = VT_BSTR;
	var2.bstrVal = TO_BSTR(image_path);

	com_array helper;
	if (!helper.create(2)) return E_OUTOFMEMORY;
	if (!helper.put_item(0, var1)) return E_OUTOFMEMORY;
	if (!helper.put_item(1, var2)) return E_OUTOFMEMORY;

	p->vt = VT_ARRAY | VT_VARIANT;
	p->parray = helper.get_ptr();
	return S_OK;
}

STDMETHODIMP MetadbHandle::GetFileInfo(IFileInfo** pp)
{
	if (m_handle.is_empty() || !pp) return E_POINTER;

	auto info_ptr = new file_info_impl;
	m_handle->get_info(*info_ptr);
	*pp = new com_object_impl_t<FileInfo>(info_ptr);
	return S_OK;
}

STDMETHODIMP MetadbHandle::RefreshStats()
{
	if (m_handle.is_empty()) return E_POINTER;

	metadb_index_hash hash;
	if (stats::hashHandle(m_handle, hash))
	{
		stats::theAPI()->dispatch_refresh(jsp::guids::metadb_index, hash);
	}
	return S_OK;
}

STDMETHODIMP MetadbHandle::SetFirstPlayed(BSTR first_played)
{
	if (m_handle.is_empty()) return E_POINTER;

	metadb_index_hash hash;
	if (stats::hashHandle(m_handle, hash))
	{
		stats::fields tmp = stats::get(hash);
		auto fp = string_utf8_from_wide(first_played);
		if (!tmp.first_played.equals(fp))
		{
			tmp.first_played = fp;
			stats::set(hash, tmp);
		}
	}
	return S_OK;
}

STDMETHODIMP MetadbHandle::SetLastPlayed(BSTR last_played)
{
	if (m_handle.is_empty()) return E_POINTER;

	metadb_index_hash hash;
	if (stats::hashHandle(m_handle, hash))
	{
		stats::fields tmp = stats::get(hash);
		auto lp = string_utf8_from_wide(last_played);
		if (!tmp.last_played.equals(lp))
		{
			tmp.last_played = lp;
			stats::set(hash, tmp);
		}
	}
	return S_OK;
}

STDMETHODIMP MetadbHandle::SetLoved(UINT loved)
{
	if (m_handle.is_empty()) return E_POINTER;

	metadb_index_hash hash;
	if (stats::hashHandle(m_handle, hash))
	{
		stats::fields tmp = stats::get(hash);
		if (tmp.loved != loved)
		{
			tmp.loved = loved;
			stats::set(hash, tmp);
		}
	}
	return S_OK;
}

STDMETHODIMP MetadbHandle::SetPlaycount(UINT playcount)
{
	if (m_handle.is_empty()) return E_POINTER;

	metadb_index_hash hash;
	if (stats::hashHandle(m_handle, hash))
	{
		stats::fields tmp = stats::get(hash);
		if (tmp.playcount != playcount)
		{
			tmp.playcount = playcount;
			stats::set(hash, tmp);
		}
	}
	return S_OK;
}

STDMETHODIMP MetadbHandle::SetRating(UINT rating)
{
	if (m_handle.is_empty()) return E_POINTER;

	metadb_index_hash hash;
	if (stats::hashHandle(m_handle, hash))
	{
		stats::fields tmp = stats::get(hash);
		if (tmp.rating != rating)
		{
			tmp.rating = rating;
			stats::set(hash, tmp);
		}
	}
	return S_OK;
}

STDMETHODIMP MetadbHandle::get_FileSize(__int64* p)
{
	if (m_handle.is_empty() || !p) return E_POINTER;

	*p = m_handle->get_filesize();
	return S_OK;
}

STDMETHODIMP MetadbHandle::get_Length(double* p)
{
	if (m_handle.is_empty() || !p) return E_POINTER;

	*p = m_handle->get_length();
	return S_OK;
}

STDMETHODIMP MetadbHandle::get_Path(BSTR* p)
{
	if (m_handle.is_empty() || !p) return E_POINTER;

	*p = TO_BSTR(file_path_display(m_handle->get_path()));
	return S_OK;
}

STDMETHODIMP MetadbHandle::get_RawPath(BSTR* p)
{
	if (m_handle.is_empty() || !p) return E_POINTER;

	*p = TO_BSTR(m_handle->get_path());
	return S_OK;
}

STDMETHODIMP MetadbHandle::get_SubSong(UINT* p)
{
	if (m_handle.is_empty() || !p) return E_POINTER;

	*p = m_handle->get_subsong_index();
	return S_OK;
}
