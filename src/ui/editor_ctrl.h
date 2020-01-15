#pragma once
#include "panel_manager.h"
#include "wtlscintilla.h"

class CDialogFindReplace;

class CScriptEditorCtrl : public CScintillaImpl<CScriptEditorCtrl>
{
public:
	CScriptEditorCtrl();

	BEGIN_MSG_MAP_EX(CScriptEditorCtrl)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
		REFLECTED_NOTIFY_CODE_HANDLER_EX(SCN_UPDATEUI, OnUpdateUI)
		REFLECTED_NOTIFY_CODE_HANDLER_EX(SCN_CHARADDED, OnCharAdded)
		REFLECTED_NOTIFY_CODE_HANDLER_EX(SCN_ZOOM, OnZoom)
		REFLECTED_COMMAND_CODE_HANDLER_EX(SCEN_CHANGE, OnChange)
	END_MSG_MAP()

	bool FindNext();
	bool FindPrevious();
	void Init();
	void Replace();
	void ReplaceAll();
	void SetContent(pfc::stringp text);

	std::map<int, int> FlagMap;

private:
	enum class IndentationStatus
	{
		isNone,
		isBlockStart,
		isBlockEnd,
		isKeyWordStart
	};

	struct EditorStyle
	{
		EditorStyle() : back(0), fore(0), case_force(0), flags(0), size(0), bold(false), italics(false), underlined(false) {}

		Colour back, fore;
		bool bold, italics, underlined;
		int case_force;
		size_t flags, size;
		std::string font;
	};

	struct Range
	{
		Position start;
		Position end;
	};

	struct StyleAndWords
	{
		bool IsEmpty() const { return words.length() == 0; }
		bool IsSingleChar() const { return words.length() == 1; }

		int styleNumber = 0;
		std::string words;
	};

	Colour ParseHex(const std::string& hex);
	IndentationStatus GetIndentState(Line line);
	LRESULT OnChange(UINT, int, CWindow);
	LRESULT OnCharAdded(LPNMHDR);
	LRESULT OnKeyDown(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnUpdateUI(LPNMHDR);
	LRESULT OnZoom(LPNMHDR);
	Position GetCaretInLine();
	Range GetSelection();
	bool Contains(const std::string& str, char ch);
	bool Includes(const StyleAndWords& symbols, const std::string& value);
	bool FindBraceMatchPos(Position& braceAtCaret, Position& braceOpposite);
	bool FindResult(Position pos);
	bool GetPropertyEx(const std::string& key, std::string& out);
	bool ParseStyle(const std::string& definition, EditorStyle& style);
	bool RangeIsAllWhitespace(Position start, Position end);
	int IndentOfBlock(Line line);
	Line GetCurrentLineNumber();
	std::string GetCurrentLine();
	std::string GetNearestWord(const std::string& wordStart, size_t searchLen, int wordIndex);
	std::string GetNearestWords(const std::string& wordStart, size_t searchLen);
	str_vec GetLinePartsInStyle(Line line, const StyleAndWords& saw);
	void AutoMarginWidth();
	void AutomaticIndentation(int ch);
	void ContinueCallTip();
	void FillFunctionDefinition(Position pos);
	void OpenFindDialog();
	void OpenGotoDialog();
	void OpenReplaceDialog();
	void SetIndentation(Line line, int indent);
	void StartAutoComplete();
	void StartCallTip();
	void TrackWidth();

	CDialogFindReplace* DlgFindReplace = nullptr;
	Position LastPosCallTip = 0;
	Position StartCalltipWord = 0;
	StyleAndWords BlockEnd;
	StyleAndWords BlockStart;
	StyleAndWords StatementEnd;
	StyleAndWords StatementIndent;
	int BraceCount = 0;
	int CurrentCallTip = 0;
	std::string CurrentCallTipWord;
	std::string FunctionDefinition;
	std::string WordCharacters = "_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	std::vector<panel_manager::api_item> apis;
};
