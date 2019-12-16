#pragma once
#include "editorctrl.h"

class CDialogGoto : public CDialogImpl<CDialogGoto>
{
public:
	CDialogGoto(CWindow parent, const pfc::string8_fast& text);

	BEGIN_MSG_MAP(CDialogGoto)
		MSG_WM_INITDIALOG(OnInitDialog)
		COMMAND_RANGE_HANDLER_EX(IDOK, IDCANCEL, OnCloseCmd)
	END_MSG_MAP()

	enum { IDD = IDD_DIALOG_GOTO };

	BOOL OnInitDialog(CWindow, LPARAM);
	void OnCloseCmd(UINT, int, CWindow);

private:
	CEdit m_edit;
	CWindow m_parent = nullptr;
	pfc::string8_fast m_text;
};
