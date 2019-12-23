#pragma once
#include "editorctrl.h"

class CDialogFindReplace : public CDialogImpl<CDialogFindReplace>
{
public:
	CDialogFindReplace(CScriptEditorCtrl* parent);

	BEGIN_MSG_MAP_EX(CDialogFindReplace)
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

	enum class mode
	{
		find,
		replace
	};

	BOOL OnInitDialog(CWindow, LPARAM);
	void OnCancel(UINT, int, CWindow);
	void OnFindNext(UINT, int, CWindow);
	void OnFindPrevious(UINT, int, CWindow);
	void OnFindTextChange(UINT, int, CWindow);
	void OnFlagCommand(UINT, int nID, CWindow);
	void OnReplace(UINT, int, CWindow);
	void OnReplaceAll(UINT, int, CWindow);
	void OnReplaceTextChange(UINT, int, CWindow);
	void SetMode(mode m);

	int m_flags = 0;
	pfc::string8_fast m_find_text, m_replace_text;

private:
	class KeyHack : public CWindowImpl<KeyHack, CWindow>
	{
	public:
		KeyHack();

		BEGIN_MSG_MAP_EX(KeyHack)
			MESSAGE_HANDLER(WM_CHAR, OnChar)
			MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
		END_MSG_MAP()

		BOOL SubclassWindow(CWindow hwnd, int cmd);
		LRESULT OnChar(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	private:
		int m_cmd = 0;
	};

	bool m_havefound = false;
	CScriptEditorCtrl* m_parent;
	std::map<int, CWindow> m_window;
	std::vector<KeyHack> m_hacks;
};
