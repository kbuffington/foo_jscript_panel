// Stolen from SciTE
// Copyright 1998-2005 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.
#pragma once
#include <SString.h>
#include <wtlscintilla.h>

enum IndentationStatus
{
	isNone, // no effect on indentation
	isBlockStart, // indentation block begin such as "{" or VB "function"
	isBlockEnd, // indentation end indicator such as "}" or VB "end"
	isKeyWordStart // Keywords that cause indentation
};

struct StyleAndWords
{
	int styleNumber;
	SString words;
	bool IsEmpty() const { return words.length() == 0; }
	bool IsSingleChar() const { return words.length() == 1; }
};

// forward declaration
struct t_sci_prop_set;

struct t_style_to_key_table
{
	int style_num;
	const char* key;
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
	DWORD GetPropertyColor(const char* key, bool* key_exist = NULL);
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
	const char* GetNearestWord(const char* wordStart, int searchLen, SString wordCharacters = NULL, int wordIndex = -1);
	int GetCaretInLine();
	int IndentOfBlock(int line);
	pfc::string8_fast GetCurrentLine();
	t_size GetLinePartsInStyle(int line, int style1, int style2, SString sv[], int len);
	void AutoMarginWidth();
	void AutomaticIndentation(char ch);
	void ContinueCallTip();
	void FillFunctionDefinition(int pos = -1);
	void Init();
	void LoadProperties(const pfc::list_t<t_sci_prop_set>& data);
	void ReadAPI();
	void RestoreDefaultStyle();
	void SetAllStylesFromTable(const t_style_to_key_table table[]);
	void SetContent(const char* text, bool clear_undo_buffer = false);
	void SetJScript();
	void SetIndentation(int line, int indent);
	void TrackWidth();

private:
	int m_nBraceCount;
	int m_nCurrentCallTip;
	int m_nLastPosCallTip;
	int m_nStartCalltipWord;
	int m_nStatementLookback;
	pfc::list_t<pfc::string_simple> m_apis;
	pfc::string8_fast m_szCurrentCallTipWord;
	pfc::string8_fast m_szFunctionDefinition;
	StyleAndWords m_BlockEnd;
	StyleAndWords m_BlockStart;
	StyleAndWords m_StatementEnd;
	StyleAndWords m_StatementIndent;
};
