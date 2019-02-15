#pragma once
#include "script_callback_invoker.h"

_COM_SMARTPTR_TYPEDEF(IActiveScriptParse, IID_IActiveScriptParse);

class script_host : public IActiveScriptSite, public IActiveScriptSiteWindow
{
public:
	script_host(host_comm* host);
	virtual ~script_host();

	HRESULT Initialize();
	HRESULT InitScriptEngine();
	HRESULT InvokeCallback(int callbackId, VARIANTARG* argv = nullptr, UINT argc = 0, VARIANT* ret = nullptr);
	HRESULT ProcessImportedScripts(IActiveScriptParsePtr& parser);
	STDMETHODIMP EnableModeless(BOOL fEnable) override;
	STDMETHODIMP GetDocVersionString(BSTR* pstr) override;
	STDMETHODIMP GetItemInfo(LPCOLESTR name, DWORD mask, IUnknown** ppunk, ITypeInfo** ppti) override;
	STDMETHODIMP GetLCID(LCID* plcid) override;
	STDMETHODIMP GetWindow(HWND* phwnd) override;
	STDMETHODIMP OnEnterScript() override;
	STDMETHODIMP OnLeaveScript() override;
	STDMETHODIMP OnScriptError(IActiveScriptError* err) override;
	STDMETHODIMP OnScriptTerminate(const VARIANT* result, const EXCEPINFO* excep) override;
	STDMETHODIMP OnStateChange(SCRIPTSTATE state) override;
	ULONG STDMETHODCALLTYPE AddRef() override;
	ULONG STDMETHODCALLTYPE Release() override;
	bool HasError();
	bool Ready();
	pfc::string8_fast ExtractValue(const std::string& source);
	void Finalize();
	void GenerateSourceContext(const pfc::string8_fast& path, DWORD& source_context);
	void ProcessScriptInfo(t_script_info& info);
	void ReportError(IActiveScriptError* err);
	void Stop();

private:
	DWORD m_lastSourceContext;
	IActiveScriptPtr m_script_engine;
	IDispatchPtr m_script_root;
	IPlmanPtr m_plman;
	IFbPtr m_fb;
	IWindowPtr m_window;
	IGdiPtr m_gdi;
	IUtilsPtr m_utils;
	IConsolePtr m_console;
	bool m_engine_inited;
	bool m_has_error;
	host_comm* m_host;
	pfc::map_t<DWORD, pfc::string8> m_contextToPathMap;
	pfc::tickcount_t m_dwStartTime;
	script_callback_invoker m_callback_invoker;
	volatile DWORD m_dwRef;

	BEGIN_COM_QI_IMPL()
		COM_QI_ENTRY_MULTI(IUnknown, IActiveScriptSite)
		COM_QI_ENTRY(IActiveScriptSite)
		COM_QI_ENTRY(IActiveScriptSiteWindow)
	END_COM_QI_IMPL()
};
