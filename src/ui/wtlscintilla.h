#pragma once

template <class T>
class CScintillaImpl : public CWindowImpl<T, CWindow, CControlWinTraits>
{
public:
	DECLARE_WND_SUPERCLASS2(nullptr, CScintillaImpl, CWindow::GetWndClassName())

	LRESULT Call(UINT Msg, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		ATLASSERT(::IsWindow(this->m_hWnd));
		return ::SendMessage(this->m_hWnd, Msg, wParam, lParam);
	}

	// Auto-generated from Scintilla.iface

	void AddText(int length, const char* text)
	{
		Call(SCI_ADDTEXT, static_cast<WPARAM>(length), reinterpret_cast<LPARAM>(text));
	}

	void AddStyledText(int length, const char* c)
	{
		Call(SCI_ADDSTYLEDTEXT, static_cast<WPARAM>(length), reinterpret_cast<LPARAM>(c));
	}

	void InsertText(int pos, const char* text)
	{
		Call(SCI_INSERTTEXT, static_cast<WPARAM>(pos), reinterpret_cast<LPARAM>(text));
	}

	void ChangeInsertion(int length, const char* text)
	{
		Call(SCI_CHANGEINSERTION, static_cast<WPARAM>(length), reinterpret_cast<LPARAM>(text));
	}

	void ClearAll()
	{
		Call(SCI_CLEARALL);
	}

	void DeleteRange(int start, int lengthDelete)
	{
		Call(SCI_DELETERANGE, static_cast<WPARAM>(start), lengthDelete);
	}

	void ClearDocumentStyle()
	{
		Call(SCI_CLEARDOCUMENTSTYLE);
	}

	int GetLength()
	{
		return Call(SCI_GETLENGTH);
	}

	int GetCharAt(int pos)
	{
		return Call(SCI_GETCHARAT, static_cast<WPARAM>(pos));
	}

	int GetCurrentPos()
	{
		return Call(SCI_GETCURRENTPOS);
	}

	int GetAnchor()
	{
		return Call(SCI_GETANCHOR);
	}

	int GetStyleAt(int pos)
	{
		return Call(SCI_GETSTYLEAT, static_cast<WPARAM>(pos));
	}

	void Redo()
	{
		Call(SCI_REDO);
	}

	void SetUndoCollection(bool collectUndo)
	{
		Call(SCI_SETUNDOCOLLECTION, static_cast<WPARAM>(collectUndo));
	}

	void SelectAll()
	{
		Call(SCI_SELECTALL);
	}

	void SetSavePoint()
	{
		Call(SCI_SETSAVEPOINT);
	}

	int GetStyledText(void* tr)
	{
		return Call(SCI_GETSTYLEDTEXT, 0, reinterpret_cast<LPARAM>(tr));
	}

	bool CanRedo()
	{
		return Call(SCI_CANREDO);
	}

	int MarkerLineFromHandle(int markerHandle)
	{
		return Call(SCI_MARKERLINEFROMHANDLE, static_cast<WPARAM>(markerHandle));
	}

	void MarkerDeleteHandle(int markerHandle)
	{
		Call(SCI_MARKERDELETEHANDLE, static_cast<WPARAM>(markerHandle));
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
		Call(SCI_SETVIEWWS, static_cast<WPARAM>(viewWS));
	}

	int GetTabDrawMode()
	{
		return Call(SCI_GETTABDRAWMODE);
	}

	void SetTabDrawMode(int tabDrawMode)
	{
		Call(SCI_SETTABDRAWMODE, static_cast<WPARAM>(tabDrawMode));
	}

	int PositionFromPoint(int x, int y)
	{
		return Call(SCI_POSITIONFROMPOINT, static_cast<WPARAM>(x), y);
	}

	int PositionFromPointClose(int x, int y)
	{
		return Call(SCI_POSITIONFROMPOINTCLOSE, static_cast<WPARAM>(x), y);
	}

	void GotoLine(int line)
	{
		Call(SCI_GOTOLINE, static_cast<WPARAM>(line));
	}

	void GotoPos(int caret)
	{
		Call(SCI_GOTOPOS, static_cast<WPARAM>(caret));
	}

	void SetAnchor(int anchor)
	{
		Call(SCI_SETANCHOR, static_cast<WPARAM>(anchor));
	}

	int GetCurLine(int length, char* text)
	{
		return Call(SCI_GETCURLINE, static_cast<WPARAM>(length), reinterpret_cast<LPARAM>(text));
	}

	int GetEndStyled()
	{
		return Call(SCI_GETENDSTYLED);
	}

	void ConvertEOLs(int eolMode)
	{
		Call(SCI_CONVERTEOLS, static_cast<WPARAM>(eolMode));
	}

	int GetEOLMode()
	{
		return Call(SCI_GETEOLMODE);
	}

	void SetEOLMode(int eolMode)
	{
		Call(SCI_SETEOLMODE, static_cast<WPARAM>(eolMode));
	}

	void StartStyling(int start, int unused)
	{
		Call(SCI_STARTSTYLING, static_cast<WPARAM>(start), unused);
	}

	void SetStyling(int length, int style)
	{
		Call(SCI_SETSTYLING, static_cast<WPARAM>(length), style);
	}

	bool GetBufferedDraw()
	{
		return Call(SCI_GETBUFFEREDDRAW);
	}

	void SetBufferedDraw(bool buffered)
	{
		Call(SCI_SETBUFFEREDDRAW, static_cast<WPARAM>(buffered));
	}

	void SetTabWidth(int tabWidth)
	{
		Call(SCI_SETTABWIDTH, static_cast<WPARAM>(tabWidth));
	}

	int GetTabWidth()
	{
		return Call(SCI_GETTABWIDTH);
	}

	void ClearTabStops(int line)
	{
		Call(SCI_CLEARTABSTOPS, static_cast<WPARAM>(line));
	}

	void AddTabStop(int line, int x)
	{
		Call(SCI_ADDTABSTOP, static_cast<WPARAM>(line), x);
	}

	int GetNextTabStop(int line, int x)
	{
		return Call(SCI_GETNEXTTABSTOP, static_cast<WPARAM>(line), x);
	}

	void SetCodePage(int codePage)
	{
		Call(SCI_SETCODEPAGE, static_cast<WPARAM>(codePage));
	}

	int GetIMEInteraction()
	{
		return Call(SCI_GETIMEINTERACTION);
	}

	void SetIMEInteraction(int imeInteraction)
	{
		Call(SCI_SETIMEINTERACTION, static_cast<WPARAM>(imeInteraction));
	}

	void MarkerDefine(int markerNumber, int markerSymbol)
	{
		Call(SCI_MARKERDEFINE, static_cast<WPARAM>(markerNumber), markerSymbol);
	}

	void MarkerSetFore(int markerNumber, int fore)
	{
		Call(SCI_MARKERSETFORE, static_cast<WPARAM>(markerNumber), fore);
	}

	void MarkerSetBack(int markerNumber, int back)
	{
		Call(SCI_MARKERSETBACK, static_cast<WPARAM>(markerNumber), back);
	}

	void MarkerSetBackSelected(int markerNumber, int back)
	{
		Call(SCI_MARKERSETBACKSELECTED, static_cast<WPARAM>(markerNumber), back);
	}

	void MarkerEnableHighlight(bool enabled)
	{
		Call(SCI_MARKERENABLEHIGHLIGHT, static_cast<WPARAM>(enabled));
	}

	int MarkerAdd(int line, int markerNumber)
	{
		return Call(SCI_MARKERADD, static_cast<WPARAM>(line), markerNumber);
	}

	void MarkerDelete(int line, int markerNumber)
	{
		Call(SCI_MARKERDELETE, static_cast<WPARAM>(line), markerNumber);
	}

	void MarkerDeleteAll(int markerNumber)
	{
		Call(SCI_MARKERDELETEALL, static_cast<WPARAM>(markerNumber));
	}

	int MarkerGet(int line)
	{
		return Call(SCI_MARKERGET, static_cast<WPARAM>(line));
	}

	int MarkerNext(int lineStart, int markerMask)
	{
		return Call(SCI_MARKERNEXT, static_cast<WPARAM>(lineStart), markerMask);
	}

	int MarkerPrevious(int lineStart, int markerMask)
	{
		return Call(SCI_MARKERPREVIOUS, static_cast<WPARAM>(lineStart), markerMask);
	}

	void MarkerDefinePixmap(int markerNumber, const char* pixmap)
	{
		Call(SCI_MARKERDEFINEPIXMAP, static_cast<WPARAM>(markerNumber), reinterpret_cast<LPARAM>(pixmap));
	}

	void MarkerAddSet(int line, int markerSet)
	{
		Call(SCI_MARKERADDSET, static_cast<WPARAM>(line), markerSet);
	}

	void MarkerSetAlpha(int markerNumber, int alpha)
	{
		Call(SCI_MARKERSETALPHA, static_cast<WPARAM>(markerNumber), alpha);
	}

	void SetMarginTypeN(int margin, int marginType)
	{
		Call(SCI_SETMARGINTYPEN, static_cast<WPARAM>(margin), marginType);
	}

	int GetMarginTypeN(int margin)
	{
		return Call(SCI_GETMARGINTYPEN, static_cast<WPARAM>(margin));
	}

	void SetMarginWidthN(int margin, int pixelWidth)
	{
		Call(SCI_SETMARGINWIDTHN, static_cast<WPARAM>(margin), pixelWidth);
	}

	int GetMarginWidthN(int margin)
	{
		return Call(SCI_GETMARGINWIDTHN, static_cast<WPARAM>(margin));
	}

	void SetMarginMaskN(int margin, int mask)
	{
		Call(SCI_SETMARGINMASKN, static_cast<WPARAM>(margin), mask);
	}

	int GetMarginMaskN(int margin)
	{
		return Call(SCI_GETMARGINMASKN, static_cast<WPARAM>(margin));
	}

	void SetMarginSensitiveN(int margin, bool sensitive)
	{
		Call(SCI_SETMARGINSENSITIVEN, static_cast<WPARAM>(margin), sensitive);
	}

	bool GetMarginSensitiveN(int margin)
	{
		return Call(SCI_GETMARGINSENSITIVEN, static_cast<WPARAM>(margin));
	}

	void SetMarginCursorN(int margin, int cursor)
	{
		Call(SCI_SETMARGINCURSORN, static_cast<WPARAM>(margin), cursor);
	}

	int GetMarginCursorN(int margin)
	{
		return Call(SCI_GETMARGINCURSORN, static_cast<WPARAM>(margin));
	}

	void SetMarginBackN(int margin, int back)
	{
		Call(SCI_SETMARGINBACKN, static_cast<WPARAM>(margin), back);
	}

	int GetMarginBackN(int margin)
	{
		return Call(SCI_GETMARGINBACKN, static_cast<WPARAM>(margin));
	}

	void SetMargins(int margins)
	{
		Call(SCI_SETMARGINS, static_cast<WPARAM>(margins));
	}

	int GetMargins()
	{
		return Call(SCI_GETMARGINS);
	}

	void StyleClearAll()
	{
		Call(SCI_STYLECLEARALL);
	}

	void StyleSetFore(int style, int fore)
	{
		Call(SCI_STYLESETFORE, static_cast<WPARAM>(style), fore);
	}

	void StyleSetBack(int style, int back)
	{
		Call(SCI_STYLESETBACK, static_cast<WPARAM>(style), back);
	}

	void StyleSetBold(int style, bool bold)
	{
		Call(SCI_STYLESETBOLD, static_cast<WPARAM>(style), bold);
	}

	void StyleSetItalic(int style, bool italic)
	{
		Call(SCI_STYLESETITALIC, static_cast<WPARAM>(style), italic);
	}

	void StyleSetSize(int style, int sizePoints)
	{
		Call(SCI_STYLESETSIZE, static_cast<WPARAM>(style), sizePoints);
	}

	void StyleSetFont(int style, const char* fontName)
	{
		Call(SCI_STYLESETFONT, static_cast<WPARAM>(style), reinterpret_cast<LPARAM>(fontName));
	}

	void StyleSetEOLFilled(int style, bool eolFilled)
	{
		Call(SCI_STYLESETEOLFILLED, static_cast<WPARAM>(style), eolFilled);
	}

	void StyleResetDefault()
	{
		Call(SCI_STYLERESETDEFAULT);
	}

	void StyleSetUnderline(int style, bool underline)
	{
		Call(SCI_STYLESETUNDERLINE, static_cast<WPARAM>(style), underline);
	}

	int StyleGetFore(int style)
	{
		return Call(SCI_STYLEGETFORE, static_cast<WPARAM>(style));
	}

	int StyleGetBack(int style)
	{
		return Call(SCI_STYLEGETBACK, static_cast<WPARAM>(style));
	}

	bool StyleGetBold(int style)
	{
		return Call(SCI_STYLEGETBOLD, static_cast<WPARAM>(style));
	}

	bool StyleGetItalic(int style)
	{
		return Call(SCI_STYLEGETITALIC, static_cast<WPARAM>(style));
	}

	int StyleGetSize(int style)
	{
		return Call(SCI_STYLEGETSIZE, static_cast<WPARAM>(style));
	}

	int StyleGetFont(int style, char* fontName)
	{
		return Call(SCI_STYLEGETFONT, static_cast<WPARAM>(style), reinterpret_cast<LPARAM>(fontName));
	}

	bool StyleGetEOLFilled(int style)
	{
		return Call(SCI_STYLEGETEOLFILLED, static_cast<WPARAM>(style));
	}

	bool StyleGetUnderline(int style)
	{
		return Call(SCI_STYLEGETUNDERLINE, static_cast<WPARAM>(style));
	}

	int StyleGetCase(int style)
	{
		return Call(SCI_STYLEGETCASE, static_cast<WPARAM>(style));
	}

	int StyleGetCharacterSet(int style)
	{
		return Call(SCI_STYLEGETCHARACTERSET, static_cast<WPARAM>(style));
	}

	bool StyleGetVisible(int style)
	{
		return Call(SCI_STYLEGETVISIBLE, static_cast<WPARAM>(style));
	}

	bool StyleGetChangeable(int style)
	{
		return Call(SCI_STYLEGETCHANGEABLE, static_cast<WPARAM>(style));
	}

	bool StyleGetHotSpot(int style)
	{
		return Call(SCI_STYLEGETHOTSPOT, static_cast<WPARAM>(style));
	}

	void StyleSetCase(int style, int caseVisible)
	{
		Call(SCI_STYLESETCASE, static_cast<WPARAM>(style), caseVisible);
	}

	void StyleSetSizeFractional(int style, int sizeHundredthPoints)
	{
		Call(SCI_STYLESETSIZEFRACTIONAL, static_cast<WPARAM>(style), sizeHundredthPoints);
	}

	int StyleGetSizeFractional(int style)
	{
		return Call(SCI_STYLEGETSIZEFRACTIONAL, static_cast<WPARAM>(style));
	}

	void StyleSetWeight(int style, int weight)
	{
		Call(SCI_STYLESETWEIGHT, static_cast<WPARAM>(style), weight);
	}

	int StyleGetWeight(int style)
	{
		return Call(SCI_STYLEGETWEIGHT, static_cast<WPARAM>(style));
	}

	void StyleSetCharacterSet(int style, int characterSet)
	{
		Call(SCI_STYLESETCHARACTERSET, static_cast<WPARAM>(style), characterSet);
	}

	void StyleSetHotSpot(int style, bool hotspot)
	{
		Call(SCI_STYLESETHOTSPOT, static_cast<WPARAM>(style), hotspot);
	}

	void SetSelFore(bool useSetting, int fore)
	{
		Call(SCI_SETSELFORE, static_cast<WPARAM>(useSetting), fore);
	}

	void SetSelBack(bool useSetting, int back)
	{
		Call(SCI_SETSELBACK, static_cast<WPARAM>(useSetting), back);
	}

	int GetSelAlpha()
	{
		return Call(SCI_GETSELALPHA);
	}

	void SetSelAlpha(int alpha)
	{
		Call(SCI_SETSELALPHA, static_cast<WPARAM>(alpha));
	}

	bool GetSelEOLFilled()
	{
		return Call(SCI_GETSELEOLFILLED);
	}

	void SetSelEOLFilled(bool filled)
	{
		Call(SCI_SETSELEOLFILLED, static_cast<WPARAM>(filled));
	}

	void SetCaretFore(int fore)
	{
		Call(SCI_SETCARETFORE, static_cast<WPARAM>(fore));
	}

	void AssignCmdKey(int keyDefinition, int sciCommand)
	{
		Call(SCI_ASSIGNCMDKEY, static_cast<WPARAM>(keyDefinition), sciCommand);
	}

	void ClearCmdKey(int keyDefinition)
	{
		Call(SCI_CLEARCMDKEY, static_cast<WPARAM>(keyDefinition));
	}

	void ClearAllCmdKeys()
	{
		Call(SCI_CLEARALLCMDKEYS);
	}

	void SetStylingEx(int length, const char* styles)
	{
		Call(SCI_SETSTYLINGEX, static_cast<WPARAM>(length), reinterpret_cast<LPARAM>(styles));
	}

	void StyleSetVisible(int style, bool visible)
	{
		Call(SCI_STYLESETVISIBLE, static_cast<WPARAM>(style), visible);
	}

	int GetCaretPeriod()
	{
		return Call(SCI_GETCARETPERIOD);
	}

	void SetCaretPeriod(int periodMilliseconds)
	{
		Call(SCI_SETCARETPERIOD, static_cast<WPARAM>(periodMilliseconds));
	}

	void SetWordChars(const char* characters)
	{
		Call(SCI_SETWORDCHARS, 0, reinterpret_cast<LPARAM>(characters));
	}

	int GetWordChars(char* characters)
	{
		return Call(SCI_GETWORDCHARS, 0, reinterpret_cast<LPARAM>(characters));
	}

	void SetCharacterCategoryOptimization(int countCharacters)
	{
		Call(SCI_SETCHARACTERCATEGORYOPTIMIZATION, static_cast<WPARAM>(countCharacters));
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
		Call(SCI_INDICSETSTYLE, static_cast<WPARAM>(indicator), indicatorStyle);
	}

	int IndicGetStyle(int indicator)
	{
		return Call(SCI_INDICGETSTYLE, static_cast<WPARAM>(indicator));
	}

	void IndicSetFore(int indicator, int fore)
	{
		Call(SCI_INDICSETFORE, static_cast<WPARAM>(indicator), fore);
	}

	int IndicGetFore(int indicator)
	{
		return Call(SCI_INDICGETFORE, static_cast<WPARAM>(indicator));
	}

	void IndicSetUnder(int indicator, bool under)
	{
		Call(SCI_INDICSETUNDER, static_cast<WPARAM>(indicator), under);
	}

	bool IndicGetUnder(int indicator)
	{
		return Call(SCI_INDICGETUNDER, static_cast<WPARAM>(indicator));
	}

	void IndicSetHoverStyle(int indicator, int indicatorStyle)
	{
		Call(SCI_INDICSETHOVERSTYLE, static_cast<WPARAM>(indicator), indicatorStyle);
	}

	int IndicGetHoverStyle(int indicator)
	{
		return Call(SCI_INDICGETHOVERSTYLE, static_cast<WPARAM>(indicator));
	}

	void IndicSetHoverFore(int indicator, int fore)
	{
		Call(SCI_INDICSETHOVERFORE, static_cast<WPARAM>(indicator), fore);
	}

	int IndicGetHoverFore(int indicator)
	{
		return Call(SCI_INDICGETHOVERFORE, static_cast<WPARAM>(indicator));
	}

	void IndicSetFlags(int indicator, int flags)
	{
		Call(SCI_INDICSETFLAGS, static_cast<WPARAM>(indicator), flags);
	}

	int IndicGetFlags(int indicator)
	{
		return Call(SCI_INDICGETFLAGS, static_cast<WPARAM>(indicator));
	}

	void SetWhitespaceFore(bool useSetting, int fore)
	{
		Call(SCI_SETWHITESPACEFORE, static_cast<WPARAM>(useSetting), fore);
	}

	void SetWhitespaceBack(bool useSetting, int back)
	{
		Call(SCI_SETWHITESPACEBACK, static_cast<WPARAM>(useSetting), back);
	}

	void SetWhitespaceSize(int size)
	{
		Call(SCI_SETWHITESPACESIZE, static_cast<WPARAM>(size));
	}

	int GetWhitespaceSize()
	{
		return Call(SCI_GETWHITESPACESIZE);
	}

	void SetLineState(int line, int state)
	{
		Call(SCI_SETLINESTATE, static_cast<WPARAM>(line), state);
	}

	int GetLineState(int line)
	{
		return Call(SCI_GETLINESTATE, static_cast<WPARAM>(line));
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
		Call(SCI_SETCARETLINEVISIBLE, static_cast<WPARAM>(show));
	}

	int GetCaretLineBack()
	{
		return Call(SCI_GETCARETLINEBACK);
	}

	void SetCaretLineBack(int back)
	{
		Call(SCI_SETCARETLINEBACK, static_cast<WPARAM>(back));
	}

	int GetCaretLineFrame()
	{
		return Call(SCI_GETCARETLINEFRAME);
	}

	void SetCaretLineFrame(int width)
	{
		Call(SCI_SETCARETLINEFRAME, static_cast<WPARAM>(width));
	}

	void StyleSetChangeable(int style, bool changeable)
	{
		Call(SCI_STYLESETCHANGEABLE, static_cast<WPARAM>(style), changeable);
	}

	void AutoCShow(int lengthEntered, const char* itemList)
	{
		Call(SCI_AUTOCSHOW, static_cast<WPARAM>(lengthEntered), reinterpret_cast<LPARAM>(itemList));
	}

	void AutoCCancel()
	{
		Call(SCI_AUTOCCANCEL);
	}

	bool AutoCActive()
	{
		return Call(SCI_AUTOCACTIVE);
	}

	int AutoCPosStart()
	{
		return Call(SCI_AUTOCPOSSTART);
	}

	void AutoCComplete()
	{
		Call(SCI_AUTOCCOMPLETE);
	}

	void AutoCStops(const char* characterSet)
	{
		Call(SCI_AUTOCSTOPS, 0, reinterpret_cast<LPARAM>(characterSet));
	}

	void AutoCSetSeparator(int separatorCharacter)
	{
		Call(SCI_AUTOCSETSEPARATOR, static_cast<WPARAM>(separatorCharacter));
	}

	int AutoCGetSeparator()
	{
		return Call(SCI_AUTOCGETSEPARATOR);
	}

	void AutoCSelect(const char* select)
	{
		Call(SCI_AUTOCSELECT, 0, reinterpret_cast<LPARAM>(select));
	}

	void AutoCSetCancelAtStart(bool cancel)
	{
		Call(SCI_AUTOCSETCANCELATSTART, static_cast<WPARAM>(cancel));
	}

	bool AutoCGetCancelAtStart()
	{
		return Call(SCI_AUTOCGETCANCELATSTART);
	}

	void AutoCSetFillUps(const char* characterSet)
	{
		Call(SCI_AUTOCSETFILLUPS, 0, reinterpret_cast<LPARAM>(characterSet));
	}

	void AutoCSetChooseSingle(bool chooseSingle)
	{
		Call(SCI_AUTOCSETCHOOSESINGLE, static_cast<WPARAM>(chooseSingle));
	}

	bool AutoCGetChooseSingle()
	{
		return Call(SCI_AUTOCGETCHOOSESINGLE);
	}

	void AutoCSetIgnoreCase(bool ignoreCase)
	{
		Call(SCI_AUTOCSETIGNORECASE, static_cast<WPARAM>(ignoreCase));
	}

	bool AutoCGetIgnoreCase()
	{
		return Call(SCI_AUTOCGETIGNORECASE);
	}

	void UserListShow(int listType, const char* itemList)
	{
		Call(SCI_USERLISTSHOW, static_cast<WPARAM>(listType), reinterpret_cast<LPARAM>(itemList));
	}

	void AutoCSetAutoHide(bool autoHide)
	{
		Call(SCI_AUTOCSETAUTOHIDE, static_cast<WPARAM>(autoHide));
	}

	bool AutoCGetAutoHide()
	{
		return Call(SCI_AUTOCGETAUTOHIDE);
	}

	void AutoCSetDropRestOfWord(bool dropRestOfWord)
	{
		Call(SCI_AUTOCSETDROPRESTOFWORD, static_cast<WPARAM>(dropRestOfWord));
	}

	bool AutoCGetDropRestOfWord()
	{
		return Call(SCI_AUTOCGETDROPRESTOFWORD);
	}

	void RegisterImage(int type, const char* xpmData)
	{
		Call(SCI_REGISTERIMAGE, static_cast<WPARAM>(type), reinterpret_cast<LPARAM>(xpmData));
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
		Call(SCI_AUTOCSETTYPESEPARATOR, static_cast<WPARAM>(separatorCharacter));
	}

	void AutoCSetMaxWidth(int characterCount)
	{
		Call(SCI_AUTOCSETMAXWIDTH, static_cast<WPARAM>(characterCount));
	}

	int AutoCGetMaxWidth()
	{
		return Call(SCI_AUTOCGETMAXWIDTH);
	}

	void AutoCSetMaxHeight(int rowCount)
	{
		Call(SCI_AUTOCSETMAXHEIGHT, static_cast<WPARAM>(rowCount));
	}

	int AutoCGetMaxHeight()
	{
		return Call(SCI_AUTOCGETMAXHEIGHT);
	}

	void SetIndent(int indentSize)
	{
		Call(SCI_SETINDENT, static_cast<WPARAM>(indentSize));
	}

	int GetIndent()
	{
		return Call(SCI_GETINDENT);
	}

	void SetUseTabs(bool useTabs)
	{
		Call(SCI_SETUSETABS, static_cast<WPARAM>(useTabs));
	}

	bool GetUseTabs()
	{
		return Call(SCI_GETUSETABS);
	}

	void SetLineIndentation(int line, int indentation)
	{
		Call(SCI_SETLINEINDENTATION, static_cast<WPARAM>(line), indentation);
	}

	int GetLineIndentation(int line)
	{
		return Call(SCI_GETLINEINDENTATION, static_cast<WPARAM>(line));
	}

	int GetLineIndentPosition(int line)
	{
		return Call(SCI_GETLINEINDENTPOSITION, static_cast<WPARAM>(line));
	}

	int GetColumn(int pos)
	{
		return Call(SCI_GETCOLUMN, static_cast<WPARAM>(pos));
	}

	int CountCharacters(int start, int end)
	{
		return Call(SCI_COUNTCHARACTERS, static_cast<WPARAM>(start), end);
	}

	int CountCodeUnits(int start, int end)
	{
		return Call(SCI_COUNTCODEUNITS, static_cast<WPARAM>(start), end);
	}

	void SetHScrollBar(bool visible)
	{
		Call(SCI_SETHSCROLLBAR, static_cast<WPARAM>(visible));
	}

	bool GetHScrollBar()
	{
		return Call(SCI_GETHSCROLLBAR);
	}

	void SetIndentationGuides(int indentView)
	{
		Call(SCI_SETINDENTATIONGUIDES, static_cast<WPARAM>(indentView));
	}

	int GetIndentationGuides()
	{
		return Call(SCI_GETINDENTATIONGUIDES);
	}

	void SetHighlightGuide(int column)
	{
		Call(SCI_SETHIGHLIGHTGUIDE, static_cast<WPARAM>(column));
	}

	int GetHighlightGuide()
	{
		return Call(SCI_GETHIGHLIGHTGUIDE);
	}

	int GetLineEndPosition(int line)
	{
		return Call(SCI_GETLINEENDPOSITION, static_cast<WPARAM>(line));
	}

	int GetCodePage()
	{
		return Call(SCI_GETCODEPAGE);
	}

	int GetCaretFore()
	{
		return Call(SCI_GETCARETFORE);
	}

	bool GetReadOnly()
	{
		return Call(SCI_GETREADONLY);
	}

	void SetCurrentPos(int caret)
	{
		Call(SCI_SETCURRENTPOS, static_cast<WPARAM>(caret));
	}

	void SetSelectionStart(int anchor)
	{
		Call(SCI_SETSELECTIONSTART, static_cast<WPARAM>(anchor));
	}

	int GetSelectionStart()
	{
		return Call(SCI_GETSELECTIONSTART);
	}

	void SetSelectionEnd(int caret)
	{
		Call(SCI_SETSELECTIONEND, static_cast<WPARAM>(caret));
	}

	int GetSelectionEnd()
	{
		return Call(SCI_GETSELECTIONEND);
	}

	void SetEmptySelection(int caret)
	{
		Call(SCI_SETEMPTYSELECTION, static_cast<WPARAM>(caret));
	}

	void SetPrintMagnification(int magnification)
	{
		Call(SCI_SETPRINTMAGNIFICATION, static_cast<WPARAM>(magnification));
	}

	int GetPrintMagnification()
	{
		return Call(SCI_GETPRINTMAGNIFICATION);
	}

	void SetPrintColourMode(int mode)
	{
		Call(SCI_SETPRINTCOLOURMODE, static_cast<WPARAM>(mode));
	}

	int GetPrintColourMode()
	{
		return Call(SCI_GETPRINTCOLOURMODE);
	}

	int FindText(int searchFlags, void* ft)
	{
		return Call(SCI_FINDTEXT, static_cast<WPARAM>(searchFlags), reinterpret_cast<LPARAM>(ft));
	}

	int FormatRange(bool draw, void* fr)
	{
		return Call(SCI_FORMATRANGE, static_cast<WPARAM>(draw), reinterpret_cast<LPARAM>(fr));
	}

	int GetFirstVisibleLine()
	{
		return Call(SCI_GETFIRSTVISIBLELINE);
	}

	int GetLine(int line, char* text)
	{
		return Call(SCI_GETLINE, static_cast<WPARAM>(line), reinterpret_cast<LPARAM>(text));
	}

	int GetLineCount()
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

	void SetSel(int anchor, int caret)
	{
		Call(SCI_SETSEL, static_cast<WPARAM>(anchor), caret);
	}

	int GetSelText(char* text)
	{
		return Call(SCI_GETSELTEXT, 0, reinterpret_cast<LPARAM>(text));
	}

	int GetTextRange(void* tr)
	{
		return Call(SCI_GETTEXTRANGE, 0, reinterpret_cast<LPARAM>(tr));
	}

	void HideSelection(bool hide)
	{
		Call(SCI_HIDESELECTION, static_cast<WPARAM>(hide));
	}

	int PointXFromPosition(int pos)
	{
		return Call(SCI_POINTXFROMPOSITION, 0, pos);
	}

	int PointYFromPosition(int pos)
	{
		return Call(SCI_POINTYFROMPOSITION, 0, pos);
	}

	int LineFromPosition(int pos)
	{
		return Call(SCI_LINEFROMPOSITION, static_cast<WPARAM>(pos));
	}

	int PositionFromLine(int line)
	{
		return Call(SCI_POSITIONFROMLINE, static_cast<WPARAM>(line));
	}

	void LineScroll(int columns, int lines)
	{
		Call(SCI_LINESCROLL, static_cast<WPARAM>(columns), lines);
	}

	void ScrollCaret()
	{
		Call(SCI_SCROLLCARET);
	}

	void ScrollRange(int secondary, int primary)
	{
		Call(SCI_SCROLLRANGE, static_cast<WPARAM>(secondary), primary);
	}

	void ReplaceSel(const char* text)
	{
		Call(SCI_REPLACESEL, 0, reinterpret_cast<LPARAM>(text));
	}

	void SetReadOnly(bool readOnly)
	{
		Call(SCI_SETREADONLY, static_cast<WPARAM>(readOnly));
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
		Call(SCI_SETTEXT, 0, reinterpret_cast<LPARAM>(text));
	}

	int GetText(int length, char* text)
	{
		return Call(SCI_GETTEXT, static_cast<WPARAM>(length), reinterpret_cast<LPARAM>(text));
	}

	int GetTextLength()
	{
		return Call(SCI_GETTEXTLENGTH);
	}

	void* GetDirectFunction()
	{
		return Call(SCI_GETDIRECTFUNCTION);
	}

	void* GetDirectPointer()
	{
		return Call(SCI_GETDIRECTPOINTER);
	}

	void SetOvertype(bool overType)
	{
		Call(SCI_SETOVERTYPE, static_cast<WPARAM>(overType));
	}

	bool GetOvertype()
	{
		return Call(SCI_GETOVERTYPE);
	}

	void SetCaretWidth(int pixelWidth)
	{
		Call(SCI_SETCARETWIDTH, static_cast<WPARAM>(pixelWidth));
	}

	int GetCaretWidth()
	{
		return Call(SCI_GETCARETWIDTH);
	}

	void SetTargetStart(int start)
	{
		Call(SCI_SETTARGETSTART, static_cast<WPARAM>(start));
	}

	int GetTargetStart()
	{
		return Call(SCI_GETTARGETSTART);
	}

	void SetTargetEnd(int end)
	{
		Call(SCI_SETTARGETEND, static_cast<WPARAM>(end));
	}

	int GetTargetEnd()
	{
		return Call(SCI_GETTARGETEND);
	}

	void SetTargetRange(int start, int end)
	{
		Call(SCI_SETTARGETRANGE, static_cast<WPARAM>(start), end);
	}

	int GetTargetText(char* text)
	{
		return Call(SCI_GETTARGETTEXT, 0, reinterpret_cast<LPARAM>(text));
	}

	void TargetFromSelection()
	{
		Call(SCI_TARGETFROMSELECTION);
	}

	void TargetWholeDocument()
	{
		Call(SCI_TARGETWHOLEDOCUMENT);
	}

	int ReplaceTarget(int length, const char* text)
	{
		return Call(SCI_REPLACETARGET, static_cast<WPARAM>(length), reinterpret_cast<LPARAM>(text));
	}

	int ReplaceTargetRE(int length, const char* text)
	{
		return Call(SCI_REPLACETARGETRE, static_cast<WPARAM>(length), reinterpret_cast<LPARAM>(text));
	}

	int SearchInTarget(int length, const char* text)
	{
		return Call(SCI_SEARCHINTARGET, static_cast<WPARAM>(length), reinterpret_cast<LPARAM>(text));
	}

	void SetSearchFlags(int searchFlags)
	{
		Call(SCI_SETSEARCHFLAGS, static_cast<WPARAM>(searchFlags));
	}

	int GetSearchFlags()
	{
		return Call(SCI_GETSEARCHFLAGS);
	}

	void CallTipShow(int pos, const char* definition)
	{
		Call(SCI_CALLTIPSHOW, static_cast<WPARAM>(pos), reinterpret_cast<LPARAM>(definition));
	}

	void CallTipCancel()
	{
		Call(SCI_CALLTIPCANCEL);
	}

	bool CallTipActive()
	{
		return Call(SCI_CALLTIPACTIVE);
	}

	int CallTipPosStart()
	{
		return Call(SCI_CALLTIPPOSSTART);
	}

	void CallTipSetPosStart(int posStart)
	{
		Call(SCI_CALLTIPSETPOSSTART, static_cast<WPARAM>(posStart));
	}

	void CallTipSetHlt(int highlightStart, int highlightEnd)
	{
		Call(SCI_CALLTIPSETHLT, static_cast<WPARAM>(highlightStart), highlightEnd);
	}

	void CallTipSetBack(int back)
	{
		Call(SCI_CALLTIPSETBACK, static_cast<WPARAM>(back));
	}

	void CallTipSetFore(int fore)
	{
		Call(SCI_CALLTIPSETFORE, static_cast<WPARAM>(fore));
	}

	void CallTipSetForeHlt(int fore)
	{
		Call(SCI_CALLTIPSETFOREHLT, static_cast<WPARAM>(fore));
	}

	void CallTipUseStyle(int tabSize)
	{
		Call(SCI_CALLTIPUSESTYLE, static_cast<WPARAM>(tabSize));
	}

	void CallTipSetPosition(bool above)
	{
		Call(SCI_CALLTIPSETPOSITION, static_cast<WPARAM>(above));
	}

	int VisibleFromDocLine(int docLine)
	{
		return Call(SCI_VISIBLEFROMDOCLINE, static_cast<WPARAM>(docLine));
	}

	int DocLineFromVisible(int displayLine)
	{
		return Call(SCI_DOCLINEFROMVISIBLE, static_cast<WPARAM>(displayLine));
	}

	int WrapCount(int docLine)
	{
		return Call(SCI_WRAPCOUNT, static_cast<WPARAM>(docLine));
	}

	void SetFoldLevel(int line, int level)
	{
		Call(SCI_SETFOLDLEVEL, static_cast<WPARAM>(line), level);
	}

	int GetFoldLevel(int line)
	{
		return Call(SCI_GETFOLDLEVEL, static_cast<WPARAM>(line));
	}

	int GetLastChild(int line, int level)
	{
		return Call(SCI_GETLASTCHILD, static_cast<WPARAM>(line), level);
	}

	int GetFoldParent(int line)
	{
		return Call(SCI_GETFOLDPARENT, static_cast<WPARAM>(line));
	}

	void ShowLines(int lineStart, int lineEnd)
	{
		Call(SCI_SHOWLINES, static_cast<WPARAM>(lineStart), lineEnd);
	}

	void HideLines(int lineStart, int lineEnd)
	{
		Call(SCI_HIDELINES, static_cast<WPARAM>(lineStart), lineEnd);
	}

	bool GetLineVisible(int line)
	{
		return Call(SCI_GETLINEVISIBLE, static_cast<WPARAM>(line));
	}

	bool GetAllLinesVisible()
	{
		return Call(SCI_GETALLLINESVISIBLE);
	}

	void SetFoldExpanded(int line, bool expanded)
	{
		Call(SCI_SETFOLDEXPANDED, static_cast<WPARAM>(line), expanded);
	}

	bool GetFoldExpanded(int line)
	{
		return Call(SCI_GETFOLDEXPANDED, static_cast<WPARAM>(line));
	}

	void ToggleFold(int line)
	{
		Call(SCI_TOGGLEFOLD, static_cast<WPARAM>(line));
	}

	void ToggleFoldShowText(int line, const char* text)
	{
		Call(SCI_TOGGLEFOLDSHOWTEXT, static_cast<WPARAM>(line), reinterpret_cast<LPARAM>(text));
	}

	void FoldDisplayTextSetStyle(int style)
	{
		Call(SCI_FOLDDISPLAYTEXTSETSTYLE, static_cast<WPARAM>(style));
	}

	int FoldDisplayTextGetStyle()
	{
		return Call(SCI_FOLDDISPLAYTEXTGETSTYLE);
	}

	void SetDefaultFoldDisplayText(const char* text)
	{
		Call(SCI_SETDEFAULTFOLDDISPLAYTEXT, 0, reinterpret_cast<LPARAM>(text));
	}

	int GetDefaultFoldDisplayText(char* text)
	{
		return Call(SCI_GETDEFAULTFOLDDISPLAYTEXT, 0, reinterpret_cast<LPARAM>(text));
	}

	void FoldLine(int line, int action)
	{
		Call(SCI_FOLDLINE, static_cast<WPARAM>(line), action);
	}

	void FoldChildren(int line, int action)
	{
		Call(SCI_FOLDCHILDREN, static_cast<WPARAM>(line), action);
	}

	void ExpandChildren(int line, int level)
	{
		Call(SCI_EXPANDCHILDREN, static_cast<WPARAM>(line), level);
	}

	void FoldAll(int action)
	{
		Call(SCI_FOLDALL, static_cast<WPARAM>(action));
	}

	void EnsureVisible(int line)
	{
		Call(SCI_ENSUREVISIBLE, static_cast<WPARAM>(line));
	}

	void SetAutomaticFold(int automaticFold)
	{
		Call(SCI_SETAUTOMATICFOLD, static_cast<WPARAM>(automaticFold));
	}

	int GetAutomaticFold()
	{
		return Call(SCI_GETAUTOMATICFOLD);
	}

	void SetFoldFlags(int flags)
	{
		Call(SCI_SETFOLDFLAGS, static_cast<WPARAM>(flags));
	}

	void EnsureVisibleEnforcePolicy(int line)
	{
		Call(SCI_ENSUREVISIBLEENFORCEPOLICY, static_cast<WPARAM>(line));
	}

	void SetTabIndents(bool tabIndents)
	{
		Call(SCI_SETTABINDENTS, static_cast<WPARAM>(tabIndents));
	}

	bool GetTabIndents()
	{
		return Call(SCI_GETTABINDENTS);
	}

	void SetBackSpaceUnIndents(bool bsUnIndents)
	{
		Call(SCI_SETBACKSPACEUNINDENTS, static_cast<WPARAM>(bsUnIndents));
	}

	bool GetBackSpaceUnIndents()
	{
		return Call(SCI_GETBACKSPACEUNINDENTS);
	}

	void SetMouseDwellTime(int periodMilliseconds)
	{
		Call(SCI_SETMOUSEDWELLTIME, static_cast<WPARAM>(periodMilliseconds));
	}

	int GetMouseDwellTime()
	{
		return Call(SCI_GETMOUSEDWELLTIME);
	}

	int WordStartPosition(int pos, bool onlyWordCharacters)
	{
		return Call(SCI_WORDSTARTPOSITION, static_cast<WPARAM>(pos), onlyWordCharacters);
	}

	int WordEndPosition(int pos, bool onlyWordCharacters)
	{
		return Call(SCI_WORDENDPOSITION, static_cast<WPARAM>(pos), onlyWordCharacters);
	}

	bool IsRangeWord(int start, int end)
	{
		return Call(SCI_ISRANGEWORD, static_cast<WPARAM>(start), end);
	}

	void SetIdleStyling(int idleStyling)
	{
		Call(SCI_SETIDLESTYLING, static_cast<WPARAM>(idleStyling));
	}

	int GetIdleStyling()
	{
		return Call(SCI_GETIDLESTYLING);
	}

	void SetWrapMode(int wrapMode)
	{
		Call(SCI_SETWRAPMODE, static_cast<WPARAM>(wrapMode));
	}

	int GetWrapMode()
	{
		return Call(SCI_GETWRAPMODE);
	}

	void SetWrapVisualFlags(int wrapVisualFlags)
	{
		Call(SCI_SETWRAPVISUALFLAGS, static_cast<WPARAM>(wrapVisualFlags));
	}

	int GetWrapVisualFlags()
	{
		return Call(SCI_GETWRAPVISUALFLAGS);
	}

	void SetWrapVisualFlagsLocation(int wrapVisualFlagsLocation)
	{
		Call(SCI_SETWRAPVISUALFLAGSLOCATION, static_cast<WPARAM>(wrapVisualFlagsLocation));
	}

	int GetWrapVisualFlagsLocation()
	{
		return Call(SCI_GETWRAPVISUALFLAGSLOCATION);
	}

	void SetWrapStartIndent(int indent)
	{
		Call(SCI_SETWRAPSTARTINDENT, static_cast<WPARAM>(indent));
	}

	int GetWrapStartIndent()
	{
		return Call(SCI_GETWRAPSTARTINDENT);
	}

	void SetWrapIndentMode(int wrapIndentMode)
	{
		Call(SCI_SETWRAPINDENTMODE, static_cast<WPARAM>(wrapIndentMode));
	}

	int GetWrapIndentMode()
	{
		return Call(SCI_GETWRAPINDENTMODE);
	}

	void SetLayoutCache(int cacheMode)
	{
		Call(SCI_SETLAYOUTCACHE, static_cast<WPARAM>(cacheMode));
	}

	int GetLayoutCache()
	{
		return Call(SCI_GETLAYOUTCACHE);
	}

	void SetScrollWidth(int pixelWidth)
	{
		Call(SCI_SETSCROLLWIDTH, static_cast<WPARAM>(pixelWidth));
	}

	int GetScrollWidth()
	{
		return Call(SCI_GETSCROLLWIDTH);
	}

	void SetScrollWidthTracking(bool tracking)
	{
		Call(SCI_SETSCROLLWIDTHTRACKING, static_cast<WPARAM>(tracking));
	}

	bool GetScrollWidthTracking()
	{
		return Call(SCI_GETSCROLLWIDTHTRACKING);
	}

	int TextWidth(int style, const char* text)
	{
		return Call(SCI_TEXTWIDTH, static_cast<WPARAM>(style), reinterpret_cast<LPARAM>(text));
	}

	void SetEndAtLastLine(bool endAtLastLine)
	{
		Call(SCI_SETENDATLASTLINE, static_cast<WPARAM>(endAtLastLine));
	}

	bool GetEndAtLastLine()
	{
		return Call(SCI_GETENDATLASTLINE);
	}

	int TextHeight(int line)
	{
		return Call(SCI_TEXTHEIGHT, static_cast<WPARAM>(line));
	}

	void SetVScrollBar(bool visible)
	{
		Call(SCI_SETVSCROLLBAR, static_cast<WPARAM>(visible));
	}

	bool GetVScrollBar()
	{
		return Call(SCI_GETVSCROLLBAR);
	}

	void AppendText(int length, const char* text)
	{
		Call(SCI_APPENDTEXT, static_cast<WPARAM>(length), reinterpret_cast<LPARAM>(text));
	}

	int GetPhasesDraw()
	{
		return Call(SCI_GETPHASESDRAW);
	}

	void SetPhasesDraw(int phases)
	{
		Call(SCI_SETPHASESDRAW, static_cast<WPARAM>(phases));
	}

	void SetFontQuality(int fontQuality)
	{
		Call(SCI_SETFONTQUALITY, static_cast<WPARAM>(fontQuality));
	}

	int GetFontQuality()
	{
		return Call(SCI_GETFONTQUALITY);
	}

	void SetFirstVisibleLine(int displayLine)
	{
		Call(SCI_SETFIRSTVISIBLELINE, static_cast<WPARAM>(displayLine));
	}

	void SetMultiPaste(int multiPaste)
	{
		Call(SCI_SETMULTIPASTE, static_cast<WPARAM>(multiPaste));
	}

	int GetMultiPaste()
	{
		return Call(SCI_GETMULTIPASTE);
	}

	int GetTag(int tagNumber, char* tagValue)
	{
		return Call(SCI_GETTAG, static_cast<WPARAM>(tagNumber), reinterpret_cast<LPARAM>(tagValue));
	}

	void LinesJoin()
	{
		Call(SCI_LINESJOIN);
	}

	void LinesSplit(int pixelWidth)
	{
		Call(SCI_LINESSPLIT, static_cast<WPARAM>(pixelWidth));
	}

	void SetFoldMarginColour(bool useSetting, int back)
	{
		Call(SCI_SETFOLDMARGINCOLOUR, static_cast<WPARAM>(useSetting), back);
	}

	void SetFoldMarginHiColour(bool useSetting, int fore)
	{
		Call(SCI_SETFOLDMARGINHICOLOUR, static_cast<WPARAM>(useSetting), fore);
	}

	void SetAccessibility(int accessibility)
	{
		Call(SCI_SETACCESSIBILITY, static_cast<WPARAM>(accessibility));
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

	int LineLength(int line)
	{
		return Call(SCI_LINELENGTH, static_cast<WPARAM>(line));
	}

	void BraceHighlight(int posA, int posB)
	{
		Call(SCI_BRACEHIGHLIGHT, static_cast<WPARAM>(posA), posB);
	}

	void BraceHighlightIndicator(bool useSetting, int indicator)
	{
		Call(SCI_BRACEHIGHLIGHTINDICATOR, static_cast<WPARAM>(useSetting), indicator);
	}

	void BraceBadLight(int pos)
	{
		Call(SCI_BRACEBADLIGHT, static_cast<WPARAM>(pos));
	}

	void BraceBadLightIndicator(bool useSetting, int indicator)
	{
		Call(SCI_BRACEBADLIGHTINDICATOR, static_cast<WPARAM>(useSetting), indicator);
	}

	int BraceMatch(int pos, int maxReStyle)
	{
		return Call(SCI_BRACEMATCH, static_cast<WPARAM>(pos), maxReStyle);
	}

	bool GetViewEOL()
	{
		return Call(SCI_GETVIEWEOL);
	}

	void SetViewEOL(bool visible)
	{
		Call(SCI_SETVIEWEOL, static_cast<WPARAM>(visible));
	}

	void* GetDocPointer()
	{
		return Call(SCI_GETDOCPOINTER);
	}

	void SetDocPointer(void* doc)
	{
		Call(SCI_SETDOCPOINTER, 0, reinterpret_cast<LPARAM>(doc));
	}

	void SetModEventMask(int eventMask)
	{
		Call(SCI_SETMODEVENTMASK, static_cast<WPARAM>(eventMask));
	}

	int GetEdgeColumn()
	{
		return Call(SCI_GETEDGECOLUMN);
	}

	void SetEdgeColumn(int column)
	{
		Call(SCI_SETEDGECOLUMN, static_cast<WPARAM>(column));
	}

	int GetEdgeMode()
	{
		return Call(SCI_GETEDGEMODE);
	}

	void SetEdgeMode(int edgeMode)
	{
		Call(SCI_SETEDGEMODE, static_cast<WPARAM>(edgeMode));
	}

	int GetEdgeColour()
	{
		return Call(SCI_GETEDGECOLOUR);
	}

	void SetEdgeColour(int edgeColour)
	{
		Call(SCI_SETEDGECOLOUR, static_cast<WPARAM>(edgeColour));
	}

	void MultiEdgeAddLine(int column, int edgeColour)
	{
		Call(SCI_MULTIEDGEADDLINE, static_cast<WPARAM>(column), edgeColour);
	}

	void MultiEdgeClearAll()
	{
		Call(SCI_MULTIEDGECLEARALL);
	}

	void SearchAnchor()
	{
		Call(SCI_SEARCHANCHOR);
	}

	int SearchNext(int searchFlags, const char* text)
	{
		return Call(SCI_SEARCHNEXT, static_cast<WPARAM>(searchFlags), reinterpret_cast<LPARAM>(text));
	}

	int SearchPrev(int searchFlags, const char* text)
	{
		return Call(SCI_SEARCHPREV, static_cast<WPARAM>(searchFlags), reinterpret_cast<LPARAM>(text));
	}

	int LinesOnScreen()
	{
		return Call(SCI_LINESONSCREEN);
	}

	void UsePopUp(int popUpMode)
	{
		Call(SCI_USEPOPUP, static_cast<WPARAM>(popUpMode));
	}

	bool SelectionIsRectangle()
	{
		return Call(SCI_SELECTIONISRECTANGLE);
	}

	void SetZoom(int zoomInPoints)
	{
		Call(SCI_SETZOOM, static_cast<WPARAM>(zoomInPoints));
	}

	int GetZoom()
	{
		return Call(SCI_GETZOOM);
	}

	void* CreateDocument(int bytes, int documentOptions)
	{
		return Call(SCI_CREATEDOCUMENT, static_cast<WPARAM>(bytes), documentOptions);
	}

	void AddRefDocument(void* doc)
	{
		Call(SCI_ADDREFDOCUMENT, 0, reinterpret_cast<LPARAM>(doc));
	}

	void ReleaseDocument(void* doc)
	{
		Call(SCI_RELEASEDOCUMENT, 0, reinterpret_cast<LPARAM>(doc));
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
		Call(SCI_SETCOMMANDEVENTS, static_cast<WPARAM>(commandEvents));
	}

	bool GetCommandEvents()
	{
		return Call(SCI_GETCOMMANDEVENTS);
	}

	void SetFocus(bool focus)
	{
		Call(SCI_SETFOCUS, static_cast<WPARAM>(focus));
	}

	bool GetFocus()
	{
		return Call(SCI_GETFOCUS);
	}

	void SetStatus(int status)
	{
		Call(SCI_SETSTATUS, static_cast<WPARAM>(status));
	}

	int GetStatus()
	{
		return Call(SCI_GETSTATUS);
	}

	void SetMouseDownCaptures(bool captures)
	{
		Call(SCI_SETMOUSEDOWNCAPTURES, static_cast<WPARAM>(captures));
	}

	bool GetMouseDownCaptures()
	{
		return Call(SCI_GETMOUSEDOWNCAPTURES);
	}

	void SetMouseWheelCaptures(bool captures)
	{
		Call(SCI_SETMOUSEWHEELCAPTURES, static_cast<WPARAM>(captures));
	}

	bool GetMouseWheelCaptures()
	{
		return Call(SCI_GETMOUSEWHEELCAPTURES);
	}

	void SetCursor(int cursorType)
	{
		Call(SCI_SETCURSOR, static_cast<WPARAM>(cursorType));
	}

	int GetCursor()
	{
		return Call(SCI_GETCURSOR);
	}

	void SetControlCharSymbol(int symbol)
	{
		Call(SCI_SETCONTROLCHARSYMBOL, static_cast<WPARAM>(symbol));
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
		Call(SCI_SETVISIBLEPOLICY, static_cast<WPARAM>(visiblePolicy), visibleSlop);
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
		Call(SCI_SETXOFFSET, static_cast<WPARAM>(xOffset));
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
		Call(SCI_SETXCARETPOLICY, static_cast<WPARAM>(caretPolicy), caretSlop);
	}

	void SetYCaretPolicy(int caretPolicy, int caretSlop)
	{
		Call(SCI_SETYCARETPOLICY, static_cast<WPARAM>(caretPolicy), caretSlop);
	}

	void SetPrintWrapMode(int wrapMode)
	{
		Call(SCI_SETPRINTWRAPMODE, static_cast<WPARAM>(wrapMode));
	}

	int GetPrintWrapMode()
	{
		return Call(SCI_GETPRINTWRAPMODE);
	}

	void SetHotspotActiveFore(bool useSetting, int fore)
	{
		Call(SCI_SETHOTSPOTACTIVEFORE, static_cast<WPARAM>(useSetting), fore);
	}

	int GetHotspotActiveFore()
	{
		return Call(SCI_GETHOTSPOTACTIVEFORE);
	}

	void SetHotspotActiveBack(bool useSetting, int back)
	{
		Call(SCI_SETHOTSPOTACTIVEBACK, static_cast<WPARAM>(useSetting), back);
	}

	int GetHotspotActiveBack()
	{
		return Call(SCI_GETHOTSPOTACTIVEBACK);
	}

	void SetHotspotActiveUnderline(bool underline)
	{
		Call(SCI_SETHOTSPOTACTIVEUNDERLINE, static_cast<WPARAM>(underline));
	}

	bool GetHotspotActiveUnderline()
	{
		return Call(SCI_GETHOTSPOTACTIVEUNDERLINE);
	}

	void SetHotspotSingleLine(bool singleLine)
	{
		Call(SCI_SETHOTSPOTSINGLELINE, static_cast<WPARAM>(singleLine));
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

	int PositionBefore(int pos)
	{
		return Call(SCI_POSITIONBEFORE, static_cast<WPARAM>(pos));
	}

	int PositionAfter(int pos)
	{
		return Call(SCI_POSITIONAFTER, static_cast<WPARAM>(pos));
	}

	int PositionRelative(int pos, int relative)
	{
		return Call(SCI_POSITIONRELATIVE, static_cast<WPARAM>(pos), relative);
	}

	int PositionRelativeCodeUnits(int pos, int relative)
	{
		return Call(SCI_POSITIONRELATIVECODEUNITS, static_cast<WPARAM>(pos), relative);
	}

	void CopyRange(int start, int end)
	{
		Call(SCI_COPYRANGE, static_cast<WPARAM>(start), end);
	}

	void CopyText(int length, const char* text)
	{
		Call(SCI_COPYTEXT, static_cast<WPARAM>(length), reinterpret_cast<LPARAM>(text));
	}

	void SetSelectionMode(int selectionMode)
	{
		Call(SCI_SETSELECTIONMODE, static_cast<WPARAM>(selectionMode));
	}

	int GetSelectionMode()
	{
		return Call(SCI_GETSELECTIONMODE);
	}

	bool GetMoveExtendsSelection()
	{
		return Call(SCI_GETMOVEEXTENDSSELECTION);
	}

	int GetLineSelStartPosition(int line)
	{
		return Call(SCI_GETLINESELSTARTPOSITION, static_cast<WPARAM>(line));
	}

	int GetLineSelEndPosition(int line)
	{
		return Call(SCI_GETLINESELENDPOSITION, static_cast<WPARAM>(line));
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
		Call(SCI_SETWHITESPACECHARS, 0, reinterpret_cast<LPARAM>(characters));
	}

	int GetWhitespaceChars(char* characters)
	{
		return Call(SCI_GETWHITESPACECHARS, 0, reinterpret_cast<LPARAM>(characters));
	}

	void SetPunctuationChars(const char* characters)
	{
		Call(SCI_SETPUNCTUATIONCHARS, 0, reinterpret_cast<LPARAM>(characters));
	}

	int GetPunctuationChars(char* characters)
	{
		return Call(SCI_GETPUNCTUATIONCHARS, 0, reinterpret_cast<LPARAM>(characters));
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
		return Call(SCI_AUTOCGETCURRENTTEXT, 0, reinterpret_cast<LPARAM>(text));
	}

	void AutoCSetCaseInsensitiveBehaviour(int behaviour)
	{
		Call(SCI_AUTOCSETCASEINSENSITIVEBEHAVIOUR, static_cast<WPARAM>(behaviour));
	}

	int AutoCGetCaseInsensitiveBehaviour()
	{
		return Call(SCI_AUTOCGETCASEINSENSITIVEBEHAVIOUR);
	}

	void AutoCSetMulti(int multi)
	{
		Call(SCI_AUTOCSETMULTI, static_cast<WPARAM>(multi));
	}

	int AutoCGetMulti()
	{
		return Call(SCI_AUTOCGETMULTI);
	}

	void AutoCSetOrder(int order)
	{
		Call(SCI_AUTOCSETORDER, static_cast<WPARAM>(order));
	}

	int AutoCGetOrder()
	{
		return Call(SCI_AUTOCGETORDER);
	}

	void Allocate(int bytes)
	{
		Call(SCI_ALLOCATE, static_cast<WPARAM>(bytes));
	}

	int TargetAsUTF8(char* s)
	{
		return Call(SCI_TARGETASUTF8, 0, reinterpret_cast<LPARAM>(s));
	}

	void SetLengthForEncode(int bytes)
	{
		Call(SCI_SETLENGTHFORENCODE, static_cast<WPARAM>(bytes));
	}

	int EncodedFromUTF8(const char* utf8, char* encoded)
	{
		return Call(SCI_ENCODEDFROMUTF8, reinterpret_cast<WPARAM>(utf8), reinterpret_cast<LPARAM>(encoded));
	}

	int FindColumn(int line, int column)
	{
		return Call(SCI_FINDCOLUMN, static_cast<WPARAM>(line), column);
	}

	int GetCaretSticky()
	{
		return Call(SCI_GETCARETSTICKY);
	}

	void SetCaretSticky(int useCaretStickyBehaviour)
	{
		Call(SCI_SETCARETSTICKY, static_cast<WPARAM>(useCaretStickyBehaviour));
	}

	void ToggleCaretSticky()
	{
		Call(SCI_TOGGLECARETSTICKY);
	}

	void SetPasteConvertEndings(bool convert)
	{
		Call(SCI_SETPASTECONVERTENDINGS, static_cast<WPARAM>(convert));
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
		Call(SCI_SETCARETLINEBACKALPHA, static_cast<WPARAM>(alpha));
	}

	int GetCaretLineBackAlpha()
	{
		return Call(SCI_GETCARETLINEBACKALPHA);
	}

	void SetCaretStyle(int caretStyle)
	{
		Call(SCI_SETCARETSTYLE, static_cast<WPARAM>(caretStyle));
	}

	int GetCaretStyle()
	{
		return Call(SCI_GETCARETSTYLE);
	}

	void SetIndicatorCurrent(int indicator)
	{
		Call(SCI_SETINDICATORCURRENT, static_cast<WPARAM>(indicator));
	}

	int GetIndicatorCurrent()
	{
		return Call(SCI_GETINDICATORCURRENT);
	}

	void SetIndicatorValue(int value)
	{
		Call(SCI_SETINDICATORVALUE, static_cast<WPARAM>(value));
	}

	int GetIndicatorValue()
	{
		return Call(SCI_GETINDICATORVALUE);
	}

	void IndicatorFillRange(int start, int lengthFill)
	{
		Call(SCI_INDICATORFILLRANGE, static_cast<WPARAM>(start), lengthFill);
	}

	void IndicatorClearRange(int start, int lengthClear)
	{
		Call(SCI_INDICATORCLEARRANGE, static_cast<WPARAM>(start), lengthClear);
	}

	int IndicatorAllOnFor(int pos)
	{
		return Call(SCI_INDICATORALLONFOR, static_cast<WPARAM>(pos));
	}

	int IndicatorValueAt(int indicator, int pos)
	{
		return Call(SCI_INDICATORVALUEAT, static_cast<WPARAM>(indicator), pos);
	}

	int IndicatorStart(int indicator, int pos)
	{
		return Call(SCI_INDICATORSTART, static_cast<WPARAM>(indicator), pos);
	}

	int IndicatorEnd(int indicator, int pos)
	{
		return Call(SCI_INDICATOREND, static_cast<WPARAM>(indicator), pos);
	}

	void SetPositionCache(int size)
	{
		Call(SCI_SETPOSITIONCACHE, static_cast<WPARAM>(size));
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
		return Call(SCI_GETCHARACTERPOINTER);
	}

	void* GetRangePointer(int start, int lengthRange)
	{
		return Call(SCI_GETRANGEPOINTER, static_cast<WPARAM>(start), lengthRange);
	}

	int GetGapPosition()
	{
		return Call(SCI_GETGAPPOSITION);
	}

	void IndicSetAlpha(int indicator, int alpha)
	{
		Call(SCI_INDICSETALPHA, static_cast<WPARAM>(indicator), alpha);
	}

	int IndicGetAlpha(int indicator)
	{
		return Call(SCI_INDICGETALPHA, static_cast<WPARAM>(indicator));
	}

	void IndicSetOutlineAlpha(int indicator, int alpha)
	{
		Call(SCI_INDICSETOUTLINEALPHA, static_cast<WPARAM>(indicator), alpha);
	}

	int IndicGetOutlineAlpha(int indicator)
	{
		return Call(SCI_INDICGETOUTLINEALPHA, static_cast<WPARAM>(indicator));
	}

	void SetExtraAscent(int extraAscent)
	{
		Call(SCI_SETEXTRAASCENT, static_cast<WPARAM>(extraAscent));
	}

	int GetExtraAscent()
	{
		return Call(SCI_GETEXTRAASCENT);
	}

	void SetExtraDescent(int extraDescent)
	{
		Call(SCI_SETEXTRADESCENT, static_cast<WPARAM>(extraDescent));
	}

	int GetExtraDescent()
	{
		return Call(SCI_GETEXTRADESCENT);
	}

	int MarkerSymbolDefined(int markerNumber)
	{
		return Call(SCI_MARKERSYMBOLDEFINED, static_cast<WPARAM>(markerNumber));
	}

	void MarginSetText(int line, const char* text)
	{
		Call(SCI_MARGINSETTEXT, static_cast<WPARAM>(line), reinterpret_cast<LPARAM>(text));
	}

	int MarginGetText(int line, char* text)
	{
		return Call(SCI_MARGINGETTEXT, static_cast<WPARAM>(line), reinterpret_cast<LPARAM>(text));
	}

	void MarginSetStyle(int line, int style)
	{
		Call(SCI_MARGINSETSTYLE, static_cast<WPARAM>(line), style);
	}

	int MarginGetStyle(int line)
	{
		return Call(SCI_MARGINGETSTYLE, static_cast<WPARAM>(line));
	}

	void MarginSetStyles(int line, const char* styles)
	{
		Call(SCI_MARGINSETSTYLES, static_cast<WPARAM>(line), reinterpret_cast<LPARAM>(styles));
	}

	int MarginGetStyles(int line, char* styles)
	{
		return Call(SCI_MARGINGETSTYLES, static_cast<WPARAM>(line), reinterpret_cast<LPARAM>(styles));
	}

	void MarginTextClearAll()
	{
		Call(SCI_MARGINTEXTCLEARALL);
	}

	void MarginSetStyleOffset(int style)
	{
		Call(SCI_MARGINSETSTYLEOFFSET, static_cast<WPARAM>(style));
	}

	int MarginGetStyleOffset()
	{
		return Call(SCI_MARGINGETSTYLEOFFSET);
	}

	void SetMarginOptions(int marginOptions)
	{
		Call(SCI_SETMARGINOPTIONS, static_cast<WPARAM>(marginOptions));
	}

	int GetMarginOptions()
	{
		return Call(SCI_GETMARGINOPTIONS);
	}

	void AnnotationSetText(int line, const char* text)
	{
		Call(SCI_ANNOTATIONSETTEXT, static_cast<WPARAM>(line), reinterpret_cast<LPARAM>(text));
	}

	int AnnotationGetText(int line, char* text)
	{
		return Call(SCI_ANNOTATIONGETTEXT, static_cast<WPARAM>(line), reinterpret_cast<LPARAM>(text));
	}

	void AnnotationSetStyle(int line, int style)
	{
		Call(SCI_ANNOTATIONSETSTYLE, static_cast<WPARAM>(line), style);
	}

	int AnnotationGetStyle(int line)
	{
		return Call(SCI_ANNOTATIONGETSTYLE, static_cast<WPARAM>(line));
	}

	void AnnotationSetStyles(int line, const char* styles)
	{
		Call(SCI_ANNOTATIONSETSTYLES, static_cast<WPARAM>(line), reinterpret_cast<LPARAM>(styles));
	}

	int AnnotationGetStyles(int line, char* styles)
	{
		return Call(SCI_ANNOTATIONGETSTYLES, static_cast<WPARAM>(line), reinterpret_cast<LPARAM>(styles));
	}

	int AnnotationGetLines(int line)
	{
		return Call(SCI_ANNOTATIONGETLINES, static_cast<WPARAM>(line));
	}

	void AnnotationClearAll()
	{
		Call(SCI_ANNOTATIONCLEARALL);
	}

	void AnnotationSetVisible(int visible)
	{
		Call(SCI_ANNOTATIONSETVISIBLE, static_cast<WPARAM>(visible));
	}

	int AnnotationGetVisible()
	{
		return Call(SCI_ANNOTATIONGETVISIBLE);
	}

	void AnnotationSetStyleOffset(int style)
	{
		Call(SCI_ANNOTATIONSETSTYLEOFFSET, static_cast<WPARAM>(style));
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
		return Call(SCI_ALLOCATEEXTENDEDSTYLES, static_cast<WPARAM>(numberStyles));
	}

	void AddUndoAction(int token, int flags)
	{
		Call(SCI_ADDUNDOACTION, static_cast<WPARAM>(token), flags);
	}

	int CharPositionFromPoint(int x, int y)
	{
		return Call(SCI_CHARPOSITIONFROMPOINT, static_cast<WPARAM>(x), y);
	}

	int CharPositionFromPointClose(int x, int y)
	{
		return Call(SCI_CHARPOSITIONFROMPOINTCLOSE, static_cast<WPARAM>(x), y);
	}

	void SetMouseSelectionRectangularSwitch(bool mouseSelectionRectangularSwitch)
	{
		Call(SCI_SETMOUSESELECTIONRECTANGULARSWITCH, static_cast<WPARAM>(mouseSelectionRectangularSwitch));
	}

	bool GetMouseSelectionRectangularSwitch()
	{
		return Call(SCI_GETMOUSESELECTIONRECTANGULARSWITCH);
	}

	void SetMultipleSelection(bool multipleSelection)
	{
		Call(SCI_SETMULTIPLESELECTION, static_cast<WPARAM>(multipleSelection));
	}

	bool GetMultipleSelection()
	{
		return Call(SCI_GETMULTIPLESELECTION);
	}

	void SetAdditionalSelectionTyping(bool additionalSelectionTyping)
	{
		Call(SCI_SETADDITIONALSELECTIONTYPING, static_cast<WPARAM>(additionalSelectionTyping));
	}

	bool GetAdditionalSelectionTyping()
	{
		return Call(SCI_GETADDITIONALSELECTIONTYPING);
	}

	void SetAdditionalCaretsBlink(bool additionalCaretsBlink)
	{
		Call(SCI_SETADDITIONALCARETSBLINK, static_cast<WPARAM>(additionalCaretsBlink));
	}

	bool GetAdditionalCaretsBlink()
	{
		return Call(SCI_GETADDITIONALCARETSBLINK);
	}

	void SetAdditionalCaretsVisible(bool additionalCaretsVisible)
	{
		Call(SCI_SETADDITIONALCARETSVISIBLE, static_cast<WPARAM>(additionalCaretsVisible));
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

	void SetSelection(int caret, int anchor)
	{
		Call(SCI_SETSELECTION, static_cast<WPARAM>(caret), anchor);
	}

	void AddSelection(int caret, int anchor)
	{
		Call(SCI_ADDSELECTION, static_cast<WPARAM>(caret), anchor);
	}

	void DropSelectionN(int selection)
	{
		Call(SCI_DROPSELECTIONN, static_cast<WPARAM>(selection));
	}

	void SetMainSelection(int selection)
	{
		Call(SCI_SETMAINSELECTION, static_cast<WPARAM>(selection));
	}

	int GetMainSelection()
	{
		return Call(SCI_GETMAINSELECTION);
	}

	void SetSelectionNCaret(int selection, int caret)
	{
		Call(SCI_SETSELECTIONNCARET, static_cast<WPARAM>(selection), caret);
	}

	int GetSelectionNCaret(int selection)
	{
		return Call(SCI_GETSELECTIONNCARET, static_cast<WPARAM>(selection));
	}

	void SetSelectionNAnchor(int selection, int anchor)
	{
		Call(SCI_SETSELECTIONNANCHOR, static_cast<WPARAM>(selection), anchor);
	}

	int GetSelectionNAnchor(int selection)
	{
		return Call(SCI_GETSELECTIONNANCHOR, static_cast<WPARAM>(selection));
	}

	void SetSelectionNCaretVirtualSpace(int selection, int space)
	{
		Call(SCI_SETSELECTIONNCARETVIRTUALSPACE, static_cast<WPARAM>(selection), space);
	}

	int GetSelectionNCaretVirtualSpace(int selection)
	{
		return Call(SCI_GETSELECTIONNCARETVIRTUALSPACE, static_cast<WPARAM>(selection));
	}

	void SetSelectionNAnchorVirtualSpace(int selection, int space)
	{
		Call(SCI_SETSELECTIONNANCHORVIRTUALSPACE, static_cast<WPARAM>(selection), space);
	}

	int GetSelectionNAnchorVirtualSpace(int selection)
	{
		return Call(SCI_GETSELECTIONNANCHORVIRTUALSPACE, static_cast<WPARAM>(selection));
	}

	void SetSelectionNStart(int selection, int anchor)
	{
		Call(SCI_SETSELECTIONNSTART, static_cast<WPARAM>(selection), anchor);
	}

	int GetSelectionNStart(int selection)
	{
		return Call(SCI_GETSELECTIONNSTART, static_cast<WPARAM>(selection));
	}

	void SetSelectionNEnd(int selection, int caret)
	{
		Call(SCI_SETSELECTIONNEND, static_cast<WPARAM>(selection), caret);
	}

	int GetSelectionNEnd(int selection)
	{
		return Call(SCI_GETSELECTIONNEND, static_cast<WPARAM>(selection));
	}

	void SetRectangularSelectionCaret(int caret)
	{
		Call(SCI_SETRECTANGULARSELECTIONCARET, static_cast<WPARAM>(caret));
	}

	int GetRectangularSelectionCaret()
	{
		return Call(SCI_GETRECTANGULARSELECTIONCARET);
	}

	void SetRectangularSelectionAnchor(int anchor)
	{
		Call(SCI_SETRECTANGULARSELECTIONANCHOR, static_cast<WPARAM>(anchor));
	}

	int GetRectangularSelectionAnchor()
	{
		return Call(SCI_GETRECTANGULARSELECTIONANCHOR);
	}

	void SetRectangularSelectionCaretVirtualSpace(int space)
	{
		Call(SCI_SETRECTANGULARSELECTIONCARETVIRTUALSPACE, static_cast<WPARAM>(space));
	}

	int GetRectangularSelectionCaretVirtualSpace()
	{
		return Call(SCI_GETRECTANGULARSELECTIONCARETVIRTUALSPACE);
	}

	void SetRectangularSelectionAnchorVirtualSpace(int space)
	{
		Call(SCI_SETRECTANGULARSELECTIONANCHORVIRTUALSPACE, static_cast<WPARAM>(space));
	}

	int GetRectangularSelectionAnchorVirtualSpace()
	{
		return Call(SCI_GETRECTANGULARSELECTIONANCHORVIRTUALSPACE);
	}

	void SetVirtualSpaceOptions(int virtualSpaceOptions)
	{
		Call(SCI_SETVIRTUALSPACEOPTIONS, static_cast<WPARAM>(virtualSpaceOptions));
	}

	int GetVirtualSpaceOptions()
	{
		return Call(SCI_GETVIRTUALSPACEOPTIONS);
	}

	void SetRectangularSelectionModifier(int modifier)
	{
		Call(SCI_SETRECTANGULARSELECTIONMODIFIER, static_cast<WPARAM>(modifier));
	}

	int GetRectangularSelectionModifier()
	{
		return Call(SCI_GETRECTANGULARSELECTIONMODIFIER);
	}

	void SetAdditionalSelFore(int fore)
	{
		Call(SCI_SETADDITIONALSELFORE, static_cast<WPARAM>(fore));
	}

	void SetAdditionalSelBack(int back)
	{
		Call(SCI_SETADDITIONALSELBACK, static_cast<WPARAM>(back));
	}

	void SetAdditionalSelAlpha(int alpha)
	{
		Call(SCI_SETADDITIONALSELALPHA, static_cast<WPARAM>(alpha));
	}

	int GetAdditionalSelAlpha()
	{
		return Call(SCI_GETADDITIONALSELALPHA);
	}

	void SetAdditionalCaretFore(int fore)
	{
		Call(SCI_SETADDITIONALCARETFORE, static_cast<WPARAM>(fore));
	}

	int GetAdditionalCaretFore()
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

	int ChangeLexerState(int start, int end)
	{
		return Call(SCI_CHANGELEXERSTATE, static_cast<WPARAM>(start), end);
	}

	int ContractedFoldNext(int lineStart)
	{
		return Call(SCI_CONTRACTEDFOLDNEXT, static_cast<WPARAM>(lineStart));
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
		Call(SCI_SETIDENTIFIER, static_cast<WPARAM>(identifier));
	}

	int GetIdentifier()
	{
		return Call(SCI_GETIDENTIFIER);
	}

	void RGBAImageSetWidth(int width)
	{
		Call(SCI_RGBAIMAGESETWIDTH, static_cast<WPARAM>(width));
	}

	void RGBAImageSetHeight(int height)
	{
		Call(SCI_RGBAIMAGESETHEIGHT, static_cast<WPARAM>(height));
	}

	void RGBAImageSetScale(int scalePercent)
	{
		Call(SCI_RGBAIMAGESETSCALE, static_cast<WPARAM>(scalePercent));
	}

	void MarkerDefineRGBAImage(int markerNumber, const char* pixels)
	{
		Call(SCI_MARKERDEFINERGBAIMAGE, static_cast<WPARAM>(markerNumber), reinterpret_cast<LPARAM>(pixels));
	}

	void RegisterRGBAImage(int type, const char* pixels)
	{
		Call(SCI_REGISTERRGBAIMAGE, static_cast<WPARAM>(type), reinterpret_cast<LPARAM>(pixels));
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
		Call(SCI_SETTECHNOLOGY, static_cast<WPARAM>(technology));
	}

	int GetTechnology()
	{
		return Call(SCI_GETTECHNOLOGY);
	}

	void* CreateLoader(int bytes, int documentOptions)
	{
		return Call(SCI_CREATELOADER, static_cast<WPARAM>(bytes), documentOptions);
	}

	void FindIndicatorShow(int start, int end)
	{
		Call(SCI_FINDINDICATORSHOW, static_cast<WPARAM>(start), end);
	}

	void FindIndicatorFlash(int start, int end)
	{
		Call(SCI_FINDINDICATORFLASH, static_cast<WPARAM>(start), end);
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
		Call(SCI_SETCARETLINEVISIBLEALWAYS, static_cast<WPARAM>(alwaysVisible));
	}

	void SetLineEndTypesAllowed(int lineEndBitSet)
	{
		Call(SCI_SETLINEENDTYPESALLOWED, static_cast<WPARAM>(lineEndBitSet));
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
		Call(SCI_SETREPRESENTATION, reinterpret_cast<WPARAM>(encodedCharacter), reinterpret_cast<LPARAM>(representation));
	}

	int GetRepresentation(const char* encodedCharacter, char* representation)
	{
		return Call(SCI_GETREPRESENTATION, reinterpret_cast<WPARAM>(encodedCharacter), reinterpret_cast<LPARAM>(representation));
	}

	void ClearRepresentation(const char* encodedCharacter)
	{
		Call(SCI_CLEARREPRESENTATION, reinterpret_cast<WPARAM>(encodedCharacter));
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
		Call(SCI_SETLEXER, static_cast<WPARAM>(lexer));
	}

	int GetLexer()
	{
		return Call(SCI_GETLEXER);
	}

	void Colourise(int start, int end)
	{
		Call(SCI_COLOURISE, static_cast<WPARAM>(start), end);
	}

	void SetProperty(const char* key, const char* value)
	{
		Call(SCI_SETPROPERTY, reinterpret_cast<WPARAM>(key), reinterpret_cast<LPARAM>(value));
	}

	void SetKeyWords(int keyWordSet, const char* keyWords)
	{
		Call(SCI_SETKEYWORDS, static_cast<WPARAM>(keyWordSet), reinterpret_cast<LPARAM>(keyWords));
	}

	void SetLexerLanguage(const char* language)
	{
		Call(SCI_SETLEXERLANGUAGE, 0, reinterpret_cast<LPARAM>(language));
	}

	void LoadLexerLibrary(const char* path)
	{
		Call(SCI_LOADLEXERLIBRARY, 0, reinterpret_cast<LPARAM>(path));
	}

	int GetProperty(const char* key, char* value)
	{
		return Call(SCI_GETPROPERTY, reinterpret_cast<WPARAM>(key), reinterpret_cast<LPARAM>(value));
	}

	int GetPropertyExpanded(const char* key, char* value)
	{
		return Call(SCI_GETPROPERTYEXPANDED, reinterpret_cast<WPARAM>(key), reinterpret_cast<LPARAM>(value));
	}

	int GetPropertyInt(const char* key, int defaultValue)
	{
		return Call(SCI_GETPROPERTYINT, reinterpret_cast<WPARAM>(key), defaultValue);
	}

	int GetLexerLanguage(char* language)
	{
		return Call(SCI_GETLEXERLANGUAGE, 0, reinterpret_cast<LPARAM>(language));
	}

	void* PrivateLexerCall(int operation, void* pointer)
	{
		return Call(SCI_PRIVATELEXERCALL, static_cast<WPARAM>(operation), reinterpret_cast<LPARAM>(pointer));
	}

	int PropertyNames(char* names)
	{
		return Call(SCI_PROPERTYNAMES, 0, reinterpret_cast<LPARAM>(names));
	}

	int PropertyType(const char* name)
	{
		return Call(SCI_PROPERTYTYPE, reinterpret_cast<WPARAM>(name));
	}

	int DescribeProperty(const char* name, char* description)
	{
		return Call(SCI_DESCRIBEPROPERTY, reinterpret_cast<WPARAM>(name), reinterpret_cast<LPARAM>(description));
	}

	int DescribeKeyWordSets(char* descriptions)
	{
		return Call(SCI_DESCRIBEKEYWORDSETS, 0, reinterpret_cast<LPARAM>(descriptions));
	}

	int GetLineEndTypesSupported()
	{
		return Call(SCI_GETLINEENDTYPESSUPPORTED);
	}

	int AllocateSubStyles(int styleBase, int numberStyles)
	{
		return Call(SCI_ALLOCATESUBSTYLES, static_cast<WPARAM>(styleBase), numberStyles);
	}

	int GetSubStylesStart(int styleBase)
	{
		return Call(SCI_GETSUBSTYLESSTART, static_cast<WPARAM>(styleBase));
	}

	int GetSubStylesLength(int styleBase)
	{
		return Call(SCI_GETSUBSTYLESLENGTH, static_cast<WPARAM>(styleBase));
	}

	int GetStyleFromSubStyle(int subStyle)
	{
		return Call(SCI_GETSTYLEFROMSUBSTYLE, static_cast<WPARAM>(subStyle));
	}

	int GetPrimaryStyleFromStyle(int style)
	{
		return Call(SCI_GETPRIMARYSTYLEFROMSTYLE, static_cast<WPARAM>(style));
	}

	void FreeSubStyles()
	{
		Call(SCI_FREESUBSTYLES);
	}

	void SetIdentifiers(int style, const char* identifiers)
	{
		Call(SCI_SETIDENTIFIERS, static_cast<WPARAM>(style), reinterpret_cast<LPARAM>(identifiers));
	}

	int DistanceToSecondaryStyles()
	{
		return Call(SCI_DISTANCETOSECONDARYSTYLES);
	}

	int GetSubStyleBases(char* styles)
	{
		return Call(SCI_GETSUBSTYLEBASES, 0, reinterpret_cast<LPARAM>(styles));
	}

	int GetNamedStyles()
	{
		return Call(SCI_GETNAMEDSTYLES);
	}

	int NameOfStyle(int style, char* name)
	{
		return Call(SCI_NAMEOFSTYLE, static_cast<WPARAM>(style), reinterpret_cast<LPARAM>(name));
	}

	int TagsOfStyle(int style, char* tags)
	{
		return Call(SCI_TAGSOFSTYLE, static_cast<WPARAM>(style), reinterpret_cast<LPARAM>(tags));
	}

	int DescriptionOfStyle(int style, char* description)
	{
		return Call(SCI_DESCRIPTIONOFSTYLE, static_cast<WPARAM>(style), reinterpret_cast<LPARAM>(description));
	}

	int GetBidirectional()
	{
		return Call(SCI_GETBIDIRECTIONAL);
	}

	void SetBidirectional(int bidirectional)
	{
		Call(SCI_SETBIDIRECTIONAL, static_cast<WPARAM>(bidirectional));
	}

	int GetLineCharacterIndex()
	{
		return Call(SCI_GETLINECHARACTERINDEX);
	}

	void AllocateLineCharacterIndex(int lineCharacterIndex)
	{
		Call(SCI_ALLOCATELINECHARACTERINDEX, static_cast<WPARAM>(lineCharacterIndex));
	}

	void ReleaseLineCharacterIndex(int lineCharacterIndex)
	{
		Call(SCI_RELEASELINECHARACTERINDEX, static_cast<WPARAM>(lineCharacterIndex));
	}

	int LineFromIndexPosition(int pos, int lineCharacterIndex)
	{
		return Call(SCI_LINEFROMINDEXPOSITION, static_cast<WPARAM>(pos), lineCharacterIndex);
	}

	int IndexPositionFromLine(int line, int lineCharacterIndex)
	{
		return Call(SCI_INDEXPOSITIONFROMLINE, static_cast<WPARAM>(line), lineCharacterIndex);
	}

};

class CScintillaCtrl : public CScintillaImpl<CScintillaCtrl>
{
	DECLARE_WND_SUPERCLASS(_T("WTL_ScintillaCtrl"), GetWndClassName())
};
