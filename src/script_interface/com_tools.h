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

struct type_info_cache
{
	type_info_cache() : m_type_info(nullptr) {}

	bool query(ULONG key, DISPID& value)
	{
		if (m_cache.count(key))
		{
			value = m_cache.at(key);
			return true;
		}
		return false;
	}

	ITypeInfoPtr m_type_info;
	std::unordered_map<ULONG, DISPID> m_cache;
};

template <class T>
class MyIDispatchImpl : public T
{
protected:
	MyIDispatchImpl<T>()
	{
		if (g_type_info_cache.m_type_info == nullptr)
		{
			g_typelib->GetTypeInfoOfGuid(__uuidof(T), &g_type_info_cache.m_type_info);
		}
	}

	virtual ~MyIDispatchImpl<T>() {}

	virtual void FinalRelease() {}

	static type_info_cache g_type_info_cache;

public:
	STDMETHODIMP GetIDsOfNames(REFIID riid, OLECHAR** names, UINT cNames, LCID lcid, DISPID* dispids) override
	{
		if (!dispids) return E_POINTER;
		for (t_size i = 0; i < cNames; ++i)
		{
			const ULONG hash = LHashValOfName(LANG_NEUTRAL, names[i]);
			if (!g_type_info_cache.query(hash, dispids[i]))
			{
				const HRESULT hr = g_type_info_cache.m_type_info->GetIDsOfNames(&names[i], 1, &dispids[i]);
				if (FAILED(hr)) return hr;
				g_type_info_cache.m_cache[hash] = dispids[i];
			}
		}
		return S_OK;
	}

	STDMETHODIMP GetTypeInfo(UINT i, LCID lcid, ITypeInfo** pp) override
	{
		if (!pp) return E_POINTER;
		if (i != 0) return DISP_E_BADINDEX;
		g_type_info_cache.m_type_info->AddRef();
		*pp = g_type_info_cache.m_type_info.GetInterfacePtr();
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
		const HRESULT hr = g_type_info_cache.m_type_info->Invoke(this, dispid, flags, params, result, excep, err);
		PFC_ASSERT(hr != RPC_E_WRONG_THREAD);
		return hr;
	}
};

template <class T>
FOOGUIDDECL type_info_cache MyIDispatchImpl<T>::g_type_info_cache;

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
		const ULONG n = Release_();
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

template <class T>
FOOGUIDDECL IDispatchPtr com_object_singleton_t<T>::_instance;

template <class T>
FOOGUIDDECL critical_section com_object_singleton_t<T>::_cs;

class com_array
{
public:
	com_array() : m_psa(nullptr), m_reader(true), m_count(0) {}

	~com_array()
	{
		if (m_reader)
		{
			reset();
		}
	}

	LONG get_count()
	{
		return m_count;
	}

	SAFEARRAY* get_ptr()
	{
		return m_psa;
	}

	bool convert(const VARIANT& v)
	{
		if (v.vt != VT_DISPATCH || !v.pdispVal) return false;

		IDispatch* pdisp = v.pdispVal;
		DISPID id_length;
		LPOLESTR slength = (LPOLESTR)L"length";
		DISPPARAMS params = { 0 };
		_variant_t ret;

		HRESULT hr = pdisp->GetIDsOfNames(IID_NULL, &slength, 1, LOCALE_USER_DEFAULT, &id_length);
		if (SUCCEEDED(hr)) hr = pdisp->Invoke(id_length, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &params, &ret, nullptr, nullptr);
		if (SUCCEEDED(hr)) hr = VariantChangeType(&ret, &ret, 0, VT_I4);
		if (FAILED(hr))
		{
			return false;
		}

		m_count = ret.lVal;
		SAFEARRAY* psa = SafeArrayCreateVector(VT_VARIANT, 0, m_count);

		for (LONG i = 0; i < m_count; ++i)
		{
			DISPID dispid = 0;
			params = { 0 };
			wchar_t buf[33];
			LPOLESTR name = buf;
			_variant_t element;
			_itow_s(i, buf, 10);

			hr = pdisp->GetIDsOfNames(IID_NULL, &name, 1, LOCALE_USER_DEFAULT, &dispid);
			if (SUCCEEDED(hr)) hr = pdisp->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &params, &element, nullptr, nullptr);
			if (SUCCEEDED(hr)) hr = SafeArrayPutElement(psa, &i, &element);
			if (FAILED(hr))
			{
				SafeArrayDestroy(psa);
				return false;
			}
		}
		m_psa = psa;
		return true;
	}

	bool convert(const VARIANT& v, pfc::bit_array_bittable& out)
	{
		if (!convert(v)) return false;
		if (m_count == 0) out.resize(0);

		for (LONG i = 0; i < m_count; ++i)
		{
			_variant_t var;
			if (!get_item(i, var, VT_I4)) return false;
			out.set(var.lVal, true);
		}
		return true;
	}

	bool convert(const VARIANT& v, pfc::string_list_impl& out)
	{
		if (!convert(v)) return false;
		for (LONG i = 0; i < m_count; ++i)
		{
			_variant_t var;
			if (!get_item(i, var, VT_BSTR)) return false;
			out.add_item(string_utf8_from_wide(var.bstrVal));
		}
		return true;
	}

	bool convert(const VARIANT& v, std::vector<Gdiplus::PointF>& out)
	{
		if (!convert(v)) return false;
		if (m_count % 2 != 0) return false;

		out.resize(m_count >> 1);

		for (LONG i = 0; i < m_count; i += 2)
		{
			_variant_t varX, varY;
			if (!get_item(i, varX, VT_R4)) return false;
			if (!get_item(i + 1, varY, VT_R4)) return false;
			out[i >> 1] = { varX.fltVal, varY.fltVal };
		}
		return true;
	}

	bool create(LONG count)
	{
		m_count = count;
		m_psa = SafeArrayCreateVector(VT_VARIANT, 0, count);
		m_reader = false;
		return m_psa != nullptr;
	}

	bool create(str_vec strings)
	{
		if (!create(strings.size())) return false;

		LONG idx = 0;
		for (const auto& str : strings)
		{
			_variant_t var;
			var.vt = VT_BSTR;
			var.bstrVal = TO_BSTR(str.c_str());
			if (!put_item(idx, var)) return false;
			idx++;
		}
		return true;
	}

	bool get_item(LONG idx, VARIANT& var, VARTYPE vt)
	{
		if (SUCCEEDED(SafeArrayGetElement(m_psa, &idx, &var)))
		{
			return SUCCEEDED(VariantChangeType(&var, &var, 0, vt));
		}
		return false;
	}

	bool put_item(LONG idx, VARIANT& var)
	{
		if (SUCCEEDED(SafeArrayPutElement(m_psa, &idx, &var)))
		{
			return true;
		}
		reset();
		return false;
	}

	void reset()
	{
		m_count = 0;
		if (m_psa)
		{
			SafeArrayDestroy(m_psa);
			m_psa = nullptr;
		}
	}

private:
	LONG m_count;
	SAFEARRAY* m_psa;
	bool m_reader;
};
