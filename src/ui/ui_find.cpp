#include "stdafx.h"
#include "ui_find.h"

CDialogFind::CDialogFind(CScriptEditorCtrl* parent) : m_parent(parent), m_flags(0) {}

BOOL CDialogFind::OnInitDialog(HWND, LPARAM)
{
	m_find_edit.SubclassWindow(GetDlgItem(IDC_EDIT_FIND_TEXT), m_hWnd, IDC_FIND_NEXT);
	return TRUE;
}

void CDialogFind::OnCancel(UINT uNotifyCode, int nID, HWND wndCtl)
{
	ShowWindow(SW_HIDE);
}

void CDialogFind::OnFindNext(UINT uNotifyCode, int nID, HWND wndCtl)
{
	m_parent->last.update(m_hWnd, m_flags, m_find_text.get_ptr());
	m_parent->FindNext();
}

void CDialogFind::OnFindPrevious(UINT uNotifyCode, int nID, HWND wndCtl)
{
	m_parent->last.update(m_hWnd, m_flags, m_find_text.get_ptr());
	m_parent->FindPrevious();
}

void CDialogFind::OnFindTextChange(UINT uNotifyCode, int nID, HWND wndCtl)
{
	uGetWindowText(GetDlgItem(IDC_EDIT_FIND_TEXT), m_find_text);
	GetDlgItem(IDC_FIND_NEXT).EnableWindow(m_find_text.get_length());
	GetDlgItem(IDC_FIND_PREVIOUS).EnableWindow(m_find_text.get_length());
}

void CDialogFind::OnFlagCommand(UINT uNotifyCode, int nID, HWND wndCtl)
{
	if (uButton_GetCheck(m_hWnd, nID))
		m_flags |= m_parent->FlagMap.at(nID);
	else
		m_flags &= ~m_parent->FlagMap.at(nID);
}
