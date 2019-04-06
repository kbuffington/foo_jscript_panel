#pragma once

class CDialogFind : public CDialogImpl<CDialogFind>
{
public:
	CDialogFind(HWND p_hedit);

	BEGIN_MSG_MAP(CDialogFind)
		MSG_WM_INITDIALOG(OnInitDialog)
		COMMAND_ID_HANDLER_EX(IDC_FIND_NEXT, OnFindNext)
		COMMAND_ID_HANDLER_EX(IDC_FIND_PREVIOUS, OnFindPrevious)
		COMMAND_ID_HANDLER_EX(IDCANCEL, OnCancel)
		COMMAND_HANDLER_EX(IDC_EDIT_FIND_TEXT, EN_CHANGE, OnFindTextChange)
		COMMAND_RANGE_HANDLER_EX(IDC_CHECK_MATCHCASE, IDC_CHECK_REGEXP, OnFlagCommand)
	END_MSG_MAP()

	enum
	{
		IDD = IDD_DIALOG_FIND
	};

	BOOL OnInitDialog(HWND hwndFocus, LPARAM lParam);
	void OnCancel(UINT uNotifyCode, int nID, HWND wndCtl);
	void OnFinalMessage(HWND hWnd) override;
	void OnFindNext(UINT uNotifyCode, int nID, HWND wndCtl);
	void OnFindPrevious(UINT uNotifyCode, int nID, HWND wndCtl);
	void OnFindTextChange(UINT uNotifyCode, int nID, HWND wndCtl);
	void OnFlagCommand(UINT uNotifyCode, int nID, HWND wndCtl);

private:
	class CEditWithReturn : public CWindowImpl<CEditWithReturn, CEdit>
	{
	public:
		using parent = CWindowImpl<CEditWithReturn, CEdit>;

		BEGIN_MSG_MAP(CEditWithReturn)
			MESSAGE_HANDLER(WM_CHAR, OnChar)
			MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
		END_MSG_MAP()

		BOOL SubclassWindow(HWND hWnd, HWND hParent);
		LRESULT OnChar(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	private:
		HWND m_parent;
	};

	CEditWithReturn m_find;
	HWND m_hedit;
	int m_flags;
	pfc::string8_fast m_text;
};
