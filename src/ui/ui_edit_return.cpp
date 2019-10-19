#include "stdafx.h"
#include "ui_edit_return.h"

BOOL CEditWithReturn::SubclassWindow(HWND hWnd, int what)
{
	m_what = what;
	return __super::SubclassWindow(hWnd);
}

LRESULT CEditWithReturn::OnChar(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CEditWithReturn::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	switch (wParam)
	{
	case VK_RETURN:
		::PostMessage(GetParent(), WM_COMMAND, MAKEWPARAM(m_what, BN_CLICKED), reinterpret_cast<LPARAM>(m_hWnd));
		return 0;
	case VK_ESCAPE:
		::PostMessage(GetParent(), WM_COMMAND, MAKEWPARAM(IDCANCEL, BN_CLICKED), reinterpret_cast<LPARAM>(m_hWnd));
		return 0;
	case VK_TAB:
		::PostMessage(GetParent(), WM_NEXTDLGCTL, 0, 0);
		return 0;
	}
	return DefWindowProc(uMsg, wParam, lParam);
}
