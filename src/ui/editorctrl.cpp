#include "stdafx.h"
#include "editorctrl.h"
#include "helpers.h"
#include "scintilla_prop_sets.h"

struct StringComparePartialNC
{
	StringComparePartialNC(t_size len_) : len(len_) {}

	int operator()(const char* s1, const char* s2) const
	{
		t_size len1 = pfc::strlen_max_t(s1, len);
		t_size len2 = pfc::strlen_max_t(s2, len);

		return pfc::stricmp_ascii_ex(s1, len1, s2, len2);
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

const t_style_to_key_table js_style_table[] =
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
	{ SCE_C_OPERATOR, "style.operator" },
	{ -1, nullptr },
};

static DWORD ParseHex(const char* hex)
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

	// len('#000000') = 7
	if (pfc::strlen_max(hex, 8) == 8)
		return 0;

	int r = int_from_hex_byte(hex + 1);
	int g = int_from_hex_byte(hex + 3);
	int b = int_from_hex_byte(hex + 5);

	return RGB(r, g, b);
}

static bool IsIdentifierChar(int ch)
{
	return __iswcsym((wint_t)ch);
}

static bool IsSymIncludes(const StyleAndWords& symbols, const std::string& value)
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
		char ch = symbols.words[0];
		return strchr(value.c_str(), ch) != 0;
	}

	return false;
}

static bool ParseStyle(const char* p_definition, t_sci_editor_style& p_style)
{
	if (p_definition == 0 || !*p_definition)
		return false;

	char* val = _strdup(p_definition);
	char* opt = val;

	while (opt)
	{
		char* cpComma = strchr(opt, ',');

		if (cpComma)
		{
			*cpComma = 0;
		}

		char* colon = strchr(opt, ':');

		if (colon)
		{
			*colon++ = '\0';
		}

		if (0 == strcmp(opt, "italics"))
		{
			p_style.flags |= ESF_ITALICS;
			p_style.italics = true;
		}
		else if (0 == strcmp(opt, "notitalics"))
		{
			p_style.flags |= ESF_ITALICS;
			p_style.italics = false;
		}
		else if (0 == strcmp(opt, "bold"))
		{
			p_style.flags |= ESF_BOLD;
			p_style.bold = true;
		}
		else if (0 == strcmp(opt, "notbold"))
		{
			p_style.flags |= ESF_BOLD;
			p_style.bold = false;
		}
		else if (0 == strcmp(opt, "font"))
		{
			p_style.flags |= ESF_FONT;
			p_style.font = colon;
		}
		else if (0 == strcmp(opt, "fore"))
		{
			p_style.flags |= ESF_FORE;
			p_style.fore = ParseHex(colon);
		}
		else if (0 == strcmp(opt, "back"))
		{
			p_style.flags |= ESF_BACK;
			p_style.back = ParseHex(colon);
		}
		else if (0 == strcmp(opt, "size"))
		{
			p_style.flags |= ESF_SIZE;
			p_style.size = atoi(colon);
		}
		else if (0 == strcmp(opt, "underlined"))
		{
			p_style.flags |= ESF_UNDERLINED;
			p_style.underlined = true;
		}
		else if (0 == strcmp(opt, "notunderlined"))
		{
			p_style.flags |= ESF_UNDERLINED;
			p_style.underlined = false;
		}
		else if (0 == strcmp(opt, "case"))
		{
			p_style.flags |= ESF_CASEFORCE;
			p_style.case_force = SC_CASE_MIXED;

			if (colon)
			{
				if (*colon == 'u')
					p_style.case_force = SC_CASE_UPPER;
				else if (*colon == 'l')
					p_style.case_force = SC_CASE_LOWER;
			}
		}

		if (cpComma)
			opt = cpComma + 1;
		else
			opt = 0;
	}

	free(val);
	return true;
}

static t_size LengthWord(const char* word, char otherSeparator)
{
	auto IsASpace = [](t_size ch)
	{
		return ch == ' ' || (ch >= 0x09 && ch <= 0x0d);
	};

	const char* endWord = 0;

	if (otherSeparator)
		endWord = strchr(word, otherSeparator);

	if (!endWord)
		endWord = strchr(word, '(');

	if (!endWord)
		endWord = word + strlen(word);

	if (endWord > word)
	{
		endWord--;
		while (endWord > word && IsASpace(*endWord))
		{
			endWord--;
		}
	}

	return endWord - word;
}

CScriptEditorCtrl::CScriptEditorCtrl() : m_BraceCount(0), m_CurrentCallTip(0), m_StartCalltipWord(0), m_LastPosCallTip(0) {}

BOOL CScriptEditorCtrl::SubclassWindow(HWND hWnd)
{
	BOOL bRet = CScintillaCtrl::SubclassWindow(hWnd);

	if (bRet)
		Init();

	return bRet;
}

DWORD CScriptEditorCtrl::GetPropertyColor(const char* key, bool* key_exist)
{
	pfc::array_t<char> buff;
	int len = GetPropertyExpanded(key, 0);

	if (key_exist)
		*key_exist = (len != 0);

	if (len == 0)
		return 0;

	buff.set_size(len + 1);
	buff[len] = 0;
	GetPropertyExpanded(key, buff.get_ptr());
	return ParseHex(buff.get_ptr());
}

IndentationStatus CScriptEditorCtrl::GetIndentState(int line)
{
	IndentationStatus indentState = isNone;
	std::string controlWords[20];
	t_size i, parts = GetLinePartsInStyle(line, m_StatementIndent.styleNumber, controlWords, _countof(controlWords));

	for (i = 0; i < parts; ++i)
	{
		if (IsSymIncludes(m_StatementIndent, controlWords[i]))
			indentState = isKeyWordStart;
	}

	parts = GetLinePartsInStyle(line, m_StatementEnd.styleNumber, controlWords, _countof(controlWords));

	for (i = 0; i < parts; ++i)
	{
		if (IsSymIncludes(m_StatementEnd, controlWords[i]))
			indentState = isNone;
	}
	std::string controlStrings[20];

	parts = GetLinePartsInStyle(line, m_BlockEnd.styleNumber, controlStrings, _countof(controlStrings));

	for (i = 0; i < parts; ++i)
	{
		if (IsSymIncludes(m_BlockEnd, controlStrings[i]))
			indentState = isBlockEnd;

		if (IsSymIncludes(m_BlockStart, controlStrings[i]))
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

	if ((selEnd == selStart) && (selStart > 0))
	{
		if (CallTipActive())
		{
			switch (ch)
			{
			case ')':
				m_BraceCount--;
				if (m_BraceCount < 1)
					CallTipCancel();
				else
					StartCallTip();
				break;

			case '(':
				m_BraceCount++;
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
				m_BraceCount++;
				StartCallTip();
			}
			else if (ch == ')')
			{
				m_BraceCount--;
			}
			else if (!IsIdentifierChar(ch))
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
				m_BraceCount = 1;
				StartCallTip();
			}
			else
			{
				AutomaticIndentation(ch);

				if (IsIdentifierChar(ch) || ch == '.')
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

		SetHighlightGuide(min(columnAtCaret, columnOpposite));
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

bool CScriptEditorCtrl::GetNearestWords(pfc::string_base& out, const char* wordStart, int searchLen, const char* separators)
{
	out.reset();

	if (m_apis.size() == 0)
		return false;

	bool status = false;

	while (!status && *separators)
	{
		char otherSeparator = *separators;
		t_size index;

		if (pfc::bsearch_t(m_apis.size(), m_apis, StringComparePartialNC(searchLen), wordStart, index))
		{
			t_size pivot = index;
			status = true;

			while ((pivot > 0) && (StringComparePartialNC(searchLen)(m_apis[pivot - 1], wordStart) == 0))
			{
				--pivot;
			}

			while (pivot <= m_apis.size() - 1)
			{
				if (StringComparePartialNC(searchLen)(m_apis[pivot], wordStart) != 0)
					break;

				t_size wordlen = LengthWord(m_apis[pivot], otherSeparator) + 1;

				out.add_string(m_apis[pivot], wordlen);
				out.add_char(' ');

				++pivot;
			}

		}

		separators++;
	}

	return status;
}

bool CScriptEditorCtrl::RangeIsAllWhitespace(int start, int end)
{
	if (start < 0)
		start = 0;

	end = min(end, GetLength());

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

	while ((startword > 0) && (IsIdentifierChar(line[startword - 1]) || line[startword - 1] == '.'))
	{
		startword--;
	}

	pfc::string8_fast root;
	root.set_string(line.c_str() + startword, current - startword);

	if (m_apis.empty())
		return false;

	pfc::string8_fast words;

	if (!GetNearestWords(words, root.get_ptr(), root.length(), "("))
		return false;

	AutoCShow(root.length(), words);

	return true;
}

bool CScriptEditorCtrl::StartCallTip()
{
	m_CurrentCallTip = 0;
	m_CurrentCallTipWord = "";
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
	} while (current > 0 && !IsIdentifierChar(line[current - 1]));

	if (current <= 0)
		return true;

	m_StartCalltipWord = current - 1;

	while (m_StartCalltipWord > 0 && (IsIdentifierChar(line[m_StartCalltipWord - 1]) || (line[m_StartCalltipWord - 1] == '.')))
	{
		--m_StartCalltipWord;
	}

	line.at(current) = '\0';
	m_CurrentCallTipWord = line.c_str() + m_StartCalltipWord;
	m_FunctionDefinition = "";
	FillFunctionDefinition(pos);
	return true;
}

const char* CScriptEditorCtrl::GetNearestWord(const char* wordStart, int searchLen, std::string wordCharacters, int wordIndex)
{
	if (m_apis.empty())
		return nullptr;

	t_size index;

	if (pfc::bsearch_t(m_apis.size(), m_apis, StringComparePartialNC(searchLen), wordStart, index))
	{
		t_size start = index;

		while (start > 0 && StringComparePartialNC(searchLen)(m_apis[start - 1], wordStart) == 0)
		{
			--start;
		}

		t_size end = index;

		while (end < m_apis.size() - 1 && StringComparePartialNC(searchLen)(m_apis[end + 1], wordStart) == 0)
		{
			++end;
		}

		for (t_size i = start; i <= end; ++i)
		{
			const char* word = m_apis[i];

			if (wordCharacters.find(word[searchLen]) == std::string::npos)
			{
				if (wordIndex <= 0)
					return word;

				--wordIndex;
			}
		}

		return nullptr;
	}
	return nullptr;
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

std::string CScriptEditorCtrl::GetCurrentLine()
{
	const int len = GetCurLine(0, 0);
	std::string text(len, '\0');
	GetCurLine(&text[0], len);
	return text.substr(0, text.length() - 1);
}

t_size CScriptEditorCtrl::GetLinePartsInStyle(int line, int style, std::string sv[], int len)
{
	for (int i = 0; i < len; ++i)
		sv[i] = "";

	std::string s;
	int part = 0;
	int thisLineStart = PositionFromLine(line);
	int nextLineStart = PositionFromLine(line + 1);

	for (int pos = thisLineStart; pos < nextLineStart; ++pos)
	{
		if (GetStyleAt(pos) == style || GetStyleAt(pos) == -1)
		{
			char c[2];
			c[0] = GetCharAt(pos);
			c[1] = '\0';
			s += c;
		}
		else if (s.length() > 0)
		{
			if (part < len)
			{
				sv[part++] = s;
			}
			s = "";
		}
	}

	if ((s.length() > 0) && (part < len))
	{
		sv[part++] = s;
	}

	return part;
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
		pfc::array_t<char> linebuf;
		bool foundBrace = false;
		int prevLineLength = LineLength(curLine - 1);
		int slen = 0;

		linebuf.set_size(prevLineLength + 2);
		GetLine(curLine - 1, linebuf.get_ptr());
		linebuf[prevLineLength] = 0;
		slen = strlen(linebuf.get_ptr());

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
		std::string controlWords[1];

		if (GetLinePartsInStyle(curLine - 1, m_BlockEnd.styleNumber, controlWords, _countof(controlWords)))
		{
			if (IsSymIncludes(m_BlockEnd, controlWords[0]))
			{
				SetIndentation(curLine - 1, IndentOfBlock(curLine - 2) - indentSize);
				indentBlock = IndentOfBlock(curLine - 1);
			}
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

	for (int i = m_StartCalltipWord; i < current; ++i)
	{
		if (line[i] == '(')
			braces++;
		else if (line[i] == ')' && braces > 0)
			braces--;
		else if (braces == 1 && line[i] == ',')
			commas++;
	}

	int startHighlight = 0;

	while (m_FunctionDefinition[startHighlight] && m_FunctionDefinition[startHighlight] != '(')
		startHighlight++;

	if (m_FunctionDefinition[startHighlight] == '(')
		startHighlight++;

	while (m_FunctionDefinition[startHighlight] && commas > 0)
	{
		if (m_FunctionDefinition[startHighlight] == ',')
			commas--;
		if (m_FunctionDefinition[startHighlight] == ')')
			commas = 0;
		else
			startHighlight++;
	}

	if (m_FunctionDefinition[startHighlight] == ',')
		startHighlight++;

	int endHighlight = startHighlight;

	while (m_FunctionDefinition[endHighlight] && m_FunctionDefinition[endHighlight] != ',' && m_FunctionDefinition[endHighlight] != ')')
	{
		endHighlight++;
	}

	CallTipSetHlt(startHighlight, endHighlight);
}

void CScriptEditorCtrl::FillFunctionDefinition(int pos)
{
	if (pos > 0)
	{
		m_LastPosCallTip = pos;
	}

	if (m_apis.size())
	{
		pfc::string8_fast words;

		if (!GetNearestWords(words, m_CurrentCallTipWord.get_ptr(), m_CurrentCallTipWord.get_length(), "("))
		{
			t_size calltip_pos = m_CurrentCallTipWord.find_first(".");

			if (calltip_pos == SIZE_MAX)
				return;

			m_CurrentCallTipWord.remove_chars(0, calltip_pos + 1);

			if (!GetNearestWords(words, m_CurrentCallTipWord.get_ptr(), m_CurrentCallTipWord.get_length(), "("))
			{
				return;
			}
		}

		const char* word = GetNearestWord(m_CurrentCallTipWord.get_ptr(), m_CurrentCallTipWord.get_length(), "_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", m_CurrentCallTip);

		if (word)
		{
			m_FunctionDefinition = word;

			CallTipShow(m_LastPosCallTip - m_CurrentCallTipWord.get_length(), m_FunctionDefinition.get_ptr());
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
	LoadProperties(g_sci_prop_sets.m_data);
}

void CScriptEditorCtrl::LoadProperties(const std::vector<t_sci_prop_set>& data)
{
	for (t_size i = 0; i < data.size(); ++i)
	{
		SetProperty(data[i].key.get_ptr(), data[i].val.get_ptr());
	}
}

void CScriptEditorCtrl::ReadAPI()
{
	auto sc = [](const char* s1, const char* s2)
	{
		int result = _stricmp(s1, s2);

		if (result == 0)
		{
			if (isalpha(*s1) && (*s1 != *s2))
			{
				return islower(*s1) ? -1 : 1;
			}
		}

		return result;
	};

	m_apis.clear();
	puResource pures = uLoadResource(core_api::get_my_instance(), uMAKEINTRESOURCE(IDR_API), "TEXT");
	pfc::string8_fast content(static_cast<const char*>(pures->GetPointer()), pures->GetSize());
	pfc::string_list_impl list;
	pfc::splitStringByLines(list, content);

	for (t_size i = 0; i < list.get_count(); ++i)
	{
		pfc::string8_fast tmp = list[i];
		if (tmp.get_length())
		{
			m_apis.emplace_back(tmp);
		}
	}
	pfc::sort_t(m_apis, sc, m_apis.size());
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

	// Additional styles
	bool sel_fore, sel_back, line_back;
	DWORD color;

	color = GetPropertyColor("style.selection.fore", &sel_fore);

	if (sel_fore)
		SetSelFore(true, color);
	else
		SetSelFore(false, 0);

	color = GetPropertyColor("style.selection.back", &sel_back);

	if (sel_back)
	{
		SetSelBack(true, color);
	}
	else
	{
		if (sel_fore)
			SetSelBack(true, RGB(0xc0, 0xc0, 0xc0));
	}

	SetSelAlpha(GetPropertyInt("style.selection.alpha", SC_ALPHA_NOALPHA));
	SetCaretFore(GetPropertyColor("style.caret.fore"));
	SetCaretWidth(GetPropertyInt("style.caret.width", 1));

	color = GetPropertyColor("style.caret.line.back", &line_back);

	if (line_back)
	{
		SetCaretLineVisible(true);
		SetCaretLineBack(color);
	}
	else
	{
		SetCaretLineVisible(false);
	}

	SetCaretLineBackAlpha(GetPropertyInt("style.caret.line.back.alpha", SC_ALPHA_NOALPHA));
}

void CScriptEditorCtrl::SetAllStylesFromTable(const t_style_to_key_table table[])
{
	const char* key = nullptr;

	for (int i = 0; table[i].style_num != -1; ++i)
	{
		if ((key = table[i].key) != nullptr)
		{
			int style_num;
			pfc::array_t<char> definition;
			t_sci_editor_style style;
			int len;

			style_num = table[i].style_num;

			len = GetPropertyExpanded(key, 0);

			if (len != 0)
			{
				definition.set_size(len + 1);
				definition[len] = 0;
				GetPropertyExpanded(key, definition.get_ptr());

				if (!ParseStyle(definition.get_ptr(), style))
					continue;

				if (style.flags & ESF_FONT)
					StyleSetFont(style_num, style.font.get_ptr());

				if (style.flags & ESF_SIZE)
					StyleSetSize(style_num, style.size);

				if (style.flags & ESF_FORE)
					StyleSetFore(style_num, style.fore);

				if (style.flags & ESF_BACK)
					StyleSetBack(style_num, style.back);

				if (style.flags & ESF_ITALICS)
					StyleSetItalic(style_num, style.italics);

				if (style.flags & ESF_BOLD)
					StyleSetBold(style_num, style.bold);

				if (style.flags & ESF_UNDERLINED)
					StyleSetUnderline(style_num, style.underlined);

				if (style.flags & ESF_CASEFORCE)
					StyleSetCase(style_num, style.case_force);
			}

			if (style_num == STYLE_DEFAULT)
				StyleClearAll();
		}
	}
}

void CScriptEditorCtrl::SetContent(const char* text, bool clear_undo_buffer)
{
	SetText(text);
	ConvertEOLs(SC_EOL_CRLF);

	if (clear_undo_buffer)
		EmptyUndoBuffer();

	GrabFocus();
	TrackWidth();
}

void CScriptEditorCtrl::SetJScript()
{
	RestoreDefaultStyle();
	SetLexer(SCLEX_CPP);
	SetKeyWords(0, js_keywords);
	SetAllStylesFromTable(js_style_table);
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
