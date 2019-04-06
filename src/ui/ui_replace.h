#pragma once

class CDialogReplace : public CDialogImpl<CDialogReplace>
{
public:
	CDialogReplace(HWND p_hedit);

	BEGIN_MSG_MAP(CDialogReplace)
		MSG_WM_INITDIALOG(OnInitDialog)
		COMMAND_ID_HANDLER_EX(IDC_FIND_NEXT, OnFindNext)
		COMMAND_ID_HANDLER_EX(IDC_FIND_PREVIOUS, OnFindPrevious)
		COMMAND_ID_HANDLER_EX(IDCANCEL, OnCancel)
		COMMAND_ID_HANDLER_EX(IDC_REPLACE, OnReplace)
		COMMAND_ID_HANDLER_EX(IDC_REPLACE_ALL, OnReplaceAll)
		COMMAND_HANDLER_EX(IDC_EDIT_FIND_TEXT, EN_CHANGE, OnFindTextChange)
		COMMAND_HANDLER_EX(IDC_EDIT_REPLACE_TEXT, EN_CHANGE, OnReplaceTextChange)
		COMMAND_RANGE_HANDLER_EX(IDC_CHECK_MATCHCASE, IDC_CHECK_REGEXP, OnFlagCommand)
	END_MSG_MAP()

	enum
	{
		IDD = IDD_DIALOG_REPLACE
	};

	BOOL OnInitDialog(HWND hwndFocus, LPARAM lParam);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl);
	LRESULT OnFindNext(WORD wNotifyCode, WORD wID, HWND hWndCtl);
	LRESULT OnFindPrevious(WORD wNotifyCode, WORD wID, HWND hWndCtl);
	LRESULT OnFindTextChange(WORD wNotifyCode, WORD wID, HWND hWndCtl);
	LRESULT OnFlagCommand(WORD wNotifyCode, WORD wID, HWND hWndCtl);
	LRESULT OnReplace(WORD wNotifyCode, WORD wID, HWND hWndCtl);
	LRESULT OnReplaceAll(WORD wNotifyCode, WORD wID, HWND hWndCtl);
	LRESULT OnReplaceTextChange(WORD wNotifyCode, WORD wID, HWND hWndCtl);
	void OnFinalMessage(HWND hWnd) override;

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

	bool m_havefound;
	CEditWithReturn m_find, m_replace;
	HWND m_hedit;
	int m_flags;
	pfc::string8_fast m_reptext;
	pfc::string8_fast m_text;
};
