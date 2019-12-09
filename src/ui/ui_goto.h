#pragma once
#include "editorctrl.h"

class CDialogGoto : public CDialogImpl<CDialogGoto>
{
public:
	CDialogGoto(HWND parent, const pfc::string8_fast& text);

	BEGIN_MSG_MAP(CDialogGoto)
		MSG_WM_INITDIALOG(OnInitDialog)
		COMMAND_RANGE_HANDLER_EX(IDOK, IDCANCEL, OnCloseCmd)
	END_MSG_MAP()

	enum { IDD = IDD_DIALOG_GOTO };

	BOOL OnInitDialog(HWND, LPARAM);
	void OnCloseCmd(UINT, int, HWND);

private:
	CEdit m_edit;
	HWND m_parent = nullptr;
	pfc::string8_fast m_text;
};
