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

void CDialogFind::OnCancel(UINT uNotifyCode, int nID, HWND wndCtl)
{
	ShowWindow(SW_HIDE);
}

void CDialogFind::OnFinalMessage(HWND hWnd)
{
	modeless_dialog_manager::g_remove(m_hWnd);
	delete this;
}

void CDialogFind::OnFindNext(UINT uNotifyCode, int nID, HWND wndCtl)
{
	if (m_text.get_length())
	{
		CDialogConf::FindNext(m_hWnd, m_hedit, m_flags, m_text.get_ptr());
	}
}

void CDialogFind::OnFindPrevious(UINT uNotifyCode, int nID, HWND wndCtl)
{
	if (m_text.get_length())
	{
		CDialogConf::FindPrevious(m_hWnd, m_hedit, m_flags, m_text.get_ptr());
	}
}

void CDialogFind::OnFindTextChange(UINT uNotifyCode, int nID, HWND wndCtl)
{
	uGetWindowText(GetDlgItem(IDC_EDIT_FIND_TEXT), m_text);
}

void CDialogFind::OnFlagCommand(UINT uNotifyCode, int nID, HWND wndCtl)
{
	int flag = 0;

	switch (nID)
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

	if (uButton_GetCheck(m_hWnd, nID))
		m_flags |= flag;
	else
		m_flags &= ~flag;
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
		::PostMessage(m_parent, WM_COMMAND, MAKEWPARAM(IDC_FIND_NEXT, BN_CLICKED), reinterpret_cast<LPARAM>(m_hWnd));
		return 0;
	case VK_ESCAPE:
		::PostMessage(m_parent, WM_COMMAND, MAKEWPARAM(IDCANCEL, BN_CLICKED), reinterpret_cast<LPARAM>(m_hWnd));
		return 0;
	case VK_TAB:
		::PostMessage(m_parent, WM_NEXTDLGCTL, 0, 0);
		return 0;
	}
	return DefWindowProc(uMsg, wParam, lParam);
}
