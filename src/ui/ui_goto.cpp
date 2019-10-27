#include "stdafx.h"
#include "ui_goto.h"

CDialogGoto::CDialogGoto(HWND parent, const pfc::string8_fast& text) : m_parent(parent), m_text(text) {}

BOOL CDialogGoto::OnInitDialog(HWND, LPARAM)
{
	m_edit = GetDlgItem(IDC_EDIT_LINENUMBER);
	uSetWindowText(m_edit, m_text);
	return TRUE;
}

void CDialogGoto::OnCloseCmd(UINT, int nID, HWND)
{
	if (nID == IDOK)
	{
		uGetWindowText(m_edit, m_text);
		const t_size i = pfc::atoui_ex(m_text.get_ptr(), m_text.get_length()) - 1;
		SendMessage(m_parent, SCI_GOTOLINE, i, 0);
	}
	EndDialog(nID);
}
