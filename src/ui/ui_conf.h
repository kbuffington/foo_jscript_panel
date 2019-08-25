#pragma once
#include "editorctrl.h"

class panel_window;
class CDialogFind;
class CDialogReplace;

class CDialogConf : public CDialogImpl<CDialogConf>, public CDialogResize<CDialogConf>
{
public:
	CDialogConf(panel_window* p_parent);
	~CDialogConf();

	BEGIN_DLGRESIZE_MAP(CDialogConf)
		DLGRESIZE_CONTROL(IDC_EDIT, DLSZ_SIZE_X | DLSZ_SIZE_Y)
		DLGRESIZE_CONTROL(IDC_RESET, DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_STATIC_ENGINE, DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_COMBO_ENGINE, DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_STATIC_EDGE, DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_COMBO_EDGE, DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_CHECK_PSEUDO_TRANSPARENT, DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDOK, DLSZ_MOVE_X | DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_APPLY, DLSZ_MOVE_X | DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDCANCEL, DLSZ_MOVE_X | DLSZ_MOVE_Y)
	END_DLGRESIZE_MAP()

	BEGIN_MSG_MAP(CDialogConf)
		MSG_WM_INITDIALOG(OnInitDialog)
		MSG_WM_NOTIFY(OnNotify)
		MESSAGE_HANDLER(UWM_KEYDOWN, OnUwmKeyDown)
		MESSAGE_HANDLER(UWM_FIND_TEXT_CHANGED, OnUwmFindTextChanged)
		COMMAND_ID_HANDLER_EX(ID_FILE_APPLY, OnFileSave)
		COMMAND_ID_HANDLER_EX(ID_FILE_IMPORT, OnFileImport)
		COMMAND_ID_HANDLER_EX(ID_FILE_EXPORT, OnFileExport)
		COMMAND_ID_HANDLER_EX(IDC_RESET, OnReset)
		COMMAND_ID_HANDLER_EX(IDC_APPLY, OnCloseCmd)
		COMMAND_RANGE_HANDLER_EX(IDOK, IDCANCEL, OnCloseCmd)
		COMMAND_RANGE_HANDLER_EX(ID_SAMPLES_BEGIN, ID_SAMPLES_END, OnSamples)
		COMMAND_RANGE_HANDLER_EX(ID_DOCS_BEGIN, ID_DOCS_END, OnDocs)
		COMMAND_RANGE_HANDLER_EX(ID_LINKS_BEGIN, ID_LINKS_END, OnLinks)
		CHAIN_MSG_MAP(CDialogResize<CDialogConf>)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	enum
	{
		IDD = IDD_DIALOG_CONF
	};

	static bool FindNext(HWND hWnd, HWND hWndEdit, t_size flags, const char* which);
	static bool FindPrevious(HWND hWnd, HWND hWndEdit, t_size flags, const char* which);
	static bool FindResult(HWND hWnd, HWND hWndEdit, int pos, const char* which);

	BOOL OnInitDialog(HWND hwndFocus, LPARAM lParam);
	LRESULT OnNotify(int idCtrl, LPNMHDR pnmh);
	LRESULT OnUwmFindTextChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnUwmKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	void Apply();
	void OnCloseCmd(UINT uNotifyCode, int nID, HWND wndCtl);
	void OnDocs(UINT uNotifyCode, int nID, HWND wndCtl);
	void OnFileSave(UINT uNotifyCode, int nID, HWND wndCtl);
	void OnFileImport(UINT uNotifyCode, int nID, HWND wndCtl);
	void OnFileExport(UINT uNotifyCode, int nID, HWND wndCtl);
	void OnLinks(UINT uNotifyCode, int nID, HWND wndCtl);
	void OnReset(UINT uNotifyCode, int nID, HWND wndCtl);
	void OnSamples(UINT uNotifyCode, int nID, HWND wndCtl);
	void OpenFindDialog();

private:
	CButton m_pseudo;
	CDialogFind* m_dlgfind;
	CDialogReplace* m_dlgreplace;
	CComboBox m_edge;
	CComboBox m_engine;
	CMenu m_menu;
	CScriptEditorCtrl m_editorctrl;
	CWindow m_edit;
	panel_window* m_parent;
	pfc::string8_fast m_caption;
	pfc::string8_fast m_lastSearchText;
	pfc::string_list_impl m_docs;
	pfc::string_list_impl m_samples;
	t_size m_lastFlags;
};
