#include "stdafx.h"
#include "ui_conf.h"
#include "ui_replace.h"

CDialogReplace::CDialogReplace(HWND p_hedit) : m_hedit(p_hedit), m_flags(0), m_havefound(false) {}

BOOL CDialogReplace::OnInitDialog(HWND, LPARAM)
{
	modeless_dialog_manager::g_add(m_hWnd);
	m_replace.SubclassWindow(GetDlgItem(IDC_EDIT_REPLACE_TEXT), m_hWnd);
	m_find.SubclassWindow(GetDlgItem(IDC_EDIT_FIND_TEXT), m_hWnd);
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
	if (m_text.get_length())
	{
		m_havefound = CDialogConf::FindNext(m_hWnd, m_hedit, m_flags, m_text.get_ptr());
	}
}

void CDialogReplace::OnFindPrevious(UINT uNotifyCode, int nID, HWND wndCtl)
{
	if (m_text.get_length())
	{
		CDialogConf::FindPrevious(m_hWnd, m_hedit, m_flags, m_text.get_ptr());
	}
}

void CDialogReplace::OnFindTextChange(UINT uNotifyCode, int nID, HWND wndCtl)
{
	uGetWindowText(GetDlgItem(IDC_EDIT_FIND_TEXT), m_text);
}

void CDialogReplace::OnFlagCommand(UINT uNotifyCode, int nID, HWND wndCtl)
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

void CDialogReplace::OnReplace(UINT uNotifyCode, int nID, HWND wndCtl)
{
	if (m_havefound)
	{
		CHARRANGE cr;
		cr.cpMin = SendMessage(m_hedit, SCI_GETSELECTIONSTART, 0, 0);
		cr.cpMax = SendMessage(m_hedit, SCI_GETSELECTIONEND, 0, 0);

		SendMessage(m_hedit, SCI_SETTARGETSTART, cr.cpMin, 0);
		SendMessage(m_hedit, SCI_SETTARGETEND, cr.cpMax, 0);
		SendMessage(m_hedit, SCI_REPLACETARGET, m_reptext.get_length(), reinterpret_cast<LPARAM>(m_reptext.get_ptr()));
		SendMessage(m_hedit, SCI_SETSEL, cr.cpMin + m_reptext.get_length(), cr.cpMin);
		m_havefound = false;
	}

	OnFindNext(0, 0, nullptr);
}

void CDialogReplace::OnReplaceAll(UINT uNotifyCode, int nID, HWND wndCtl)
{
	SendMessage(m_hedit, SCI_BEGINUNDOACTION, 0, 0);
	SendMessage(m_hedit, SCI_SETTARGETSTART, 0, 0);
	SendMessage(m_hedit, SCI_SETTARGETEND, 0, 0);

	while (true)
	{
		const int start_pos = SendMessage(m_hedit, SCI_GETTARGETEND, 0, 0);
		const int end_pos = SendMessage(m_hedit, SCI_GETLENGTH, 0, 0);

		SendMessage(m_hedit, SCI_SETTARGETSTART, start_pos, 0);
		SendMessage(m_hedit, SCI_SETTARGETEND, end_pos, 0);
		SendMessage(m_hedit, SCI_SETSEARCHFLAGS, m_flags, 0);

		const int occurance = SendMessage(m_hedit, SCI_SEARCHINTARGET, m_text.get_length(), reinterpret_cast<LPARAM>(m_text.get_ptr()));

		if (occurance == -1)
		{
			MessageBeep(MB_ICONINFORMATION);
			break;
		}

		SendMessage(m_hedit, SCI_REPLACETARGET, m_reptext.get_length(), reinterpret_cast<LPARAM>(m_reptext.get_ptr()));
		SendMessage(m_hedit, SCI_SETSEL, occurance + m_reptext.get_length(), occurance);
	}

	SendMessage(m_hedit, SCI_ENDUNDOACTION, 0, 0);
}

void CDialogReplace::OnReplaceTextChange(UINT uNotifyCode, int nID, HWND wndCtl)
{
	uGetWindowText(GetDlgItem(IDC_EDIT_REPLACE_TEXT), m_reptext);
}

BOOL CDialogReplace::CEditWithReturn::SubclassWindow(HWND hWnd, HWND hParent)
{
	m_parent = hParent;
	return parent::SubclassWindow(hWnd);
}

LRESULT CDialogReplace::CEditWithReturn::OnChar(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CDialogReplace::CEditWithReturn::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	switch (wParam)
	{
	case VK_RETURN:
		::PostMessage(m_parent, WM_COMMAND, MAKEWPARAM(IDC_REPLACE, BN_CLICKED), reinterpret_cast<LPARAM>(m_hWnd));
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
