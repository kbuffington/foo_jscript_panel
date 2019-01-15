#pragma once
#include "script_callback_invoker.h"

_COM_SMARTPTR_TYPEDEF(IActiveScriptParse, IID_IActiveScriptParse);

class script_host : public IActiveScriptSite, public IActiveScriptSiteWindow
{
public:
	script_host(host_comm* host);
	virtual ~script_host();

	HRESULT InitScriptEngineByName(const char* engineName);
	HRESULT Initialize();
	HRESULT InvokeCallback(int callbackId, VARIANTARG* argv = NULL, UINT argc = 0, VARIANT* ret = NULL);
	HRESULT ProcessImportedScripts(IActiveScriptParsePtr& parser);
	STDMETHODIMP EnableModeless(BOOL fEnable);
	STDMETHODIMP GetDocVersionString(BSTR* pstr);
	STDMETHODIMP GetItemInfo(LPCOLESTR name, DWORD mask, IUnknown** ppunk, ITypeInfo** ppti);
	STDMETHODIMP GetLCID(LCID* plcid);
	STDMETHODIMP GetWindow(HWND* phwnd);
	STDMETHODIMP OnEnterScript();
	STDMETHODIMP OnLeaveScript();
	STDMETHODIMP OnScriptError(IActiveScriptError* err);
	STDMETHODIMP OnScriptTerminate(const VARIANT* result, const EXCEPINFO* excep);
	STDMETHODIMP OnStateChange(SCRIPTSTATE state);
	STDMETHOD_(ULONG, AddRef)();
	STDMETHOD_(ULONG, Release)();
	bool HasError();
	bool Ready();
	void Finalize();
	void GenerateSourceContext(const pfc::string8_fast& path, DWORD& source_context);
	void ProcessScriptInfo(t_script_info& info);
	void ReportError(IActiveScriptError* err);
	void Stop();

private:
	DWORD m_lastSourceContext;
	IActiveScriptPtr m_script_engine;
	IDispatchPtr m_script_root;
	IFbPlaylistManagerPtr m_playlistman;
	IFbUtilsPtr m_fb2k;
	IFbWindowPtr m_window;
	IGdiUtilsPtr m_gdi;
	IJSUtilsPtr m_utils;
	IJSConsolePtr m_console;
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
