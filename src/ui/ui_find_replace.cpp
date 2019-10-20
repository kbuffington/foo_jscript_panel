#include "stdafx.h"
#include "ui_find_replace.h"


BOOL CDialogFindReplace::OnInitDialog(HWND, LPARAM)
{
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
}

void CDialogFindReplace::SetMode(bool find)
{
	int cmd = find ? SW_HIDE : SW_SHOW;

	uSetWindowText(m_hWnd, find ? "Find" : "Replace");
	ShowWindow(SW_SHOW);
	SetFocus();
}
