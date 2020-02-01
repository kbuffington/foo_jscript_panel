#include "stdafx.h"
#include "ui_input_box.h"

CInputBox::CInputBox(pfc::stringp prompt, pfc::stringp caption, pfc::stringp value) : m_prompt(prompt), m_caption(caption), m_value(value) {}

BOOL CInputBox::OnInitDialog(CWindow, LPARAM)
{
	m_edit = GetDlgItem(IDC_EDIT_VALUE);
	m_label = GetDlgItem(IDC_LABEL_PROMPT);

	uSetWindowText(m_hWnd, m_caption);
	uSetWindowText(m_label, m_prompt);
	uSetWindowText(m_edit, m_value);
	CenterWindow();
	return FALSE;
}

void CInputBox::OnCloseCmd(UINT, int nID, CWindow)
{
	uGetWindowText(m_edit, m_value);
	EndDialog(nID);
}
