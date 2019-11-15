#pragma once
#include "script_interface.h"

class TitleFormat : public IDisposableImpl4<ITitleFormat>
{
protected:
	TitleFormat(BSTR expr);
	~TitleFormat();

	void FinalRelease() override;

	titleformat_object::ptr m_obj;

public:
	STDMETHODIMP get__ptr(void** pp) override;
	STDMETHODIMP Eval(BSTR* p) override;
	STDMETHODIMP EvalWithMetadb(IMetadbHandle* handle, BSTR* p) override;
	STDMETHODIMP EvalWithMetadbs(IMetadbHandleList* handles, VARIANT* p) override;
};
