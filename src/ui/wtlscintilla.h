#pragma once

template <class T>
class CScintillaImpl : public CWindowImpl<T, CWindow, CControlWinTraits>
{
public:
	DECLARE_WND_SUPERCLASS2(_T("WTL_ScintillaCtrl"), CScintillaImpl, CWindow::GetWndClassName())

	using Colour = int;
	using Line = int;
	using Position = int;

	void SetFnPtr()
	{
		ATLASSERT(::IsWindow(this->m_hWnd));
		fn = reinterpret_cast<FunctionDirect>(::SendMessage(this->m_hWnd, SCI_GETDIRECTFUNCTION, 0, 0));
		ptr = ::SendMessage(this->m_hWnd, SCI_GETDIRECTPOINTER, 0, 0);
	}

	// Auto-generated from Scintilla.iface by scintilla_iface.js

	void AddText(Position length, const char* text)
	{
		Call(SCI_ADDTEXT, length, reinterpret_cast<intptr_t>(text));
	}

	void AddStyledText(Position length, const char* c)
	{
		Call(SCI_ADDSTYLEDTEXT, length, reinterpret_cast<intptr_t>(c));
	}

	void InsertText(Position pos, const char* text)
	{
		Call(SCI_INSERTTEXT, pos, reinterpret_cast<intptr_t>(text));
	}

	void ChangeInsertion(Position length, const char* text)
	{
		Call(SCI_CHANGEINSERTION, length, reinterpret_cast<intptr_t>(text));
	}

	void ClearAll()
	{
		Call(SCI_CLEARALL);
	}

	void DeleteRange(Position start, Position lengthDelete)
	{
		Call(SCI_DELETERANGE, start, lengthDelete);
	}

	void ClearDocumentStyle()
	{
		Call(SCI_CLEARDOCUMENTSTYLE);
	}

	Position GetLength()
	{
		return Call(SCI_GETLENGTH);
	}

	int GetCharAt(Position pos)
	{
		return Call(SCI_GETCHARAT, pos);
	}

	Position GetCurrentPos()
	{
		return Call(SCI_GETCURRENTPOS);
	}

	Position GetAnchor()
	{
		return Call(SCI_GETANCHOR);
	}

	int GetStyleAt(Position pos)
	{
		return Call(SCI_GETSTYLEAT, pos);
	}

	void Redo()
	{
		Call(SCI_REDO);
	}

	void SetUndoCollection(bool collectUndo)
	{
		Call(SCI_SETUNDOCOLLECTION, collectUndo);
	}

	void SelectAll()
	{
		Call(SCI_SELECTALL);
	}

	void SetSavePoint()
	{
		Call(SCI_SETSAVEPOINT);
	}

	Position GetStyledText(void* tr)
	{
		return Call(SCI_GETSTYLEDTEXT, 0, reinterpret_cast<intptr_t>(tr));
	}

	bool CanRedo()
	{
		return Call(SCI_CANREDO);
	}

	int MarkerLineFromHandle(int markerHandle)
	{
		return Call(SCI_MARKERLINEFROMHANDLE, markerHandle);
	}

	void MarkerDeleteHandle(int markerHandle)
	{
		Call(SCI_MARKERDELETEHANDLE, markerHandle);
	}

	bool GetUndoCollection()
	{
		return Call(SCI_GETUNDOCOLLECTION);
	}

	int GetViewWS()
	{
		return Call(SCI_GETVIEWWS);
	}

	void SetViewWS(int viewWS)
	{
		Call(SCI_SETVIEWWS, viewWS);
	}

	int GetTabDrawMode()
	{
		return Call(SCI_GETTABDRAWMODE);
	}

	void SetTabDrawMode(int tabDrawMode)
	{
		Call(SCI_SETTABDRAWMODE, tabDrawMode);
	}

	Position PositionFromPoint(int x, int y)
	{
		return Call(SCI_POSITIONFROMPOINT, x, y);
	}

	Position PositionFromPointClose(int x, int y)
	{
		return Call(SCI_POSITIONFROMPOINTCLOSE, x, y);
	}

	void GotoLine(Line line)
	{
		Call(SCI_GOTOLINE, line);
	}

	void GotoPos(Position caret)
	{
		Call(SCI_GOTOPOS, caret);
	}

	void SetAnchor(Position anchor)
	{
		Call(SCI_SETANCHOR, anchor);
	}

	Position GetCurLine(Position length, char* text)
	{
		return Call(SCI_GETCURLINE, length, reinterpret_cast<intptr_t>(text));
	}

	Position GetEndStyled()
	{
		return Call(SCI_GETENDSTYLED);
	}

	void ConvertEOLs(int eolMode)
	{
		Call(SCI_CONVERTEOLS, eolMode);
	}

	int GetEOLMode()
	{
		return Call(SCI_GETEOLMODE);
	}

	void SetEOLMode(int eolMode)
	{
		Call(SCI_SETEOLMODE, eolMode);
	}

	void StartStyling(Position start, int unused)
	{
		Call(SCI_STARTSTYLING, start, unused);
	}

	void SetStyling(Position length, int style)
	{
		Call(SCI_SETSTYLING, length, style);
	}

	bool GetBufferedDraw()
	{
		return Call(SCI_GETBUFFEREDDRAW);
	}

	void SetBufferedDraw(bool buffered)
	{
		Call(SCI_SETBUFFEREDDRAW, buffered);
	}

	void SetTabWidth(int tabWidth)
	{
		Call(SCI_SETTABWIDTH, tabWidth);
	}

	int GetTabWidth()
	{
		return Call(SCI_GETTABWIDTH);
	}

	void SetTabMinimumWidth(int pixels)
	{
		Call(SCI_SETTABMINIMUMWIDTH, pixels);
	}

	int GetTabMinimumWidth()
	{
		return Call(SCI_GETTABMINIMUMWIDTH);
	}

	void ClearTabStops(Line line)
	{
		Call(SCI_CLEARTABSTOPS, line);
	}

	void AddTabStop(Line line, int x)
	{
		Call(SCI_ADDTABSTOP, line, x);
	}

	int GetNextTabStop(Line line, int x)
	{
		return Call(SCI_GETNEXTTABSTOP, line, x);
	}

	void SetCodePage(int codePage)
	{
		Call(SCI_SETCODEPAGE, codePage);
	}

	int GetIMEInteraction()
	{
		return Call(SCI_GETIMEINTERACTION);
	}

	void SetIMEInteraction(int imeInteraction)
	{
		Call(SCI_SETIMEINTERACTION, imeInteraction);
	}

	void MarkerDefine(int markerNumber, int markerSymbol)
	{
		Call(SCI_MARKERDEFINE, markerNumber, markerSymbol);
	}

	void MarkerSetFore(int markerNumber, Colour fore)
	{
		Call(SCI_MARKERSETFORE, markerNumber, fore);
	}

	void MarkerSetBack(int markerNumber, Colour back)
	{
		Call(SCI_MARKERSETBACK, markerNumber, back);
	}

	void MarkerSetBackSelected(int markerNumber, Colour back)
	{
		Call(SCI_MARKERSETBACKSELECTED, markerNumber, back);
	}

	void MarkerEnableHighlight(bool enabled)
	{
		Call(SCI_MARKERENABLEHIGHLIGHT, enabled);
	}

	int MarkerAdd(Line line, int markerNumber)
	{
		return Call(SCI_MARKERADD, line, markerNumber);
	}

	void MarkerDelete(Line line, int markerNumber)
	{
		Call(SCI_MARKERDELETE, line, markerNumber);
	}

	void MarkerDeleteAll(int markerNumber)
	{
		Call(SCI_MARKERDELETEALL, markerNumber);
	}

	int MarkerGet(Line line)
	{
		return Call(SCI_MARKERGET, line);
	}

	Line MarkerNext(Line lineStart, int markerMask)
	{
		return Call(SCI_MARKERNEXT, lineStart, markerMask);
	}

	Line MarkerPrevious(Line lineStart, int markerMask)
	{
		return Call(SCI_MARKERPREVIOUS, lineStart, markerMask);
	}

	void MarkerDefinePixmap(int markerNumber, const char* pixmap)
	{
		Call(SCI_MARKERDEFINEPIXMAP, markerNumber, reinterpret_cast<intptr_t>(pixmap));
	}

	void MarkerAddSet(Line line, int markerSet)
	{
		Call(SCI_MARKERADDSET, line, markerSet);
	}

	void MarkerSetAlpha(int markerNumber, int alpha)
	{
		Call(SCI_MARKERSETALPHA, markerNumber, alpha);
	}

	void SetMarginTypeN(int margin, int marginType)
	{
		Call(SCI_SETMARGINTYPEN, margin, marginType);
	}

	int GetMarginTypeN(int margin)
	{
		return Call(SCI_GETMARGINTYPEN, margin);
	}

	void SetMarginWidthN(int margin, int pixelWidth)
	{
		Call(SCI_SETMARGINWIDTHN, margin, pixelWidth);
	}

	int GetMarginWidthN(int margin)
	{
		return Call(SCI_GETMARGINWIDTHN, margin);
	}

	void SetMarginMaskN(int margin, int mask)
	{
		Call(SCI_SETMARGINMASKN, margin, mask);
	}

	int GetMarginMaskN(int margin)
	{
		return Call(SCI_GETMARGINMASKN, margin);
	}

	void SetMarginSensitiveN(int margin, bool sensitive)
	{
		Call(SCI_SETMARGINSENSITIVEN, margin, sensitive);
	}

	bool GetMarginSensitiveN(int margin)
	{
		return Call(SCI_GETMARGINSENSITIVEN, margin);
	}

	void SetMarginCursorN(int margin, int cursor)
	{
		Call(SCI_SETMARGINCURSORN, margin, cursor);
	}

	int GetMarginCursorN(int margin)
	{
		return Call(SCI_GETMARGINCURSORN, margin);
	}

	void SetMarginBackN(int margin, Colour back)
	{
		Call(SCI_SETMARGINBACKN, margin, back);
	}

	Colour GetMarginBackN(int margin)
	{
		return Call(SCI_GETMARGINBACKN, margin);
	}

	void SetMargins(int margins)
	{
		Call(SCI_SETMARGINS, margins);
	}

	int GetMargins()
	{
		return Call(SCI_GETMARGINS);
	}

	void StyleClearAll()
	{
		Call(SCI_STYLECLEARALL);
	}

	void StyleSetFore(int style, Colour fore)
	{
		Call(SCI_STYLESETFORE, style, fore);
	}

	void StyleSetBack(int style, Colour back)
	{
		Call(SCI_STYLESETBACK, style, back);
	}

	void StyleSetBold(int style, bool bold)
	{
		Call(SCI_STYLESETBOLD, style, bold);
	}

	void StyleSetItalic(int style, bool italic)
	{
		Call(SCI_STYLESETITALIC, style, italic);
	}

	void StyleSetSize(int style, int sizePoints)
	{
		Call(SCI_STYLESETSIZE, style, sizePoints);
	}

	void StyleSetFont(int style, const char* fontName)
	{
		Call(SCI_STYLESETFONT, style, reinterpret_cast<intptr_t>(fontName));
	}

	void StyleSetEOLFilled(int style, bool eolFilled)
	{
		Call(SCI_STYLESETEOLFILLED, style, eolFilled);
	}

	void StyleResetDefault()
	{
		Call(SCI_STYLERESETDEFAULT);
	}

	void StyleSetUnderline(int style, bool underline)
	{
		Call(SCI_STYLESETUNDERLINE, style, underline);
	}

	Colour StyleGetFore(int style)
	{
		return Call(SCI_STYLEGETFORE, style);
	}

	Colour StyleGetBack(int style)
	{
		return Call(SCI_STYLEGETBACK, style);
	}

	bool StyleGetBold(int style)
	{
		return Call(SCI_STYLEGETBOLD, style);
	}

	bool StyleGetItalic(int style)
	{
		return Call(SCI_STYLEGETITALIC, style);
	}

	int StyleGetSize(int style)
	{
		return Call(SCI_STYLEGETSIZE, style);
	}

	int StyleGetFont(int style, char* fontName)
	{
		return Call(SCI_STYLEGETFONT, style, reinterpret_cast<intptr_t>(fontName));
	}

	bool StyleGetEOLFilled(int style)
	{
		return Call(SCI_STYLEGETEOLFILLED, style);
	}

	bool StyleGetUnderline(int style)
	{
		return Call(SCI_STYLEGETUNDERLINE, style);
	}

	int StyleGetCase(int style)
	{
		return Call(SCI_STYLEGETCASE, style);
	}

	int StyleGetCharacterSet(int style)
	{
		return Call(SCI_STYLEGETCHARACTERSET, style);
	}

	bool StyleGetVisible(int style)
	{
		return Call(SCI_STYLEGETVISIBLE, style);
	}

	bool StyleGetChangeable(int style)
	{
		return Call(SCI_STYLEGETCHANGEABLE, style);
	}

	bool StyleGetHotSpot(int style)
	{
		return Call(SCI_STYLEGETHOTSPOT, style);
	}

	void StyleSetCase(int style, int caseVisible)
	{
		Call(SCI_STYLESETCASE, style, caseVisible);
	}

	void StyleSetSizeFractional(int style, int sizeHundredthPoints)
	{
		Call(SCI_STYLESETSIZEFRACTIONAL, style, sizeHundredthPoints);
	}

	int StyleGetSizeFractional(int style)
	{
		return Call(SCI_STYLEGETSIZEFRACTIONAL, style);
	}

	void StyleSetWeight(int style, int weight)
	{
		Call(SCI_STYLESETWEIGHT, style, weight);
	}

	int StyleGetWeight(int style)
	{
		return Call(SCI_STYLEGETWEIGHT, style);
	}

	void StyleSetCharacterSet(int style, int characterSet)
	{
		Call(SCI_STYLESETCHARACTERSET, style, characterSet);
	}

	void StyleSetHotSpot(int style, bool hotspot)
	{
		Call(SCI_STYLESETHOTSPOT, style, hotspot);
	}

	void SetSelFore(bool useSetting, Colour fore)
	{
		Call(SCI_SETSELFORE, useSetting, fore);
	}

	void SetSelBack(bool useSetting, Colour back)
	{
		Call(SCI_SETSELBACK, useSetting, back);
	}

	int GetSelAlpha()
	{
		return Call(SCI_GETSELALPHA);
	}

	void SetSelAlpha(int alpha)
	{
		Call(SCI_SETSELALPHA, alpha);
	}

	bool GetSelEOLFilled()
	{
		return Call(SCI_GETSELEOLFILLED);
	}

	void SetSelEOLFilled(bool filled)
	{
		Call(SCI_SETSELEOLFILLED, filled);
	}

	void SetCaretFore(Colour fore)
	{
		Call(SCI_SETCARETFORE, fore);
	}

	void AssignCmdKey(int keyDefinition, int sciCommand)
	{
		Call(SCI_ASSIGNCMDKEY, keyDefinition, sciCommand);
	}

	void ClearCmdKey(int keyDefinition)
	{
		Call(SCI_CLEARCMDKEY, keyDefinition);
	}

	void ClearAllCmdKeys()
	{
		Call(SCI_CLEARALLCMDKEYS);
	}

	void SetStylingEx(Position length, const char* styles)
	{
		Call(SCI_SETSTYLINGEX, length, reinterpret_cast<intptr_t>(styles));
	}

	void StyleSetVisible(int style, bool visible)
	{
		Call(SCI_STYLESETVISIBLE, style, visible);
	}

	int GetCaretPeriod()
	{
		return Call(SCI_GETCARETPERIOD);
	}

	void SetCaretPeriod(int periodMilliseconds)
	{
		Call(SCI_SETCARETPERIOD, periodMilliseconds);
	}

	void SetWordChars(const char* characters)
	{
		Call(SCI_SETWORDCHARS, 0, reinterpret_cast<intptr_t>(characters));
	}

	int GetWordChars(char* characters)
	{
		return Call(SCI_GETWORDCHARS, 0, reinterpret_cast<intptr_t>(characters));
	}

	void SetCharacterCategoryOptimization(int countCharacters)
	{
		Call(SCI_SETCHARACTERCATEGORYOPTIMIZATION, countCharacters);
	}

	int GetCharacterCategoryOptimization()
	{
		return Call(SCI_GETCHARACTERCATEGORYOPTIMIZATION);
	}

	void BeginUndoAction()
	{
		Call(SCI_BEGINUNDOACTION);
	}

	void EndUndoAction()
	{
		Call(SCI_ENDUNDOACTION);
	}

	void IndicSetStyle(int indicator, int indicatorStyle)
	{
		Call(SCI_INDICSETSTYLE, indicator, indicatorStyle);
	}

	int IndicGetStyle(int indicator)
	{
		return Call(SCI_INDICGETSTYLE, indicator);
	}

	void IndicSetFore(int indicator, Colour fore)
	{
		Call(SCI_INDICSETFORE, indicator, fore);
	}

	Colour IndicGetFore(int indicator)
	{
		return Call(SCI_INDICGETFORE, indicator);
	}

	void IndicSetUnder(int indicator, bool under)
	{
		Call(SCI_INDICSETUNDER, indicator, under);
	}

	bool IndicGetUnder(int indicator)
	{
		return Call(SCI_INDICGETUNDER, indicator);
	}

	void IndicSetHoverStyle(int indicator, int indicatorStyle)
	{
		Call(SCI_INDICSETHOVERSTYLE, indicator, indicatorStyle);
	}

	int IndicGetHoverStyle(int indicator)
	{
		return Call(SCI_INDICGETHOVERSTYLE, indicator);
	}

	void IndicSetHoverFore(int indicator, Colour fore)
	{
		Call(SCI_INDICSETHOVERFORE, indicator, fore);
	}

	Colour IndicGetHoverFore(int indicator)
	{
		return Call(SCI_INDICGETHOVERFORE, indicator);
	}

	void IndicSetFlags(int indicator, int flags)
	{
		Call(SCI_INDICSETFLAGS, indicator, flags);
	}

	int IndicGetFlags(int indicator)
	{
		return Call(SCI_INDICGETFLAGS, indicator);
	}

	void SetWhitespaceFore(bool useSetting, Colour fore)
	{
		Call(SCI_SETWHITESPACEFORE, useSetting, fore);
	}

	void SetWhitespaceBack(bool useSetting, Colour back)
	{
		Call(SCI_SETWHITESPACEBACK, useSetting, back);
	}

	void SetWhitespaceSize(int size)
	{
		Call(SCI_SETWHITESPACESIZE, size);
	}

	int GetWhitespaceSize()
	{
		return Call(SCI_GETWHITESPACESIZE);
	}

	void SetLineState(Line line, int state)
	{
		Call(SCI_SETLINESTATE, line, state);
	}

	int GetLineState(Line line)
	{
		return Call(SCI_GETLINESTATE, line);
	}

	int GetMaxLineState()
	{
		return Call(SCI_GETMAXLINESTATE);
	}

	bool GetCaretLineVisible()
	{
		return Call(SCI_GETCARETLINEVISIBLE);
	}

	void SetCaretLineVisible(bool show)
	{
		Call(SCI_SETCARETLINEVISIBLE, show);
	}

	Colour GetCaretLineBack()
	{
		return Call(SCI_GETCARETLINEBACK);
	}

	void SetCaretLineBack(Colour back)
	{
		Call(SCI_SETCARETLINEBACK, back);
	}

	int GetCaretLineFrame()
	{
		return Call(SCI_GETCARETLINEFRAME);
	}

	void SetCaretLineFrame(int width)
	{
		Call(SCI_SETCARETLINEFRAME, width);
	}

	void StyleSetChangeable(int style, bool changeable)
	{
		Call(SCI_STYLESETCHANGEABLE, style, changeable);
	}

	void AutoCShow(Position lengthEntered, const char* itemList)
	{
		Call(SCI_AUTOCSHOW, lengthEntered, reinterpret_cast<intptr_t>(itemList));
	}

	void AutoCCancel()
	{
		Call(SCI_AUTOCCANCEL);
	}

	bool AutoCActive()
	{
		return Call(SCI_AUTOCACTIVE);
	}

	Position AutoCPosStart()
	{
		return Call(SCI_AUTOCPOSSTART);
	}

	void AutoCComplete()
	{
		Call(SCI_AUTOCCOMPLETE);
	}

	void AutoCStops(const char* characterSet)
	{
		Call(SCI_AUTOCSTOPS, 0, reinterpret_cast<intptr_t>(characterSet));
	}

	void AutoCSetSeparator(int separatorCharacter)
	{
		Call(SCI_AUTOCSETSEPARATOR, separatorCharacter);
	}

	int AutoCGetSeparator()
	{
		return Call(SCI_AUTOCGETSEPARATOR);
	}

	void AutoCSelect(const char* select)
	{
		Call(SCI_AUTOCSELECT, 0, reinterpret_cast<intptr_t>(select));
	}

	void AutoCSetCancelAtStart(bool cancel)
	{
		Call(SCI_AUTOCSETCANCELATSTART, cancel);
	}

	bool AutoCGetCancelAtStart()
	{
		return Call(SCI_AUTOCGETCANCELATSTART);
	}

	void AutoCSetFillUps(const char* characterSet)
	{
		Call(SCI_AUTOCSETFILLUPS, 0, reinterpret_cast<intptr_t>(characterSet));
	}

	void AutoCSetChooseSingle(bool chooseSingle)
	{
		Call(SCI_AUTOCSETCHOOSESINGLE, chooseSingle);
	}

	bool AutoCGetChooseSingle()
	{
		return Call(SCI_AUTOCGETCHOOSESINGLE);
	}

	void AutoCSetIgnoreCase(bool ignoreCase)
	{
		Call(SCI_AUTOCSETIGNORECASE, ignoreCase);
	}

	bool AutoCGetIgnoreCase()
	{
		return Call(SCI_AUTOCGETIGNORECASE);
	}

	void UserListShow(int listType, const char* itemList)
	{
		Call(SCI_USERLISTSHOW, listType, reinterpret_cast<intptr_t>(itemList));
	}

	void AutoCSetAutoHide(bool autoHide)
	{
		Call(SCI_AUTOCSETAUTOHIDE, autoHide);
	}

	bool AutoCGetAutoHide()
	{
		return Call(SCI_AUTOCGETAUTOHIDE);
	}

	void AutoCSetDropRestOfWord(bool dropRestOfWord)
	{
		Call(SCI_AUTOCSETDROPRESTOFWORD, dropRestOfWord);
	}

	bool AutoCGetDropRestOfWord()
	{
		return Call(SCI_AUTOCGETDROPRESTOFWORD);
	}

	void RegisterImage(int type, const char* xpmData)
	{
		Call(SCI_REGISTERIMAGE, type, reinterpret_cast<intptr_t>(xpmData));
	}

	void ClearRegisteredImages()
	{
		Call(SCI_CLEARREGISTEREDIMAGES);
	}

	int AutoCGetTypeSeparator()
	{
		return Call(SCI_AUTOCGETTYPESEPARATOR);
	}

	void AutoCSetTypeSeparator(int separatorCharacter)
	{
		Call(SCI_AUTOCSETTYPESEPARATOR, separatorCharacter);
	}

	void AutoCSetMaxWidth(int characterCount)
	{
		Call(SCI_AUTOCSETMAXWIDTH, characterCount);
	}

	int AutoCGetMaxWidth()
	{
		return Call(SCI_AUTOCGETMAXWIDTH);
	}

	void AutoCSetMaxHeight(int rowCount)
	{
		Call(SCI_AUTOCSETMAXHEIGHT, rowCount);
	}

	int AutoCGetMaxHeight()
	{
		return Call(SCI_AUTOCGETMAXHEIGHT);
	}

	void SetIndent(int indentSize)
	{
		Call(SCI_SETINDENT, indentSize);
	}

	int GetIndent()
	{
		return Call(SCI_GETINDENT);
	}

	void SetUseTabs(bool useTabs)
	{
		Call(SCI_SETUSETABS, useTabs);
	}

	bool GetUseTabs()
	{
		return Call(SCI_GETUSETABS);
	}

	void SetLineIndentation(Line line, int indentation)
	{
		Call(SCI_SETLINEINDENTATION, line, indentation);
	}

	int GetLineIndentation(Line line)
	{
		return Call(SCI_GETLINEINDENTATION, line);
	}

	Position GetLineIndentPosition(Line line)
	{
		return Call(SCI_GETLINEINDENTPOSITION, line);
	}

	Position GetColumn(Position pos)
	{
		return Call(SCI_GETCOLUMN, pos);
	}

	Position CountCharacters(Position start, Position end)
	{
		return Call(SCI_COUNTCHARACTERS, start, end);
	}

	Position CountCodeUnits(Position start, Position end)
	{
		return Call(SCI_COUNTCODEUNITS, start, end);
	}

	void SetHScrollBar(bool visible)
	{
		Call(SCI_SETHSCROLLBAR, visible);
	}

	bool GetHScrollBar()
	{
		return Call(SCI_GETHSCROLLBAR);
	}

	void SetIndentationGuides(int indentView)
	{
		Call(SCI_SETINDENTATIONGUIDES, indentView);
	}

	int GetIndentationGuides()
	{
		return Call(SCI_GETINDENTATIONGUIDES);
	}

	void SetHighlightGuide(Position column)
	{
		Call(SCI_SETHIGHLIGHTGUIDE, column);
	}

	Position GetHighlightGuide()
	{
		return Call(SCI_GETHIGHLIGHTGUIDE);
	}

	Position GetLineEndPosition(Line line)
	{
		return Call(SCI_GETLINEENDPOSITION, line);
	}

	int GetCodePage()
	{
		return Call(SCI_GETCODEPAGE);
	}

	Colour GetCaretFore()
	{
		return Call(SCI_GETCARETFORE);
	}

	bool GetReadOnly()
	{
		return Call(SCI_GETREADONLY);
	}

	void SetCurrentPos(Position caret)
	{
		Call(SCI_SETCURRENTPOS, caret);
	}

	void SetSelectionStart(Position anchor)
	{
		Call(SCI_SETSELECTIONSTART, anchor);
	}

	Position GetSelectionStart()
	{
		return Call(SCI_GETSELECTIONSTART);
	}

	void SetSelectionEnd(Position caret)
	{
		Call(SCI_SETSELECTIONEND, caret);
	}

	Position GetSelectionEnd()
	{
		return Call(SCI_GETSELECTIONEND);
	}

	void SetEmptySelection(Position caret)
	{
		Call(SCI_SETEMPTYSELECTION, caret);
	}

	void SetPrintMagnification(int magnification)
	{
		Call(SCI_SETPRINTMAGNIFICATION, magnification);
	}

	int GetPrintMagnification()
	{
		return Call(SCI_GETPRINTMAGNIFICATION);
	}

	void SetPrintColourMode(int mode)
	{
		Call(SCI_SETPRINTCOLOURMODE, mode);
	}

	int GetPrintColourMode()
	{
		return Call(SCI_GETPRINTCOLOURMODE);
	}

	Position FindText(int searchFlags, void* ft)
	{
		return Call(SCI_FINDTEXT, searchFlags, reinterpret_cast<intptr_t>(ft));
	}

	Position FormatRange(bool draw, void* fr)
	{
		return Call(SCI_FORMATRANGE, draw, reinterpret_cast<intptr_t>(fr));
	}

	Line GetFirstVisibleLine()
	{
		return Call(SCI_GETFIRSTVISIBLELINE);
	}

	Position GetLine(Line line, char* text)
	{
		return Call(SCI_GETLINE, line, reinterpret_cast<intptr_t>(text));
	}

	Line GetLineCount()
	{
		return Call(SCI_GETLINECOUNT);
	}

	void SetMarginLeft(int pixelWidth)
	{
		Call(SCI_SETMARGINLEFT, 0, pixelWidth);
	}

	int GetMarginLeft()
	{
		return Call(SCI_GETMARGINLEFT);
	}

	void SetMarginRight(int pixelWidth)
	{
		Call(SCI_SETMARGINRIGHT, 0, pixelWidth);
	}

	int GetMarginRight()
	{
		return Call(SCI_GETMARGINRIGHT);
	}

	bool GetModify()
	{
		return Call(SCI_GETMODIFY);
	}

	void SetSel(Position anchor, Position caret)
	{
		Call(SCI_SETSEL, anchor, caret);
	}

	Position GetSelText(char* text)
	{
		return Call(SCI_GETSELTEXT, 0, reinterpret_cast<intptr_t>(text));
	}

	Position GetTextRange(void* tr)
	{
		return Call(SCI_GETTEXTRANGE, 0, reinterpret_cast<intptr_t>(tr));
	}

	void HideSelection(bool hide)
	{
		Call(SCI_HIDESELECTION, hide);
	}

	int PointXFromPosition(Position pos)
	{
		return Call(SCI_POINTXFROMPOSITION, 0, pos);
	}

	int PointYFromPosition(Position pos)
	{
		return Call(SCI_POINTYFROMPOSITION, 0, pos);
	}

	Line LineFromPosition(Position pos)
	{
		return Call(SCI_LINEFROMPOSITION, pos);
	}

	Position PositionFromLine(Line line)
	{
		return Call(SCI_POSITIONFROMLINE, line);
	}

	void LineScroll(Position columns, Line lines)
	{
		Call(SCI_LINESCROLL, columns, lines);
	}

	void ScrollCaret()
	{
		Call(SCI_SCROLLCARET);
	}

	void ScrollRange(Position secondary, Position primary)
	{
		Call(SCI_SCROLLRANGE, secondary, primary);
	}

	void ReplaceSel(const char* text)
	{
		Call(SCI_REPLACESEL, 0, reinterpret_cast<intptr_t>(text));
	}

	void SetReadOnly(bool readOnly)
	{
		Call(SCI_SETREADONLY, readOnly);
	}

	void Null()
	{
		Call(SCI_NULL);
	}

	bool CanPaste()
	{
		return Call(SCI_CANPASTE);
	}

	bool CanUndo()
	{
		return Call(SCI_CANUNDO);
	}

	void EmptyUndoBuffer()
	{
		Call(SCI_EMPTYUNDOBUFFER);
	}

	void Undo()
	{
		Call(SCI_UNDO);
	}

	void Cut()
	{
		Call(SCI_CUT);
	}

	void Copy()
	{
		Call(SCI_COPY);
	}

	void Paste()
	{
		Call(SCI_PASTE);
	}

	void Clear()
	{
		Call(SCI_CLEAR);
	}

	void SetText(const char* text)
	{
		Call(SCI_SETTEXT, 0, reinterpret_cast<intptr_t>(text));
	}

	Position GetText(Position length, char* text)
	{
		return Call(SCI_GETTEXT, length, reinterpret_cast<intptr_t>(text));
	}

	Position GetTextLength()
	{
		return Call(SCI_GETTEXTLENGTH);
	}

	void* GetDirectFunction()
	{
		return reinterpret_cast<void*>(Call(SCI_GETDIRECTFUNCTION));
	}

	void* GetDirectPointer()
	{
		return reinterpret_cast<void*>(Call(SCI_GETDIRECTPOINTER));
	}

	void SetOvertype(bool overType)
	{
		Call(SCI_SETOVERTYPE, overType);
	}

	bool GetOvertype()
	{
		return Call(SCI_GETOVERTYPE);
	}

	void SetCaretWidth(int pixelWidth)
	{
		Call(SCI_SETCARETWIDTH, pixelWidth);
	}

	int GetCaretWidth()
	{
		return Call(SCI_GETCARETWIDTH);
	}

	void SetTargetStart(Position start)
	{
		Call(SCI_SETTARGETSTART, start);
	}

	Position GetTargetStart()
	{
		return Call(SCI_GETTARGETSTART);
	}

	void SetTargetEnd(Position end)
	{
		Call(SCI_SETTARGETEND, end);
	}

	Position GetTargetEnd()
	{
		return Call(SCI_GETTARGETEND);
	}

	void SetTargetRange(Position start, Position end)
	{
		Call(SCI_SETTARGETRANGE, start, end);
	}

	Position GetTargetText(char* text)
	{
		return Call(SCI_GETTARGETTEXT, 0, reinterpret_cast<intptr_t>(text));
	}

	void TargetFromSelection()
	{
		Call(SCI_TARGETFROMSELECTION);
	}

	void TargetWholeDocument()
	{
		Call(SCI_TARGETWHOLEDOCUMENT);
	}

	Position ReplaceTarget(Position length, const char* text)
	{
		return Call(SCI_REPLACETARGET, length, reinterpret_cast<intptr_t>(text));
	}

	Position ReplaceTargetRE(Position length, const char* text)
	{
		return Call(SCI_REPLACETARGETRE, length, reinterpret_cast<intptr_t>(text));
	}

	Position SearchInTarget(Position length, const char* text)
	{
		return Call(SCI_SEARCHINTARGET, length, reinterpret_cast<intptr_t>(text));
	}

	void SetSearchFlags(int searchFlags)
	{
		Call(SCI_SETSEARCHFLAGS, searchFlags);
	}

	int GetSearchFlags()
	{
		return Call(SCI_GETSEARCHFLAGS);
	}

	void CallTipShow(Position pos, const char* definition)
	{
		Call(SCI_CALLTIPSHOW, pos, reinterpret_cast<intptr_t>(definition));
	}

	void CallTipCancel()
	{
		Call(SCI_CALLTIPCANCEL);
	}

	bool CallTipActive()
	{
		return Call(SCI_CALLTIPACTIVE);
	}

	Position CallTipPosStart()
	{
		return Call(SCI_CALLTIPPOSSTART);
	}

	void CallTipSetPosStart(Position posStart)
	{
		Call(SCI_CALLTIPSETPOSSTART, posStart);
	}

	void CallTipSetHlt(int highlightStart, int highlightEnd)
	{
		Call(SCI_CALLTIPSETHLT, highlightStart, highlightEnd);
	}

	void CallTipSetBack(Colour back)
	{
		Call(SCI_CALLTIPSETBACK, back);
	}

	void CallTipSetFore(Colour fore)
	{
		Call(SCI_CALLTIPSETFORE, fore);
	}

	void CallTipSetForeHlt(Colour fore)
	{
		Call(SCI_CALLTIPSETFOREHLT, fore);
	}

	void CallTipUseStyle(int tabSize)
	{
		Call(SCI_CALLTIPUSESTYLE, tabSize);
	}

	void CallTipSetPosition(bool above)
	{
		Call(SCI_CALLTIPSETPOSITION, above);
	}

	Line VisibleFromDocLine(Line docLine)
	{
		return Call(SCI_VISIBLEFROMDOCLINE, docLine);
	}

	Line DocLineFromVisible(Line displayLine)
	{
		return Call(SCI_DOCLINEFROMVISIBLE, displayLine);
	}

	Line WrapCount(Line docLine)
	{
		return Call(SCI_WRAPCOUNT, docLine);
	}

	void SetFoldLevel(Line line, int level)
	{
		Call(SCI_SETFOLDLEVEL, line, level);
	}

	int GetFoldLevel(Line line)
	{
		return Call(SCI_GETFOLDLEVEL, line);
	}

	Line GetLastChild(Line line, int level)
	{
		return Call(SCI_GETLASTCHILD, line, level);
	}

	Line GetFoldParent(Line line)
	{
		return Call(SCI_GETFOLDPARENT, line);
	}

	void ShowLines(Line lineStart, Line lineEnd)
	{
		Call(SCI_SHOWLINES, lineStart, lineEnd);
	}

	void HideLines(Line lineStart, Line lineEnd)
	{
		Call(SCI_HIDELINES, lineStart, lineEnd);
	}

	bool GetLineVisible(Line line)
	{
		return Call(SCI_GETLINEVISIBLE, line);
	}

	bool GetAllLinesVisible()
	{
		return Call(SCI_GETALLLINESVISIBLE);
	}

	void SetFoldExpanded(Line line, bool expanded)
	{
		Call(SCI_SETFOLDEXPANDED, line, expanded);
	}

	bool GetFoldExpanded(Line line)
	{
		return Call(SCI_GETFOLDEXPANDED, line);
	}

	void ToggleFold(Line line)
	{
		Call(SCI_TOGGLEFOLD, line);
	}

	void ToggleFoldShowText(Line line, const char* text)
	{
		Call(SCI_TOGGLEFOLDSHOWTEXT, line, reinterpret_cast<intptr_t>(text));
	}

	void FoldDisplayTextSetStyle(int style)
	{
		Call(SCI_FOLDDISPLAYTEXTSETSTYLE, style);
	}

	int FoldDisplayTextGetStyle()
	{
		return Call(SCI_FOLDDISPLAYTEXTGETSTYLE);
	}

	void SetDefaultFoldDisplayText(const char* text)
	{
		Call(SCI_SETDEFAULTFOLDDISPLAYTEXT, 0, reinterpret_cast<intptr_t>(text));
	}

	int GetDefaultFoldDisplayText(char* text)
	{
		return Call(SCI_GETDEFAULTFOLDDISPLAYTEXT, 0, reinterpret_cast<intptr_t>(text));
	}

	void FoldLine(Line line, int action)
	{
		Call(SCI_FOLDLINE, line, action);
	}

	void FoldChildren(Line line, int action)
	{
		Call(SCI_FOLDCHILDREN, line, action);
	}

	void ExpandChildren(Line line, int level)
	{
		Call(SCI_EXPANDCHILDREN, line, level);
	}

	void FoldAll(int action)
	{
		Call(SCI_FOLDALL, action);
	}

	void EnsureVisible(Line line)
	{
		Call(SCI_ENSUREVISIBLE, line);
	}

	void SetAutomaticFold(int automaticFold)
	{
		Call(SCI_SETAUTOMATICFOLD, automaticFold);
	}

	int GetAutomaticFold()
	{
		return Call(SCI_GETAUTOMATICFOLD);
	}

	void SetFoldFlags(int flags)
	{
		Call(SCI_SETFOLDFLAGS, flags);
	}

	void EnsureVisibleEnforcePolicy(Line line)
	{
		Call(SCI_ENSUREVISIBLEENFORCEPOLICY, line);
	}

	void SetTabIndents(bool tabIndents)
	{
		Call(SCI_SETTABINDENTS, tabIndents);
	}

	bool GetTabIndents()
	{
		return Call(SCI_GETTABINDENTS);
	}

	void SetBackSpaceUnIndents(bool bsUnIndents)
	{
		Call(SCI_SETBACKSPACEUNINDENTS, bsUnIndents);
	}

	bool GetBackSpaceUnIndents()
	{
		return Call(SCI_GETBACKSPACEUNINDENTS);
	}

	void SetMouseDwellTime(int periodMilliseconds)
	{
		Call(SCI_SETMOUSEDWELLTIME, periodMilliseconds);
	}

	int GetMouseDwellTime()
	{
		return Call(SCI_GETMOUSEDWELLTIME);
	}

	Position WordStartPosition(Position pos, bool onlyWordCharacters)
	{
		return Call(SCI_WORDSTARTPOSITION, pos, onlyWordCharacters);
	}

	Position WordEndPosition(Position pos, bool onlyWordCharacters)
	{
		return Call(SCI_WORDENDPOSITION, pos, onlyWordCharacters);
	}

	bool IsRangeWord(Position start, Position end)
	{
		return Call(SCI_ISRANGEWORD, start, end);
	}

	void SetIdleStyling(int idleStyling)
	{
		Call(SCI_SETIDLESTYLING, idleStyling);
	}

	int GetIdleStyling()
	{
		return Call(SCI_GETIDLESTYLING);
	}

	void SetWrapMode(int wrapMode)
	{
		Call(SCI_SETWRAPMODE, wrapMode);
	}

	int GetWrapMode()
	{
		return Call(SCI_GETWRAPMODE);
	}

	void SetWrapVisualFlags(int wrapVisualFlags)
	{
		Call(SCI_SETWRAPVISUALFLAGS, wrapVisualFlags);
	}

	int GetWrapVisualFlags()
	{
		return Call(SCI_GETWRAPVISUALFLAGS);
	}

	void SetWrapVisualFlagsLocation(int wrapVisualFlagsLocation)
	{
		Call(SCI_SETWRAPVISUALFLAGSLOCATION, wrapVisualFlagsLocation);
	}

	int GetWrapVisualFlagsLocation()
	{
		return Call(SCI_GETWRAPVISUALFLAGSLOCATION);
	}

	void SetWrapStartIndent(int indent)
	{
		Call(SCI_SETWRAPSTARTINDENT, indent);
	}

	int GetWrapStartIndent()
	{
		return Call(SCI_GETWRAPSTARTINDENT);
	}

	void SetWrapIndentMode(int wrapIndentMode)
	{
		Call(SCI_SETWRAPINDENTMODE, wrapIndentMode);
	}

	int GetWrapIndentMode()
	{
		return Call(SCI_GETWRAPINDENTMODE);
	}

	void SetLayoutCache(int cacheMode)
	{
		Call(SCI_SETLAYOUTCACHE, cacheMode);
	}

	int GetLayoutCache()
	{
		return Call(SCI_GETLAYOUTCACHE);
	}

	void SetScrollWidth(int pixelWidth)
	{
		Call(SCI_SETSCROLLWIDTH, pixelWidth);
	}

	int GetScrollWidth()
	{
		return Call(SCI_GETSCROLLWIDTH);
	}

	void SetScrollWidthTracking(bool tracking)
	{
		Call(SCI_SETSCROLLWIDTHTRACKING, tracking);
	}

	bool GetScrollWidthTracking()
	{
		return Call(SCI_GETSCROLLWIDTHTRACKING);
	}

	int TextWidth(int style, const char* text)
	{
		return Call(SCI_TEXTWIDTH, style, reinterpret_cast<intptr_t>(text));
	}

	void SetEndAtLastLine(bool endAtLastLine)
	{
		Call(SCI_SETENDATLASTLINE, endAtLastLine);
	}

	bool GetEndAtLastLine()
	{
		return Call(SCI_GETENDATLASTLINE);
	}

	int TextHeight(Line line)
	{
		return Call(SCI_TEXTHEIGHT, line);
	}

	void SetVScrollBar(bool visible)
	{
		Call(SCI_SETVSCROLLBAR, visible);
	}

	bool GetVScrollBar()
	{
		return Call(SCI_GETVSCROLLBAR);
	}

	void AppendText(Position length, const char* text)
	{
		Call(SCI_APPENDTEXT, length, reinterpret_cast<intptr_t>(text));
	}

	int GetPhasesDraw()
	{
		return Call(SCI_GETPHASESDRAW);
	}

	void SetPhasesDraw(int phases)
	{
		Call(SCI_SETPHASESDRAW, phases);
	}

	void SetFontQuality(int fontQuality)
	{
		Call(SCI_SETFONTQUALITY, fontQuality);
	}

	int GetFontQuality()
	{
		return Call(SCI_GETFONTQUALITY);
	}

	void SetFirstVisibleLine(Line displayLine)
	{
		Call(SCI_SETFIRSTVISIBLELINE, displayLine);
	}

	void SetMultiPaste(int multiPaste)
	{
		Call(SCI_SETMULTIPASTE, multiPaste);
	}

	int GetMultiPaste()
	{
		return Call(SCI_GETMULTIPASTE);
	}

	int GetTag(int tagNumber, char* tagValue)
	{
		return Call(SCI_GETTAG, tagNumber, reinterpret_cast<intptr_t>(tagValue));
	}

	void LinesJoin()
	{
		Call(SCI_LINESJOIN);
	}

	void LinesSplit(int pixelWidth)
	{
		Call(SCI_LINESSPLIT, pixelWidth);
	}

	void SetFoldMarginColour(bool useSetting, Colour back)
	{
		Call(SCI_SETFOLDMARGINCOLOUR, useSetting, back);
	}

	void SetFoldMarginHiColour(bool useSetting, Colour fore)
	{
		Call(SCI_SETFOLDMARGINHICOLOUR, useSetting, fore);
	}

	void SetAccessibility(int accessibility)
	{
		Call(SCI_SETACCESSIBILITY, accessibility);
	}

	int GetAccessibility()
	{
		return Call(SCI_GETACCESSIBILITY);
	}

	void LineDown()
	{
		Call(SCI_LINEDOWN);
	}

	void LineDownExtend()
	{
		Call(SCI_LINEDOWNEXTEND);
	}

	void LineUp()
	{
		Call(SCI_LINEUP);
	}

	void LineUpExtend()
	{
		Call(SCI_LINEUPEXTEND);
	}

	void CharLeft()
	{
		Call(SCI_CHARLEFT);
	}

	void CharLeftExtend()
	{
		Call(SCI_CHARLEFTEXTEND);
	}

	void CharRight()
	{
		Call(SCI_CHARRIGHT);
	}

	void CharRightExtend()
	{
		Call(SCI_CHARRIGHTEXTEND);
	}

	void WordLeft()
	{
		Call(SCI_WORDLEFT);
	}

	void WordLeftExtend()
	{
		Call(SCI_WORDLEFTEXTEND);
	}

	void WordRight()
	{
		Call(SCI_WORDRIGHT);
	}

	void WordRightExtend()
	{
		Call(SCI_WORDRIGHTEXTEND);
	}

	void Home()
	{
		Call(SCI_HOME);
	}

	void HomeExtend()
	{
		Call(SCI_HOMEEXTEND);
	}

	void LineEnd()
	{
		Call(SCI_LINEEND);
	}

	void LineEndExtend()
	{
		Call(SCI_LINEENDEXTEND);
	}

	void DocumentStart()
	{
		Call(SCI_DOCUMENTSTART);
	}

	void DocumentStartExtend()
	{
		Call(SCI_DOCUMENTSTARTEXTEND);
	}

	void DocumentEnd()
	{
		Call(SCI_DOCUMENTEND);
	}

	void DocumentEndExtend()
	{
		Call(SCI_DOCUMENTENDEXTEND);
	}

	void PageUp()
	{
		Call(SCI_PAGEUP);
	}

	void PageUpExtend()
	{
		Call(SCI_PAGEUPEXTEND);
	}

	void PageDown()
	{
		Call(SCI_PAGEDOWN);
	}

	void PageDownExtend()
	{
		Call(SCI_PAGEDOWNEXTEND);
	}

	void EditToggleOvertype()
	{
		Call(SCI_EDITTOGGLEOVERTYPE);
	}

	void Cancel()
	{
		Call(SCI_CANCEL);
	}

	void DeleteBack()
	{
		Call(SCI_DELETEBACK);
	}

	void Tab()
	{
		Call(SCI_TAB);
	}

	void BackTab()
	{
		Call(SCI_BACKTAB);
	}

	void NewLine()
	{
		Call(SCI_NEWLINE);
	}

	void FormFeed()
	{
		Call(SCI_FORMFEED);
	}

	void VCHome()
	{
		Call(SCI_VCHOME);
	}

	void VCHomeExtend()
	{
		Call(SCI_VCHOMEEXTEND);
	}

	void ZoomIn()
	{
		Call(SCI_ZOOMIN);
	}

	void ZoomOut()
	{
		Call(SCI_ZOOMOUT);
	}

	void DelWordLeft()
	{
		Call(SCI_DELWORDLEFT);
	}

	void DelWordRight()
	{
		Call(SCI_DELWORDRIGHT);
	}

	void DelWordRightEnd()
	{
		Call(SCI_DELWORDRIGHTEND);
	}

	void LineCut()
	{
		Call(SCI_LINECUT);
	}

	void LineDelete()
	{
		Call(SCI_LINEDELETE);
	}

	void LineTranspose()
	{
		Call(SCI_LINETRANSPOSE);
	}

	void LineReverse()
	{
		Call(SCI_LINEREVERSE);
	}

	void LineDuplicate()
	{
		Call(SCI_LINEDUPLICATE);
	}

	void LowerCase()
	{
		Call(SCI_LOWERCASE);
	}

	void UpperCase()
	{
		Call(SCI_UPPERCASE);
	}

	void LineScrollDown()
	{
		Call(SCI_LINESCROLLDOWN);
	}

	void LineScrollUp()
	{
		Call(SCI_LINESCROLLUP);
	}

	void DeleteBackNotLine()
	{
		Call(SCI_DELETEBACKNOTLINE);
	}

	void HomeDisplay()
	{
		Call(SCI_HOMEDISPLAY);
	}

	void HomeDisplayExtend()
	{
		Call(SCI_HOMEDISPLAYEXTEND);
	}

	void LineEndDisplay()
	{
		Call(SCI_LINEENDDISPLAY);
	}

	void LineEndDisplayExtend()
	{
		Call(SCI_LINEENDDISPLAYEXTEND);
	}

	void HomeWrap()
	{
		Call(SCI_HOMEWRAP);
	}

	void HomeWrapExtend()
	{
		Call(SCI_HOMEWRAPEXTEND);
	}

	void LineEndWrap()
	{
		Call(SCI_LINEENDWRAP);
	}

	void LineEndWrapExtend()
	{
		Call(SCI_LINEENDWRAPEXTEND);
	}

	void VCHomeWrap()
	{
		Call(SCI_VCHOMEWRAP);
	}

	void VCHomeWrapExtend()
	{
		Call(SCI_VCHOMEWRAPEXTEND);
	}

	void LineCopy()
	{
		Call(SCI_LINECOPY);
	}

	void MoveCaretInsideView()
	{
		Call(SCI_MOVECARETINSIDEVIEW);
	}

	Position LineLength(Line line)
	{
		return Call(SCI_LINELENGTH, line);
	}

	void BraceHighlight(Position posA, Position posB)
	{
		Call(SCI_BRACEHIGHLIGHT, posA, posB);
	}

	void BraceHighlightIndicator(bool useSetting, int indicator)
	{
		Call(SCI_BRACEHIGHLIGHTINDICATOR, useSetting, indicator);
	}

	void BraceBadLight(Position pos)
	{
		Call(SCI_BRACEBADLIGHT, pos);
	}

	void BraceBadLightIndicator(bool useSetting, int indicator)
	{
		Call(SCI_BRACEBADLIGHTINDICATOR, useSetting, indicator);
	}

	Position BraceMatch(Position pos, int maxReStyle)
	{
		return Call(SCI_BRACEMATCH, pos, maxReStyle);
	}

	bool GetViewEOL()
	{
		return Call(SCI_GETVIEWEOL);
	}

	void SetViewEOL(bool visible)
	{
		Call(SCI_SETVIEWEOL, visible);
	}

	void* GetDocPointer()
	{
		return reinterpret_cast<void*>(Call(SCI_GETDOCPOINTER));
	}

	void SetDocPointer(void* doc)
	{
		Call(SCI_SETDOCPOINTER, 0, reinterpret_cast<intptr_t>(doc));
	}

	void SetModEventMask(int eventMask)
	{
		Call(SCI_SETMODEVENTMASK, eventMask);
	}

	Position GetEdgeColumn()
	{
		return Call(SCI_GETEDGECOLUMN);
	}

	void SetEdgeColumn(Position column)
	{
		Call(SCI_SETEDGECOLUMN, column);
	}

	int GetEdgeMode()
	{
		return Call(SCI_GETEDGEMODE);
	}

	void SetEdgeMode(int edgeMode)
	{
		Call(SCI_SETEDGEMODE, edgeMode);
	}

	Colour GetEdgeColour()
	{
		return Call(SCI_GETEDGECOLOUR);
	}

	void SetEdgeColour(Colour edgeColour)
	{
		Call(SCI_SETEDGECOLOUR, edgeColour);
	}

	void MultiEdgeAddLine(Position column, Colour edgeColour)
	{
		Call(SCI_MULTIEDGEADDLINE, column, edgeColour);
	}

	void MultiEdgeClearAll()
	{
		Call(SCI_MULTIEDGECLEARALL);
	}

	void SearchAnchor()
	{
		Call(SCI_SEARCHANCHOR);
	}

	Position SearchNext(int searchFlags, const char* text)
	{
		return Call(SCI_SEARCHNEXT, searchFlags, reinterpret_cast<intptr_t>(text));
	}

	Position SearchPrev(int searchFlags, const char* text)
	{
		return Call(SCI_SEARCHPREV, searchFlags, reinterpret_cast<intptr_t>(text));
	}

	Line LinesOnScreen()
	{
		return Call(SCI_LINESONSCREEN);
	}

	void UsePopUp(int popUpMode)
	{
		Call(SCI_USEPOPUP, popUpMode);
	}

	bool SelectionIsRectangle()
	{
		return Call(SCI_SELECTIONISRECTANGLE);
	}

	void SetZoom(int zoomInPoints)
	{
		Call(SCI_SETZOOM, zoomInPoints);
	}

	int GetZoom()
	{
		return Call(SCI_GETZOOM);
	}

	void* CreateDocument(Position bytes, int documentOptions)
	{
		return reinterpret_cast<void*>(Call(SCI_CREATEDOCUMENT, bytes, documentOptions));
	}

	void AddRefDocument(void* doc)
	{
		Call(SCI_ADDREFDOCUMENT, 0, reinterpret_cast<intptr_t>(doc));
	}

	void ReleaseDocument(void* doc)
	{
		Call(SCI_RELEASEDOCUMENT, 0, reinterpret_cast<intptr_t>(doc));
	}

	int GetDocumentOptions()
	{
		return Call(SCI_GETDOCUMENTOPTIONS);
	}

	int GetModEventMask()
	{
		return Call(SCI_GETMODEVENTMASK);
	}

	void SetCommandEvents(bool commandEvents)
	{
		Call(SCI_SETCOMMANDEVENTS, commandEvents);
	}

	bool GetCommandEvents()
	{
		return Call(SCI_GETCOMMANDEVENTS);
	}

	void SetFocus(bool focus)
	{
		Call(SCI_SETFOCUS, focus);
	}

	bool GetFocus()
	{
		return Call(SCI_GETFOCUS);
	}

	void SetStatus(int status)
	{
		Call(SCI_SETSTATUS, status);
	}

	int GetStatus()
	{
		return Call(SCI_GETSTATUS);
	}

	void SetMouseDownCaptures(bool captures)
	{
		Call(SCI_SETMOUSEDOWNCAPTURES, captures);
	}

	bool GetMouseDownCaptures()
	{
		return Call(SCI_GETMOUSEDOWNCAPTURES);
	}

	void SetMouseWheelCaptures(bool captures)
	{
		Call(SCI_SETMOUSEWHEELCAPTURES, captures);
	}

	bool GetMouseWheelCaptures()
	{
		return Call(SCI_GETMOUSEWHEELCAPTURES);
	}

	void SetCursor(int cursorType)
	{
		Call(SCI_SETCURSOR, cursorType);
	}

	int GetCursor()
	{
		return Call(SCI_GETCURSOR);
	}

	void SetControlCharSymbol(int symbol)
	{
		Call(SCI_SETCONTROLCHARSYMBOL, symbol);
	}

	int GetControlCharSymbol()
	{
		return Call(SCI_GETCONTROLCHARSYMBOL);
	}

	void WordPartLeft()
	{
		Call(SCI_WORDPARTLEFT);
	}

	void WordPartLeftExtend()
	{
		Call(SCI_WORDPARTLEFTEXTEND);
	}

	void WordPartRight()
	{
		Call(SCI_WORDPARTRIGHT);
	}

	void WordPartRightExtend()
	{
		Call(SCI_WORDPARTRIGHTEXTEND);
	}

	void SetVisiblePolicy(int visiblePolicy, int visibleSlop)
	{
		Call(SCI_SETVISIBLEPOLICY, visiblePolicy, visibleSlop);
	}

	void DelLineLeft()
	{
		Call(SCI_DELLINELEFT);
	}

	void DelLineRight()
	{
		Call(SCI_DELLINERIGHT);
	}

	void SetXOffset(int xOffset)
	{
		Call(SCI_SETXOFFSET, xOffset);
	}

	int GetXOffset()
	{
		return Call(SCI_GETXOFFSET);
	}

	void ChooseCaretX()
	{
		Call(SCI_CHOOSECARETX);
	}

	void GrabFocus()
	{
		Call(SCI_GRABFOCUS);
	}

	void SetXCaretPolicy(int caretPolicy, int caretSlop)
	{
		Call(SCI_SETXCARETPOLICY, caretPolicy, caretSlop);
	}

	void SetYCaretPolicy(int caretPolicy, int caretSlop)
	{
		Call(SCI_SETYCARETPOLICY, caretPolicy, caretSlop);
	}

	void SetPrintWrapMode(int wrapMode)
	{
		Call(SCI_SETPRINTWRAPMODE, wrapMode);
	}

	int GetPrintWrapMode()
	{
		return Call(SCI_GETPRINTWRAPMODE);
	}

	void SetHotspotActiveFore(bool useSetting, Colour fore)
	{
		Call(SCI_SETHOTSPOTACTIVEFORE, useSetting, fore);
	}

	Colour GetHotspotActiveFore()
	{
		return Call(SCI_GETHOTSPOTACTIVEFORE);
	}

	void SetHotspotActiveBack(bool useSetting, Colour back)
	{
		Call(SCI_SETHOTSPOTACTIVEBACK, useSetting, back);
	}

	Colour GetHotspotActiveBack()
	{
		return Call(SCI_GETHOTSPOTACTIVEBACK);
	}

	void SetHotspotActiveUnderline(bool underline)
	{
		Call(SCI_SETHOTSPOTACTIVEUNDERLINE, underline);
	}

	bool GetHotspotActiveUnderline()
	{
		return Call(SCI_GETHOTSPOTACTIVEUNDERLINE);
	}

	void SetHotspotSingleLine(bool singleLine)
	{
		Call(SCI_SETHOTSPOTSINGLELINE, singleLine);
	}

	bool GetHotspotSingleLine()
	{
		return Call(SCI_GETHOTSPOTSINGLELINE);
	}

	void ParaDown()
	{
		Call(SCI_PARADOWN);
	}

	void ParaDownExtend()
	{
		Call(SCI_PARADOWNEXTEND);
	}

	void ParaUp()
	{
		Call(SCI_PARAUP);
	}

	void ParaUpExtend()
	{
		Call(SCI_PARAUPEXTEND);
	}

	Position PositionBefore(Position pos)
	{
		return Call(SCI_POSITIONBEFORE, pos);
	}

	Position PositionAfter(Position pos)
	{
		return Call(SCI_POSITIONAFTER, pos);
	}

	Position PositionRelative(Position pos, Position relative)
	{
		return Call(SCI_POSITIONRELATIVE, pos, relative);
	}

	Position PositionRelativeCodeUnits(Position pos, Position relative)
	{
		return Call(SCI_POSITIONRELATIVECODEUNITS, pos, relative);
	}

	void CopyRange(Position start, Position end)
	{
		Call(SCI_COPYRANGE, start, end);
	}

	void CopyText(Position length, const char* text)
	{
		Call(SCI_COPYTEXT, length, reinterpret_cast<intptr_t>(text));
	}

	void SetSelectionMode(int selectionMode)
	{
		Call(SCI_SETSELECTIONMODE, selectionMode);
	}

	int GetSelectionMode()
	{
		return Call(SCI_GETSELECTIONMODE);
	}

	bool GetMoveExtendsSelection()
	{
		return Call(SCI_GETMOVEEXTENDSSELECTION);
	}

	Position GetLineSelStartPosition(Line line)
	{
		return Call(SCI_GETLINESELSTARTPOSITION, line);
	}

	Position GetLineSelEndPosition(Line line)
	{
		return Call(SCI_GETLINESELENDPOSITION, line);
	}

	void LineDownRectExtend()
	{
		Call(SCI_LINEDOWNRECTEXTEND);
	}

	void LineUpRectExtend()
	{
		Call(SCI_LINEUPRECTEXTEND);
	}

	void CharLeftRectExtend()
	{
		Call(SCI_CHARLEFTRECTEXTEND);
	}

	void CharRightRectExtend()
	{
		Call(SCI_CHARRIGHTRECTEXTEND);
	}

	void HomeRectExtend()
	{
		Call(SCI_HOMERECTEXTEND);
	}

	void VCHomeRectExtend()
	{
		Call(SCI_VCHOMERECTEXTEND);
	}

	void LineEndRectExtend()
	{
		Call(SCI_LINEENDRECTEXTEND);
	}

	void PageUpRectExtend()
	{
		Call(SCI_PAGEUPRECTEXTEND);
	}

	void PageDownRectExtend()
	{
		Call(SCI_PAGEDOWNRECTEXTEND);
	}

	void StutteredPageUp()
	{
		Call(SCI_STUTTEREDPAGEUP);
	}

	void StutteredPageUpExtend()
	{
		Call(SCI_STUTTEREDPAGEUPEXTEND);
	}

	void StutteredPageDown()
	{
		Call(SCI_STUTTEREDPAGEDOWN);
	}

	void StutteredPageDownExtend()
	{
		Call(SCI_STUTTEREDPAGEDOWNEXTEND);
	}

	void WordLeftEnd()
	{
		Call(SCI_WORDLEFTEND);
	}

	void WordLeftEndExtend()
	{
		Call(SCI_WORDLEFTENDEXTEND);
	}

	void WordRightEnd()
	{
		Call(SCI_WORDRIGHTEND);
	}

	void WordRightEndExtend()
	{
		Call(SCI_WORDRIGHTENDEXTEND);
	}

	void SetWhitespaceChars(const char* characters)
	{
		Call(SCI_SETWHITESPACECHARS, 0, reinterpret_cast<intptr_t>(characters));
	}

	int GetWhitespaceChars(char* characters)
	{
		return Call(SCI_GETWHITESPACECHARS, 0, reinterpret_cast<intptr_t>(characters));
	}

	void SetPunctuationChars(const char* characters)
	{
		Call(SCI_SETPUNCTUATIONCHARS, 0, reinterpret_cast<intptr_t>(characters));
	}

	int GetPunctuationChars(char* characters)
	{
		return Call(SCI_GETPUNCTUATIONCHARS, 0, reinterpret_cast<intptr_t>(characters));
	}

	void SetCharsDefault()
	{
		Call(SCI_SETCHARSDEFAULT);
	}

	int AutoCGetCurrent()
	{
		return Call(SCI_AUTOCGETCURRENT);
	}

	int AutoCGetCurrentText(char* text)
	{
		return Call(SCI_AUTOCGETCURRENTTEXT, 0, reinterpret_cast<intptr_t>(text));
	}

	void AutoCSetCaseInsensitiveBehaviour(int behaviour)
	{
		Call(SCI_AUTOCSETCASEINSENSITIVEBEHAVIOUR, behaviour);
	}

	int AutoCGetCaseInsensitiveBehaviour()
	{
		return Call(SCI_AUTOCGETCASEINSENSITIVEBEHAVIOUR);
	}

	void AutoCSetMulti(int multi)
	{
		Call(SCI_AUTOCSETMULTI, multi);
	}

	int AutoCGetMulti()
	{
		return Call(SCI_AUTOCGETMULTI);
	}

	void AutoCSetOrder(int order)
	{
		Call(SCI_AUTOCSETORDER, order);
	}

	int AutoCGetOrder()
	{
		return Call(SCI_AUTOCGETORDER);
	}

	void Allocate(Position bytes)
	{
		Call(SCI_ALLOCATE, bytes);
	}

	Position TargetAsUTF8(char* s)
	{
		return Call(SCI_TARGETASUTF8, 0, reinterpret_cast<intptr_t>(s));
	}

	void SetLengthForEncode(Position bytes)
	{
		Call(SCI_SETLENGTHFORENCODE, bytes);
	}

	Position EncodedFromUTF8(const char* utf8, char* encoded)
	{
		return Call(SCI_ENCODEDFROMUTF8, reinterpret_cast<uintptr_t>(utf8), reinterpret_cast<intptr_t>(encoded));
	}

	Position FindColumn(Line line, Position column)
	{
		return Call(SCI_FINDCOLUMN, line, column);
	}

	int GetCaretSticky()
	{
		return Call(SCI_GETCARETSTICKY);
	}

	void SetCaretSticky(int useCaretStickyBehaviour)
	{
		Call(SCI_SETCARETSTICKY, useCaretStickyBehaviour);
	}

	void ToggleCaretSticky()
	{
		Call(SCI_TOGGLECARETSTICKY);
	}

	void SetPasteConvertEndings(bool convert)
	{
		Call(SCI_SETPASTECONVERTENDINGS, convert);
	}

	bool GetPasteConvertEndings()
	{
		return Call(SCI_GETPASTECONVERTENDINGS);
	}

	void SelectionDuplicate()
	{
		Call(SCI_SELECTIONDUPLICATE);
	}

	void SetCaretLineBackAlpha(int alpha)
	{
		Call(SCI_SETCARETLINEBACKALPHA, alpha);
	}

	int GetCaretLineBackAlpha()
	{
		return Call(SCI_GETCARETLINEBACKALPHA);
	}

	void SetCaretStyle(int caretStyle)
	{
		Call(SCI_SETCARETSTYLE, caretStyle);
	}

	int GetCaretStyle()
	{
		return Call(SCI_GETCARETSTYLE);
	}

	void SetIndicatorCurrent(int indicator)
	{
		Call(SCI_SETINDICATORCURRENT, indicator);
	}

	int GetIndicatorCurrent()
	{
		return Call(SCI_GETINDICATORCURRENT);
	}

	void SetIndicatorValue(int value)
	{
		Call(SCI_SETINDICATORVALUE, value);
	}

	int GetIndicatorValue()
	{
		return Call(SCI_GETINDICATORVALUE);
	}

	void IndicatorFillRange(Position start, Position lengthFill)
	{
		Call(SCI_INDICATORFILLRANGE, start, lengthFill);
	}

	void IndicatorClearRange(Position start, Position lengthClear)
	{
		Call(SCI_INDICATORCLEARRANGE, start, lengthClear);
	}

	int IndicatorAllOnFor(Position pos)
	{
		return Call(SCI_INDICATORALLONFOR, pos);
	}

	int IndicatorValueAt(int indicator, Position pos)
	{
		return Call(SCI_INDICATORVALUEAT, indicator, pos);
	}

	int IndicatorStart(int indicator, Position pos)
	{
		return Call(SCI_INDICATORSTART, indicator, pos);
	}

	int IndicatorEnd(int indicator, Position pos)
	{
		return Call(SCI_INDICATOREND, indicator, pos);
	}

	void SetPositionCache(int size)
	{
		Call(SCI_SETPOSITIONCACHE, size);
	}

	int GetPositionCache()
	{
		return Call(SCI_GETPOSITIONCACHE);
	}

	void CopyAllowLine()
	{
		Call(SCI_COPYALLOWLINE);
	}

	void* GetCharacterPointer()
	{
		return reinterpret_cast<void*>(Call(SCI_GETCHARACTERPOINTER));
	}

	void* GetRangePointer(Position start, Position lengthRange)
	{
		return reinterpret_cast<void*>(Call(SCI_GETRANGEPOINTER, start, lengthRange));
	}

	Position GetGapPosition()
	{
		return Call(SCI_GETGAPPOSITION);
	}

	void IndicSetAlpha(int indicator, int alpha)
	{
		Call(SCI_INDICSETALPHA, indicator, alpha);
	}

	int IndicGetAlpha(int indicator)
	{
		return Call(SCI_INDICGETALPHA, indicator);
	}

	void IndicSetOutlineAlpha(int indicator, int alpha)
	{
		Call(SCI_INDICSETOUTLINEALPHA, indicator, alpha);
	}

	int IndicGetOutlineAlpha(int indicator)
	{
		return Call(SCI_INDICGETOUTLINEALPHA, indicator);
	}

	void SetExtraAscent(int extraAscent)
	{
		Call(SCI_SETEXTRAASCENT, extraAscent);
	}

	int GetExtraAscent()
	{
		return Call(SCI_GETEXTRAASCENT);
	}

	void SetExtraDescent(int extraDescent)
	{
		Call(SCI_SETEXTRADESCENT, extraDescent);
	}

	int GetExtraDescent()
	{
		return Call(SCI_GETEXTRADESCENT);
	}

	int MarkerSymbolDefined(int markerNumber)
	{
		return Call(SCI_MARKERSYMBOLDEFINED, markerNumber);
	}

	void MarginSetText(Line line, const char* text)
	{
		Call(SCI_MARGINSETTEXT, line, reinterpret_cast<intptr_t>(text));
	}

	int MarginGetText(Line line, char* text)
	{
		return Call(SCI_MARGINGETTEXT, line, reinterpret_cast<intptr_t>(text));
	}

	void MarginSetStyle(Line line, int style)
	{
		Call(SCI_MARGINSETSTYLE, line, style);
	}

	int MarginGetStyle(Line line)
	{
		return Call(SCI_MARGINGETSTYLE, line);
	}

	void MarginSetStyles(Line line, const char* styles)
	{
		Call(SCI_MARGINSETSTYLES, line, reinterpret_cast<intptr_t>(styles));
	}

	int MarginGetStyles(Line line, char* styles)
	{
		return Call(SCI_MARGINGETSTYLES, line, reinterpret_cast<intptr_t>(styles));
	}

	void MarginTextClearAll()
	{
		Call(SCI_MARGINTEXTCLEARALL);
	}

	void MarginSetStyleOffset(int style)
	{
		Call(SCI_MARGINSETSTYLEOFFSET, style);
	}

	int MarginGetStyleOffset()
	{
		return Call(SCI_MARGINGETSTYLEOFFSET);
	}

	void SetMarginOptions(int marginOptions)
	{
		Call(SCI_SETMARGINOPTIONS, marginOptions);
	}

	int GetMarginOptions()
	{
		return Call(SCI_GETMARGINOPTIONS);
	}

	void AnnotationSetText(Line line, const char* text)
	{
		Call(SCI_ANNOTATIONSETTEXT, line, reinterpret_cast<intptr_t>(text));
	}

	int AnnotationGetText(Line line, char* text)
	{
		return Call(SCI_ANNOTATIONGETTEXT, line, reinterpret_cast<intptr_t>(text));
	}

	void AnnotationSetStyle(Line line, int style)
	{
		Call(SCI_ANNOTATIONSETSTYLE, line, style);
	}

	int AnnotationGetStyle(Line line)
	{
		return Call(SCI_ANNOTATIONGETSTYLE, line);
	}

	void AnnotationSetStyles(Line line, const char* styles)
	{
		Call(SCI_ANNOTATIONSETSTYLES, line, reinterpret_cast<intptr_t>(styles));
	}

	int AnnotationGetStyles(Line line, char* styles)
	{
		return Call(SCI_ANNOTATIONGETSTYLES, line, reinterpret_cast<intptr_t>(styles));
	}

	int AnnotationGetLines(Line line)
	{
		return Call(SCI_ANNOTATIONGETLINES, line);
	}

	void AnnotationClearAll()
	{
		Call(SCI_ANNOTATIONCLEARALL);
	}

	void AnnotationSetVisible(int visible)
	{
		Call(SCI_ANNOTATIONSETVISIBLE, visible);
	}

	int AnnotationGetVisible()
	{
		return Call(SCI_ANNOTATIONGETVISIBLE);
	}

	void AnnotationSetStyleOffset(int style)
	{
		Call(SCI_ANNOTATIONSETSTYLEOFFSET, style);
	}

	int AnnotationGetStyleOffset()
	{
		return Call(SCI_ANNOTATIONGETSTYLEOFFSET);
	}

	void ReleaseAllExtendedStyles()
	{
		Call(SCI_RELEASEALLEXTENDEDSTYLES);
	}

	int AllocateExtendedStyles(int numberStyles)
	{
		return Call(SCI_ALLOCATEEXTENDEDSTYLES, numberStyles);
	}

	void AddUndoAction(int token, int flags)
	{
		Call(SCI_ADDUNDOACTION, token, flags);
	}

	Position CharPositionFromPoint(int x, int y)
	{
		return Call(SCI_CHARPOSITIONFROMPOINT, x, y);
	}

	Position CharPositionFromPointClose(int x, int y)
	{
		return Call(SCI_CHARPOSITIONFROMPOINTCLOSE, x, y);
	}

	void SetMouseSelectionRectangularSwitch(bool mouseSelectionRectangularSwitch)
	{
		Call(SCI_SETMOUSESELECTIONRECTANGULARSWITCH, mouseSelectionRectangularSwitch);
	}

	bool GetMouseSelectionRectangularSwitch()
	{
		return Call(SCI_GETMOUSESELECTIONRECTANGULARSWITCH);
	}

	void SetMultipleSelection(bool multipleSelection)
	{
		Call(SCI_SETMULTIPLESELECTION, multipleSelection);
	}

	bool GetMultipleSelection()
	{
		return Call(SCI_GETMULTIPLESELECTION);
	}

	void SetAdditionalSelectionTyping(bool additionalSelectionTyping)
	{
		Call(SCI_SETADDITIONALSELECTIONTYPING, additionalSelectionTyping);
	}

	bool GetAdditionalSelectionTyping()
	{
		return Call(SCI_GETADDITIONALSELECTIONTYPING);
	}

	void SetAdditionalCaretsBlink(bool additionalCaretsBlink)
	{
		Call(SCI_SETADDITIONALCARETSBLINK, additionalCaretsBlink);
	}

	bool GetAdditionalCaretsBlink()
	{
		return Call(SCI_GETADDITIONALCARETSBLINK);
	}

	void SetAdditionalCaretsVisible(bool additionalCaretsVisible)
	{
		Call(SCI_SETADDITIONALCARETSVISIBLE, additionalCaretsVisible);
	}

	bool GetAdditionalCaretsVisible()
	{
		return Call(SCI_GETADDITIONALCARETSVISIBLE);
	}

	int GetSelections()
	{
		return Call(SCI_GETSELECTIONS);
	}

	bool GetSelectionEmpty()
	{
		return Call(SCI_GETSELECTIONEMPTY);
	}

	void ClearSelections()
	{
		Call(SCI_CLEARSELECTIONS);
	}

	void SetSelection(Position caret, Position anchor)
	{
		Call(SCI_SETSELECTION, caret, anchor);
	}

	void AddSelection(Position caret, Position anchor)
	{
		Call(SCI_ADDSELECTION, caret, anchor);
	}

	void DropSelectionN(int selection)
	{
		Call(SCI_DROPSELECTIONN, selection);
	}

	void SetMainSelection(int selection)
	{
		Call(SCI_SETMAINSELECTION, selection);
	}

	int GetMainSelection()
	{
		return Call(SCI_GETMAINSELECTION);
	}

	void SetSelectionNCaret(int selection, Position caret)
	{
		Call(SCI_SETSELECTIONNCARET, selection, caret);
	}

	Position GetSelectionNCaret(int selection)
	{
		return Call(SCI_GETSELECTIONNCARET, selection);
	}

	void SetSelectionNAnchor(int selection, Position anchor)
	{
		Call(SCI_SETSELECTIONNANCHOR, selection, anchor);
	}

	Position GetSelectionNAnchor(int selection)
	{
		return Call(SCI_GETSELECTIONNANCHOR, selection);
	}

	void SetSelectionNCaretVirtualSpace(int selection, Position space)
	{
		Call(SCI_SETSELECTIONNCARETVIRTUALSPACE, selection, space);
	}

	Position GetSelectionNCaretVirtualSpace(int selection)
	{
		return Call(SCI_GETSELECTIONNCARETVIRTUALSPACE, selection);
	}

	void SetSelectionNAnchorVirtualSpace(int selection, Position space)
	{
		Call(SCI_SETSELECTIONNANCHORVIRTUALSPACE, selection, space);
	}

	Position GetSelectionNAnchorVirtualSpace(int selection)
	{
		return Call(SCI_GETSELECTIONNANCHORVIRTUALSPACE, selection);
	}

	void SetSelectionNStart(int selection, Position anchor)
	{
		Call(SCI_SETSELECTIONNSTART, selection, anchor);
	}

	Position GetSelectionNStart(int selection)
	{
		return Call(SCI_GETSELECTIONNSTART, selection);
	}

	void SetSelectionNEnd(int selection, Position caret)
	{
		Call(SCI_SETSELECTIONNEND, selection, caret);
	}

	Position GetSelectionNEnd(int selection)
	{
		return Call(SCI_GETSELECTIONNEND, selection);
	}

	void SetRectangularSelectionCaret(Position caret)
	{
		Call(SCI_SETRECTANGULARSELECTIONCARET, caret);
	}

	Position GetRectangularSelectionCaret()
	{
		return Call(SCI_GETRECTANGULARSELECTIONCARET);
	}

	void SetRectangularSelectionAnchor(Position anchor)
	{
		Call(SCI_SETRECTANGULARSELECTIONANCHOR, anchor);
	}

	Position GetRectangularSelectionAnchor()
	{
		return Call(SCI_GETRECTANGULARSELECTIONANCHOR);
	}

	void SetRectangularSelectionCaretVirtualSpace(Position space)
	{
		Call(SCI_SETRECTANGULARSELECTIONCARETVIRTUALSPACE, space);
	}

	Position GetRectangularSelectionCaretVirtualSpace()
	{
		return Call(SCI_GETRECTANGULARSELECTIONCARETVIRTUALSPACE);
	}

	void SetRectangularSelectionAnchorVirtualSpace(Position space)
	{
		Call(SCI_SETRECTANGULARSELECTIONANCHORVIRTUALSPACE, space);
	}

	Position GetRectangularSelectionAnchorVirtualSpace()
	{
		return Call(SCI_GETRECTANGULARSELECTIONANCHORVIRTUALSPACE);
	}

	void SetVirtualSpaceOptions(int virtualSpaceOptions)
	{
		Call(SCI_SETVIRTUALSPACEOPTIONS, virtualSpaceOptions);
	}

	int GetVirtualSpaceOptions()
	{
		return Call(SCI_GETVIRTUALSPACEOPTIONS);
	}

	void SetRectangularSelectionModifier(int modifier)
	{
		Call(SCI_SETRECTANGULARSELECTIONMODIFIER, modifier);
	}

	int GetRectangularSelectionModifier()
	{
		return Call(SCI_GETRECTANGULARSELECTIONMODIFIER);
	}

	void SetAdditionalSelFore(Colour fore)
	{
		Call(SCI_SETADDITIONALSELFORE, fore);
	}

	void SetAdditionalSelBack(Colour back)
	{
		Call(SCI_SETADDITIONALSELBACK, back);
	}

	void SetAdditionalSelAlpha(int alpha)
	{
		Call(SCI_SETADDITIONALSELALPHA, alpha);
	}

	int GetAdditionalSelAlpha()
	{
		return Call(SCI_GETADDITIONALSELALPHA);
	}

	void SetAdditionalCaretFore(Colour fore)
	{
		Call(SCI_SETADDITIONALCARETFORE, fore);
	}

	Colour GetAdditionalCaretFore()
	{
		return Call(SCI_GETADDITIONALCARETFORE);
	}

	void RotateSelection()
	{
		Call(SCI_ROTATESELECTION);
	}

	void SwapMainAnchorCaret()
	{
		Call(SCI_SWAPMAINANCHORCARET);
	}

	void MultipleSelectAddNext()
	{
		Call(SCI_MULTIPLESELECTADDNEXT);
	}

	void MultipleSelectAddEach()
	{
		Call(SCI_MULTIPLESELECTADDEACH);
	}

	int ChangeLexerState(Position start, Position end)
	{
		return Call(SCI_CHANGELEXERSTATE, start, end);
	}

	Line ContractedFoldNext(Line lineStart)
	{
		return Call(SCI_CONTRACTEDFOLDNEXT, lineStart);
	}

	void VerticalCentreCaret()
	{
		Call(SCI_VERTICALCENTRECARET);
	}

	void MoveSelectedLinesUp()
	{
		Call(SCI_MOVESELECTEDLINESUP);
	}

	void MoveSelectedLinesDown()
	{
		Call(SCI_MOVESELECTEDLINESDOWN);
	}

	void SetIdentifier(int identifier)
	{
		Call(SCI_SETIDENTIFIER, identifier);
	}

	int GetIdentifier()
	{
		return Call(SCI_GETIDENTIFIER);
	}

	void RGBAImageSetWidth(int width)
	{
		Call(SCI_RGBAIMAGESETWIDTH, width);
	}

	void RGBAImageSetHeight(int height)
	{
		Call(SCI_RGBAIMAGESETHEIGHT, height);
	}

	void RGBAImageSetScale(int scalePercent)
	{
		Call(SCI_RGBAIMAGESETSCALE, scalePercent);
	}

	void MarkerDefineRGBAImage(int markerNumber, const char* pixels)
	{
		Call(SCI_MARKERDEFINERGBAIMAGE, markerNumber, reinterpret_cast<intptr_t>(pixels));
	}

	void RegisterRGBAImage(int type, const char* pixels)
	{
		Call(SCI_REGISTERRGBAIMAGE, type, reinterpret_cast<intptr_t>(pixels));
	}

	void ScrollToStart()
	{
		Call(SCI_SCROLLTOSTART);
	}

	void ScrollToEnd()
	{
		Call(SCI_SCROLLTOEND);
	}

	void SetTechnology(int technology)
	{
		Call(SCI_SETTECHNOLOGY, technology);
	}

	int GetTechnology()
	{
		return Call(SCI_GETTECHNOLOGY);
	}

	void* CreateLoader(Position bytes, int documentOptions)
	{
		return reinterpret_cast<void*>(Call(SCI_CREATELOADER, bytes, documentOptions));
	}

	void FindIndicatorShow(Position start, Position end)
	{
		Call(SCI_FINDINDICATORSHOW, start, end);
	}

	void FindIndicatorFlash(Position start, Position end)
	{
		Call(SCI_FINDINDICATORFLASH, start, end);
	}

	void FindIndicatorHide()
	{
		Call(SCI_FINDINDICATORHIDE);
	}

	void VCHomeDisplay()
	{
		Call(SCI_VCHOMEDISPLAY);
	}

	void VCHomeDisplayExtend()
	{
		Call(SCI_VCHOMEDISPLAYEXTEND);
	}

	bool GetCaretLineVisibleAlways()
	{
		return Call(SCI_GETCARETLINEVISIBLEALWAYS);
	}

	void SetCaretLineVisibleAlways(bool alwaysVisible)
	{
		Call(SCI_SETCARETLINEVISIBLEALWAYS, alwaysVisible);
	}

	void SetLineEndTypesAllowed(int lineEndBitSet)
	{
		Call(SCI_SETLINEENDTYPESALLOWED, lineEndBitSet);
	}

	int GetLineEndTypesAllowed()
	{
		return Call(SCI_GETLINEENDTYPESALLOWED);
	}

	int GetLineEndTypesActive()
	{
		return Call(SCI_GETLINEENDTYPESACTIVE);
	}

	void SetRepresentation(const char* encodedCharacter, const char* representation)
	{
		Call(SCI_SETREPRESENTATION, reinterpret_cast<uintptr_t>(encodedCharacter), reinterpret_cast<intptr_t>(representation));
	}

	int GetRepresentation(const char* encodedCharacter, char* representation)
	{
		return Call(SCI_GETREPRESENTATION, reinterpret_cast<uintptr_t>(encodedCharacter), reinterpret_cast<intptr_t>(representation));
	}

	void ClearRepresentation(const char* encodedCharacter)
	{
		Call(SCI_CLEARREPRESENTATION, reinterpret_cast<uintptr_t>(encodedCharacter));
	}

	void StartRecord()
	{
		Call(SCI_STARTRECORD);
	}

	void StopRecord()
	{
		Call(SCI_STOPRECORD);
	}

	void SetLexer(int lexer)
	{
		Call(SCI_SETLEXER, lexer);
	}

	int GetLexer()
	{
		return Call(SCI_GETLEXER);
	}

	void Colourise(Position start, Position end)
	{
		Call(SCI_COLOURISE, start, end);
	}

	void SetProperty(const char* key, const char* value)
	{
		Call(SCI_SETPROPERTY, reinterpret_cast<uintptr_t>(key), reinterpret_cast<intptr_t>(value));
	}

	void SetKeyWords(int keyWordSet, const char* keyWords)
	{
		Call(SCI_SETKEYWORDS, keyWordSet, reinterpret_cast<intptr_t>(keyWords));
	}

	void SetLexerLanguage(const char* language)
	{
		Call(SCI_SETLEXERLANGUAGE, 0, reinterpret_cast<intptr_t>(language));
	}

	void LoadLexerLibrary(const char* path)
	{
		Call(SCI_LOADLEXERLIBRARY, 0, reinterpret_cast<intptr_t>(path));
	}

	int GetProperty(const char* key, char* value)
	{
		return Call(SCI_GETPROPERTY, reinterpret_cast<uintptr_t>(key), reinterpret_cast<intptr_t>(value));
	}

	int GetPropertyExpanded(const char* key, char* value)
	{
		return Call(SCI_GETPROPERTYEXPANDED, reinterpret_cast<uintptr_t>(key), reinterpret_cast<intptr_t>(value));
	}

	int GetPropertyInt(const char* key, int defaultValue)
	{
		return Call(SCI_GETPROPERTYINT, reinterpret_cast<uintptr_t>(key), defaultValue);
	}

	int GetLexerLanguage(char* language)
	{
		return Call(SCI_GETLEXERLANGUAGE, 0, reinterpret_cast<intptr_t>(language));
	}

	void* PrivateLexerCall(int operation, void* pointer)
	{
		return reinterpret_cast<void*>(Call(SCI_PRIVATELEXERCALL, operation, reinterpret_cast<intptr_t>(pointer)));
	}

	int PropertyNames(char* names)
	{
		return Call(SCI_PROPERTYNAMES, 0, reinterpret_cast<intptr_t>(names));
	}

	int PropertyType(const char* name)
	{
		return Call(SCI_PROPERTYTYPE, reinterpret_cast<uintptr_t>(name));
	}

	int DescribeProperty(const char* name, char* description)
	{
		return Call(SCI_DESCRIBEPROPERTY, reinterpret_cast<uintptr_t>(name), reinterpret_cast<intptr_t>(description));
	}

	int DescribeKeyWordSets(char* descriptions)
	{
		return Call(SCI_DESCRIBEKEYWORDSETS, 0, reinterpret_cast<intptr_t>(descriptions));
	}

	int GetLineEndTypesSupported()
	{
		return Call(SCI_GETLINEENDTYPESSUPPORTED);
	}

	int AllocateSubStyles(int styleBase, int numberStyles)
	{
		return Call(SCI_ALLOCATESUBSTYLES, styleBase, numberStyles);
	}

	int GetSubStylesStart(int styleBase)
	{
		return Call(SCI_GETSUBSTYLESSTART, styleBase);
	}

	int GetSubStylesLength(int styleBase)
	{
		return Call(SCI_GETSUBSTYLESLENGTH, styleBase);
	}

	int GetStyleFromSubStyle(int subStyle)
	{
		return Call(SCI_GETSTYLEFROMSUBSTYLE, subStyle);
	}

	int GetPrimaryStyleFromStyle(int style)
	{
		return Call(SCI_GETPRIMARYSTYLEFROMSTYLE, style);
	}

	void FreeSubStyles()
	{
		Call(SCI_FREESUBSTYLES);
	}

	void SetIdentifiers(int style, const char* identifiers)
	{
		Call(SCI_SETIDENTIFIERS, style, reinterpret_cast<intptr_t>(identifiers));
	}

	int DistanceToSecondaryStyles()
	{
		return Call(SCI_DISTANCETOSECONDARYSTYLES);
	}

	int GetSubStyleBases(char* styles)
	{
		return Call(SCI_GETSUBSTYLEBASES, 0, reinterpret_cast<intptr_t>(styles));
	}

	int GetNamedStyles()
	{
		return Call(SCI_GETNAMEDSTYLES);
	}

	int NameOfStyle(int style, char* name)
	{
		return Call(SCI_NAMEOFSTYLE, style, reinterpret_cast<intptr_t>(name));
	}

	int TagsOfStyle(int style, char* tags)
	{
		return Call(SCI_TAGSOFSTYLE, style, reinterpret_cast<intptr_t>(tags));
	}

	int DescriptionOfStyle(int style, char* description)
	{
		return Call(SCI_DESCRIPTIONOFSTYLE, style, reinterpret_cast<intptr_t>(description));
	}

	int GetBidirectional()
	{
		return Call(SCI_GETBIDIRECTIONAL);
	}

	void SetBidirectional(int bidirectional)
	{
		Call(SCI_SETBIDIRECTIONAL, bidirectional);
	}

	int GetLineCharacterIndex()
	{
		return Call(SCI_GETLINECHARACTERINDEX);
	}

	void AllocateLineCharacterIndex(int lineCharacterIndex)
	{
		Call(SCI_ALLOCATELINECHARACTERINDEX, lineCharacterIndex);
	}

	void ReleaseLineCharacterIndex(int lineCharacterIndex)
	{
		Call(SCI_RELEASELINECHARACTERINDEX, lineCharacterIndex);
	}

	Line LineFromIndexPosition(Position pos, int lineCharacterIndex)
	{
		return Call(SCI_LINEFROMINDEXPOSITION, pos, lineCharacterIndex);
	}

	Position IndexPositionFromLine(Line line, int lineCharacterIndex)
	{
		return Call(SCI_INDEXPOSITIONFROMLINE, line, lineCharacterIndex);
	}

private:
	using FunctionDirect = intptr_t(*)(intptr_t ptr, unsigned int msg, uintptr_t wParam, intptr_t lParam);

	intptr_t Call(unsigned int msg, uintptr_t wParam = 0, intptr_t lParam = 0)
	{
		return fn(ptr, msg, wParam, lParam);
	}

	FunctionDirect fn;
	intptr_t ptr;
};
