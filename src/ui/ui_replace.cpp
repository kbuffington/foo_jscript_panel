#include "stdafx.h"
#include "ui_replace.h"

CDialogReplace::CDialogReplace(CScriptEditorCtrl* parent) : m_parent(parent), m_flags(0), m_havefound(false) {}

BOOL CDialogReplace::OnInitDialog(HWND, LPARAM)
{
	modeless_dialog_manager::g_add(m_hWnd);
	m_find_edit.SubclassWindow(GetDlgItem(IDC_EDIT_FIND_TEXT), m_hWnd, IDC_FIND_NEXT);
	m_replace_edit.SubclassWindow(GetDlgItem(IDC_EDIT_REPLACE_TEXT), m_hWnd, IDC_REPLACE);
	return TRUE;
}

void CDialogReplace::OnCancel(UINT uNotifyCode, int nID, HWND wndCtl)
{
	ShowWindow(SW_HIDE);
}

void CDialogReplace::OnFinalMessage(HWND hWnd)
{
	modeless_dialog_manager::g_remove(m_hWnd);
	delete this;
}

void CDialogReplace::OnFindNext(UINT uNotifyCode, int nID, HWND wndCtl)
{
	m_parent->last.update(m_hWnd, m_flags, m_find_text.get_ptr());
	m_havefound = m_parent->FindNext();
}

void CDialogReplace::OnFindPrevious(UINT uNotifyCode, int nID, HWND wndCtl)
{
	m_parent->last.update(m_hWnd, m_flags, m_find_text.get_ptr());
	m_parent->FindPrevious();
}

void CDialogReplace::OnFindTextChange(UINT uNotifyCode, int nID, HWND wndCtl)
{
	uGetWindowText(GetDlgItem(IDC_EDIT_FIND_TEXT), m_find_text);
	GetDlgItem(IDC_FIND_NEXT).EnableWindow(m_find_text.get_length());
	GetDlgItem(IDC_FIND_PREVIOUS).EnableWindow(m_find_text.get_length());
	GetDlgItem(IDC_REPLACE).EnableWindow(m_find_text.get_length());
	GetDlgItem(IDC_REPLACE_ALL).EnableWindow(m_find_text.get_length());
}

void CDialogReplace::OnFlagCommand(UINT uNotifyCode, int nID, HWND wndCtl)
{
	if (uButton_GetCheck(m_hWnd, nID))
		m_flags |= m_parent->FlagMap.at(nID);
	else
		m_flags &= ~m_parent->FlagMap.at(nID);
}

void CDialogReplace::OnReplace(UINT uNotifyCode, int nID, HWND wndCtl)
{
	if (m_havefound)
	{
		m_parent->last.update(m_hWnd, m_flags, m_find_text.get_ptr(), m_replace_text.get_ptr());
		m_parent->Replace();
		m_havefound = false;
	}

	OnFindNext(0, 0, nullptr);
}

void CDialogReplace::OnReplaceAll(UINT uNotifyCode, int nID, HWND wndCtl)
{
	m_parent->last.update(m_hWnd, m_flags, m_find_text.get_ptr(), m_replace_text.get_ptr());
	m_parent->ReplaceAll();
}

void CDialogReplace::OnReplaceTextChange(UINT uNotifyCode, int nID, HWND wndCtl)
{
	uGetWindowText(GetDlgItem(IDC_EDIT_REPLACE_TEXT), m_replace_text);
}
