#pragma once

class CDialogPref : public CDialogImpl<CDialogPref>, public CWinDataExchange<CDialogPref>, public preferences_page_instance
{
public:
	CDialogPref(preferences_page_callback::ptr callback);

	BEGIN_DDX_MAP(CDialogPref)
		DDX_CONTROL_HANDLE(IDC_LIST_EDITOR_PROP, m_props)
	END_DDX_MAP()

	BEGIN_MSG_MAP(CDialogPref)
		MSG_WM_INITDIALOG(OnInitDialog)
		COMMAND_HANDLER_EX(IDC_EXPORT, BN_CLICKED, OnButtonExportBnClicked)
		COMMAND_HANDLER_EX(IDC_IMPORT, BN_CLICKED, OnButtonImportBnClicked)
		NOTIFY_HANDLER_EX(IDC_LIST_EDITOR_PROP, NM_DBLCLK, OnPropNMDblClk)
	END_MSG_MAP()

	enum
	{
		IDD = IDD_DIALOG_PREF
	};

	BOOL OnInitDialog(HWND hwndFocus, LPARAM lParam);
	HWND get_wnd();
	LRESULT OnPropNMDblClk(LPNMHDR pnmh);
	t_size get_state();
	void LoadProps(bool reset = false);
	void OnButtonExportBnClicked(WORD wNotifyCode, WORD wID, HWND hWndCtl);
	void OnButtonImportBnClicked(WORD wNotifyCode, WORD wID, HWND hWndCtl);
	void OnChanged();
	void OnEditChange(WORD, WORD, HWND);
	void uGetItemText(int nItem, int nSubItem, pfc::string_base& out);
	void apply();
	void reset();

private:
	CListViewCtrl m_props;
	preferences_page_callback::ptr m_callback;
};

class js_preferences_page_impl : public preferences_page_v3
{
public:
	GUID get_guid();
	GUID get_parent_guid();
	bool get_help_url(pfc::string_base& p_out);
	const char* get_name();
	preferences_page_instance::ptr instantiate(HWND parent, preferences_page_callback::ptr callback);
};
