#include "stdafx.h"
#include "editorctrl.h"
#include "helpers.h"
#include "scintilla_prop_sets.h"

enum
{
	ESF_NONE = 0,
	ESF_FONT = 1 << 0,
	ESF_SIZE = 1 << 1,
	ESF_FORE = 1 << 2,
	ESF_BACK = 1 << 3,
	ESF_BOLD = 1 << 4,
	ESF_ITALICS = 1 << 5,
	ESF_UNDERLINED = 1 << 6,
	ESF_CASEFORCE = 1 << 7,
};

struct StringComparePartialNC
{
	StringComparePartialNC(t_size p_len) : len(p_len) {}

	int operator()(const std::string& s1, const std::string& s2) const
	{
		t_size len1 = pfc::strlen_max_t(s1.c_str(), len);
		t_size len2 = pfc::strlen_max_t(s2.c_str(), len);

		return pfc::stricmp_ascii_ex(s1.c_str(), len1, s2.c_str(), len2);
	}

	t_size len;
};

const char js_keywords[] = "abstract boolean break byte case catch char class const continue"
	" debugger default delete do double else enum export extends false final"
	" finally float for function goto if implements import in instanceof int"
	" interface long native new null package private protected public return"
	" short static super switch synchronized this throw throws transient true"
	" try typeof var void while with enum byvalue cast future generic inner"
	" operator outer rest Array Math RegExp window fb gdi utils plman console";

static const std::vector<std::pair<int, const char*>> js_style_table =
{
	{ STYLE_DEFAULT, "style.default" },
	{ STYLE_LINENUMBER, "style.linenumber" },
	{ STYLE_BRACELIGHT, "style.bracelight" },
	{ STYLE_BRACEBAD, "style.bracebad" },
	{ SCE_C_COMMENT, "style.comment" },
	{ SCE_C_COMMENTLINE, "style.comment" },
	{ SCE_C_COMMENTDOC, "style.comment" },
	{ SCE_C_COMMENTLINEDOC, "style.comment" },
	{ SCE_C_COMMENTDOCKEYWORD, "style.comment" },
	{ SCE_C_COMMENTDOCKEYWORDERROR, "style.comment" },
	{ SCE_C_WORD, "style.keyword" },
	{ SCE_C_IDENTIFIER, "style.indentifier" },
	{ SCE_C_NUMBER, "style.number" },
	{ SCE_C_STRING, "style.string" },
	{ SCE_C_CHARACTER, "style.string" },
	{ SCE_C_OPERATOR, "style.operator" }
};

CScriptEditorCtrl::CScriptEditorCtrl()
	: BraceCount(0)
	, CurrentCallTip(0)
	, StartCalltipWord(0)
	, LastPosCallTip(0)
	, WordCharacters("_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ")
	, apis(panel_manager::instance().get_apis()) {}

BOOL CScriptEditorCtrl::SubclassWindow(HWND hWnd)
{
	BOOL bRet = CScintillaCtrl::SubclassWindow(hWnd);

	if (bRet)
		Init();

	return bRet;
}

DWORD CScriptEditorCtrl::ParseHex(const std::string& hex)
{
	auto int_from_hex_digit = [](int ch)
	{
		if ((ch >= '0') && (ch <= '9'))
		{
			return ch - '0';
		}
		else if (ch >= 'A' && ch <= 'F')
		{
			return ch - 'A' + 10;
		}
		else if (ch >= 'a' && ch <= 'f')
		{
			return ch - 'a' + 10;
		}
		else
		{
			return 0;
		}
	};

	auto int_from_hex_byte = [int_from_hex_digit](const char* hex_byte)
	{
		return (int_from_hex_digit(hex_byte[0]) << 4) | (int_from_hex_digit(hex_byte[1]));
	};

	if (hex.length() > 7) return 0;

	int r = int_from_hex_byte(hex.c_str() + 1);
	int g = int_from_hex_byte(hex.c_str() + 3);
	int b = int_from_hex_byte(hex.c_str() + 5);

	return RGB(r, g, b);
}

CScriptEditorCtrl::IndentationStatus CScriptEditorCtrl::GetIndentState(int line)
{
	IndentationStatus indentState = isNone;
	for (const std::string& sIndent : GetLinePartsInStyle(line, StatementIndent))
	{
		if (Includes(StatementIndent, sIndent))
			indentState = isKeyWordStart;
	}
	for (const std::string& sEnd : GetLinePartsInStyle(line, StatementEnd))
	{
		if (Includes(StatementEnd, sEnd))
			indentState = isNone;
	}
	for (const std::string& sBlock : GetLinePartsInStyle(line, BlockEnd))
	{
		if (Includes(BlockEnd, sBlock))
			indentState = isBlockEnd;
		if (Includes(BlockStart, sBlock))
			indentState = isBlockStart;
	}
	return indentState;
}

LRESULT CScriptEditorCtrl::OnChange(UINT uNotifyCode, int nID, HWND wndCtl)
{
	AutoMarginWidth();
	return 0;
}

LRESULT CScriptEditorCtrl::OnCharAdded(LPNMHDR pnmh)
{
	SCNotification* notification = (SCNotification*)pnmh;
	int ch = notification->ch;
	Sci_CharacterRange crange = GetSelection();
	int selStart = crange.cpMin;
	int selEnd = crange.cpMax;

	if (selEnd == selStart && selStart > 0)
	{
		if (CallTipActive())
		{
			switch (ch)
			{
			case ')':
				BraceCount--;
				if (BraceCount < 1)
					CallTipCancel();
				else
					StartCallTip();
				break;

			case '(':
				BraceCount++;
				StartCallTip();
				break;

			default:
				ContinueCallTip();
				break;
			}
		}
		else if (AutoCActive())
		{
			if (ch == '(')
			{
				BraceCount++;
				StartCallTip();
			}
			else if (ch == ')')
			{
				BraceCount--;
			}
			else if (!Contains(WordCharacters, ch))
			{
				AutoCCancel();

				if (ch == '.')
					StartAutoComplete();
			}
		}
		else
		{
			if (ch == '(')
			{
				BraceCount = 1;
				StartCallTip();
			}
			else
			{
				AutomaticIndentation(ch);

				if (Contains(WordCharacters, ch) || ch == '.')
					StartAutoComplete();
			}
		}
	}

	return 0;
}

LRESULT CScriptEditorCtrl::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	::PostMessage(::GetAncestor(m_hWnd, GA_PARENT), UWM_KEYDOWN, wParam, lParam);
	return TRUE;
}

LRESULT CScriptEditorCtrl::OnUpdateUI(LPNMHDR pnmn)
{
	int braceAtCaret = -1;
	int braceOpposite = -1;

	FindBraceMatchPos(braceAtCaret, braceOpposite);

	if (braceAtCaret != -1 && braceOpposite == -1)
	{
		BraceBadLight(braceAtCaret);
		SetHighlightGuide(0);
	}
	else
	{
		BraceHighlight(braceAtCaret, braceOpposite);

		int columnAtCaret = GetColumn(braceAtCaret);
		int columnOpposite = GetColumn(braceOpposite);

		SetHighlightGuide(std::min(columnAtCaret, columnOpposite));
	}

	return 0;
}

LRESULT CScriptEditorCtrl::OnZoom(LPNMHDR pnmn)
{
	AutoMarginWidth();
	return 0;
}

Sci_CharacterRange CScriptEditorCtrl::GetSelection()
{
	Sci_CharacterRange crange;
	crange.cpMin = GetSelectionStart();
	crange.cpMax = GetSelectionEnd();
	return crange;
}

bool CScriptEditorCtrl::Contains(const std::string& str, char ch)
{
	return str.find(ch) != std::string::npos;
}

bool CScriptEditorCtrl::FindBraceMatchPos(int& braceAtCaret, int& braceOpposite)
{
	auto IsBraceChar = [](int ch)
	{
		return ch == '[' || ch == ']' || ch == '(' || ch == ')' || ch == '{' || ch == '}';
	};

	bool isInside = false;
	int caretPos = GetCurrentPos();

	braceAtCaret = -1;
	braceOpposite = -1;

	char charBefore = 0;
	int lengthDoc = GetLength();

	if (lengthDoc > 0 && caretPos > 0)
	{
		int posBefore = PositionBefore(caretPos);

		if (posBefore == caretPos - 1)
		{
			charBefore = GetCharAt(posBefore);
		}
	}

	if (charBefore && IsBraceChar(charBefore))
	{
		braceAtCaret = caretPos - 1;
	}

	bool colonMode = false;
	bool isAfter = true;

	if (lengthDoc > 0 && braceAtCaret < 0 && caretPos < lengthDoc)
	{
		char charAfter = GetCharAt(caretPos);

		if (charAfter && IsBraceChar(charAfter))
		{
			braceAtCaret = caretPos;
			isAfter = false;
		}
	}

	if (braceAtCaret >= 0)
	{
		if (colonMode)
		{
			int lineStart = LineFromPosition(braceAtCaret);
			int lineMaxSubord = GetLastChild(lineStart, -1);

			braceOpposite = GetLineEndPosition(lineMaxSubord);
		}
		else
		{
			braceOpposite = BraceMatch(braceAtCaret);
		}

		if (braceOpposite > braceAtCaret)
			isInside = isAfter;
		else
			isInside = !isAfter;
	}

	return isInside;
}

bool CScriptEditorCtrl::GetPropertyEx(const std::string& key, std::string& out)
{
	out.clear();
	int len = GetPropertyExpanded(key.c_str(), 0);
	if (len == 0) return false;

	std::vector<char> buff(len + 1);
	buff[len] = 0;
	GetPropertyExpanded(key.c_str(), buff.data());
	out = buff.data();
	return true;
}

bool CScriptEditorCtrl::Includes(const StyleAndWords& symbols, const std::string& value)
{
	if (symbols.IsEmpty())
	{
		return false;
	}
	else if (isalpha(symbols.words[0]))
	{
		t_size lenVal = value.length();
		const char* symbol = symbols.words.c_str();

		while (symbol)
		{
			const char* symbolEnd = strchr(symbol, ' ');
			t_size lenSymbol = strlen(symbol);

			if (symbolEnd)
				lenSymbol = symbolEnd - symbol;

			if (lenSymbol == lenVal)
			{
				if (strncmp(symbol, value.c_str(), lenSymbol) == 0)
				{
					return true;
				}
			}

			symbol = symbolEnd;

			if (symbol)
				symbol++;
		}
	}
	else
	{
		return Contains(value, symbols.words[0]);
	}
	return false;
}

bool CScriptEditorCtrl::ParseStyle(const std::string& definition, EditorStyle& style)
{
	if (definition.empty()) return false;

	slist values = helpers::split_string(definition, ",");

	for (const auto& value : values)
	{
		slist tmp = helpers::split_string(value, ":");
		auto opt = tmp[0].c_str();

		const char* secondary = tmp.size() == 2 ? tmp[1].c_str() : nullptr;

		if (strcmp(opt, "italics") == 0)
		{
			style.flags |= ESF_ITALICS;
			style.italics = true;
		}
		else if (strcmp(opt, "notitalics") == 0)
		{
			style.flags |= ESF_ITALICS;
			style.italics = false;
		}
		else if (strcmp(opt, "bold") == 0)
		{
			style.flags |= ESF_BOLD;
			style.bold = true;
		}
		else if (strcmp(opt, "notbold") == 0)
		{
			style.flags |= ESF_BOLD;
			style.bold = false;
		}
		else if (strcmp(opt, "font") == 0 && secondary)
		{
			style.flags |= ESF_FONT;
			style.font = secondary;
		}
		else if (strcmp(opt, "fore") == 0 && secondary)
		{
			style.flags |= ESF_FORE;
			style.fore = ParseHex(secondary);
		}
		else if (strcmp(opt, "back") == 0 && secondary)
		{
			style.flags |= ESF_BACK;
			style.back = ParseHex(secondary);
		}
		else if (strcmp(opt, "size") == 0 && secondary)
		{
			style.flags |= ESF_SIZE;
			style.size = atoi(secondary);
		}
		else if (strcmp(opt, "underlined") == 0)
		{
			style.flags |= ESF_UNDERLINED;
			style.underlined = true;
		}
		else if (strcmp(opt, "notunderlined") == 0)
		{
			style.flags |= ESF_UNDERLINED;
			style.underlined = false;
		}
		else if (strcmp(opt, "case") == 0)
		{
			style.flags |= ESF_CASEFORCE;
			style.case_force = SC_CASE_MIXED;

			if (secondary)
			{
				if (*secondary == 'u')
					style.case_force = SC_CASE_UPPER;
				else if (*secondary == 'l')
					style.case_force = SC_CASE_LOWER;
			}
		}
	}
	return true;
}

bool CScriptEditorCtrl::RangeIsAllWhitespace(int start, int end)
{
	if (start < 0)
		start = 0;

	end = std::min(end, GetLength());

	for (int i = start; i < end; ++i)
	{
		char c = GetCharAt(i);

		if ((c != ' ') && (c != '\t'))
			return false;
	}

	return true;
}

bool CScriptEditorCtrl::StartAutoComplete()
{
	std::string line = GetCurrentLine();
	int current = GetCaretInLine();

	int startword = current;

	while (startword > 0 && (Contains(WordCharacters, line[startword - 1]) || line[startword - 1] == '.'))
	{
		startword--;
	}

	std::string root = line.substr(startword, current - startword);

	std::string words = GetNearestWords(root.c_str(), root.length());
	if (words.empty()) return false;

	AutoCShow(root.length(), words.c_str());
	return true;
}

bool CScriptEditorCtrl::StartCallTip()
{
	CurrentCallTip = 0;
	CurrentCallTipWord = "";
	std::string line = GetCurrentLine();
	int current = GetCaretInLine();
	int pos = GetCurrentPos();
	int braces = 0;

	do
	{
		while (current > 0 && (braces || line[current - 1] != '('))
		{
			if (line[current - 1] == '(')
				braces--;
			else if (line[current - 1] == ')')
				braces++;

			current--;
			pos--;
		}

		if (current > 0)
		{
			current--;
			pos--;
		}
		else
		{
			break;
		}

		while (current > 0 && isspace(line[current - 1]))
		{
			current--;
			pos--;
		}
	} while (current > 0 && !Contains(WordCharacters, line[current - 1]));

	if (current <= 0)
		return true;

	StartCalltipWord = current - 1;

	while (StartCalltipWord > 0 && (Contains(WordCharacters, line[StartCalltipWord - 1]) || line[StartCalltipWord - 1] == '.'))
	{
		--StartCalltipWord;
	}

	line.at(current) = '\0';
	CurrentCallTipWord = line.c_str() + StartCalltipWord;
	FunctionDefinition = "";
	FillFunctionDefinition(pos);
	return true;
}

int CScriptEditorCtrl::GetCaretInLine()
{
	int caret = GetCurrentPos();
	int line = LineFromPosition(caret);
	int lineStart = PositionFromLine(line);
	return caret - lineStart;
}

int CScriptEditorCtrl::IndentOfBlock(int line)
{
	if (line < 0)
		return 0;

	int indentSize = GetIndent();
	int indentBlock = GetLineIndentation(line);
	int backLine = line;
	IndentationStatus indentState = isNone;

	int lineLimit = line - 10;
	if (lineLimit < 0)
		lineLimit = 0;

	while ((backLine >= lineLimit) && (indentState == 0))
	{
		indentState = GetIndentState(backLine);

		if (indentState != 0)
		{
			indentBlock = GetLineIndentation(backLine);

			if (indentState == isBlockStart)
			{
				indentBlock += indentSize;
			}

			if (indentState == isBlockEnd)
			{
				if (indentBlock < 0)
					indentBlock = 0;
			}

			if ((indentState == isKeyWordStart) && (backLine == line))
				indentBlock += indentSize;
		}

		backLine--;
	}

	return indentBlock;
}

slist CScriptEditorCtrl::GetLinePartsInStyle(int line, const StyleAndWords& saw)
{
	const bool separateCharacters = saw.IsSingleChar();
	const int thisLineStart = PositionFromLine(line);
	const int nextLineStart = PositionFromLine(line + 1);
	slist sv;
	std::string s;

	for (int pos = thisLineStart; pos < nextLineStart; ++pos)
	{
		if (GetStyleAt(pos) == saw.styleNumber)
		{
			if (separateCharacters)
			{
				if (s.length() > 0)
				{
					sv.push_back(s);
				}
				s = "";
			}
			s += GetCharAt(pos);
		}
		else if (s.length() > 0)
		{
			sv.push_back(s);
			s = "";
		}
	}

	if (s.length() > 0)
	{
		sv.push_back(s);
	}

	return sv;
}

std::string CScriptEditorCtrl::GetCurrentLine()
{
	const int len = GetCurLine(0, 0);
	std::string text(len, '\0');
	GetCurLine(&text[0], len);
	return text.substr(0, text.length() - 1);
}

std::string CScriptEditorCtrl::GetNearestWord(const char* wordStart, int searchLen, int wordIndex)
{
	auto it = std::find_if(apis.begin(), apis.end(), [=](const std::string& item)
	{
		return StringComparePartialNC(searchLen)(wordStart, item) == 0;
	});

	for (; it < apis.end(); ++it)
	{
		const char* word = it->c_str();
		if (!word[searchLen] || !Contains(WordCharacters, word[searchLen]))
		{
			if (wordIndex <= 0)
			{
				return *it;
			}
			wordIndex--;
		}
	}
	return std::string();
}

std::string CScriptEditorCtrl::GetNearestWords(const char* wordStart, int searchLen)
{
	std::string words;
	auto it = std::find_if(apis.begin(), apis.end(), [=](const std::string& item)
	{
		return StringComparePartialNC(searchLen)(wordStart, item) == 0;
	});

	for (; it < apis.end(); ++it)
	{
		if (StringComparePartialNC(searchLen)(wordStart, *it) != 0)
		{
			break;
		}

		const t_size len = std::min({ it->find('('), it->find(' '), it->length() });
		if (words.length()) words.append(" ");
		words.append(*it, 0, len);
	}
	return words;
}

void CScriptEditorCtrl::AutoMarginWidth()
{
	int linenumwidth = 1;
	int marginwidth, oldmarginwidth;
	int linecount;

	linecount = GetLineCount();

	while (linecount >= 10)
	{
		linecount /= 10;
		++linenumwidth;
	}

	oldmarginwidth = GetMarginWidthN(0);
	marginwidth = 4 + linenumwidth * (TextWidth(STYLE_LINENUMBER, "9"));

	if (oldmarginwidth != marginwidth)
		SetMarginWidthN(0, marginwidth);
}

void CScriptEditorCtrl::AutomaticIndentation(int ch)
{
	Sci_CharacterRange crange = GetSelection();
	int selStart = crange.cpMin;
	int curLine = LineFromPosition(GetCurrentPos());
	int thisLineStart = PositionFromLine(curLine);
	int indentSize = GetIndent();
	int indentBlock = IndentOfBlock(curLine - 1);

	if (curLine > 0)
	{
		bool foundBrace = false;
		int prevLineLength = LineLength(curLine - 1);
		int slen = 0;

		std::vector<char> linebuf(prevLineLength + 2);
		GetLine(curLine - 1, linebuf.data());
		linebuf[prevLineLength] = 0;
		slen = strlen(linebuf.data());

		for (int pos = slen - 1; pos >= 0 && linebuf[pos]; --pos)
		{
			char c = linebuf[pos];

			if (c == '\t' || c == ' ' || c == '\r' || c == '\n')
			{
				continue;
			}
			else if (c == '{' || c == '[' || c == '(')
			{
				foundBrace = true;
			}

			break;
		}

		if (foundBrace)
			indentBlock += indentSize;
	}

	if (ch == '}')
	{
		if (RangeIsAllWhitespace(thisLineStart, selStart - 1))
		{
			SetIndentation(curLine, indentBlock - indentSize);
		}
	}
	else if (ch == '{')
	{
		if ((GetIndentState(curLine - 1) == isKeyWordStart))
		{
			if (RangeIsAllWhitespace(thisLineStart, selStart - 1))
			{
				SetIndentation(curLine, indentBlock - indentSize);
			}
		}
	}
	else if ((ch == '\r' || ch == '\n') && (selStart == thisLineStart))
	{
		const slist controlWords = GetLinePartsInStyle(curLine - 1, BlockEnd);
		if (controlWords.size() && Includes(BlockEnd, controlWords[0]))
		{
			SetIndentation(curLine - 1, IndentOfBlock(curLine - 2) - indentSize);
			indentBlock = IndentOfBlock(curLine - 1);
		}
		SetIndentation(curLine, indentBlock);
	}
}

void CScriptEditorCtrl::ContinueCallTip()
{
	std::string line = GetCurrentLine();
	int current = GetCaretInLine();
	int braces = 0;
	int commas = 0;

	for (int i = StartCalltipWord; i < current; ++i)
	{
		if (line[i] == '(')
			braces++;
		else if (line[i] == ')' && braces > 0)
			braces--;
		else if (braces == 1 && line[i] == ',')
			commas++;
	}

	int startHighlight = 0;

	while (FunctionDefinition[startHighlight] && FunctionDefinition[startHighlight] != '(')
		startHighlight++;

	if (FunctionDefinition[startHighlight] == '(')
		startHighlight++;

	while (FunctionDefinition[startHighlight] && commas > 0)
	{
		if (FunctionDefinition[startHighlight] == ',')
			commas--;
		if (FunctionDefinition[startHighlight] == ')')
			commas = 0;
		else
			startHighlight++;
	}

	if (FunctionDefinition[startHighlight] == ',')
		startHighlight++;

	int endHighlight = startHighlight;

	while (FunctionDefinition[endHighlight] && FunctionDefinition[endHighlight] != ',' && FunctionDefinition[endHighlight] != ')')
	{
		endHighlight++;
	}

	CallTipSetHlt(startHighlight, endHighlight);
}

void CScriptEditorCtrl::FillFunctionDefinition(int pos)
{
	if (pos > 0)
	{
		LastPosCallTip = pos;
	}

	std::string words = GetNearestWords(CurrentCallTipWord.c_str(), CurrentCallTipWord.length());
	if (words.length())
	{
		std::string word = GetNearestWord(CurrentCallTipWord.c_str(), CurrentCallTipWord.length(), CurrentCallTip);

		if (word.length())
		{
			FunctionDefinition = word;

			CallTipShow(LastPosCallTip - CurrentCallTipWord.length(), FunctionDefinition.c_str());
			ContinueCallTip();
		}
	}
}

void CScriptEditorCtrl::Init()
{
	StyleResetDefault();

	SetCodePage(SC_CP_UTF8);
	SetEOLMode(SC_EOL_CRLF);
	SetModEventMask(SC_MOD_INSERTTEXT | SC_MOD_DELETETEXT | SC_PERFORMED_UNDO | SC_PERFORMED_REDO);
	UsePopUp(true);

	// Disable Ctrl + some char
	const int ctrlcode[22] = { 'Q', 'W', 'E', 'R', 'I', 'O', 'P', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 'B', 'N', 'M', 186, 187, 226 };

	for (int i = 0; i < _countof(ctrlcode); ++i)
	{
		ClearCmdKey(MAKELONG(ctrlcode[i], SCMOD_CTRL));
	}

	// Disable Ctrl+Shift+some char
	for (int i = 48; i < 122; ++i)
	{
		ClearCmdKey(MAKELONG(i, SCMOD_CTRL | SCMOD_SHIFT));
	}

	// Shortcut keys
	AssignCmdKey(MAKELONG(SCK_NEXT, SCMOD_CTRL), SCI_PARADOWN);
	AssignCmdKey(MAKELONG(SCK_PRIOR, SCMOD_CTRL), SCI_PARAUP);
	AssignCmdKey(MAKELONG(SCK_NEXT, (SCMOD_CTRL | SCMOD_SHIFT)), SCI_PARADOWNEXTEND);
	AssignCmdKey(MAKELONG(SCK_PRIOR, (SCMOD_CTRL | SCMOD_SHIFT)), SCI_PARAUPEXTEND);
	AssignCmdKey(MAKELONG(SCK_HOME, SCMOD_NORM), SCI_VCHOMEWRAP);
	AssignCmdKey(MAKELONG(SCK_END, SCMOD_NORM), SCI_LINEENDWRAP);
	AssignCmdKey(MAKELONG(SCK_HOME, SCMOD_SHIFT), SCI_VCHOMEWRAPEXTEND);
	AssignCmdKey(MAKELONG(SCK_END, SCMOD_SHIFT), SCI_LINEENDWRAPEXTEND);

	// Tabs and indentation
	SetUseTabs(false);
	SetTabIndents(false);
	SetBackSpaceUnIndents(true);
	SetTabWidth(4);
	SetIndent(4);

	// Scroll
	SetEndAtLastLine(true);
	SetScrollWidthTracking(true);
	SetScrollWidth(1);

	// Auto complete
	AutoCSetIgnoreCase(true);

	// Load properties
	for (const auto& [key, val] : g_sci_prop_sets.m_data)
	{
		SetProperty(key, val);
	}
}

void CScriptEditorCtrl::RestoreDefaultStyle()
{
	// Clear and reset all styles
	ClearDocumentStyle();
	StyleResetDefault();

	// Enable line numbering
	SetMarginWidthN(1, 0);
	SetMarginWidthN(2, 0);
	SetMarginWidthN(3, 0);
	SetMarginWidthN(4, 0);
	SetMarginTypeN(0, SC_MARGIN_NUMBER);

	SetCaretLineBackAlpha(GetPropertyInt("style.caret.line.back.alpha", SC_ALPHA_NOALPHA));
	SetCaretLineVisible(false);
	SetCaretWidth(GetPropertyInt("style.caret.width", 1));
	SetSelAlpha(GetPropertyInt("style.selection.alpha", SC_ALPHA_NOALPHA));
	SetSelFore(false, 0);

	std::string colour;

	if (GetPropertyEx("style.selection.fore", colour))
	{
		SetSelFore(true, ParseHex(colour));
		SetSelBack(true, RGB(0xc0, 0xc0, 0xc0));
	}

	if (GetPropertyEx("style.selection.back", colour))
	{
		SetSelBack(true, ParseHex(colour));
	}

	if (GetPropertyEx("style.caret.fore", colour))
	{
		SetCaretFore(ParseHex(colour));
	}

	if (GetPropertyEx("style.caret.line.back", colour))
	{
		SetCaretLineVisible(true);
		SetCaretLineBack(ParseHex(colour));
	}
}

void CScriptEditorCtrl::SetAllStylesFromTable()
{
	for (const auto& [style_num, key] : js_style_table)
	{
		std::string tmp;
		if (GetPropertyEx(key, tmp))
		{
			EditorStyle style;
			if (!ParseStyle(tmp, style)) continue;

			if (style.flags & ESF_FONT) StyleSetFont(style_num, style.font.c_str());
			if (style.flags & ESF_SIZE) StyleSetSize(style_num, style.size);
			if (style.flags & ESF_FORE) StyleSetFore(style_num, style.fore);
			if (style.flags & ESF_BACK) StyleSetBack(style_num, style.back);
			if (style.flags & ESF_ITALICS) StyleSetItalic(style_num, style.italics);
			if (style.flags & ESF_BOLD) StyleSetBold(style_num, style.bold);
			if (style.flags & ESF_UNDERLINED) StyleSetUnderline(style_num, style.underlined);
			if (style.flags & ESF_CASEFORCE) StyleSetCase(style_num, style.case_force);
		}
		if (style_num == STYLE_DEFAULT) StyleClearAll();
	}
}

void CScriptEditorCtrl::SetContent(const char* text)
{
	SetText(text);
	ConvertEOLs(SC_EOL_CRLF);
	GrabFocus();
	TrackWidth();
}

void CScriptEditorCtrl::SetJScript()
{
	RestoreDefaultStyle();
	SetLexer(SCLEX_CPP);
	SetKeyWords(0, js_keywords);
	SetAllStylesFromTable();
}

void CScriptEditorCtrl::SetIndentation(int line, int indent)
{
	if (indent < 0)
		return;

	Sci_CharacterRange crange = GetSelection();
	int posBefore = GetLineIndentPosition(line);
	SetLineIndentation(line, indent);
	int posAfter = GetLineIndentPosition(line);
	int posDifference = posAfter - posBefore;
	if (posAfter > posBefore)
	{
		if (crange.cpMin >= posBefore)
		{
			crange.cpMin += posDifference;
		}

		if (crange.cpMax >= posBefore)
		{
			crange.cpMax += posDifference;
		}
	}
	else if (posAfter < posBefore)
	{
		if (crange.cpMin >= posAfter)
		{
			if (crange.cpMin >= posBefore)
				crange.cpMin += posDifference;
			else
				crange.cpMin = posAfter;
		}

		if (crange.cpMax >= posAfter)
		{
			if (crange.cpMax >= posBefore)
				crange.cpMax += posDifference;
			else
				crange.cpMax = posAfter;
		}
	}

	SetSel(crange.cpMin, crange.cpMax);
}

void CScriptEditorCtrl::TrackWidth()
{
	int max_width = 1;

	for (int i = 0; i < GetLineCount(); ++i)
	{
		int pos = GetLineEndPosition(i);
		int width = PointXFromPosition(pos);

		if (width > max_width)
			max_width = width;
	}

	SetScrollWidth(max_width);
}
