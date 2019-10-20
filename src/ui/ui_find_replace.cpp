#include "stdafx.h"
#include "ui_find_replace.h"

CDialogFindReplace::CDialogFindReplace(CScriptEditorCtrl* parent) : m_parent(parent), m_flags(0), m_havefound(false) {}

BOOL CDialogFindReplace::OnInitDialog(HWND, LPARAM)
{
	m_find_edit = GetDlgItem(IDC_EDIT_FIND);
	m_replace_edit = GetDlgItem(IDC_EDIT_REPLACE);
	m_find_edit_ret.SubclassWindow(m_find_edit, IDC_BTN_NEXT);
	m_replace_edit_ret.SubclassWindow(m_replace_edit, IDC_BTN_REPLACE);
	m_next_btn = GetDlgItem(IDC_BTN_NEXT);
	m_previous_btn = GetDlgItem(IDC_BTN_PREVIOUS);
	m_replace_btn = GetDlgItem(IDC_BTN_REPLACE);
	m_replace_all_btn = GetDlgItem(IDC_BTN_REPLACE_ALL);
	m_replace_label = GetDlgItem(IDC_LABEL_REPLACE);
	return TRUE;
}

void CDialogFindReplace::OnCancel(UINT uNotifyCode, int nID, HWND wndCtl)
{
	ShowWindow(SW_HIDE);
}

void CDialogFindReplace::OnFindNext(UINT uNotifyCode, int nID, HWND wndCtl)
{
	m_parent->last.update(m_hWnd, m_flags, m_find_text.get_ptr(), m_replace_text.get_ptr());
	m_havefound = m_parent->FindNext();
}

void CDialogFindReplace::OnFindPrevious(UINT uNotifyCode, int nID, HWND wndCtl)
{
	m_parent->last.update(m_hWnd, m_flags, m_find_text.get_ptr(), m_replace_text.get_ptr());
	m_parent->FindPrevious();
}

void CDialogFindReplace::OnFindTextChange(UINT uNotifyCode, int nID, HWND wndCtl)
{
	uGetWindowText(m_find_edit, m_find_text);
	m_next_btn.EnableWindow(m_find_text.get_length());
	m_previous_btn.EnableWindow(m_find_text.get_length());
	m_replace_btn.EnableWindow(m_find_text.get_length());
	m_replace_all_btn.EnableWindow(m_find_text.get_length());
}

void CDialogFindReplace::OnFlagCommand(UINT uNotifyCode, int nID, HWND wndCtl)
{
	if (uButton_GetCheck(m_hWnd, nID))
		m_flags |= m_parent->FlagMap.at(nID);
	else
		m_flags &= ~m_parent->FlagMap.at(nID);
}

void CDialogFindReplace::OnReplace(UINT uNotifyCode, int nID, HWND wndCtl)
{
	if (m_havefound)
	{
		m_parent->last.update(m_hWnd, m_flags, m_find_text.get_ptr(), m_replace_text.get_ptr());
		m_parent->Replace();
		m_havefound = false;
	}

	OnFindNext(0, 0, nullptr);
}

void CDialogFindReplace::OnReplaceAll(UINT uNotifyCode, int nID, HWND wndCtl)
{
	m_parent->last.update(m_hWnd, m_flags, m_find_text.get_ptr(), m_replace_text.get_ptr());
	m_parent->ReplaceAll();
}

void CDialogFindReplace::OnReplaceTextChange(UINT uNotifyCode, int nID, HWND wndCtl)
{
	uGetWindowText(m_replace_edit, m_replace_text);
}

void CDialogFindReplace::SetMode(bool find)
{
	int cmd = find ? SW_HIDE : SW_SHOW;
	m_replace_label.ShowWindow(cmd);
	m_replace_edit.ShowWindow(cmd);
	m_replace_btn.ShowWindow(cmd);
	m_replace_all_btn.ShowWindow(cmd);

	uSetWindowText(m_hWnd, find ? "Find" : "Replace");
	ShowWindow(SW_SHOW);
	SetFocus();
}
