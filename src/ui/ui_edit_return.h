#pragma once

class CEditWithReturn : public CWindowImpl<CEditWithReturn, CEdit>
{
public:
	using parent = CWindowImpl<CEditWithReturn, CEdit>;

	BEGIN_MSG_MAP(CEditWithReturn)
		MESSAGE_HANDLER(WM_CHAR, OnChar)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	END_MSG_MAP()

	BOOL SubclassWindow(HWND hWnd, HWND hParent, int what);
	LRESULT OnChar(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

private:
	HWND m_parent;
	int m_what;
};
