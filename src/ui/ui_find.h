#pragma once

class CDialogFind : public CDialogImpl<CDialogFind>, public CDialogResize<CDialogFind>
{
public:
	CDialogFind(HWND p_hedit);

	BEGIN_DLGRESIZE_MAP(CDialogFind)
		DLGRESIZE_CONTROL(IDC_EDIT_FINDWHAT, DLSZ_SIZE_X)
		DLGRESIZE_CONTROL(IDC_FINDUP, DLSZ_MOVE_X)
		DLGRESIZE_CONTROL(IDC_FINDDOWN, DLSZ_MOVE_X)
		DLGRESIZE_CONTROL(IDCANCEL, DLSZ_MOVE_X)
	END_DLGRESIZE_MAP()

	BEGIN_MSG_MAP(CDialogFind)
		MSG_WM_INITDIALOG(OnInitDialog)
		COMMAND_ID_HANDLER_EX(IDC_FINDUP, OnFindUp)
		COMMAND_ID_HANDLER_EX(IDC_FINDDOWN, OnFindDown)
		COMMAND_HANDLER_EX(IDC_EDIT_FINDWHAT, EN_CHANGE, OnEditFindWhatEnChange)
		COMMAND_RANGE_HANDLER_EX(IDC_CHECK_MATCHCASE, IDC_CHECK_REGEXP, OnFlagCommand)
		COMMAND_ID_HANDLER_EX(IDCANCEL, OnCancel)
		CHAIN_MSG_MAP(CDialogResize<CDialogFind>)
	END_MSG_MAP()

	enum
	{
		IDD = IDD_DIALOG_FIND
	};

	BOOL OnInitDialog(HWND hwndFocus, LPARAM lParam);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl);
	LRESULT OnEditFindWhatEnChange(WORD wNotifyCode, WORD wID, HWND hWndCtl);
	LRESULT OnFindDown(WORD wNotifyCode, WORD wID, HWND hWndCtl);
	LRESULT OnFindUp(WORD wNotifyCode, WORD wID, HWND hWndCtl);
	LRESULT OnFlagCommand(WORD wNotifyCode, WORD wID, HWND hWndCtl);
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

	CEditWithReturn m_find;
	HWND m_hedit;
	int m_flags;
	pfc::string8_fast m_text;
};
