#pragma once
#include "editorctrl.h"

// Forward declarations
class js_panel_window;
class CDialogFind;
class CDialogReplace;

class CDialogConf : public CDialogImpl<CDialogConf>, public CDialogResize<CDialogConf>
{
public:
	CDialogConf(js_panel_window* p_parent);
	virtual ~CDialogConf();

	BEGIN_DLGRESIZE_MAP(CDialogConf)
		DLGRESIZE_CONTROL(IDC_EDIT, DLSZ_SIZE_X | DLSZ_SIZE_Y)
		DLGRESIZE_CONTROL(IDC_RESET, DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_STATIC_ENGINE, DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_COMBO_ENGINE, DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_STATIC_EDGE, DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_COMBO_EDGE, DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_CHECK_PSEUDO_TRANSPARENT, DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_CHECK_GRABFOCUS, DLSZ_MOVE_Y)
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
		COMMAND_RANGE_HANDLER_EX(ID_SAMPLES_BEGIN, ID_SAMPLES_END, OnSamples)
		COMMAND_RANGE_HANDLER_EX(ID_DOCS_BEGIN, ID_DOCS_END, OnDocs)
		COMMAND_RANGE_HANDLER_EX(ID_HELP_BEGIN, ID_HELP_END, OnHelp)
		COMMAND_RANGE_HANDLER_EX(IDOK, IDCANCEL, OnCloseCmd)
		COMMAND_ID_HANDLER_EX(IDC_APPLY, OnCloseCmd)
		COMMAND_ID_HANDLER_EX(IDC_RESET, OnReset)
		CHAIN_MSG_MAP(CDialogResize<CDialogConf>)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	enum
	{
		IDD = IDD_DIALOG_CONF
	};

	BOOL OnInitDialog(HWND hwndFocus, LPARAM lParam);
	LRESULT OnCloseCmd(WORD wNotifyCode, WORD wID, HWND hWndCtl);
	LRESULT OnDocs(WORD wNotifyCode, WORD wID, HWND hWndCtl);
	LRESULT OnFileSave(WORD wNotifyCode, WORD wID, HWND hWndCtl);
	LRESULT OnFileImport(WORD wNotifyCode, WORD wID, HWND hWndCtl);
	LRESULT OnFileExport(WORD wNotifyCode, WORD wID, HWND hWndCtl);
	LRESULT OnHelp(WORD wNotifyCode, WORD wID, HWND hWndCtl);
	LRESULT OnNotify(int idCtrl, LPNMHDR pnmh);
	LRESULT OnReset(WORD wNotifyCode, WORD wID, HWND hWndCtl);
	LRESULT OnSamples(WORD wNotifyCode, WORD wID, HWND hWndCtl);
	LRESULT OnUwmFindTextChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnUwmKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	static bool FindNext(HWND hWnd, HWND hWndEdit, t_size flags, const char* which);
	static bool FindPrevious(HWND hWnd, HWND hWndEdit, t_size flags, const char* which);
	static bool FindResult(HWND hWnd, HWND hWndEdit, int pos, const char* which);
	void Apply();
	void OpenFindDialog();

private:
	CDialogFind* m_dlgfind;
	CDialogReplace* m_dlgreplace;
	CMenu m_menu;
	CScriptEditorCtrl m_editorctrl;
	js_panel_window* m_parent;
	pfc::string8_fast m_caption;
	pfc::string8_fast m_lastSearchText;
	pfc::string_list_impl m_docs;
	pfc::string_list_impl m_samples;
	t_size m_lastFlags;
};
