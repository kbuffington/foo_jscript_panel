#include "stdafx.h"
#include "ui_conf.h"
#include "ui_replace.h"

CDialogReplace::CDialogReplace(CScriptEditorCtrl* parent) : m_parent(parent), m_flags(0), m_havefound(false) {}

BOOL CDialogReplace::OnInitDialog(HWND, LPARAM)
{
	modeless_dialog_manager::g_add(m_hWnd);
	m_replace.SubclassWindow(GetDlgItem(IDC_EDIT_REPLACE_TEXT), m_hWnd);
	m_find.SubclassWindow(GetDlgItem(IDC_EDIT_FIND_TEXT), m_hWnd);
	GetDlgItem(IDC_FIND_NEXT).EnableWindow(false);
	GetDlgItem(IDC_FIND_PREVIOUS).EnableWindow(false);
	GetDlgItem(IDC_REPLACE).EnableWindow(false);
	GetDlgItem(IDC_REPLACE_ALL).EnableWindow(false);
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
		m_parent->last.find = m_text;
		m_parent->last.flags = m_flags;
		m_parent->last.wnd = m_hWnd;
		m_havefound = m_parent->FindNext();
	}
}

void CDialogReplace::OnFindPrevious(UINT uNotifyCode, int nID, HWND wndCtl)
{
	if (m_text.get_length())
	{
		m_parent->last.find = m_text;
		m_parent->last.flags = m_flags;
		m_parent->last.wnd = m_hWnd;
		m_parent->FindPrevious();
	}
}

void CDialogReplace::OnFindTextChange(UINT uNotifyCode, int nID, HWND wndCtl)
{
	uGetWindowText(GetDlgItem(IDC_EDIT_FIND_TEXT), m_text);
	GetDlgItem(IDC_FIND_NEXT).EnableWindow(m_text.get_length());
	GetDlgItem(IDC_FIND_PREVIOUS).EnableWindow(m_text.get_length());
	GetDlgItem(IDC_REPLACE).EnableWindow(m_text.get_length() && m_reptext.get_length());
	GetDlgItem(IDC_REPLACE_ALL).EnableWindow(m_text.get_length() && m_reptext.get_length());
}

void CDialogReplace::OnFlagCommand(UINT uNotifyCode, int nID, HWND wndCtl)
{
	if (uButton_GetCheck(m_hWnd, nID))
		m_flags |= m_parent->Flags.at(nID);
	else
		m_flags &= ~m_parent->Flags.at(nID);
}

void CDialogReplace::OnReplace(UINT uNotifyCode, int nID, HWND wndCtl)
{
	if (m_havefound)
	{
		m_parent->last.find = m_text;
		m_parent->last.flags = m_flags;
		m_parent->last.replace = m_reptext;
		m_parent->last.wnd = m_hWnd;
		m_parent->Replace();
		m_havefound = false;
	}

	OnFindNext(0, 0, nullptr);
}

void CDialogReplace::OnReplaceAll(UINT uNotifyCode, int nID, HWND wndCtl)
{
	m_parent->last.find = m_text;
	m_parent->last.flags = m_flags;
	m_parent->last.replace = m_reptext;
	m_parent->last.wnd = m_hWnd;
	m_parent->ReplaceAll();
}

void CDialogReplace::OnReplaceTextChange(UINT uNotifyCode, int nID, HWND wndCtl)
{
	uGetWindowText(GetDlgItem(IDC_EDIT_REPLACE_TEXT), m_reptext);
	GetDlgItem(IDC_REPLACE).EnableWindow(m_text.get_length() && m_reptext.get_length());
	GetDlgItem(IDC_REPLACE_ALL).EnableWindow(m_text.get_length() && m_reptext.get_length());
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
