#pragma once
#include "editorctrl.h"

class panel_window;

class CDialogConf : public CDialogImpl<CDialogConf>
{
public:
	CDialogConf(panel_window* p_parent);
	~CDialogConf();

	BEGIN_MSG_MAP(CDialogConf)
		CHAIN_MSG_MAP_MEMBER(m_resizer)
		MSG_WM_INITDIALOG(OnInitDialog)
		MSG_WM_NOTIFY(OnNotify)
		COMMAND_ID_HANDLER_EX(ID_FILE_IMPORT, OnFileImport)
		COMMAND_ID_HANDLER_EX(ID_FILE_EXPORT, OnFileExport)
		COMMAND_ID_HANDLER_EX(IDC_BTN_RESET, OnReset)
		COMMAND_ID_HANDLER_EX(IDC_BTN_APPLY, OnCloseCmd)
		COMMAND_RANGE_HANDLER_EX(IDOK, IDCANCEL, OnCloseCmd)
		COMMAND_RANGE_HANDLER_EX(ID_SAMPLES_BEGIN, ID_SAMPLES_END, OnSamples)
		COMMAND_RANGE_HANDLER_EX(ID_DOCS_BEGIN, ID_DOCS_END, OnDocs)
		COMMAND_RANGE_HANDLER_EX(ID_LINKS_BEGIN, ID_LINKS_END, OnLinks)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	enum { IDD = IDD_DIALOG_CONF };

	BOOL OnInitDialog(HWND, LPARAM);
	LRESULT OnNotify(int, LPNMHDR);
	void Apply();
	void BuildMenu();
	void OnCloseCmd(UINT, int, HWND);
	void OnDocs(UINT, int, HWND);
	void OnFileImport(UINT, int, HWND);
	void OnFileExport(UINT, int, HWND);
	void OnLinks(UINT, int, HWND);
	void OnReset(UINT, int, HWND);
	void OnSamples(UINT, int, HWND);

private:
	CCheckBox m_pseudo_check;
	CComboBox m_edge_combo;
	CComboBox m_engine_combo;
	CDialogResizeHelper m_resizer;
	CMenu m_menu;
	CScriptEditorCtrl m_editorctrl;
	panel_window* m_parent;
	pfc::string8_fast m_caption;
	pfc::string_list_impl m_docs;
	pfc::string_list_impl m_samples;
};
