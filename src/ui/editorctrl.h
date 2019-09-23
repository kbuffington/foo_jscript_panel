// Stolen from SciTE
// Copyright 1998-2005 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.
#pragma once
#include "panel_manager.h"

#include <wtlscintilla.h>

enum IndentationStatus
{
	isNone,
	isBlockStart,
	isBlockEnd,
	isKeyWordStart
};

struct StyleAndWords
{
	int styleNumber;
	std::string words;
	bool IsEmpty() const { return words.length() == 0; }
	bool IsSingleChar() const { return words.length() == 1; }
};

class CScriptEditorCtrl : public CScintillaCtrl
{
public:
	CScriptEditorCtrl();

	BEGIN_MSG_MAP(CScriptEditorCtrl)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
		REFLECTED_NOTIFY_CODE_HANDLER_EX(SCN_UPDATEUI, OnUpdateUI)
		REFLECTED_NOTIFY_CODE_HANDLER_EX(SCN_CHARADDED, OnCharAdded)
		REFLECTED_NOTIFY_CODE_HANDLER_EX(SCN_ZOOM, OnZoom)
		REFLECTED_COMMAND_CODE_HANDLER_EX(SCEN_CHANGE, OnChange)
	END_MSG_MAP()

	BOOL SubclassWindow(HWND hWnd);
	DWORD GetPropertyColor(const char* key, bool* key_exist = nullptr);
	IndentationStatus GetIndentState(int line);
	LRESULT OnChange(UINT uNotifyCode, int nID, HWND wndCtl);
	LRESULT OnCharAdded(LPNMHDR pnmh);
	LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnUpdateUI(LPNMHDR pnmn);
	LRESULT OnZoom(LPNMHDR pnmn);
	Sci_CharacterRange GetSelection();
	bool FindBraceMatchPos(int& braceAtCaret, int& braceOpposite);
	bool GetNearestWords(pfc::string_base& out, const char* wordStart, int searchLen, const char* separators);
	bool RangeIsAllWhitespace(int start, int end);
	bool StartAutoComplete();
	bool StartCallTip();
	const char* GetNearestWord(const char* wordStart, int searchLen, std::string wordCharacters = nullptr, int wordIndex = -1);
	int GetCaretInLine();
	int IndentOfBlock(int line);
	std::string GetCurrentLine();
	t_size GetLinePartsInStyle(int line, int style, std::string sv[], int len);
	void AutoMarginWidth();
	void AutomaticIndentation(int ch);
	void ContinueCallTip();
	void FillFunctionDefinition(int pos = -1);
	void Init();
	void RestoreDefaultStyle();
	void SetAllStylesFromTable();
	void SetContent(const char* text, bool clear_undo_buffer = false);
	void SetJScript();
	void SetIndentation(int line, int indent);
	void TrackWidth();

private:
	StyleAndWords m_BlockEnd;
	StyleAndWords m_BlockStart;
	StyleAndWords m_StatementEnd;
	StyleAndWords m_StatementIndent;
	int m_BraceCount;
	int m_CurrentCallTip;
	int m_LastPosCallTip;
	int m_StartCalltipWord;
	panel_manager::api_list m_apis;
	pfc::string8_fast m_CurrentCallTipWord;
	pfc::string8_fast m_FunctionDefinition;
};
