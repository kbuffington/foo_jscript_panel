#pragma once
#include "script_info.h"

_COM_SMARTPTR_TYPEDEF(IActiveScriptParse, IID_IActiveScriptParse);
_COM_SMARTPTR_TYPEDEF(IConsole, __uuidof(IConsole));
_COM_SMARTPTR_TYPEDEF(IFb, __uuidof(IFb));
_COM_SMARTPTR_TYPEDEF(IGdi, __uuidof(IGdi));
_COM_SMARTPTR_TYPEDEF(IGdiGraphics, __uuidof(IGdiGraphics));
_COM_SMARTPTR_TYPEDEF(IPlman, __uuidof(IPlman));
_COM_SMARTPTR_TYPEDEF(IUtils, __uuidof(IUtils));
_COM_SMARTPTR_TYPEDEF(IWindow, __uuidof(IWindow));

class script_host : public IActiveScriptSite, public IActiveScriptSiteWindow
{
public:
	script_host(panel_window* host);
	virtual ~script_host();

	COM_QI_TWO(IActiveScriptSite, IActiveScriptSiteWindow)

	DWORD GenerateSourceContext(const pfc::string8_fast& path);
	HRESULT Initialise();
	HRESULT InitCallbackMap();
	HRESULT InitScriptEngine();
	HRESULT ProcessScripts(IActiveScriptParsePtr& parser);
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
	std::string ExtractValue(const std::string& source);
	void Finalise();
	void InvokeCallback(callback_id id, VARIANTARG* argv = nullptr, size_t argc = 0, VARIANT* ret = nullptr);
	void ProcessScriptInfo();

	script_info m_info{};

private:
	DWORD m_last_source_context = 0;
	IActiveScriptPtr m_script_engine;
	IConsolePtr m_console;
	IDispatchPtr m_script_root;
	IFbPtr m_fb;
	IGdiPtr m_gdi;
	IPlmanPtr m_plman;
	IUtilsPtr m_utils;
	IWindowPtr m_window;
	bool m_engine_inited = false;
	bool m_has_error = false;
	panel_window* m_host;
	pfc::refcounter m_counter = 1;
	std::unordered_map<DWORD, pfc::string8_fast> m_context_to_path_map;
	std::unordered_map<callback_id, DISPID> m_callback_map;
};
