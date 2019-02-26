#include "stdafx.h"
#include "helpers.h"
#include "host_comm.h"
#include "script_host.h"
#include "console.h"
#include "fb.h"
#include "gdi.h"
#include "plman.h"
#include "utils.h"
#include "window.h"

script_host::script_host(host_comm* host)
	: m_host(host)
	, m_window(new com_object_impl_t<Window, false>(host))
	, m_gdi(com_object_singleton_t<Gdi>::instance())
	, m_fb(com_object_singleton_t<Fb>::instance())
	, m_utils(com_object_singleton_t<Utils>::instance())
	, m_plman(com_object_singleton_t<Plman>::instance())
	, m_console(com_object_singleton_t<Console>::instance())
	, m_dwStartTime(0)
	, m_dwRef(1)
	, m_engine_inited(false)
	, m_has_error(false)
	, m_lastSourceContext(0) {}

script_host::~script_host() {}

HRESULT script_host::Initialize()
{
	Finalize();

	m_has_error = false;

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
	if (SUCCEEDED(hr)) hr = ProcessImportedScripts(parser);
	if (SUCCEEDED(hr))
	{
		DWORD source_context;
		GenerateSourceContext("<main>", source_context);
		hr = parser->ParseScriptText(string_wide_from_utf8_fast(m_host->m_script_code), nullptr, nullptr, nullptr, source_context, 0, SCRIPTTEXT_HOSTMANAGESSOURCE | SCRIPTTEXT_ISVISIBLE, nullptr, nullptr);
	}

	if (SUCCEEDED(hr))
	{
		m_engine_inited = true;
	}
	else
	{
		m_engine_inited = false;
		m_has_error = true;
	}

	m_callback_invoker.Init(m_script_root);
	return hr;
}

HRESULT script_host::InitScriptEngine()
{
	HRESULT hr = E_FAIL;
	const DWORD classContext = CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER;

	if (helpers::supports_chakra() && _stricmp(m_host->m_script_engine_str, "Chakra") == 0)
	{
		static const CLSID jscript9clsid = { 0x16d51579, 0xa30b, 0x4c8b,{ 0xa2, 0x76, 0x0f, 0xf4, 0xdc, 0x41, 0xe7, 0x55 } };
		hr = m_script_engine.CreateInstance(jscript9clsid, nullptr, classContext);
	}

	if (FAILED(hr))
	{
		hr = m_script_engine.CreateInstance("jscript", nullptr, classContext);
	}

	if (SUCCEEDED(hr))
	{
		IActiveScriptProperty* pActScriProp = nullptr;
		m_script_engine->QueryInterface(IID_IActiveScriptProperty, (void**)&pActScriProp);
		VARIANT scriptLangVersion;
		scriptLangVersion.vt = VT_I4;
		scriptLangVersion.lVal = SCRIPTLANGUAGEVERSION_5_8 + 1;
		pActScriProp->SetProperty(SCRIPTPROP_INVOKEVERSIONING, nullptr, &scriptLangVersion);
		pActScriProp->Release();
	}
	return hr;
}

HRESULT script_host::InvokeCallback(t_size callbackId, VARIANTARG* argv, t_size argc, VARIANT* ret)
{
	if (HasError()) return E_FAIL;
	if (!Ready()) return E_FAIL;

	HRESULT hr = E_FAIL;

	try
	{
		hr = m_callback_invoker.Invoke(callbackId, argv, argc, ret);
	}
	catch (...) {}

	return hr;
}

HRESULT script_host::ProcessImportedScripts(IActiveScriptParsePtr& parser)
{
	pfc::string_formatter error_text;
	const t_size count = m_host->m_script_info.imports.get_count();
	for (t_size i = 0; i < count; ++i)
	{
		pfc::string8_fast path = m_host->m_script_info.expand_import(i);
		pfc::string8_fast code = helpers::read_file(path);
		if (code.get_length())
		{
			DWORD source_context;
			GenerateSourceContext(path, source_context);
			HRESULT hr = parser->ParseScriptText(string_wide_from_utf8_fast(code), nullptr, nullptr, nullptr, source_context, 0, SCRIPTTEXT_HOSTMANAGESSOURCE | SCRIPTTEXT_ISVISIBLE, nullptr, nullptr);
			if (FAILED(hr)) return hr;
		}
		else
		{
			error_text << "\nError: Failed to load " << path;
		}
	}

	if (error_text.get_length())
	{
		FB2K_console_formatter() << m_host->m_script_info.build_info_string() << error_text;
	}
	return S_OK;
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
	*phwnd = m_host->GetHWND();
	return S_OK;
}

STDMETHODIMP script_host::OnEnterScript()
{
	m_dwStartTime = pfc::getTickCount();
	return S_OK;
}

STDMETHODIMP script_host::OnLeaveScript()
{
	return S_OK;
}

STDMETHODIMP script_host::OnScriptError(IActiveScriptError* err)
{
	m_has_error = true;

	if (!err) return E_POINTER;

	ReportError(err);
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
	return InterlockedIncrement(&m_dwRef);
}

ULONG STDMETHODCALLTYPE script_host::Release()
{
	ULONG n = InterlockedDecrement(&m_dwRef);

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
	return m_engine_inited && m_script_engine;
}

pfc::string8_fast script_host::ExtractValue(const std::string& source)
{
	char q = '"';
	t_size first = source.find_first_of(q);
	t_size last = source.find_last_of(q);
	if (first < last && last < source.length())
	{
		return source.substr(first + 1, last - first - 1).c_str();
	}
	return "";
}

void script_host::Finalize()
{
	InvokeCallback(CallbackIds::on_script_unload);

	if (Ready())
	{
		// Call GC explicitly
		IActiveScriptGarbageCollector* gc = nullptr;
		if (SUCCEEDED(m_script_engine->QueryInterface(IID_IActiveScriptGarbageCollector, (void**)&gc)))
		{
			gc->CollectGarbage(SCRIPTGCTYPE_EXHAUSTIVE);
			gc->Release();
		}

		m_script_engine->SetScriptState(SCRIPTSTATE_DISCONNECTED);
		m_script_engine->SetScriptState(SCRIPTSTATE_CLOSED);
		m_script_engine->Close();
		//m_script_engine->InterruptScriptThread(SCRIPTTHREADID_ALL, nullptr, 0);
		m_engine_inited = false;
	}

	m_contextToPathMap.remove_all();
	m_callback_invoker.Reset();

	if (m_script_engine)
	{
		m_script_engine.Release();
	}

	if (m_script_root)
	{
		m_script_root.Release();
	}
}

void script_host::GenerateSourceContext(const pfc::string8_fast& path, DWORD& source_context)
{
	source_context = m_lastSourceContext++;
	m_contextToPathMap[source_context] = path;
}

void script_host::ProcessScriptInfo(t_script_info& info)
{
	info.clear();
	info.id = (t_size)m_host->GetHWND();

	std::string source(m_host->m_script_code);
	t_size start = source.find("// ==PREPROCESSOR==");
	t_size end = source.find("// ==/PREPROCESSOR==");
	t_size argh = std::string::npos;

	if (start == argh || end == argh || start > end)
	{
		return;
	}

	std::string pre = source.substr(start + 21, end - start - 21);

	pfc::string_list_impl lines;
	pfc::splitStringByLines(lines, pre.c_str());

	for (t_size i = 0; i < lines.get_count(); ++i)
	{
		std::string line = lines[i];
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
		else if (line.find("@feature") < argh && line.find("dragdrop") < argh)
		{
			info.dragdrop = true;
		}
		else if (line.find("@import") < argh)
		{
			info.imports.add_item(ExtractValue(line));
		}
	}
}

void script_host::ReportError(IActiveScriptError* err)
{
	if (!err) return;

	DWORD ctx = 0;
	ULONG line = 0;
	LONG charpos = 0;
	EXCEPINFO excep = { 0 };
	_bstr_t sourceline;
	_bstr_t name;

	if (FAILED(err->GetSourcePosition(&ctx, &line, &charpos)))
	{
		line = 0;
		charpos = 0;
	}

	if (FAILED(err->GetSourceLineText(sourceline.GetAddress())))
	{
		sourceline = L"<source text only available at compile time>";
	}

	if (FAILED(err->GetExceptionInfo(&excep)))
	{
		return;
	}

	// Do a deferred fill-in if necessary
	if (excep.pfnDeferredFillIn)
	{
		(*excep.pfnDeferredFillIn)(&excep);
	}

	pfc::string_formatter formatter;
	formatter << m_host->m_script_info.build_info_string() << "\n";

	if (excep.bstrSource && excep.bstrDescription)
	{
		formatter << string_utf8_from_wide(excep.bstrSource) << ":\n";
		formatter << string_utf8_from_wide(excep.bstrDescription) << "\n";
	}
	else
	{
		pfc::string8_fast errorMessage;

		if (uFormatSystemErrorMessage(errorMessage, excep.scode))
			formatter << errorMessage;
		else
			formatter << "Unknown error code: 0x" << pfc::format_hex_lowercase((t_size)excep.scode);
	}

	if (m_contextToPathMap.exists(ctx))
	{
		formatter << "File: " << m_contextToPathMap[ctx] << "\n";
	}

	formatter << "Line: " << (t_size)(line + 1) << ", Col: " << (t_size)(charpos + 1) << "\n";
	formatter << string_utf8_from_wide(sourceline);
	if (name.length() > 0) formatter << "\nAt: " << name;

	if (excep.bstrSource) SysFreeString(excep.bstrSource);
	if (excep.bstrDescription) SysFreeString(excep.bstrDescription);
	if (excep.bstrHelpFile) SysFreeString(excep.bstrHelpFile);

	FB2K_console_formatter() << formatter;
	main_thread_callback_add(fb2k::service_new<helpers::popup_msg>(formatter, JSP_NAME_VERSION));
	MessageBeep(MB_ICONASTERISK);
	SendMessage(m_host->GetHWND(), UWM_SCRIPT_ERROR, 0, 0);
}

void script_host::Stop()
{
	m_engine_inited = false;
	if (m_script_engine) m_script_engine->SetScriptState(SCRIPTSTATE_DISCONNECTED);
}
