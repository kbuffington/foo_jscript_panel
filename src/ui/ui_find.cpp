#include "stdafx.h"
#include "ui_conf.h"
#include "ui_find.h"

CDialogFind::CDialogFind(CScriptEditorCtrl* parent) : m_parent(parent), m_flags(0) {}

BOOL CDialogFind::OnInitDialog(HWND, LPARAM)
{
	modeless_dialog_manager::g_add(m_hWnd);
	m_find.SubclassWindow(GetDlgItem(IDC_EDIT_FIND_TEXT), m_hWnd);
	GetDlgItem(IDC_FIND_NEXT).EnableWindow(false);
	GetDlgItem(IDC_FIND_PREVIOUS).EnableWindow(false);
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
	m_parent->last.find = m_text;
	m_parent->last.flags = m_flags;
	m_parent->last.wnd = m_hWnd;
	m_parent->FindNext();
}

void CDialogFind::OnFindPrevious(UINT uNotifyCode, int nID, HWND wndCtl)
{
	m_parent->last.find = m_text;
	m_parent->last.flags = m_flags;
	m_parent->last.wnd = m_hWnd;
	m_parent->FindPrevious();
}

void CDialogFind::OnFindTextChange(UINT uNotifyCode, int nID, HWND wndCtl)
{
	uGetWindowText(GetDlgItem(IDC_EDIT_FIND_TEXT), m_text);
	GetDlgItem(IDC_FIND_NEXT).EnableWindow(m_text.get_length());
	GetDlgItem(IDC_FIND_PREVIOUS).EnableWindow(m_text.get_length());
}

void CDialogFind::OnFlagCommand(UINT uNotifyCode, int nID, HWND wndCtl)
{
	if (uButton_GetCheck(m_hWnd, nID))
		m_flags |= m_parent->Flags.at(nID);
	else
		m_flags &= ~m_parent->Flags.at(nID);
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
