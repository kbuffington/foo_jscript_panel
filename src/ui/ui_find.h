#pragma once
#include "editorctrl.h"
#include "ui_edit_return.h"

class CDialogFind : public CDialogImpl<CDialogFind>
{
public:
	CDialogFind(CScriptEditorCtrl* parent);

	BEGIN_MSG_MAP(CDialogFind)
		MSG_WM_INITDIALOG(OnInitDialog)
		COMMAND_ID_HANDLER_EX(IDC_FIND_NEXT, OnFindNext)
		COMMAND_ID_HANDLER_EX(IDC_FIND_PREVIOUS, OnFindPrevious)
		COMMAND_ID_HANDLER_EX(IDCANCEL, OnCancel)
		COMMAND_HANDLER_EX(IDC_EDIT_FIND_TEXT, EN_CHANGE, OnFindTextChange)
		COMMAND_RANGE_HANDLER_EX(IDC_CHECK_MATCHCASE, IDC_CHECK_REGEXP, OnFlagCommand)
	END_MSG_MAP()

	enum
	{
		IDD = IDD_DIALOG_FIND
	};

	BOOL OnInitDialog(HWND, LPARAM);
	void OnCancel(UINT uNotifyCode, int nID, HWND wndCtl);
	void OnFinalMessage(HWND hWnd) override;
	void OnFindNext(UINT uNotifyCode, int nID, HWND wndCtl);
	void OnFindPrevious(UINT uNotifyCode, int nID, HWND wndCtl);
	void OnFindTextChange(UINT uNotifyCode, int nID, HWND wndCtl);
	void OnFlagCommand(UINT uNotifyCode, int nID, HWND wndCtl);

private:
	CEditWithReturn m_find_edit;
	CScriptEditorCtrl* m_parent;
	int m_flags;
	pfc::string8_fast m_find_text;
};
