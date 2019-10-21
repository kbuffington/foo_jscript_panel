#pragma once
#include "editorctrl.h"

class CDialogFindReplace : public CDialogImpl<CDialogFindReplace>
{
public:
	CDialogFindReplace(CScriptEditorCtrl* parent);

	BEGIN_MSG_MAP(CDialogFindReplace)
		MSG_WM_INITDIALOG(OnInitDialog)
		COMMAND_ID_HANDLER_EX(IDC_BTN_NEXT, OnFindNext)
		COMMAND_ID_HANDLER_EX(IDC_BTN_PREVIOUS, OnFindPrevious)
		COMMAND_ID_HANDLER_EX(IDCANCEL, OnCancel)
		COMMAND_ID_HANDLER_EX(IDC_BTN_REPLACE, OnReplace)
		COMMAND_ID_HANDLER_EX(IDC_BTN_REPLACE_ALL, OnReplaceAll)
		COMMAND_HANDLER_EX(IDC_EDIT_FIND, EN_CHANGE, OnFindTextChange)
		COMMAND_HANDLER_EX(IDC_EDIT_REPLACE, EN_CHANGE, OnReplaceTextChange)
		COMMAND_RANGE_HANDLER_EX(IDC_CHECK_MATCHCASE, IDC_CHECK_REGEXP, OnFlagCommand)
	END_MSG_MAP()

	enum { IDD = IDD_DIALOG_FIND_REPLACE };

	enum mode
	{
		find,
		replace
	};

	BOOL OnInitDialog(HWND, LPARAM);
	void OnCancel(UINT uNotifyCode, int nID, HWND wndCtl);
	void OnFindNext(UINT uNotifyCode, int nID, HWND wndCtl);
	void OnFindPrevious(UINT uNotifyCode, int nID, HWND wndCtl);
	void OnFindTextChange(UINT uNotifyCode, int nID, HWND wndCtl);
	void OnFlagCommand(UINT uNotifyCode, int nID, HWND wndCtl);
	void OnReplace(UINT uNotifyCode, int nID, HWND wndCtl);
	void OnReplaceAll(UINT uNotifyCode, int nID, HWND wndCtl);
	void OnReplaceTextChange(UINT uNotifyCode, int nID, HWND wndCtl);
	void SetMode(mode m);

private:
	class TabHack : public CWindowImpl<TabHack, CWindow>
	{
	public:
		BEGIN_MSG_MAP(TabHack)
			MESSAGE_HANDLER(WM_CHAR, OnChar)
			MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
		END_MSG_MAP()

		BOOL SubclassWindow(HWND wnd);
		LRESULT OnChar(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	};

	bool m_havefound;
	CScriptEditorCtrl* m_parent;
	int m_flags;
	pfc::string8_fast m_find_text, m_replace_text;
	std::map<int, CWindow> m_window;
	std::vector<TabHack> m_hacks;
};
