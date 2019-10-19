#include "stdafx.h"
#include "ui_goto.h"

CDialogGoto::CDialogGoto(HWND parent, const pfc::string8_fast& text) : m_parent(parent), m_text(text) {}

BOOL CDialogGoto::OnInitDialog(HWND, LPARAM)
{
	uSetWindowText(GetDlgItem(IDC_EDIT_LINENUMBER), m_text);
	return TRUE;
}

void CDialogGoto::OnCloseCmd(UINT uNotifyCode, int nID, HWND wndCtl)
{
	if (nID == IDOK)
	{
		uGetWindowText(GetDlgItem(IDC_EDIT_LINENUMBER), m_text);
		const t_size i = pfc::atoui_ex(m_text.get_ptr(), m_text.length()) - 1;
		SendMessage(m_parent, SCI_GOTOLINE, i, 0);
	}
	EndDialog(nID);
}
