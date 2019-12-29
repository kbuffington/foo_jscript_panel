#pragma once
#include "script_interface.h"

class FileInfo : public IDisposableImpl4<IFileInfo>
{
protected:
	FileInfo(file_info_impl* info_ptr);
	~FileInfo();

	void FinalRelease() override;

	file_info_impl* m_info_ptr;

public:
	STDMETHODIMP get__ptr(void** pp) override;
	STDMETHODIMP InfoFind(BSTR name, int* p) override;
	STDMETHODIMP InfoName(UINT idx, BSTR* p) override;
	STDMETHODIMP InfoValue(UINT idx, BSTR* p) override;
	STDMETHODIMP MetaFind(BSTR name, int* p) override;
	STDMETHODIMP MetaName(UINT idx, BSTR* p) override;
	STDMETHODIMP MetaValue(UINT idx, UINT vidx, BSTR* p) override;
	STDMETHODIMP MetaValueCount(UINT idx, UINT* p) override;
	STDMETHODIMP get_InfoCount(UINT* p) override;
	STDMETHODIMP get_MetaCount(UINT* p) override;
};
