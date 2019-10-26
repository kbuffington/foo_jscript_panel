#pragma once
#include "panel_manager.h"
#include "wtlscintilla.h"

class CDialogFindReplace;

class CScriptEditorCtrl : public CScintillaImpl<CScriptEditorCtrl>
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

	BOOL SubclassWindow(HWND hwnd);
	bool FindNext();
	bool FindPrevious();
	void Replace();
	void ReplaceAll();
	void SetContent(const char* text);
	void SetJScript();

	std::map<int, int> FlagMap;

private:
	enum IndentationStatus
	{
		isNone,
		isBlockStart,
		isBlockEnd,
		isKeyWordStart
	};

	struct EditorStyle
	{
		EditorStyle() : back(0), fore(0), case_force(0), flags(0), size(0), bold(false), italics(false), underlined(false) {}

		DWORD back, fore;
		bool bold, italics, underlined;
		int case_force;
		std::string font;
		t_size flags, size;
	};

	struct StyleAndWords
	{
		int styleNumber;
		std::string words;
		bool IsEmpty() const { return words.length() == 0; }
		bool IsSingleChar() const { return words.length() == 1; }
	};

	DWORD ParseHex(const std::string& hex);
	IndentationStatus GetIndentState(int line);
	LRESULT OnChange(UINT uNotifyCode, int nID, HWND wndCtl);
	LRESULT OnCharAdded(LPNMHDR pnmh);
	LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnUpdateUI(LPNMHDR pnmh);
	LRESULT OnZoom(LPNMHDR pnmh);
	Sci_CharacterRange GetSelection();
	bool Contains(const std::string& str, char ch);
	bool Includes(const StyleAndWords& symbols, const std::string& value);
	bool FindBraceMatchPos(int& braceAtCaret, int& braceOpposite);
	bool FindResult(int pos);
	bool GetPropertyEx(const std::string& key, std::string& out);
	bool ParseStyle(const std::string& definition, EditorStyle& style);
	bool RangeIsAllWhitespace(int start, int end);
	bool StartAutoComplete();
	bool StartCallTip();
	int GetCaretInLine();
	int IndentOfBlock(int line);
	std::string GetCurrentLine();
	std::string GetNearestWord(const char* wordStart, t_size searchLen, int wordIndex);
	std::string GetNearestWords(const char* wordStart, t_size searchLen);
	str_list GetLinePartsInStyle(int line, const StyleAndWords& saw);
	void AutoMarginWidth();
	void AutomaticIndentation(int ch);
	void ContinueCallTip();
	void FillFunctionDefinition(int pos);
	void Init();
	void OpenFindDialog();
	void OpenGotoDialog();
	void OpenReplaceDialog();
	void RestoreDefaultStyle();
	void SetAllStylesFromTable();
	void SetIndentation(int line, int indent);
	void TrackWidth();

	CDialogFindReplace* DlgFindReplace;
	StyleAndWords BlockEnd;
	StyleAndWords BlockStart;
	StyleAndWords StatementEnd;
	StyleAndWords StatementIndent;
	int BraceCount;
	int CurrentCallTip;
	int LastPosCallTip;
	int StartCalltipWord;
	std::string CurrentCallTipWord;
	std::string FunctionDefinition;
	std::string WordCharacters;
	std::vector<panel_manager::api_item> apis;
};
