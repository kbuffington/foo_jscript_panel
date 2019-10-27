#include "stdafx.h"
#include "editorctrl.h"
#include "scintilla_properties.h"
#include "ui_find_replace.h"
#include "ui_goto.h"

// Large portions taken from SciTE
// Copyright 1998-2005 by Neil Hodgson <neilh@scintilla.org>
// https://github.com/marc2k3/foo_jscript_panel/tree/master/foo_jscript_panel/licenses

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
		const t_size len1 = pfc::strlen_max_t(s1.c_str(), len);
		const t_size len2 = pfc::strlen_max_t(s2.c_str(), len);

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
	, DlgFindReplace(nullptr)
	, WordCharacters("_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ")
	, apis(panel_manager::instance().get_apis())
{
	FlagMap =
	{
		{ IDC_CHECK_MATCHCASE, SCFIND_MATCHCASE },
		{ IDC_CHECK_WHOLEWORD, SCFIND_WHOLEWORD },
		{ IDC_CHECK_WORDSTART, SCFIND_WORDSTART },
		{ IDC_CHECK_REGEXP, SCFIND_REGEXP }
	};
}

BOOL CScriptEditorCtrl::SubclassWindow(HWND hwnd)
{
	const BOOL bRet = CScintillaImpl::SubclassWindow(hwnd);

	if (bRet)
	{
		SetFnPtr();
		Init();
	}

	return bRet;
}

CScriptEditorCtrl::Colour CScriptEditorCtrl::ParseHex(const std::string& hex)
{
	const auto int_from_hex_digit = [](int ch)
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

	const auto int_from_hex_byte = [int_from_hex_digit](const char* hex_byte)
	{
		return (int_from_hex_digit(hex_byte[0]) << 4) | (int_from_hex_digit(hex_byte[1]));
	};

	if (hex.length() > 7) return 0;

	const int r = int_from_hex_byte(hex.c_str() + 1);
	const int g = int_from_hex_byte(hex.c_str() + 3);
	const int b = int_from_hex_byte(hex.c_str() + 5);

	return RGB(r, g, b);
}

CScriptEditorCtrl::IndentationStatus CScriptEditorCtrl::GetIndentState(Line line)
{
	IndentationStatus indentState = IndentationStatus::isNone;
	for (const std::string& sIndent : GetLinePartsInStyle(line, StatementIndent))
	{
		if (Includes(StatementIndent, sIndent))
			indentState = IndentationStatus::isKeyWordStart;
	}
	for (const std::string& sEnd : GetLinePartsInStyle(line, StatementEnd))
	{
		if (Includes(StatementEnd, sEnd))
			indentState = IndentationStatus::isNone;
	}
	for (const std::string& sBlock : GetLinePartsInStyle(line, BlockEnd))
	{
		if (Includes(BlockEnd, sBlock))
			indentState = IndentationStatus::isBlockEnd;
		if (Includes(BlockStart, sBlock))
			indentState = IndentationStatus::isBlockStart;
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
	const SCNotification* notification = (SCNotification*)pnmh;
	const int ch = notification->ch;
	const auto range = GetSelection();

	if (range.start == range.end && range.start > 0)
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
	const auto modifiers = (IsKeyPressed(VK_SHIFT) ? SCMOD_SHIFT : 0) | (IsKeyPressed(VK_CONTROL) ? SCMOD_CTRL : 0) | (IsKeyPressed(VK_MENU) ? SCMOD_ALT : 0);

	if (modifiers == SCMOD_CTRL)
	{
		switch (wParam)
		{
		case 'F':
			OpenFindDialog();
			break;

		case 'H':
			OpenReplaceDialog();
			break;

		case 'G':
			fb2k::inMainThread([&]()
			{
				OpenGotoDialog();
			});
			break;

		case 'S':
			::PostMessage(GetParent(), WM_COMMAND, MAKEWPARAM(IDC_BTN_APPLY, BN_CLICKED), reinterpret_cast<LPARAM>(m_hWnd));
			break;
		}
	}
	else if (wParam == VK_F3 && (modifiers == 0 || modifiers == SCMOD_SHIFT))
	{
		if (!DlgFindReplace || DlgFindReplace->m_find_text.is_empty())
		{
			OpenFindDialog();
		}
		else
		{
			if (modifiers == 0) // Next
			{
				FindNext();
			}
			else if (modifiers == SCMOD_SHIFT) // Previous
			{
				FindPrevious();
			}
		}
	}

	bHandled = FALSE;
	return 1;
}

LRESULT CScriptEditorCtrl::OnUpdateUI(LPNMHDR pnmh)
{
	Position braceAtCaret = -1;
	Position braceOpposite = -1;

	FindBraceMatchPos(braceAtCaret, braceOpposite);

	if (braceAtCaret != -1 && braceOpposite == -1)
	{
		BraceBadLight(braceAtCaret);
		SetHighlightGuide(0);
	}
	else
	{
		BraceHighlight(braceAtCaret, braceOpposite);

		const auto columnAtCaret = GetColumn(braceAtCaret);
		const auto columnOpposite = GetColumn(braceOpposite);

		SetHighlightGuide(std::min(columnAtCaret, columnOpposite));
	}

	return 0;
}

LRESULT CScriptEditorCtrl::OnZoom(LPNMHDR pnmh)
{
	AutoMarginWidth();
	return 0;
}

CScriptEditorCtrl::Position CScriptEditorCtrl::GetCaretInLine()
{
	const auto caret = GetCurrentPos();
	const auto line = LineFromPosition(caret);
	const auto lineStart = PositionFromLine(line);
	return caret - lineStart;
}

CScriptEditorCtrl::Range CScriptEditorCtrl::GetSelection()
{
	Range range;
	range.start = GetSelectionStart();
	range.end = GetSelectionEnd();
	return range;
}

bool CScriptEditorCtrl::Contains(const std::string& str, char ch)
{
	return str.find(ch) != std::string::npos;
}

bool CScriptEditorCtrl::FindBraceMatchPos(Position& braceAtCaret, Position& braceOpposite)
{
	const auto IsBraceChar = [](int ch)
	{
		return ch == '[' || ch == ']' || ch == '(' || ch == ')' || ch == '{' || ch == '}';
	};

	auto caretPos = GetCurrentPos();
	bool isInside = false;

	braceAtCaret = -1;
	braceOpposite = -1;

	char charBefore = 0;
	const auto lengthDoc = GetLength();

	if (lengthDoc > 0 && caretPos > 0)
	{
		const auto posBefore = PositionBefore(caretPos);

		if (posBefore == caretPos - 1)
		{
			charBefore = GetCharAt(posBefore);
		}
	}

	if (charBefore && IsBraceChar(charBefore))
	{
		braceAtCaret = caretPos - 1;
	}

	bool isAfter = true;

	if (lengthDoc > 0 && braceAtCaret < 0 && caretPos < lengthDoc)
	{
		const char charAfter = GetCharAt(caretPos);

		if (charAfter && IsBraceChar(charAfter))
		{
			braceAtCaret = caretPos;
			isAfter = false;
		}
	}

	if (braceAtCaret >= 0)
	{
		braceOpposite = BraceMatch(braceAtCaret, 0);

		if (braceOpposite > braceAtCaret)
			isInside = isAfter;
		else
			isInside = !isAfter;
	}

	return isInside;
}

bool CScriptEditorCtrl::FindNext()
{
	CharRight();
	SearchAnchor();
	const auto pos = SearchNext(DlgFindReplace->m_flags, DlgFindReplace->m_find_text.get_ptr());
	return FindResult(pos);
}

bool CScriptEditorCtrl::FindPrevious()
{
	SearchAnchor();
	const auto pos = SearchPrev(DlgFindReplace->m_flags, DlgFindReplace->m_find_text.get_ptr());
	return FindResult(pos);
}

bool CScriptEditorCtrl::FindResult(Position pos)
{
	if (pos != -1)
	{
		// Scroll to view
		ScrollCaret();
		return true;
	}

	pfc::string8_fast msg;
	msg << "Cannot find \"" << DlgFindReplace->m_find_text.get_ptr() << "\"";
	uMessageBox(DlgFindReplace->m_hWnd, msg, JSP_NAME, MB_SYSTEMMODAL | MB_ICONINFORMATION);
	return false;
}

bool CScriptEditorCtrl::GetPropertyEx(const std::string& key, std::string& out)
{
	out.clear();
	const auto len = GetPropertyExpanded(key.c_str(), nullptr);
	if (len == 0) return false;

	std::vector<char> buff(len + 1);
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
		const auto lenVal = value.length();
		auto symbol = symbols.words.c_str();

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

	auto values = helpers::split_string(definition, ",");

	for (const auto& value : values)
	{
		auto tmp = helpers::split_string(value, ":");
		auto opt = tmp[0];
		auto secondary = tmp.size() == 2 ? tmp[1] : "";

		if (opt.compare("italics") == 0)
		{
			style.flags |= ESF_ITALICS;
			style.italics = true;
		}
		else if (opt.compare("notitalics") == 0)
		{
			style.flags |= ESF_ITALICS;
			style.italics = false;
		}
		else if (opt.compare("bold") == 0)
		{
			style.flags |= ESF_BOLD;
			style.bold = true;
		}
		else if (opt.compare("notbold") == 0)
		{
			style.flags |= ESF_BOLD;
			style.bold = false;
		}
		else if (opt.compare("font") == 0)
		{
			style.flags |= ESF_FONT;
			style.font = secondary;
		}
		else if (opt.compare("fore") == 0)
		{
			style.flags |= ESF_FORE;
			style.fore = ParseHex(secondary);
		}
		else if (opt.compare("back") == 0)
		{
			style.flags |= ESF_BACK;
			style.back = ParseHex(secondary);
		}
		else if (opt.compare("size") == 0 && secondary.length())
		{
			style.flags |= ESF_SIZE;
			style.size = atoi(secondary.c_str());
		}
		else if (opt.compare("underlined") == 0)
		{
			style.flags |= ESF_UNDERLINED;
			style.underlined = true;
		}
		else if (opt.compare("notunderlined") == 0)
		{
			style.flags |= ESF_UNDERLINED;
			style.underlined = false;
		}
		else if (opt.compare("case") == 0)
		{
			style.flags |= ESF_CASEFORCE;
			style.case_force = SC_CASE_MIXED;

			if (secondary.length())
			{
				if (secondary.at(0) == 'u')
					style.case_force = SC_CASE_UPPER;
				else if (secondary.at(0) == 'l')
					style.case_force = SC_CASE_LOWER;
			}
		}
	}
	return true;
}

bool CScriptEditorCtrl::RangeIsAllWhitespace(Position start, Position end)
{
	if (start < 0)
		start = 0;

	end = std::min(end, GetLength());

	for (auto i = start; i < end; ++i)
	{
		const auto c = GetCharAt(i);

		if ((c != ' ') && (c != '\t'))
			return false;
	}

	return true;
}

int CScriptEditorCtrl::IndentOfBlock(Line line)
{
	if (line < 0)
		return 0;

	const auto indentSize = GetIndent();
	auto indentBlock = GetLineIndentation(line);
	auto backLine = line;
	auto indentState = IndentationStatus::isNone;

	auto lineLimit = line - 10;
	if (lineLimit < 0)
		lineLimit = 0;

	while ((backLine >= lineLimit) && (indentState == IndentationStatus::isNone))
	{
		indentState = GetIndentState(backLine);

		if (indentState != IndentationStatus::isNone)
		{
			indentBlock = GetLineIndentation(backLine);

			if (indentState == IndentationStatus::isBlockStart)
			{
				indentBlock += indentSize;
			}

			if (indentState == IndentationStatus::isBlockEnd)
			{
				if (indentBlock < 0)
					indentBlock = 0;
			}

			if ((indentState == IndentationStatus::isKeyWordStart) && (backLine == line))
				indentBlock += indentSize;
		}

		backLine--;
	}

	return indentBlock;
}

std::string CScriptEditorCtrl::GetCurrentLine()
{
	const auto len = GetCurLine(0, nullptr);
	std::vector<char> buff(len + 1);
	GetCurLine(buff.size(), buff.data());
	return buff.data();
}

std::string CScriptEditorCtrl::GetNearestWord(const char* wordStart, t_size searchLen, int wordIndex)
{
	auto it = std::find_if(apis.begin(), apis.end(), [=](const panel_manager::api_item& item)
	{
		return StringComparePartialNC(searchLen)(wordStart, item.first) == 0;
	});

	for (; it < apis.end(); ++it)
	{
		if (searchLen >= it->first.length() || !Contains(WordCharacters, it->first.at(searchLen)))
		{
			if (wordIndex <= 0)
			{
				return it->first;
			}
			wordIndex--;
		}
	}
	return std::string();
}

std::string CScriptEditorCtrl::GetNearestWords(const char* wordStart, t_size searchLen)
{
	std::string words;
	auto it = std::find_if(apis.begin(), apis.end(), [=](const panel_manager::api_item& item)
	{
		return StringComparePartialNC(searchLen)(wordStart, item.first) == 0;
	});

	for (; it < apis.end(); ++it)
	{
		if (StringComparePartialNC(searchLen)(wordStart, it->first) != 0)
		{
			break;
		}

		if (words.length()) words.append(" ");
		words.append(it->first, 0, it->second);
	}
	return words;
}

str_list CScriptEditorCtrl::GetLinePartsInStyle(Line line, const StyleAndWords& saw)
{
	const auto thisLineStart = PositionFromLine(line);
	const auto nextLineStart = PositionFromLine(line + 1);
	const bool separateCharacters = saw.IsSingleChar();
	str_list sv;
	std::string s;

	for (auto pos = thisLineStart; pos < nextLineStart; ++pos)
	{
		if (GetStyleAt(pos) == saw.styleNumber)
		{
			if (separateCharacters)
			{
				if (s.length() > 0)
				{
					sv.emplace_back(s);
				}
				s = "";
			}
			s += GetCharAt(pos);
		}
		else if (s.length() > 0)
		{
			sv.emplace_back(s);
			s = "";
		}
	}

	if (s.length() > 0)
	{
		sv.emplace_back(s);
	}

	return sv;
}

void CScriptEditorCtrl::AutoMarginWidth()
{
	auto linecount = GetLineCount();
	int linenumwidth = 1;
	int marginwidth = 0;
	int oldmarginwidth = 0;

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
	const auto selStart = GetSelectionStart();
	const auto curLine = LineFromPosition(GetCurrentPos());
	const auto thisLineStart = PositionFromLine(curLine);
	const auto indentSize = GetIndent();
	auto indentBlock = IndentOfBlock(curLine - 1);

	if (curLine > 0)
	{
		bool foundBrace = false;
		const auto prevLineLength = LineLength(curLine - 1);
		int slen = 0;

		std::vector<char> linebuf(prevLineLength + 2);
		GetLine(curLine - 1, linebuf.data());
		linebuf[prevLineLength] = 0;
		slen = strlen(linebuf.data());

		for (int pos = slen - 1; pos >= 0 && linebuf[pos]; --pos)
		{
			const auto c = linebuf[pos];

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
		if ((GetIndentState(curLine - 1) == IndentationStatus::isKeyWordStart))
		{
			if (RangeIsAllWhitespace(thisLineStart, selStart - 1))
			{
				SetIndentation(curLine, indentBlock - indentSize);
			}
		}
	}
	else if ((ch == '\r' || ch == '\n') && (selStart == thisLineStart))
	{
		const auto controlWords = GetLinePartsInStyle(curLine - 1, BlockEnd);
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
	auto line = GetCurrentLine();
	const auto current = GetCaretInLine();
	int braces = 0;
	int commas = 0;

	for (auto i = StartCalltipWord; i < current; ++i)
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

void CScriptEditorCtrl::FillFunctionDefinition(Position pos)
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

	for (auto i = 0; i < _countof(ctrlcode); ++i)
	{
		ClearCmdKey(MAKELONG(ctrlcode[i], SCMOD_CTRL));
	}

	// Disable Ctrl+Shift+some char
	for (auto i = 48; i < 122; ++i)
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
	SetTabIndents(true);
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
	for (const auto& [key, value] : g_scintilla_properties.m_data)
	{
		SetProperty(key, value);
	}
}

void CScriptEditorCtrl::OpenFindDialog()
{
	if (!DlgFindReplace) DlgFindReplace = fb2k::newDialogEx<CDialogFindReplace>(m_hWnd, this);
	DlgFindReplace->SetMode(CDialogFindReplace::mode::find);
}

void CScriptEditorCtrl::OpenGotoDialog()
{
	const auto cur_pos = GetCurrentPos();
	const auto cur_line = LineFromPosition(cur_pos) + 1;

	modal_dialog_scope scope(m_hWnd);
	CDialogGoto dlg(m_hWnd, std::to_string(cur_line).c_str());
	dlg.DoModal(m_hWnd);
}

void CScriptEditorCtrl::OpenReplaceDialog()
{
	if (!DlgFindReplace) DlgFindReplace = fb2k::newDialogEx<CDialogFindReplace>(m_hWnd, this);
	DlgFindReplace->SetMode(CDialogFindReplace::mode::replace);
}

void CScriptEditorCtrl::Replace()
{
	const char* replace = DlgFindReplace->m_replace_text.get_ptr();
	const auto range = GetSelection();
	SetTargetStart(range.start);
	SetTargetEnd(range.end);
	ReplaceTarget(strlen(replace), replace);
	SetSel(range.start + strlen(replace), range.end);
}

void CScriptEditorCtrl::ReplaceAll()
{
	BeginUndoAction();
	SetTargetStart(0);
	SetTargetEnd(0);
	SetSearchFlags(DlgFindReplace->m_flags);
	auto find = DlgFindReplace->m_find_text.get_ptr();
	auto replace = DlgFindReplace->m_replace_text.get_ptr();

	while (true)
	{
		SetTargetStart(GetTargetEnd());
		SetTargetEnd(GetLength());

		const auto occurance = SearchInTarget(strlen(find), find);

		if (occurance == -1)
		{
			MessageBeep(MB_ICONINFORMATION);
			break;
		}

		ReplaceTarget(strlen(replace), replace);
		SetSel(occurance + strlen(replace), occurance);
	}

	EndUndoAction();
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

void CScriptEditorCtrl::SetIndentation(Line line, int indent)
{
	if (indent < 0)
		return;

	auto range = GetSelection();
	const auto posBefore = GetLineIndentPosition(line);
	SetLineIndentation(line, indent);
	const auto posAfter = GetLineIndentPosition(line);
	const auto posDifference = posAfter - posBefore;
	if (posAfter > posBefore)
	{
		if (range.start >= posBefore)
		{
			range.start += posDifference;
		}

		if (range.end >= posBefore)
		{
			range.end += posDifference;
		}
	}
	else if (posAfter < posBefore)
	{
		if (range.start >= posAfter)
		{
			if (range.start >= posBefore)
				range.start += posDifference;
			else
				range.start = posAfter;
		}

		if (range.end >= posAfter)
		{
			if (range.end >= posBefore)
				range.end += posDifference;
			else
				range.end = posAfter;
		}
	}

	SetSel(range.start, range.end);
}

void CScriptEditorCtrl::StartAutoComplete()
{
	std::string line = GetCurrentLine();
	const auto current = GetCaretInLine();

	auto startword = current;

	while (startword > 0 && (Contains(WordCharacters, line[startword - 1]) || line[startword - 1] == '.'))
	{
		startword--;
	}

	std::string root = line.substr(startword, current - startword);

	std::string words = GetNearestWords(root.c_str(), root.length());
	if (words.length())
	{
		AutoCShow(root.length(), words.c_str());
	}
}

void CScriptEditorCtrl::StartCallTip()
{
	CurrentCallTip = 0;
	CurrentCallTipWord = "";
	std::string line = GetCurrentLine();
	auto current = GetCaretInLine();
	auto pos = GetCurrentPos();
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

	if (current <= 0) return;

	StartCalltipWord = current - 1;

	while (StartCalltipWord > 0 && (Contains(WordCharacters, line[StartCalltipWord - 1]) || line[StartCalltipWord - 1] == '.'))
	{
		--StartCalltipWord;
	}

	line.at(current) = '\0';
	CurrentCallTipWord = line.c_str() + StartCalltipWord;
	FunctionDefinition = "";
	FillFunctionDefinition(pos);
}

void CScriptEditorCtrl::TrackWidth()
{
	int max_width = 1;

	for (int i = 0; i < GetLineCount(); ++i)
	{
		const auto pos = GetLineEndPosition(i);
		const auto width = PointXFromPosition(pos);

		if (width > max_width)
			max_width = width;
	}

	SetScrollWidth(max_width);
}
