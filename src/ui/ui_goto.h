#pragma once

class CDialogGoto : public CDialogImpl<CDialogGoto>
{
public:
	CDialogGoto(HWND p_hedit);

	BEGIN_MSG_MAP(CDialogGoto)
		MSG_WM_INITDIALOG(OnInitDialog)
		COMMAND_RANGE_HANDLER_EX(IDOK, IDCANCEL, OnCloseCmd)
	END_MSG_MAP()

	enum
	{
		IDD = IDD_DIALOG_GOTO
	};

	BOOL OnInitDialog(HWND hwndFocus, LPARAM lParam);
	LRESULT OnCloseCmd(WORD wNotifyCode, WORD wID, HWND hWndCtl);

private:
	HWND m_hedit;
};
