#include "stdafx.h"
#include "ui_goto.h"

CDialogGoto::CDialogGoto(HWND p_hedit) : m_hedit(p_hedit) {}

BOOL CDialogGoto::OnInitDialog(HWND hwndFocus, LPARAM lParam)
{
	const int cur_pos = SendMessage(m_hedit, SCI_GETCURRENTPOS, 0, 0);
	const int cur_line = SendMessage(m_hedit, SCI_LINEFROMPOSITION, cur_pos, 0) + 1;

	uSetWindowText(GetDlgItem(IDC_EDIT_LINENUMBER), pfc::format_int(cur_line).get_ptr());

	return TRUE;
}

void CDialogGoto::OnCloseCmd(UINT uNotifyCode, int nID, HWND wndCtl)
{
	if (nID == IDOK)
	{
		pfc::string8_fast text;
		uGetWindowText(GetDlgItem(IDC_EDIT_LINENUMBER), text);
		const t_size i = pfc::atoui_ex(text.get_ptr(), text.length()) - 1;
		SendMessage(m_hedit, SCI_GOTOLINE, i, 0);
	}
	EndDialog(nID);
}
