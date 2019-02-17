#pragma once

extern ITypeLibPtr g_typelib;
struct IDisposable;
struct IGdiObj;

#define COM_QI_BEGIN(first) \
	STDMETHODIMP QueryInterface(REFIID riid, void** ppv) override \
	{ \
		if (!ppv) return E_POINTER; \
		IUnknown* temp = nullptr; \
		if (riid == __uuidof(IUnknown)) temp = static_cast<IUnknown*>(static_cast<first*>(this)); \
		else if (riid == __uuidof(first)) temp = static_cast<first*>(this);

#define COM_QI_ENTRY(entry) \
		else if (riid == __uuidof(entry)) temp = static_cast<entry*>(this);

#define COM_QI_END() \
		else { *ppv = nullptr; return E_NOINTERFACE; } \
		temp->AddRef(); \
		*ppv = temp; \
		return S_OK; \
	}

#define COM_QI_ONE(one) COM_QI_BEGIN(one) COM_QI_END()
#define COM_QI_TWO(one, two) COM_QI_BEGIN(one) COM_QI_ENTRY(two) COM_QI_END()
#define COM_QI_THREE(one, two, three) COM_QI_BEGIN(one) COM_QI_ENTRY(two) COM_QI_ENTRY(three) COM_QI_END()
#define COM_QI_FOUR(one, two, three, four) COM_QI_BEGIN(one) COM_QI_ENTRY(two) COM_QI_ENTRY(three) COM_QI_ENTRY(four) COM_QI_END()

template <class T>
class MyIDispatchImpl : public T
{
protected:
	MyIDispatchImpl<T>()
	{
		g_typelib->GetTypeInfoOfGuid(__uuidof(T), &m_type_info);
	}

	virtual ~MyIDispatchImpl<T>() {}

	virtual void FinalRelease() {}

	ITypeInfoPtr m_type_info;

public:
	STDMETHODIMP GetIDsOfNames(REFIID riid, OLECHAR** names, UINT cNames, LCID lcid, DISPID* dispids) override
	{
		if (!dispids) return E_POINTER;
		for (t_size i = 0; i < cNames; ++i)
		{
			HRESULT hr = m_type_info->GetIDsOfNames(&names[i], 1, &dispids[i]);
			if (FAILED(hr)) return hr;
		}
		return S_OK;
	}

	STDMETHODIMP GetTypeInfo(UINT i, LCID lcid, ITypeInfo** pp) override
	{
		if (!pp) return E_POINTER;
		if (i != 0) return DISP_E_BADINDEX;
		m_type_info->AddRef();
		*pp = m_type_info.GetInterfacePtr();
		return S_OK;
	}

	STDMETHODIMP GetTypeInfoCount(UINT* n) override
	{
		if (!n) return E_POINTER;
		*n = 1;
		return S_OK;
	}

	STDMETHODIMP Invoke(DISPID dispid, REFIID riid, LCID lcid, WORD flags, DISPPARAMS* params, VARIANT* result, EXCEPINFO* excep, UINT* err) override
	{
		HRESULT hr = m_type_info->Invoke(this, dispid, flags, params, result, excep, err);
		PFC_ASSERT(hr != RPC_E_WRONG_THREAD);
		return hr;
	}
};

template <class T>
class IDispatchImpl3 : public MyIDispatchImpl<T>
{
protected:
	IDispatchImpl3<T>() {}
	~IDispatchImpl3<T>() {}

public:
	COM_QI_TWO(IDispatch, T)
};

template <class T>
class IDisposableImpl4 : public MyIDispatchImpl<T>
{
protected:
	IDisposableImpl4<T>() {}
	~IDisposableImpl4() {}

public:
	COM_QI_THREE(IDispatch, IDisposable, T)

	STDMETHODIMP Dispose() override
	{
		this->FinalRelease();
		return S_OK;
	}
};

template <class T, class T2>
class GdiObj : public MyIDispatchImpl<T>
{
protected:
	GdiObj<T, T2>(T2* p) : m_ptr(p) {}
	~GdiObj<T, T2>() {}

	void FinalRelease() override
	{
		if (m_ptr)
		{
			delete m_ptr;
			m_ptr = nullptr;
		}
	}

	T2* m_ptr;

public:
	COM_QI_FOUR(IDispatch, IDisposable, IGdiObj, T)

	STDMETHODIMP Dispose() override
	{
		FinalRelease();
		return S_OK;
	}

	STDMETHODIMP get__ptr(void** pp) override
	{
		*pp = m_ptr;
		return S_OK;
	}
};

template <typename _Base, bool _AddRef = true>
class com_object_impl_t : public _Base
{
public:
	ULONG STDMETHODCALLTYPE AddRef() override
	{
		return AddRef_();
	}

	ULONG STDMETHODCALLTYPE Release() override
	{
		ULONG n = Release_();
		if (n == 0)
		{
			this->FinalRelease();
			delete this;
		}
		return n;
	}

	TEMPLATE_CONSTRUCTOR_FORWARD_FLOOD_WITH_INITIALIZER(com_object_impl_t, _Base, { Construct_(); })

private:
	~com_object_impl_t() {}

	ULONG AddRef_()
	{
		return InterlockedIncrement(&m_dwRef);
	}

	ULONG Release_()
	{
		return InterlockedDecrement(&m_dwRef);
	}

	void Construct_()
	{
		m_dwRef = 0;
		if (_AddRef)
			AddRef_();
	}

	volatile LONG m_dwRef;
};

template <class T>
class com_object_singleton_t
{
public:
	static T* instance()
	{
		if (!_instance)
		{
			insync(_cs);

			if (!_instance)
			{
				_instance = new com_object_impl_t<T, false>();
			}
		}

		return reinterpret_cast<T*>(_instance.GetInterfacePtr());
	}

private:
	static IDispatchPtr _instance;
	static critical_section _cs;

	PFC_CLASS_NOT_COPYABLE_EX(com_object_singleton_t)
};

template <class T> FOOGUIDDECL IDispatchPtr com_object_singleton_t<T>::_instance;
template <class T> FOOGUIDDECL critical_section com_object_singleton_t<T>::_cs;
