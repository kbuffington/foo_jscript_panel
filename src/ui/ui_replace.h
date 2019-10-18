#pragma once
#include "editorctrl.h"
#include "ui_edit_return.h"

class CDialogReplace : public CDialogImpl<CDialogReplace>
{
public:
	CDialogReplace(CScriptEditorCtrl* parent);

	BEGIN_MSG_MAP(CDialogReplace)
		MSG_WM_INITDIALOG(OnInitDialog)
		COMMAND_ID_HANDLER_EX(IDC_FIND_NEXT, OnFindNext)
		COMMAND_ID_HANDLER_EX(IDC_FIND_PREVIOUS, OnFindPrevious)
		COMMAND_ID_HANDLER_EX(IDCANCEL, OnCancel)
		COMMAND_ID_HANDLER_EX(IDC_REPLACE, OnReplace)
		COMMAND_ID_HANDLER_EX(IDC_REPLACE_ALL, OnReplaceAll)
		COMMAND_HANDLER_EX(IDC_EDIT_FIND_TEXT, EN_CHANGE, OnFindTextChange)
		COMMAND_HANDLER_EX(IDC_EDIT_REPLACE_TEXT, EN_CHANGE, OnReplaceTextChange)
		COMMAND_RANGE_HANDLER_EX(IDC_CHECK_MATCHCASE, IDC_CHECK_REGEXP, OnFlagCommand)
	END_MSG_MAP()

	enum
	{
		IDD = IDD_DIALOG_REPLACE
	};

	BOOL OnInitDialog(HWND, LPARAM);
	void OnCancel(UINT uNotifyCode, int nID, HWND wndCtl);
	void OnFinalMessage(HWND hWnd) override;
	void OnFindNext(UINT uNotifyCode, int nID, HWND wndCtl);
	void OnFindPrevious(UINT uNotifyCode, int nID, HWND wndCtl);
	void OnFindTextChange(UINT uNotifyCode, int nID, HWND wndCtl);
	void OnFlagCommand(UINT uNotifyCode, int nID, HWND wndCtl);
	void OnReplace(UINT uNotifyCode, int nID, HWND wndCtl);
	void OnReplaceAll(UINT uNotifyCode, int nID, HWND wndCtl);
	void OnReplaceTextChange(UINT uNotifyCode, int nID, HWND wndCtl);

private:
	bool m_havefound;
	CEditWithReturn m_find_edit, m_replace_edit;
	CScriptEditorCtrl* m_parent;
	int m_flags;
	pfc::string8_fast m_find_text, m_replace_text;
};
