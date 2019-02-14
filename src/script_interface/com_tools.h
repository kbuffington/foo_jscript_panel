#pragma once

extern ITypeLibPtr g_typelib;
struct IDisposable;
struct IGdiObj;

//-- IUnknown ---
#define BEGIN_COM_QI_IMPL() \
	public:\
		STDMETHODIMP QueryInterface(REFIID riid, void** ppv) override \
		{ \
			if (!ppv) return E_INVALIDARG; \

// C2594: ambiguous conversions
#define COM_QI_ENTRY_MULTI(Ibase, Iimpl) \
			if (riid == __uuidof(Ibase)) \
			{ \
				*ppv = static_cast<Ibase*>(static_cast<Iimpl*>(this)); \
				reinterpret_cast<IUnknown*>(*ppv)->AddRef(); \
				return S_OK; \
			}

#define COM_QI_ENTRY(Iimpl) \
			COM_QI_ENTRY_MULTI(Iimpl, Iimpl);

#define END_COM_QI_IMPL() \
			*ppv = nullptr; \
			return E_NOINTERFACE; \
		} \
	private:

class name_to_id_cache
{
public:
	bool lookup(ULONG hash, DISPID* p_dispid) const;
	void add(ULONG hash, DISPID dispid);

protected:
	using name_to_id_map = pfc::map_t<ULONG, DISPID>;

	name_to_id_map m_map;
};

class type_info_cache_holder
{
public:
	type_info_cache_holder();

	HRESULT GetIDsOfNames(LPOLESTR* rgszNames, UINT cNames, MEMBERID* pMemId);
	HRESULT GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo** ppTInfo);
	HRESULT Invoke(PVOID pvInstance, MEMBERID memid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr);
	ITypeInfo* get_ptr() throw();
	bool empty() throw();
	bool valid() throw();
	void init_from_typelib(ITypeLib* p_typeLib, const GUID& guid);
	void set_type_info(ITypeInfo* type_info);

protected:
	ITypeInfoPtr m_type_info;
	name_to_id_cache m_cache;
};

//-- IDispatch --
template <class T>
class MyIDispatchImpl : public T
{
protected:
	MyIDispatchImpl<T>()
	{
		if (g_type_info_cache_holder.empty() && g_typelib)
		{
			g_type_info_cache_holder.init_from_typelib(g_typelib, __uuidof(T));
		}
	}

	virtual ~MyIDispatchImpl<T>() {}
	virtual void FinalRelease() {}

	static type_info_cache_holder g_type_info_cache_holder;

public:
	STDMETHOD(GetIDsOfNames)(REFIID riid, OLECHAR** names, UINT cnames, LCID lcid, DISPID* dispids) override
	{
		if (g_type_info_cache_holder.empty()) return E_UNEXPECTED;
		return g_type_info_cache_holder.GetIDsOfNames(names, cnames, dispids);
	}

	STDMETHOD(GetTypeInfo)(UINT i, LCID lcid, ITypeInfo** pp) override
	{
		return g_type_info_cache_holder.GetTypeInfo(i, lcid, pp);
	}

	STDMETHOD(GetTypeInfoCount)(UINT* n) override
	{
		if (!n) return E_INVALIDARG;
		*n = 1;
		return S_OK;
	}

	STDMETHOD(Invoke)(DISPID dispid, REFIID riid, LCID lcid, WORD flag, DISPPARAMS* params, VARIANT* result, EXCEPINFO* excep, UINT* err) override
	{
		if (g_type_info_cache_holder.empty()) return E_UNEXPECTED;
		return g_type_info_cache_holder.Invoke(this, dispid, flag, params, result, excep, err);
	}
};

template <class T> FOOGUIDDECL type_info_cache_holder MyIDispatchImpl<T>::g_type_info_cache_holder;

//-- IDispatch impl -- [T] [IDispatch] [IUnknown]
template <class T>
class IDispatchImpl3 : public MyIDispatchImpl<T>
{
	BEGIN_COM_QI_IMPL()
		COM_QI_ENTRY_MULTI(IUnknown, IDispatch)
		COM_QI_ENTRY(T)
		COM_QI_ENTRY(IDispatch)
	END_COM_QI_IMPL()

protected:
	IDispatchImpl3<T>() {}
	virtual ~IDispatchImpl3<T>() {}
};

//-- IDisposable impl -- [T] [IDisposable] [IDispatch] [IUnknown]
template <class T>
class IDisposableImpl4 : public MyIDispatchImpl<T>
{
	BEGIN_COM_QI_IMPL()
		COM_QI_ENTRY_MULTI(IUnknown, IDispatch)
		COM_QI_ENTRY(T)
		COM_QI_ENTRY(IDisposable)
		COM_QI_ENTRY(IDispatch)
	END_COM_QI_IMPL()

protected:
	IDisposableImpl4<T>() {}
	virtual ~IDisposableImpl4() {}

public:
	STDMETHODIMP Dispose() override
	{
		this->FinalRelease();
		return S_OK;
	}
};

template <class T, class T2>
class GdiObj : public MyIDispatchImpl<T>
{
	BEGIN_COM_QI_IMPL()
		COM_QI_ENTRY_MULTI(IUnknown, IDispatch)
		COM_QI_ENTRY(T)
		COM_QI_ENTRY(IGdiObj)
		COM_QI_ENTRY(IDisposable)
		COM_QI_ENTRY(IDispatch)
	END_COM_QI_IMPL()

protected:
	GdiObj<T, T2>(T2* p) : m_ptr(p) {}

	virtual ~GdiObj<T, T2>() {}

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
	// Default Dispose
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
	STDMETHODIMP_(ULONG) AddRef() override
	{
		return AddRef_();
	}

	STDMETHODIMP_(ULONG) Release() override
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
	virtual ~com_object_impl_t() {}

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
