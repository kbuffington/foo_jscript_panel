#pragma once
#include "script_interface.h"

class MetadbHandleList : public IDisposableImpl4<IMetadbHandleList>
{
protected:
	MetadbHandleList(metadb_handle_list_cref handles);
	~MetadbHandleList();

	void FinalRelease() override;

	metadb_handle_list m_handles;

public:
	STDMETHODIMP get__ptr(void** pp) override;
	STDMETHODIMP Add(IMetadbHandle* handle) override;
	STDMETHODIMP AddRange(IMetadbHandleList* handles) override;
	STDMETHODIMP AttachImage(BSTR path, UINT art_id) override;
	STDMETHODIMP BSearch(IMetadbHandle* handle, int* p) override;
	STDMETHODIMP CalcTotalDuration(double* p) override;
	STDMETHODIMP CalcTotalSize(UINT64* p) override;
	STDMETHODIMP Clone(IMetadbHandleList** pp) override;
	STDMETHODIMP Convert(VARIANT* p) override;
	STDMETHODIMP Find(IMetadbHandle* handle, int* p) override;
	STDMETHODIMP GetLibraryRelativePaths(VARIANT* p) override;
	STDMETHODIMP Insert(UINT index, IMetadbHandle* handle) override;
	STDMETHODIMP InsertRange(UINT index, IMetadbHandleList* handles) override;
	STDMETHODIMP MakeDifference(IMetadbHandleList* handles) override;
	STDMETHODIMP MakeIntersection(IMetadbHandleList* handles) override;
	STDMETHODIMP MakeUnion(IMetadbHandleList* handles) override;
	STDMETHODIMP OptimiseFileLayout(VARIANT_BOOL minimise) override;
	STDMETHODIMP OrderByFormat(__interface ITitleFormat* script, int direction) override;
	STDMETHODIMP OrderByPath() override;
	STDMETHODIMP OrderByRelativePath() override;
	STDMETHODIMP RefreshStats() override;
	STDMETHODIMP Remove(IMetadbHandle* handle) override;
	STDMETHODIMP RemoveAll() override;
	STDMETHODIMP RemoveAttachedImage(UINT art_id) override;
	STDMETHODIMP RemoveAttachedImages() override;
	STDMETHODIMP RemoveById(UINT index) override;
	STDMETHODIMP RemoveRange(UINT from, UINT count) override;
	STDMETHODIMP Sort() override;
	STDMETHODIMP UpdateFileInfoFromJSON(BSTR str) override;
	STDMETHODIMP get_Count(UINT* p) override;
	STDMETHODIMP get_Item(UINT index, IMetadbHandle** pp) override;
	STDMETHODIMP put_Item(UINT index, IMetadbHandle* handle) override;
};
