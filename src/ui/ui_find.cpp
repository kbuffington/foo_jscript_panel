#include "stdafx.h"
#include "ui_conf.h"
#include "ui_find.h"

CDialogFind::CDialogFind(HWND p_hedit) : m_hedit(p_hedit), m_flags(0) {}

BOOL CDialogFind::OnInitDialog(HWND hwndFocus, LPARAM lParam)
{
	modeless_dialog_manager::g_add(m_hWnd);
	m_find.SubclassWindow(GetDlgItem(IDC_EDIT_FIND_TEXT), m_hWnd);
	return TRUE;
}

LRESULT CDialogFind::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{
	ShowWindow(SW_HIDE);
	return 0;
}

LRESULT CDialogFind::OnFindNext(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{
	if (m_text.get_length())
	{
		CDialogConf::FindNext(m_hWnd, m_hedit, m_flags, m_text.get_ptr());
	}
	return 0;
}

LRESULT CDialogFind::OnFindPrevious(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{
	if (m_text.get_length())
	{
		CDialogConf::FindPrevious(m_hWnd, m_hedit, m_flags, m_text.get_ptr());
	}
	return 0;
}

LRESULT CDialogFind::OnFindTextChange(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{
	uGetWindowText(GetDlgItem(IDC_EDIT_FIND_TEXT), m_text);
	return 0;
}

LRESULT CDialogFind::OnFlagCommand(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{
	bool check = uButton_GetCheck(m_hWnd, wID);
	int flag = 0;

	switch (wID)
	{
	case IDC_CHECK_MATCHCASE:
		flag = SCFIND_MATCHCASE;
		break;

	case IDC_CHECK_WHOLEWORD:
		flag = SCFIND_WHOLEWORD;
		break;

	case IDC_CHECK_WORDSTART:
		flag = SCFIND_WORDSTART;
		break;

	case IDC_CHECK_REGEXP:
		flag = SCFIND_REGEXP;
		break;
	}

	if (check)
		m_flags |= flag;
	else
		m_flags &= ~flag;

	return 0;
}

void CDialogFind::OnFinalMessage(HWND hWnd)
{
	modeless_dialog_manager::g_remove(m_hWnd);
	delete this;
}

BOOL CDialogFind::CEditWithReturn::SubclassWindow(HWND hWnd, HWND hParent)
{
	m_parent = hParent;
	return parent::SubclassWindow(hWnd);
}

LRESULT CDialogFind::CEditWithReturn::OnChar(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	switch (wParam)
	{
	case '\n':
	case '\r':
	case '\t':
	case '\x1b':
		return 0;
	}

	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CDialogFind::CEditWithReturn::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	switch (wParam)
	{
	case VK_RETURN:
		::PostMessage(m_parent, WM_COMMAND, MAKEWPARAM(IDC_FIND_NEXT, BN_CLICKED), (LPARAM)m_hWnd);
		return FALSE;

	case VK_ESCAPE:
		::PostMessage(m_parent, WM_COMMAND, MAKEWPARAM(IDCANCEL, BN_CLICKED), (LPARAM)m_hWnd);
		return FALSE;

	case VK_TAB:
		::PostMessage(m_parent, WM_NEXTDLGCTL, 0, 0);
		return FALSE;
	}

	return DefWindowProc(uMsg, wParam, lParam);
}
