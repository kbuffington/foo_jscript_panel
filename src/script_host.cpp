#include "stdafx.h"
#include "panel_base.h"
#include "script_host.h"

script_host::script_host(panel_base* host)
	: m_host(host)
	, m_window(new com_object_impl_t<Window, false>(host))
	, m_gdi(com_object_singleton_t<Gdi>::instance())
	, m_fb(com_object_singleton_t<Fb>::instance())
	, m_utils(com_object_singleton_t<Utils>::instance())
	, m_plman(com_object_singleton_t<Plman>::instance())
	, m_console(com_object_singleton_t<Console>::instance()) {}

script_host::~script_host() {}

DWORD script_host::GenerateSourceContext(const pfc::string8_fast& path)
{
	m_context_to_path_map[++m_last_source_context] = path;
	return m_last_source_context;
}

HRESULT script_host::Initialise()
{
	IActiveScriptParsePtr parser;
	ProcessScriptInfo(m_host->m_script_info);

	HRESULT hr = InitScriptEngine();
	if (SUCCEEDED(hr)) hr = m_script_engine->SetScriptSite(this);
	if (SUCCEEDED(hr)) hr = m_script_engine->QueryInterface(&parser);
	if (SUCCEEDED(hr)) hr = parser->InitNew();
	if (SUCCEEDED(hr)) hr = m_script_engine->AddNamedItem(L"window", SCRIPTITEM_ISVISIBLE);
	if (SUCCEEDED(hr)) hr = m_script_engine->AddNamedItem(L"gdi", SCRIPTITEM_ISVISIBLE);
	if (SUCCEEDED(hr)) hr = m_script_engine->AddNamedItem(L"fb", SCRIPTITEM_ISVISIBLE);
	if (SUCCEEDED(hr)) hr = m_script_engine->AddNamedItem(L"utils", SCRIPTITEM_ISVISIBLE);
	if (SUCCEEDED(hr)) hr = m_script_engine->AddNamedItem(L"plman", SCRIPTITEM_ISVISIBLE);
	if (SUCCEEDED(hr)) hr = m_script_engine->AddNamedItem(L"console", SCRIPTITEM_ISVISIBLE);
	if (SUCCEEDED(hr)) hr = m_script_engine->SetScriptState(SCRIPTSTATE_CONNECTED);
	if (SUCCEEDED(hr)) hr = m_script_engine->GetScriptDispatch(nullptr, &m_script_root);
	if (SUCCEEDED(hr)) hr = ProcessScripts(parser);
	if (SUCCEEDED(hr)) hr = InitCallbackMap();
	if (SUCCEEDED(hr))
	{
		m_engine_inited = true;
		m_has_error = false;
	}
	else
	{
		m_engine_inited = false;
		m_has_error = true;
	}
	return hr;
}

HRESULT script_host::InitCallbackMap()
{
	const auto check_features = [&](callback_id id)
	{
		switch (id)
		{
		case callback_id::on_char:
		case callback_id::on_key_down:
		case callback_id::on_key_up:
			m_host->m_grabfocus = true;
			break;
		case callback_id::on_drag_drop:
		case callback_id::on_drag_enter:
		case callback_id::on_drag_leave:
		case callback_id::on_drag_over:
			m_host->m_dragdrop = true;
			break;
		}
	};

	m_callback_map.clear();
	if (!m_script_root) return E_POINTER;
	for (const auto& [id, name] : callback_id_names)
	{
		auto cname = const_cast<LPOLESTR>(name);
		DISPID dispId;
		if (SUCCEEDED(m_script_root->GetIDsOfNames(IID_NULL, &cname, 1, LOCALE_USER_DEFAULT, &dispId)))
		{
			m_callback_map[id] = dispId;
			check_features(id);
		}
	}
	return S_OK;
}

HRESULT script_host::InitScriptEngine()
{
	HRESULT hr = E_FAIL;
	static constexpr DWORD classContext = CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER;

	if (helpers::supports_chakra() && _stricmp(m_host->m_script_engine_str, "Chakra") == 0)
	{
		static constexpr CLSID jscript9clsid = { 0x16d51579, 0xa30b, 0x4c8b,{ 0xa2, 0x76, 0x0f, 0xf4, 0xdc, 0x41, 0xe7, 0x55 } };
		hr = m_script_engine.CreateInstance(jscript9clsid, nullptr, classContext);
	}

	if (FAILED(hr))
	{
		hr = m_script_engine.CreateInstance("jscript", nullptr, classContext);
	}

	if (SUCCEEDED(hr))
	{
		IActiveScriptProperty* pActScriProp = nullptr;
		m_script_engine->QueryInterface(IID_IActiveScriptProperty, reinterpret_cast<void**>(&pActScriProp));
		VARIANT scriptLangVersion;
		scriptLangVersion.vt = VT_I4;
		scriptLangVersion.lVal = SCRIPTLANGUAGEVERSION_5_8 + 1;
		pActScriProp->SetProperty(SCRIPTPROP_INVOKEVERSIONING, nullptr, &scriptLangVersion);
		pActScriProp->Release();
	}
	return hr;
}

HRESULT script_host::ProcessScripts(IActiveScriptParsePtr& parser)
{
	HRESULT hr = S_OK;
	bool import_error = false;
	pfc::string8_fast path, code;
	const size_t count = m_host->m_script_info.imports.size();

	for (size_t i = 0; i <= count; ++i)
	{
		if (i < count) // import
		{
			path = m_host->m_script_info.expand_import(i).c_str();
			code = helpers::read_file(path);
			if (code.is_empty())
			{
				if (!import_error)
				{
					import_error = true;
					FB2K_console_formatter() << m_host->m_script_info.build_info_string();
				}
				FB2K_console_formatter() << "Error: Failed to load " << path;
			}
		}
		else // main
		{
			path = "<main>";
			code = m_host->m_script_code;
		}

		const DWORD source_context = GenerateSourceContext(path);
		hr = parser->ParseScriptText(string_wide_from_utf8_fast(code), nullptr, nullptr, nullptr, source_context, 0, SCRIPTTEXT_HOSTMANAGESSOURCE | SCRIPTTEXT_ISVISIBLE, nullptr, nullptr);
		if (FAILED(hr)) break;
	}
	return hr;
}

STDMETHODIMP script_host::EnableModeless(BOOL fEnable)
{
	return S_OK;
}

STDMETHODIMP script_host::GetDocVersionString(BSTR* pstr)
{
	return E_NOTIMPL;
}

STDMETHODIMP script_host::GetItemInfo(LPCOLESTR name, DWORD mask, IUnknown** ppunk, ITypeInfo** ppti)
{
	if (ppti) *ppti = nullptr;
	if (ppunk) *ppunk = nullptr;
	if (mask & SCRIPTINFO_IUNKNOWN)
	{
		if (!name) return E_INVALIDARG;
		if (!ppunk) return E_POINTER;

		if (wcscmp(name, L"window") == 0)
		{
			(*ppunk) = m_window;
			(*ppunk)->AddRef();
			return S_OK;
		}
		else if (wcscmp(name, L"gdi") == 0)
		{
			(*ppunk) = m_gdi;
			(*ppunk)->AddRef();
			return S_OK;
		}
		else if (wcscmp(name, L"fb") == 0)
		{
			(*ppunk) = m_fb;
			(*ppunk)->AddRef();
			return S_OK;
		}
		else if (wcscmp(name, L"utils") == 0)
		{
			(*ppunk) = m_utils;
			(*ppunk)->AddRef();
			return S_OK;
		}
		else if (wcscmp(name, L"plman") == 0)
		{
			(*ppunk) = m_plman;
			(*ppunk)->AddRef();
			return S_OK;
		}
		else if (wcscmp(name, L"console") == 0)
		{
			(*ppunk) = m_console;
			(*ppunk)->AddRef();
			return S_OK;
		}
	}
	return TYPE_E_ELEMENTNOTFOUND;
}

STDMETHODIMP script_host::GetLCID(LCID* plcid)
{
	return E_NOTIMPL;
}

STDMETHODIMP script_host::GetWindow(HWND* phwnd)
{
	*phwnd = m_host->get_hwnd();
	return S_OK;
}

STDMETHODIMP script_host::OnEnterScript()
{
	return S_OK;
}

STDMETHODIMP script_host::OnLeaveScript()
{
	return S_OK;
}

STDMETHODIMP script_host::OnScriptError(IActiveScriptError* err)
{
	if (!err) return E_POINTER;

	m_engine_inited = false;
	m_has_error = true;

	DWORD ctx = 0;
	EXCEPINFO excep = { 0 };
	LONG charpos = 0;
	ULONG line = 0;
	_bstr_t sourceline;
	pfc::string_formatter formatter;

	formatter << m_host->m_script_info.build_info_string() << "\n";

	if (SUCCEEDED(err->GetExceptionInfo(&excep)))
	{
		if (excep.pfnDeferredFillIn)
		{
			(*excep.pfnDeferredFillIn)(&excep);
		}

		if (excep.bstrSource && excep.bstrDescription)
		{
			formatter << string_utf8_from_wide(excep.bstrSource) << ":\n";
			formatter << string_utf8_from_wide(excep.bstrDescription) << "\n";
		}
		else
		{
			pfc::string8_fast errorMessage;
			if (uFormatSystemErrorMessage(errorMessage, excep.scode))
			{
				formatter << errorMessage;
			}
			else
			{
				formatter << "Unknown error code: 0x" << pfc::format_hex_lowercase(TO_UINT(excep.scode));
			}
		}
	}

	if (SUCCEEDED(err->GetSourcePosition(&ctx, &line, &charpos)))
	{
		if (m_context_to_path_map.count(ctx))
		{
			formatter << "File: " << m_context_to_path_map.at(ctx) << "\n";
		}
		formatter << "Line: " << (line + 1) << ", Col: " << (charpos + 1) << "\n";
	}

	if (SUCCEEDED(err->GetSourceLineText(sourceline.GetAddress())))
	{
		formatter << string_utf8_from_wide(sourceline);
	}

	FB2K_console_formatter() << formatter;

	fb2k::inMainThread([=]()
		{
			popup_message::g_show(formatter, PFC_string_formatter() << jsp::component_name << " v" << jsp::component_version);
		});

	if (excep.bstrSource) SysFreeString(excep.bstrSource);
	if (excep.bstrDescription) SysFreeString(excep.bstrDescription);
	if (excep.bstrHelpFile) SysFreeString(excep.bstrHelpFile);
	if (m_script_engine) m_script_engine->SetScriptState(SCRIPTSTATE_DISCONNECTED);

	MessageBeep(MB_ICONASTERISK);

	const auto& tooltip_param = m_host->panel_tooltip();
	if (tooltip_param && tooltip_param->tooltip_hwnd)
		SendMessage(tooltip_param->tooltip_hwnd, TTM_ACTIVATE, FALSE, 0);

	m_host->repaint();
	m_host->unload_script();
	return S_OK;
}

STDMETHODIMP script_host::OnScriptTerminate(const VARIANT* result, const EXCEPINFO* excep)
{
	return E_NOTIMPL;
}

STDMETHODIMP script_host::OnStateChange(SCRIPTSTATE state)
{
	return E_NOTIMPL;
}

ULONG STDMETHODCALLTYPE script_host::AddRef()
{
	return InterlockedIncrement(&m_ref_count);
}

ULONG STDMETHODCALLTYPE script_host::Release()
{
	const ULONG n = InterlockedDecrement(&m_ref_count);
	if (n == 0)
	{
		delete this;
	}
	return n;
}

bool script_host::HasError()
{
	return m_has_error;
}

bool script_host::Ready()
{
	return m_script_root && m_engine_inited && m_script_engine;
}

std::string script_host::ExtractValue(const std::string& source)
{
	constexpr char q = '"';
	const size_t first = source.find_first_of(q);
	const size_t last = source.find_last_of(q);
	if (first < last && last < source.length())
	{
		return source.substr(first + 1, last - first - 1);
	}
	return "";
}

void script_host::Finalise()
{
	InvokeCallback(callback_id::on_script_unload);

	if (Ready())
	{
		IActiveScriptGarbageCollector* gc = nullptr;
		if (SUCCEEDED(m_script_engine->QueryInterface(IID_IActiveScriptGarbageCollector, reinterpret_cast<void**>(&gc))))
		{
			gc->CollectGarbage(SCRIPTGCTYPE_EXHAUSTIVE);
			gc->Release();
		}

		m_script_engine->SetScriptState(SCRIPTSTATE_DISCONNECTED);
		m_script_engine->SetScriptState(SCRIPTSTATE_CLOSED);
		m_script_engine->Close();
		m_engine_inited = false;
	}

	m_context_to_path_map.clear();
	m_callback_map.clear();

	if (m_script_engine)
	{
		m_script_engine.Release();
	}

	if (m_script_root)
	{
		m_script_root.Release();
	}
}

void script_host::InvokeCallback(callback_id id, VARIANTARG* argv, size_t argc, VARIANT* ret)
{
	if (Ready() && m_callback_map.count(id))
	{
		DISPPARAMS param = { argv, nullptr, argc, 0 };
		m_script_root->Invoke(m_callback_map.at(id), IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &param, ret, nullptr, nullptr);
	}
}

void script_host::ProcessScriptInfo(script_info& info)
{
	info.clear();
	info.id = reinterpret_cast<size_t>(m_host->get_hwnd());

	std::string source(m_host->m_script_code);
	const size_t start = source.find("// ==PREPROCESSOR==");
	const size_t end = source.find("// ==/PREPROCESSOR==");
	constexpr size_t argh = std::string::npos;

	if (start == argh || end == argh || start > end)
	{
		return;
	}

	std::string pre = source.substr(start + 21, end - start - 21);
	str_vec lines = helpers::split_string(pre, "\r\n");

	for (const std::string& line : lines)
	{
		if (line.find("@name") < argh)
		{
			info.name = ExtractValue(line);
		}
		else if (line.find("@author") < argh)
		{
			info.author = ExtractValue(line);
		}
		else if (line.find("@version") < argh)
		{
			info.version = ExtractValue(line);
		}
		else if (line.find("@import") < argh)
		{
			info.imports.emplace_back(ExtractValue(line));
		}
	}
}
