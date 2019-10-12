#pragma once

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
	script_host(host_comm* host);
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
	bool Ready();
	std::string ExtractValue(const std::string& source);
	void Finalise();
	void InvokeCallback(t_size callbackId, VARIANTARG* argv = nullptr, t_size argc = 0, VARIANT* ret = nullptr);
	void ProcessScriptInfo(t_script_info& info);

private:
	DWORD m_last_source_context;
	IActiveScriptPtr m_script_engine;
	IConsolePtr m_console;
	IDispatchPtr m_script_root;
	IFbPtr m_fb;
	IGdiPtr m_gdi;
	IPlmanPtr m_plman;
	IUtilsPtr m_utils;
	IWindowPtr m_window;
	bool m_engine_inited;
	host_comm* m_host;
	std::unordered_map<DWORD, pfc::string8_fast> m_context_to_path_map;
	std::unordered_map<t_size, DISPID> m_callback_map;
	volatile DWORD m_ref_count;
};
