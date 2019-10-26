#include "stdafx.h"
#include "ui_find_replace.h"

static constexpr std::array<int, 12> ids =
{
	IDC_EDIT_FIND,
	IDC_EDIT_REPLACE,
	IDC_CHECK_MATCHCASE,
	IDC_CHECK_WHOLEWORD,
	IDC_CHECK_WORDSTART,
	IDC_CHECK_REGEXP,
	IDC_BTN_NEXT,
	IDC_BTN_PREVIOUS,
	IDC_BTN_REPLACE,
	IDC_BTN_REPLACE_ALL,
	IDC_LABEL_REPLACE,
	IDCANCEL
};

CDialogFindReplace::CDialogFindReplace(CScriptEditorCtrl* parent) : m_parent(parent), m_flags(0), m_havefound(false)
{
	m_hacks.assign(ids.size(), TabHack());
}

BOOL CDialogFindReplace::OnInitDialog(HWND, LPARAM)
{
	for (t_size i = 0; i < ids.size(); ++i)
	{
		const int id = ids[i];
		HWND hwnd = GetDlgItem(id);
		m_window[id] = hwnd;
		m_hacks[i].SubclassWindow(hwnd);
	}
	return TRUE;
}

void CDialogFindReplace::OnCancel(UINT uNotifyCode, int nID, HWND wndCtl)
{
	ShowWindow(SW_HIDE);
}

void CDialogFindReplace::OnFindNext(UINT uNotifyCode, int nID, HWND wndCtl)
{
	m_havefound = m_parent->FindNext();
}

void CDialogFindReplace::OnFindPrevious(UINT uNotifyCode, int nID, HWND wndCtl)
{
	m_parent->FindPrevious();
}

void CDialogFindReplace::OnFindTextChange(UINT uNotifyCode, int nID, HWND wndCtl)
{
	uGetWindowText(m_window.at(IDC_EDIT_FIND), m_find_text);
	m_window.at(IDC_BTN_NEXT).EnableWindow(m_find_text.get_length());
	m_window.at(IDC_BTN_PREVIOUS).EnableWindow(m_find_text.get_length());
	m_window.at(IDC_BTN_REPLACE).EnableWindow(m_find_text.get_length());
	m_window.at(IDC_BTN_REPLACE_ALL).EnableWindow(m_find_text.get_length());
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
		m_parent->Replace();
		m_havefound = false;
	}

	OnFindNext(0, 0, nullptr);
}

void CDialogFindReplace::OnReplaceAll(UINT uNotifyCode, int nID, HWND wndCtl)
{
	m_parent->ReplaceAll();
}

void CDialogFindReplace::OnReplaceTextChange(UINT uNotifyCode, int nID, HWND wndCtl)
{
	uGetWindowText(m_window.at(IDC_EDIT_REPLACE), m_replace_text);
}

void CDialogFindReplace::SetMode(mode m)
{
	bool find = m == mode::find;
	int cmd = find ? SW_HIDE : SW_SHOW;
	m_window.at(IDC_LABEL_REPLACE).ShowWindow(cmd);
	m_window.at(IDC_EDIT_REPLACE).ShowWindow(cmd);
	m_window.at(IDC_BTN_REPLACE).ShowWindow(cmd);
	m_window.at(IDC_BTN_REPLACE_ALL).ShowWindow(cmd);

	uSetWindowText(m_hWnd, find ? "Find" : "Replace");
	ShowWindow(SW_SHOW);
	SetFocus();
}

BOOL CDialogFindReplace::TabHack::SubclassWindow(HWND hwnd)
{
	return __super::SubclassWindow(hwnd);
}

LRESULT CDialogFindReplace::TabHack::OnChar(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CDialogFindReplace::TabHack::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	switch (wParam)
	{
	case VK_ESCAPE:
		::PostMessage(GetParent(), WM_COMMAND, MAKEWPARAM(IDCANCEL, BN_CLICKED), reinterpret_cast<LPARAM>(m_hWnd));
		return 0;
	case VK_TAB:
		::PostMessage(GetParent(), WM_NEXTDLGCTL, IsKeyPressed(VK_SHIFT) ? 1 : 0, 0);
		return 0;
	}
	return DefWindowProc(uMsg, wParam, lParam);
}
