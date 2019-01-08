#pragma once

#include "script_preprocessor.h"
#include "script_interface_impl.h"
#include "config.h"
#include "user_message.h"
#include "host_drop_target.h"
#include "host_timer_dispatcher.h"
#include "script_callback_invoker.h"

// Smart pointers for Active Scripting
_COM_SMARTPTR_TYPEDEF(IActiveScriptParse, IID_IActiveScriptParse);

class HostComm : public js_panel_vars
{
protected:
	HostComm();
	virtual ~HostComm();

	HBITMAP m_gr_bmp;
	HBITMAP m_gr_bmp_bk;
	HDC m_hdc;
	HWND m_hwnd;
	POINT m_max_size;
	POINT m_min_size;
	bool m_paint_pending;
	bool m_suppress_drawing;
	int m_height;
	int m_instance_type;
	int m_width;
	panel_tooltip_param_ptr m_panel_tooltip_param_ptr;
	t_script_info m_script_info;
	ui_selection_holder::ptr m_selection_holder;

public:
	enum
	{
		KInstanceTypeCUI = 0,
		KInstanceTypeDUI,
	};

	HDC GetHDC();
	HWND GetHWND();
	POINT& MaxSize();
	POINT& MinSize();
	int GetHeight();
	int GetWidth();
	panel_tooltip_param_ptr& PanelTooltipParam();
	t_script_info& ScriptInfo();
	t_size GetInstanceType();
	virtual DWORD GetColourCUI(unsigned type, const GUID& guid) = 0;
	virtual DWORD GetColourDUI(unsigned type) = 0;
	virtual HFONT GetFontCUI(unsigned type, const GUID& guid) = 0;
	virtual HFONT GetFontDUI(unsigned type) = 0;
	void Redraw();
	void RefreshBackground(LPRECT lprcUpdate = NULL);
	void Repaint(bool force = false);
	void RepaintRect(LONG x, LONG y, LONG w, LONG h, bool force = false);
};

class ScriptHost : public IActiveScriptSite, public IActiveScriptSiteWindow
{
public:
	ScriptHost(HostComm* host);
	virtual ~ScriptHost();

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
	void ReportError(IActiveScriptError* err);
	void Stop();

private:
	DWORD m_lastSourceContext;
	HostComm* m_host;
	IActiveScriptPtr m_script_engine;
	IDispatchPtr m_script_root;
	IFbPlaylistManagerPtr m_playlistman;
	IFbUtilsPtr m_fb2k;
	IFbWindowPtr m_window;
	IGdiUtilsPtr m_gdi;
	IJSUtilsPtr m_utils;
	IJSConsolePtr m_console;
	ScriptCallbackInvoker m_callback_invoker;
	bool m_engine_inited;
	bool m_has_error;
	pfc::map_t<DWORD, pfc::string8> m_contextToPathMap;
	pfc::tickcount_t m_dwStartTime;
	volatile DWORD m_dwRef;

	BEGIN_COM_QI_IMPL()
		COM_QI_ENTRY_MULTI(IUnknown, IActiveScriptSite)
		COM_QI_ENTRY(IActiveScriptSite)
		COM_QI_ENTRY(IActiveScriptSiteWindow)
	END_COM_QI_IMPL()
};
