#pragma once

template <class T>
class CScintillaImpl : public CWindowImpl<T, CWindow, CControlWinTraits>
{
public:
	DECLARE_WND_SUPERCLASS2(NULL, CScintillaImpl, CWindow::GetWndClassName())

	LRESULT Call(UINT Msg, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		ATLASSERT(::IsWindow(this->m_hWnd));
		return ::SendMessage(this->m_hWnd, Msg, wParam, lParam);
	}

	void AddText(int length, const char* text)
	{
		Call(SCI_ADDTEXT, (WPARAM)length, (LPARAM)text);
	}

	void AddStyledText(int length, const char* c)
	{
		Call(SCI_ADDSTYLEDTEXT, (WPARAM)length, (LPARAM)c);
	}

	void InsertText(int pos, const char* text)
	{
		Call(SCI_INSERTTEXT, (WPARAM)pos, (LPARAM)text);
	}

	void ChangeInsertion(int length, const char* text)
	{
		Call(SCI_CHANGEINSERTION, (WPARAM)length, (LPARAM)text);
	}

	void ClearAll()
	{
		Call(SCI_CLEARALL, 0, 0);
	}

	void DeleteRange(int start, int lengthDelete)
	{
		Call(SCI_DELETERANGE, (WPARAM)start, (LPARAM)lengthDelete);
	}

	void ClearDocumentStyle()
	{
		Call(SCI_CLEARDOCUMENTSTYLE, 0, 0);
	}

	int GetLength()
	{
		return Call(SCI_GETLENGTH, 0, 0);
	}

	int GetCharAt(int pos)
	{
		return Call(SCI_GETCHARAT, (WPARAM)pos, 0);
	}

	int GetCurrentPos()
	{
		return Call(SCI_GETCURRENTPOS, 0, 0);
	}

	int GetAnchor()
	{
		return Call(SCI_GETANCHOR, 0, 0);
	}

	int GetStyleAt(int pos)
	{
		return Call(SCI_GETSTYLEAT, (WPARAM)pos, 0);
	}

	void Redo()
	{
		Call(SCI_REDO, 0, 0);
	}

	void SetUndoCollection(bool collectUndo)
	{
		Call(SCI_SETUNDOCOLLECTION, (WPARAM)collectUndo, 0);
	}

	void SelectAll()
	{
		Call(SCI_SELECTALL, 0, 0);
	}

	void SetSavePoint()
	{
		Call(SCI_SETSAVEPOINT, 0, 0);
	}

	int GetStyledText(void* tr)
	{
		return Call(SCI_GETSTYLEDTEXT, 0, (LPARAM)tr);
	}

	bool CanRedo()
	{
		return Call(SCI_CANREDO, 0, 0);
	}

	int MarkerLineFromHandle(int markerHandle)
	{
		return Call(SCI_MARKERLINEFROMHANDLE, (WPARAM)markerHandle, 0);
	}

	void MarkerDeleteHandle(int markerHandle)
	{
		Call(SCI_MARKERDELETEHANDLE, (WPARAM)markerHandle, 0);
	}

	bool GetUndoCollection()
	{
		return Call(SCI_GETUNDOCOLLECTION, 0, 0);
	}

	int GetViewWS()
	{
		return Call(SCI_GETVIEWWS, 0, 0);
	}

	void SetViewWS(int viewWS)
	{
		Call(SCI_SETVIEWWS, (WPARAM)viewWS, 0);
	}

	int GetTabDrawMode()
	{
		return Call(SCI_GETTABDRAWMODE, 0, 0);
	}

	void SetTabDrawMode(int tabDrawMode)
	{
		Call(SCI_SETTABDRAWMODE, (WPARAM)tabDrawMode, 0);
	}

	int PositionFromPoint(int x, int y)
	{
		return Call(SCI_POSITIONFROMPOINT, (WPARAM)x, (LPARAM)y);
	}

	int PositionFromPointClose(int x, int y)
	{
		return Call(SCI_POSITIONFROMPOINTCLOSE, (WPARAM)x, (LPARAM)y);
	}

	void GotoLine(int line)
	{
		Call(SCI_GOTOLINE, (WPARAM)line, 0);
	}

	void GotoPos(int caret)
	{
		Call(SCI_GOTOPOS, (WPARAM)caret, 0);
	}

	void SetAnchor(int anchor)
	{
		Call(SCI_SETANCHOR, (WPARAM)anchor, 0);
	}

	int GetCurLine(int length, char* text)
	{
		return Call(SCI_GETCURLINE, (WPARAM)length, (LPARAM)text);
	}

	int GetEndStyled()
	{
		return Call(SCI_GETENDSTYLED, 0, 0);
	}

	void ConvertEOLs(int eolMode)
	{
		Call(SCI_CONVERTEOLS, (WPARAM)eolMode, 0);
	}

	int GetEOLMode()
	{
		return Call(SCI_GETEOLMODE, 0, 0);
	}

	void SetEOLMode(int eolMode)
	{
		Call(SCI_SETEOLMODE, (WPARAM)eolMode, 0);
	}

	void StartStyling(int start, int unused)
	{
		Call(SCI_STARTSTYLING, (WPARAM)start, (LPARAM)unused);
	}

	void SetStyling(int length, int style)
	{
		Call(SCI_SETSTYLING, (WPARAM)length, (LPARAM)style);
	}

	bool GetBufferedDraw()
	{
		return Call(SCI_GETBUFFEREDDRAW, 0, 0);
	}

	void SetBufferedDraw(bool buffered)
	{
		Call(SCI_SETBUFFEREDDRAW, (WPARAM)buffered, 0);
	}

	void SetTabWidth(int tabWidth)
	{
		Call(SCI_SETTABWIDTH, (WPARAM)tabWidth, 0);
	}

	int GetTabWidth()
	{
		return Call(SCI_GETTABWIDTH, 0, 0);
	}

	void ClearTabStops(int line)
	{
		Call(SCI_CLEARTABSTOPS, (WPARAM)line, 0);
	}

	void AddTabStop(int line, int x)
	{
		Call(SCI_ADDTABSTOP, (WPARAM)line, (LPARAM)x);
	}

	int GetNextTabStop(int line, int x)
	{
		return Call(SCI_GETNEXTTABSTOP, (WPARAM)line, (LPARAM)x);
	}

	void SetCodePage(int codePage)
	{
		Call(SCI_SETCODEPAGE, (WPARAM)codePage, 0);
	}

	int GetIMEInteraction()
	{
		return Call(SCI_GETIMEINTERACTION, 0, 0);
	}

	void SetIMEInteraction(int imeInteraction)
	{
		Call(SCI_SETIMEINTERACTION, (WPARAM)imeInteraction, 0);
	}

	void MarkerDefine(int markerNumber, int markerSymbol)
	{
		Call(SCI_MARKERDEFINE, (WPARAM)markerNumber, (LPARAM)markerSymbol);
	}

	void MarkerSetFore(int markerNumber, int fore)
	{
		Call(SCI_MARKERSETFORE, (WPARAM)markerNumber, (LPARAM)fore);
	}

	void MarkerSetBack(int markerNumber, int back)
	{
		Call(SCI_MARKERSETBACK, (WPARAM)markerNumber, (LPARAM)back);
	}

	void MarkerSetBackSelected(int markerNumber, int back)
	{
		Call(SCI_MARKERSETBACKSELECTED, (WPARAM)markerNumber, (LPARAM)back);
	}

	void MarkerEnableHighlight(bool enabled)
	{
		Call(SCI_MARKERENABLEHIGHLIGHT, (WPARAM)enabled, 0);
	}

	int MarkerAdd(int line, int markerNumber)
	{
		return Call(SCI_MARKERADD, (WPARAM)line, (LPARAM)markerNumber);
	}

	void MarkerDelete(int line, int markerNumber)
	{
		Call(SCI_MARKERDELETE, (WPARAM)line, (LPARAM)markerNumber);
	}

	void MarkerDeleteAll(int markerNumber)
	{
		Call(SCI_MARKERDELETEALL, (WPARAM)markerNumber, 0);
	}

	int MarkerGet(int line)
	{
		return Call(SCI_MARKERGET, (WPARAM)line, 0);
	}

	int MarkerNext(int lineStart, int markerMask)
	{
		return Call(SCI_MARKERNEXT, (WPARAM)lineStart, (LPARAM)markerMask);
	}

	int MarkerPrevious(int lineStart, int markerMask)
	{
		return Call(SCI_MARKERPREVIOUS, (WPARAM)lineStart, (LPARAM)markerMask);
	}

	void MarkerDefinePixmap(int markerNumber, const char* pixmap)
	{
		Call(SCI_MARKERDEFINEPIXMAP, (WPARAM)markerNumber, (LPARAM)pixmap);
	}

	void MarkerAddSet(int line, int markerSet)
	{
		Call(SCI_MARKERADDSET, (WPARAM)line, (LPARAM)markerSet);
	}

	void MarkerSetAlpha(int markerNumber, int alpha)
	{
		Call(SCI_MARKERSETALPHA, (WPARAM)markerNumber, (LPARAM)alpha);
	}

	void SetMarginTypeN(int margin, int marginType)
	{
		Call(SCI_SETMARGINTYPEN, (WPARAM)margin, (LPARAM)marginType);
	}

	int GetMarginTypeN(int margin)
	{
		return Call(SCI_GETMARGINTYPEN, (WPARAM)margin, 0);
	}

	void SetMarginWidthN(int margin, int pixelWidth)
	{
		Call(SCI_SETMARGINWIDTHN, (WPARAM)margin, (LPARAM)pixelWidth);
	}

	int GetMarginWidthN(int margin)
	{
		return Call(SCI_GETMARGINWIDTHN, (WPARAM)margin, 0);
	}

	void SetMarginMaskN(int margin, int mask)
	{
		Call(SCI_SETMARGINMASKN, (WPARAM)margin, (LPARAM)mask);
	}

	int GetMarginMaskN(int margin)
	{
		return Call(SCI_GETMARGINMASKN, (WPARAM)margin, 0);
	}

	void SetMarginSensitiveN(int margin, bool sensitive)
	{
		Call(SCI_SETMARGINSENSITIVEN, (WPARAM)margin, (LPARAM)sensitive);
	}

	bool GetMarginSensitiveN(int margin)
	{
		return Call(SCI_GETMARGINSENSITIVEN, (WPARAM)margin, 0);
	}

	void SetMarginCursorN(int margin, int cursor)
	{
		Call(SCI_SETMARGINCURSORN, (WPARAM)margin, (LPARAM)cursor);
	}

	int GetMarginCursorN(int margin)
	{
		return Call(SCI_GETMARGINCURSORN, (WPARAM)margin, 0);
	}

	void SetMarginBackN(int margin, int back)
	{
		Call(SCI_SETMARGINBACKN, (WPARAM)margin, (LPARAM)back);
	}

	int GetMarginBackN(int margin)
	{
		return Call(SCI_GETMARGINBACKN, (WPARAM)margin, 0);
	}

	void SetMargins(int margins)
	{
		Call(SCI_SETMARGINS, (WPARAM)margins, 0);
	}

	int GetMargins()
	{
		return Call(SCI_GETMARGINS, 0, 0);
	}

	void StyleClearAll()
	{
		Call(SCI_STYLECLEARALL, 0, 0);
	}

	void StyleSetFore(int style, int fore)
	{
		Call(SCI_STYLESETFORE, (WPARAM)style, (LPARAM)fore);
	}

	void StyleSetBack(int style, int back)
	{
		Call(SCI_STYLESETBACK, (WPARAM)style, (LPARAM)back);
	}

	void StyleSetBold(int style, bool bold)
	{
		Call(SCI_STYLESETBOLD, (WPARAM)style, (LPARAM)bold);
	}

	void StyleSetItalic(int style, bool italic)
	{
		Call(SCI_STYLESETITALIC, (WPARAM)style, (LPARAM)italic);
	}

	void StyleSetSize(int style, int sizePoints)
	{
		Call(SCI_STYLESETSIZE, (WPARAM)style, (LPARAM)sizePoints);
	}

	void StyleSetFont(int style, const char* fontName)
	{
		Call(SCI_STYLESETFONT, (WPARAM)style, (LPARAM)fontName);
	}

	void StyleSetEOLFilled(int style, bool eolFilled)
	{
		Call(SCI_STYLESETEOLFILLED, (WPARAM)style, (LPARAM)eolFilled);
	}

	void StyleResetDefault()
	{
		Call(SCI_STYLERESETDEFAULT, 0, 0);
	}

	void StyleSetUnderline(int style, bool underline)
	{
		Call(SCI_STYLESETUNDERLINE, (WPARAM)style, (LPARAM)underline);
	}

	int StyleGetFore(int style)
	{
		return Call(SCI_STYLEGETFORE, (WPARAM)style, 0);
	}

	int StyleGetBack(int style)
	{
		return Call(SCI_STYLEGETBACK, (WPARAM)style, 0);
	}

	bool StyleGetBold(int style)
	{
		return Call(SCI_STYLEGETBOLD, (WPARAM)style, 0);
	}

	bool StyleGetItalic(int style)
	{
		return Call(SCI_STYLEGETITALIC, (WPARAM)style, 0);
	}

	int StyleGetSize(int style)
	{
		return Call(SCI_STYLEGETSIZE, (WPARAM)style, 0);
	}

	int StyleGetFont(int style, char* fontName)
	{
		return Call(SCI_STYLEGETFONT, (WPARAM)style, (LPARAM)fontName);
	}

	bool StyleGetEOLFilled(int style)
	{
		return Call(SCI_STYLEGETEOLFILLED, (WPARAM)style, 0);
	}

	bool StyleGetUnderline(int style)
	{
		return Call(SCI_STYLEGETUNDERLINE, (WPARAM)style, 0);
	}

	int StyleGetCase(int style)
	{
		return Call(SCI_STYLEGETCASE, (WPARAM)style, 0);
	}

	int StyleGetCharacterSet(int style)
	{
		return Call(SCI_STYLEGETCHARACTERSET, (WPARAM)style, 0);
	}

	bool StyleGetVisible(int style)
	{
		return Call(SCI_STYLEGETVISIBLE, (WPARAM)style, 0);
	}

	bool StyleGetChangeable(int style)
	{
		return Call(SCI_STYLEGETCHANGEABLE, (WPARAM)style, 0);
	}

	bool StyleGetHotSpot(int style)
	{
		return Call(SCI_STYLEGETHOTSPOT, (WPARAM)style, 0);
	}

	void StyleSetCase(int style, int caseVisible)
	{
		Call(SCI_STYLESETCASE, (WPARAM)style, (LPARAM)caseVisible);
	}

	void StyleSetSizeFractional(int style, int sizeHundredthPoints)
	{
		Call(SCI_STYLESETSIZEFRACTIONAL, (WPARAM)style, (LPARAM)sizeHundredthPoints);
	}

	int StyleGetSizeFractional(int style)
	{
		return Call(SCI_STYLEGETSIZEFRACTIONAL, (WPARAM)style, 0);
	}

	void StyleSetWeight(int style, int weight)
	{
		Call(SCI_STYLESETWEIGHT, (WPARAM)style, (LPARAM)weight);
	}

	int StyleGetWeight(int style)
	{
		return Call(SCI_STYLEGETWEIGHT, (WPARAM)style, 0);
	}

	void StyleSetCharacterSet(int style, int characterSet)
	{
		Call(SCI_STYLESETCHARACTERSET, (WPARAM)style, (LPARAM)characterSet);
	}

	void StyleSetHotSpot(int style, bool hotspot)
	{
		Call(SCI_STYLESETHOTSPOT, (WPARAM)style, (LPARAM)hotspot);
	}

	void SetSelFore(bool useSetting, int fore)
	{
		Call(SCI_SETSELFORE, (WPARAM)useSetting, (LPARAM)fore);
	}

	void SetSelBack(bool useSetting, int back)
	{
		Call(SCI_SETSELBACK, (WPARAM)useSetting, (LPARAM)back);
	}

	int GetSelAlpha()
	{
		return Call(SCI_GETSELALPHA, 0, 0);
	}

	void SetSelAlpha(int alpha)
	{
		Call(SCI_SETSELALPHA, (WPARAM)alpha, 0);
	}

	bool GetSelEOLFilled()
	{
		return Call(SCI_GETSELEOLFILLED, 0, 0);
	}

	void SetSelEOLFilled(bool filled)
	{
		Call(SCI_SETSELEOLFILLED, (WPARAM)filled, 0);
	}

	void SetCaretFore(int fore)
	{
		Call(SCI_SETCARETFORE, (WPARAM)fore, 0);
	}

	void AssignCmdKey(int keyDefinition, int sciCommand)
	{
		Call(SCI_ASSIGNCMDKEY, (WPARAM)keyDefinition, (LPARAM)sciCommand);
	}

	void ClearCmdKey(int keyDefinition)
	{
		Call(SCI_CLEARCMDKEY, (WPARAM)keyDefinition, 0);
	}

	void ClearAllCmdKeys()
	{
		Call(SCI_CLEARALLCMDKEYS, 0, 0);
	}

	void SetStylingEx(int length, const char* styles)
	{
		Call(SCI_SETSTYLINGEX, (WPARAM)length, (LPARAM)styles);
	}

	void StyleSetVisible(int style, bool visible)
	{
		Call(SCI_STYLESETVISIBLE, (WPARAM)style, (LPARAM)visible);
	}

	int GetCaretPeriod()
	{
		return Call(SCI_GETCARETPERIOD, 0, 0);
	}

	void SetCaretPeriod(int periodMilliseconds)
	{
		Call(SCI_SETCARETPERIOD, (WPARAM)periodMilliseconds, 0);
	}

	void SetWordChars(const char* characters)
	{
		Call(SCI_SETWORDCHARS, 0, (LPARAM)characters);
	}

	int GetWordChars(char* characters)
	{
		return Call(SCI_GETWORDCHARS, 0, (LPARAM)characters);
	}

	void SetCharacterCategoryOptimization(int countCharacters)
	{
		Call(SCI_SETCHARACTERCATEGORYOPTIMIZATION, (WPARAM)countCharacters, 0);
	}

	int GetCharacterCategoryOptimization()
	{
		return Call(SCI_GETCHARACTERCATEGORYOPTIMIZATION, 0, 0);
	}

	void BeginUndoAction()
	{
		Call(SCI_BEGINUNDOACTION, 0, 0);
	}

	void EndUndoAction()
	{
		Call(SCI_ENDUNDOACTION, 0, 0);
	}

	void IndicSetStyle(int indicator, int indicatorStyle)
	{
		Call(SCI_INDICSETSTYLE, (WPARAM)indicator, (LPARAM)indicatorStyle);
	}

	int IndicGetStyle(int indicator)
	{
		return Call(SCI_INDICGETSTYLE, (WPARAM)indicator, 0);
	}

	void IndicSetFore(int indicator, int fore)
	{
		Call(SCI_INDICSETFORE, (WPARAM)indicator, (LPARAM)fore);
	}

	int IndicGetFore(int indicator)
	{
		return Call(SCI_INDICGETFORE, (WPARAM)indicator, 0);
	}

	void IndicSetUnder(int indicator, bool under)
	{
		Call(SCI_INDICSETUNDER, (WPARAM)indicator, (LPARAM)under);
	}

	bool IndicGetUnder(int indicator)
	{
		return Call(SCI_INDICGETUNDER, (WPARAM)indicator, 0);
	}

	void IndicSetHoverStyle(int indicator, int indicatorStyle)
	{
		Call(SCI_INDICSETHOVERSTYLE, (WPARAM)indicator, (LPARAM)indicatorStyle);
	}

	int IndicGetHoverStyle(int indicator)
	{
		return Call(SCI_INDICGETHOVERSTYLE, (WPARAM)indicator, 0);
	}

	void IndicSetHoverFore(int indicator, int fore)
	{
		Call(SCI_INDICSETHOVERFORE, (WPARAM)indicator, (LPARAM)fore);
	}

	int IndicGetHoverFore(int indicator)
	{
		return Call(SCI_INDICGETHOVERFORE, (WPARAM)indicator, 0);
	}

	void IndicSetFlags(int indicator, int flags)
	{
		Call(SCI_INDICSETFLAGS, (WPARAM)indicator, (LPARAM)flags);
	}

	int IndicGetFlags(int indicator)
	{
		return Call(SCI_INDICGETFLAGS, (WPARAM)indicator, 0);
	}

	void SetWhitespaceFore(bool useSetting, int fore)
	{
		Call(SCI_SETWHITESPACEFORE, (WPARAM)useSetting, (LPARAM)fore);
	}

	void SetWhitespaceBack(bool useSetting, int back)
	{
		Call(SCI_SETWHITESPACEBACK, (WPARAM)useSetting, (LPARAM)back);
	}

	void SetWhitespaceSize(int size)
	{
		Call(SCI_SETWHITESPACESIZE, (WPARAM)size, 0);
	}

	int GetWhitespaceSize()
	{
		return Call(SCI_GETWHITESPACESIZE, 0, 0);
	}

	void SetLineState(int line, int state)
	{
		Call(SCI_SETLINESTATE, (WPARAM)line, (LPARAM)state);
	}

	int GetLineState(int line)
	{
		return Call(SCI_GETLINESTATE, (WPARAM)line, 0);
	}

	int GetMaxLineState()
	{
		return Call(SCI_GETMAXLINESTATE, 0, 0);
	}

	bool GetCaretLineVisible()
	{
		return Call(SCI_GETCARETLINEVISIBLE, 0, 0);
	}

	void SetCaretLineVisible(bool show)
	{
		Call(SCI_SETCARETLINEVISIBLE, (WPARAM)show, 0);
	}

	int GetCaretLineBack()
	{
		return Call(SCI_GETCARETLINEBACK, 0, 0);
	}

	void SetCaretLineBack(int back)
	{
		Call(SCI_SETCARETLINEBACK, (WPARAM)back, 0);
	}

	int GetCaretLineFrame()
	{
		return Call(SCI_GETCARETLINEFRAME, 0, 0);
	}

	void SetCaretLineFrame(int width)
	{
		Call(SCI_SETCARETLINEFRAME, (WPARAM)width, 0);
	}

	void StyleSetChangeable(int style, bool changeable)
	{
		Call(SCI_STYLESETCHANGEABLE, (WPARAM)style, (LPARAM)changeable);
	}

	void AutoCShow(int lengthEntered, const char* itemList)
	{
		Call(SCI_AUTOCSHOW, (WPARAM)lengthEntered, (LPARAM)itemList);
	}

	void AutoCCancel()
	{
		Call(SCI_AUTOCCANCEL, 0, 0);
	}

	bool AutoCActive()
	{
		return Call(SCI_AUTOCACTIVE, 0, 0);
	}

	int AutoCPosStart()
	{
		return Call(SCI_AUTOCPOSSTART, 0, 0);
	}

	void AutoCComplete()
	{
		Call(SCI_AUTOCCOMPLETE, 0, 0);
	}

	void AutoCStops(const char* characterSet)
	{
		Call(SCI_AUTOCSTOPS, 0, (LPARAM)characterSet);
	}

	void AutoCSetSeparator(int separatorCharacter)
	{
		Call(SCI_AUTOCSETSEPARATOR, (WPARAM)separatorCharacter, 0);
	}

	int AutoCGetSeparator()
	{
		return Call(SCI_AUTOCGETSEPARATOR, 0, 0);
	}

	void AutoCSelect(const char* select)
	{
		Call(SCI_AUTOCSELECT, 0, (LPARAM)select);
	}

	void AutoCSetCancelAtStart(bool cancel)
	{
		Call(SCI_AUTOCSETCANCELATSTART, (WPARAM)cancel, 0);
	}

	bool AutoCGetCancelAtStart()
	{
		return Call(SCI_AUTOCGETCANCELATSTART, 0, 0);
	}

	void AutoCSetFillUps(const char* characterSet)
	{
		Call(SCI_AUTOCSETFILLUPS, 0, (LPARAM)characterSet);
	}

	void AutoCSetChooseSingle(bool chooseSingle)
	{
		Call(SCI_AUTOCSETCHOOSESINGLE, (WPARAM)chooseSingle, 0);
	}

	bool AutoCGetChooseSingle()
	{
		return Call(SCI_AUTOCGETCHOOSESINGLE, 0, 0);
	}

	void AutoCSetIgnoreCase(bool ignoreCase)
	{
		Call(SCI_AUTOCSETIGNORECASE, (WPARAM)ignoreCase, 0);
	}

	bool AutoCGetIgnoreCase()
	{
		return Call(SCI_AUTOCGETIGNORECASE, 0, 0);
	}

	void UserListShow(int listType, const char* itemList)
	{
		Call(SCI_USERLISTSHOW, (WPARAM)listType, (LPARAM)itemList);
	}

	void AutoCSetAutoHide(bool autoHide)
	{
		Call(SCI_AUTOCSETAUTOHIDE, (WPARAM)autoHide, 0);
	}

	bool AutoCGetAutoHide()
	{
		return Call(SCI_AUTOCGETAUTOHIDE, 0, 0);
	}

	void AutoCSetDropRestOfWord(bool dropRestOfWord)
	{
		Call(SCI_AUTOCSETDROPRESTOFWORD, (WPARAM)dropRestOfWord, 0);
	}

	bool AutoCGetDropRestOfWord()
	{
		return Call(SCI_AUTOCGETDROPRESTOFWORD, 0, 0);
	}

	void RegisterImage(int type, const char* xpmData)
	{
		Call(SCI_REGISTERIMAGE, (WPARAM)type, (LPARAM)xpmData);
	}

	void ClearRegisteredImages()
	{
		Call(SCI_CLEARREGISTEREDIMAGES, 0, 0);
	}

	int AutoCGetTypeSeparator()
	{
		return Call(SCI_AUTOCGETTYPESEPARATOR, 0, 0);
	}

	void AutoCSetTypeSeparator(int separatorCharacter)
	{
		Call(SCI_AUTOCSETTYPESEPARATOR, (WPARAM)separatorCharacter, 0);
	}

	void AutoCSetMaxWidth(int characterCount)
	{
		Call(SCI_AUTOCSETMAXWIDTH, (WPARAM)characterCount, 0);
	}

	int AutoCGetMaxWidth()
	{
		return Call(SCI_AUTOCGETMAXWIDTH, 0, 0);
	}

	void AutoCSetMaxHeight(int rowCount)
	{
		Call(SCI_AUTOCSETMAXHEIGHT, (WPARAM)rowCount, 0);
	}

	int AutoCGetMaxHeight()
	{
		return Call(SCI_AUTOCGETMAXHEIGHT, 0, 0);
	}

	void SetIndent(int indentSize)
	{
		Call(SCI_SETINDENT, (WPARAM)indentSize, 0);
	}

	int GetIndent()
	{
		return Call(SCI_GETINDENT, 0, 0);
	}

	void SetUseTabs(bool useTabs)
	{
		Call(SCI_SETUSETABS, (WPARAM)useTabs, 0);
	}

	bool GetUseTabs()
	{
		return Call(SCI_GETUSETABS, 0, 0);
	}

	void SetLineIndentation(int line, int indentation)
	{
		Call(SCI_SETLINEINDENTATION, (WPARAM)line, (LPARAM)indentation);
	}

	int GetLineIndentation(int line)
	{
		return Call(SCI_GETLINEINDENTATION, (WPARAM)line, 0);
	}

	int GetLineIndentPosition(int line)
	{
		return Call(SCI_GETLINEINDENTPOSITION, (WPARAM)line, 0);
	}

	int GetColumn(int pos)
	{
		return Call(SCI_GETCOLUMN, (WPARAM)pos, 0);
	}

	int CountCharacters(int start, int end)
	{
		return Call(SCI_COUNTCHARACTERS, (WPARAM)start, (LPARAM)end);
	}

	int CountCodeUnits(int start, int end)
	{
		return Call(SCI_COUNTCODEUNITS, (WPARAM)start, (LPARAM)end);
	}

	void SetHScrollBar(bool visible)
	{
		Call(SCI_SETHSCROLLBAR, (WPARAM)visible, 0);
	}

	bool GetHScrollBar()
	{
		return Call(SCI_GETHSCROLLBAR, 0, 0);
	}

	void SetIndentationGuides(int indentView)
	{
		Call(SCI_SETINDENTATIONGUIDES, (WPARAM)indentView, 0);
	}

	int GetIndentationGuides()
	{
		return Call(SCI_GETINDENTATIONGUIDES, 0, 0);
	}

	void SetHighlightGuide(int column)
	{
		Call(SCI_SETHIGHLIGHTGUIDE, (WPARAM)column, 0);
	}

	int GetHighlightGuide()
	{
		return Call(SCI_GETHIGHLIGHTGUIDE, 0, 0);
	}

	int GetLineEndPosition(int line)
	{
		return Call(SCI_GETLINEENDPOSITION, (WPARAM)line, 0);
	}

	int GetCodePage()
	{
		return Call(SCI_GETCODEPAGE, 0, 0);
	}

	int GetCaretFore()
	{
		return Call(SCI_GETCARETFORE, 0, 0);
	}

	bool GetReadOnly()
	{
		return Call(SCI_GETREADONLY, 0, 0);
	}

	void SetCurrentPos(int caret)
	{
		Call(SCI_SETCURRENTPOS, (WPARAM)caret, 0);
	}

	void SetSelectionStart(int anchor)
	{
		Call(SCI_SETSELECTIONSTART, (WPARAM)anchor, 0);
	}

	int GetSelectionStart()
	{
		return Call(SCI_GETSELECTIONSTART, 0, 0);
	}

	void SetSelectionEnd(int caret)
	{
		Call(SCI_SETSELECTIONEND, (WPARAM)caret, 0);
	}

	int GetSelectionEnd()
	{
		return Call(SCI_GETSELECTIONEND, 0, 0);
	}

	void SetEmptySelection(int caret)
	{
		Call(SCI_SETEMPTYSELECTION, (WPARAM)caret, 0);
	}

	void SetPrintMagnification(int magnification)
	{
		Call(SCI_SETPRINTMAGNIFICATION, (WPARAM)magnification, 0);
	}

	int GetPrintMagnification()
	{
		return Call(SCI_GETPRINTMAGNIFICATION, 0, 0);
	}

	void SetPrintColourMode(int mode)
	{
		Call(SCI_SETPRINTCOLOURMODE, (WPARAM)mode, 0);
	}

	int GetPrintColourMode()
	{
		return Call(SCI_GETPRINTCOLOURMODE, 0, 0);
	}

	int FindText(int searchFlags, void* ft)
	{
		return Call(SCI_FINDTEXT, (WPARAM)searchFlags, (LPARAM)ft);
	}

	int FormatRange(bool draw, void* fr)
	{
		return Call(SCI_FORMATRANGE, (WPARAM)draw, (LPARAM)fr);
	}

	int GetFirstVisibleLine()
	{
		return Call(SCI_GETFIRSTVISIBLELINE, 0, 0);
	}

	int GetLine(int line, char* text)
	{
		return Call(SCI_GETLINE, (WPARAM)line, (LPARAM)text);
	}

	int GetLineCount()
	{
		return Call(SCI_GETLINECOUNT, 0, 0);
	}

	void SetMarginLeft(int pixelWidth)
	{
		Call(SCI_SETMARGINLEFT, 0, (LPARAM)pixelWidth);
	}

	int GetMarginLeft()
	{
		return Call(SCI_GETMARGINLEFT, 0, 0);
	}

	void SetMarginRight(int pixelWidth)
	{
		Call(SCI_SETMARGINRIGHT, 0, (LPARAM)pixelWidth);
	}

	int GetMarginRight()
	{
		return Call(SCI_GETMARGINRIGHT, 0, 0);
	}

	bool GetModify()
	{
		return Call(SCI_GETMODIFY, 0, 0);
	}

	void SetSel(int anchor, int caret)
	{
		Call(SCI_SETSEL, (WPARAM)anchor, (LPARAM)caret);
	}

	int GetSelText(char* text)
	{
		return Call(SCI_GETSELTEXT, 0, (LPARAM)text);
	}

	int GetTextRange(void* tr)
	{
		return Call(SCI_GETTEXTRANGE, 0, (LPARAM)tr);
	}

	void HideSelection(bool hide)
	{
		Call(SCI_HIDESELECTION, (WPARAM)hide, 0);
	}

	int PointXFromPosition(int pos)
	{
		return Call(SCI_POINTXFROMPOSITION, 0, (LPARAM)pos);
	}

	int PointYFromPosition(int pos)
	{
		return Call(SCI_POINTYFROMPOSITION, 0, (LPARAM)pos);
	}

	int LineFromPosition(int pos)
	{
		return Call(SCI_LINEFROMPOSITION, (WPARAM)pos, 0);
	}

	int PositionFromLine(int line)
	{
		return Call(SCI_POSITIONFROMLINE, (WPARAM)line, 0);
	}

	void LineScroll(int columns, int lines)
	{
		Call(SCI_LINESCROLL, (WPARAM)columns, (LPARAM)lines);
	}

	void ScrollCaret()
	{
		Call(SCI_SCROLLCARET, 0, 0);
	}

	void ScrollRange(int secondary, int primary)
	{
		Call(SCI_SCROLLRANGE, (WPARAM)secondary, (LPARAM)primary);
	}

	void ReplaceSel(const char* text)
	{
		Call(SCI_REPLACESEL, 0, (LPARAM)text);
	}

	void SetReadOnly(bool readOnly)
	{
		Call(SCI_SETREADONLY, (WPARAM)readOnly, 0);
	}

	void Null()
	{
		Call(SCI_NULL, 0, 0);
	}

	bool CanPaste()
	{
		return Call(SCI_CANPASTE, 0, 0);
	}

	bool CanUndo()
	{
		return Call(SCI_CANUNDO, 0, 0);
	}

	void EmptyUndoBuffer()
	{
		Call(SCI_EMPTYUNDOBUFFER, 0, 0);
	}

	void Undo()
	{
		Call(SCI_UNDO, 0, 0);
	}

	void Cut()
	{
		Call(SCI_CUT, 0, 0);
	}

	void Copy()
	{
		Call(SCI_COPY, 0, 0);
	}

	void Paste()
	{
		Call(SCI_PASTE, 0, 0);
	}

	void Clear()
	{
		Call(SCI_CLEAR, 0, 0);
	}

	void SetText(const char* text)
	{
		Call(SCI_SETTEXT, 0, (LPARAM)text);
	}

	int GetText(int length, char* text)
	{
		return Call(SCI_GETTEXT, (WPARAM)length, (LPARAM)text);
	}

	int GetTextLength()
	{
		return Call(SCI_GETTEXTLENGTH, 0, 0);
	}

	void* GetDirectFunction()
	{
		return Call(SCI_GETDIRECTFUNCTION, 0, 0);
	}

	void* GetDirectPointer()
	{
		return Call(SCI_GETDIRECTPOINTER, 0, 0);
	}

	void SetOvertype(bool overType)
	{
		Call(SCI_SETOVERTYPE, (WPARAM)overType, 0);
	}

	bool GetOvertype()
	{
		return Call(SCI_GETOVERTYPE, 0, 0);
	}

	void SetCaretWidth(int pixelWidth)
	{
		Call(SCI_SETCARETWIDTH, (WPARAM)pixelWidth, 0);
	}

	int GetCaretWidth()
	{
		return Call(SCI_GETCARETWIDTH, 0, 0);
	}

	void SetTargetStart(int start)
	{
		Call(SCI_SETTARGETSTART, (WPARAM)start, 0);
	}

	int GetTargetStart()
	{
		return Call(SCI_GETTARGETSTART, 0, 0);
	}

	void SetTargetEnd(int end)
	{
		Call(SCI_SETTARGETEND, (WPARAM)end, 0);
	}

	int GetTargetEnd()
	{
		return Call(SCI_GETTARGETEND, 0, 0);
	}

	void SetTargetRange(int start, int end)
	{
		Call(SCI_SETTARGETRANGE, (WPARAM)start, (LPARAM)end);
	}

	int GetTargetText(char* text)
	{
		return Call(SCI_GETTARGETTEXT, 0, (LPARAM)text);
	}

	void TargetFromSelection()
	{
		Call(SCI_TARGETFROMSELECTION, 0, 0);
	}

	void TargetWholeDocument()
	{
		Call(SCI_TARGETWHOLEDOCUMENT, 0, 0);
	}

	int ReplaceTarget(int length, const char* text)
	{
		return Call(SCI_REPLACETARGET, (WPARAM)length, (LPARAM)text);
	}

	int ReplaceTargetRE(int length, const char* text)
	{
		return Call(SCI_REPLACETARGETRE, (WPARAM)length, (LPARAM)text);
	}

	int SearchInTarget(int length, const char* text)
	{
		return Call(SCI_SEARCHINTARGET, (WPARAM)length, (LPARAM)text);
	}

	void SetSearchFlags(int searchFlags)
	{
		Call(SCI_SETSEARCHFLAGS, (WPARAM)searchFlags, 0);
	}

	int GetSearchFlags()
	{
		return Call(SCI_GETSEARCHFLAGS, 0, 0);
	}

	void CallTipShow(int pos, const char* definition)
	{
		Call(SCI_CALLTIPSHOW, (WPARAM)pos, (LPARAM)definition);
	}

	void CallTipCancel()
	{
		Call(SCI_CALLTIPCANCEL, 0, 0);
	}

	bool CallTipActive()
	{
		return Call(SCI_CALLTIPACTIVE, 0, 0);
	}

	int CallTipPosStart()
	{
		return Call(SCI_CALLTIPPOSSTART, 0, 0);
	}

	void CallTipSetPosStart(int posStart)
	{
		Call(SCI_CALLTIPSETPOSSTART, (WPARAM)posStart, 0);
	}

	void CallTipSetHlt(int highlightStart, int highlightEnd)
	{
		Call(SCI_CALLTIPSETHLT, (WPARAM)highlightStart, (LPARAM)highlightEnd);
	}

	void CallTipSetBack(int back)
	{
		Call(SCI_CALLTIPSETBACK, (WPARAM)back, 0);
	}

	void CallTipSetFore(int fore)
	{
		Call(SCI_CALLTIPSETFORE, (WPARAM)fore, 0);
	}

	void CallTipSetForeHlt(int fore)
	{
		Call(SCI_CALLTIPSETFOREHLT, (WPARAM)fore, 0);
	}

	void CallTipUseStyle(int tabSize)
	{
		Call(SCI_CALLTIPUSESTYLE, (WPARAM)tabSize, 0);
	}

	void CallTipSetPosition(bool above)
	{
		Call(SCI_CALLTIPSETPOSITION, (WPARAM)above, 0);
	}

	int VisibleFromDocLine(int docLine)
	{
		return Call(SCI_VISIBLEFROMDOCLINE, (WPARAM)docLine, 0);
	}

	int DocLineFromVisible(int displayLine)
	{
		return Call(SCI_DOCLINEFROMVISIBLE, (WPARAM)displayLine, 0);
	}

	int WrapCount(int docLine)
	{
		return Call(SCI_WRAPCOUNT, (WPARAM)docLine, 0);
	}

	void SetFoldLevel(int line, int level)
	{
		Call(SCI_SETFOLDLEVEL, (WPARAM)line, (LPARAM)level);
	}

	int GetFoldLevel(int line)
	{
		return Call(SCI_GETFOLDLEVEL, (WPARAM)line, 0);
	}

	int GetLastChild(int line, int level)
	{
		return Call(SCI_GETLASTCHILD, (WPARAM)line, (LPARAM)level);
	}

	int GetFoldParent(int line)
	{
		return Call(SCI_GETFOLDPARENT, (WPARAM)line, 0);
	}

	void ShowLines(int lineStart, int lineEnd)
	{
		Call(SCI_SHOWLINES, (WPARAM)lineStart, (LPARAM)lineEnd);
	}

	void HideLines(int lineStart, int lineEnd)
	{
		Call(SCI_HIDELINES, (WPARAM)lineStart, (LPARAM)lineEnd);
	}

	bool GetLineVisible(int line)
	{
		return Call(SCI_GETLINEVISIBLE, (WPARAM)line, 0);
	}

	bool GetAllLinesVisible()
	{
		return Call(SCI_GETALLLINESVISIBLE, 0, 0);
	}

	void SetFoldExpanded(int line, bool expanded)
	{
		Call(SCI_SETFOLDEXPANDED, (WPARAM)line, (LPARAM)expanded);
	}

	bool GetFoldExpanded(int line)
	{
		return Call(SCI_GETFOLDEXPANDED, (WPARAM)line, 0);
	}

	void ToggleFold(int line)
	{
		Call(SCI_TOGGLEFOLD, (WPARAM)line, 0);
	}

	void ToggleFoldShowText(int line, const char* text)
	{
		Call(SCI_TOGGLEFOLDSHOWTEXT, (WPARAM)line, (LPARAM)text);
	}

	void FoldDisplayTextSetStyle(int style)
	{
		Call(SCI_FOLDDISPLAYTEXTSETSTYLE, (WPARAM)style, 0);
	}

	int FoldDisplayTextGetStyle()
	{
		return Call(SCI_FOLDDISPLAYTEXTGETSTYLE, 0, 0);
	}

	void SetDefaultFoldDisplayText(const char* text)
	{
		Call(SCI_SETDEFAULTFOLDDISPLAYTEXT, 0, (LPARAM)text);
	}

	int GetDefaultFoldDisplayText(char* text)
	{
		return Call(SCI_GETDEFAULTFOLDDISPLAYTEXT, 0, (LPARAM)text);
	}

	void FoldLine(int line, int action)
	{
		Call(SCI_FOLDLINE, (WPARAM)line, (LPARAM)action);
	}

	void FoldChildren(int line, int action)
	{
		Call(SCI_FOLDCHILDREN, (WPARAM)line, (LPARAM)action);
	}

	void ExpandChildren(int line, int level)
	{
		Call(SCI_EXPANDCHILDREN, (WPARAM)line, (LPARAM)level);
	}

	void FoldAll(int action)
	{
		Call(SCI_FOLDALL, (WPARAM)action, 0);
	}

	void EnsureVisible(int line)
	{
		Call(SCI_ENSUREVISIBLE, (WPARAM)line, 0);
	}

	void SetAutomaticFold(int automaticFold)
	{
		Call(SCI_SETAUTOMATICFOLD, (WPARAM)automaticFold, 0);
	}

	int GetAutomaticFold()
	{
		return Call(SCI_GETAUTOMATICFOLD, 0, 0);
	}

	void SetFoldFlags(int flags)
	{
		Call(SCI_SETFOLDFLAGS, (WPARAM)flags, 0);
	}

	void EnsureVisibleEnforcePolicy(int line)
	{
		Call(SCI_ENSUREVISIBLEENFORCEPOLICY, (WPARAM)line, 0);
	}

	void SetTabIndents(bool tabIndents)
	{
		Call(SCI_SETTABINDENTS, (WPARAM)tabIndents, 0);
	}

	bool GetTabIndents()
	{
		return Call(SCI_GETTABINDENTS, 0, 0);
	}

	void SetBackSpaceUnIndents(bool bsUnIndents)
	{
		Call(SCI_SETBACKSPACEUNINDENTS, (WPARAM)bsUnIndents, 0);
	}

	bool GetBackSpaceUnIndents()
	{
		return Call(SCI_GETBACKSPACEUNINDENTS, 0, 0);
	}

	void SetMouseDwellTime(int periodMilliseconds)
	{
		Call(SCI_SETMOUSEDWELLTIME, (WPARAM)periodMilliseconds, 0);
	}

	int GetMouseDwellTime()
	{
		return Call(SCI_GETMOUSEDWELLTIME, 0, 0);
	}

	int WordStartPosition(int pos, bool onlyWordCharacters)
	{
		return Call(SCI_WORDSTARTPOSITION, (WPARAM)pos, (LPARAM)onlyWordCharacters);
	}

	int WordEndPosition(int pos, bool onlyWordCharacters)
	{
		return Call(SCI_WORDENDPOSITION, (WPARAM)pos, (LPARAM)onlyWordCharacters);
	}

	bool IsRangeWord(int start, int end)
	{
		return Call(SCI_ISRANGEWORD, (WPARAM)start, (LPARAM)end);
	}

	void SetIdleStyling(int idleStyling)
	{
		Call(SCI_SETIDLESTYLING, (WPARAM)idleStyling, 0);
	}

	int GetIdleStyling()
	{
		return Call(SCI_GETIDLESTYLING, 0, 0);
	}

	void SetWrapMode(int wrapMode)
	{
		Call(SCI_SETWRAPMODE, (WPARAM)wrapMode, 0);
	}

	int GetWrapMode()
	{
		return Call(SCI_GETWRAPMODE, 0, 0);
	}

	void SetWrapVisualFlags(int wrapVisualFlags)
	{
		Call(SCI_SETWRAPVISUALFLAGS, (WPARAM)wrapVisualFlags, 0);
	}

	int GetWrapVisualFlags()
	{
		return Call(SCI_GETWRAPVISUALFLAGS, 0, 0);
	}

	void SetWrapVisualFlagsLocation(int wrapVisualFlagsLocation)
	{
		Call(SCI_SETWRAPVISUALFLAGSLOCATION, (WPARAM)wrapVisualFlagsLocation, 0);
	}

	int GetWrapVisualFlagsLocation()
	{
		return Call(SCI_GETWRAPVISUALFLAGSLOCATION, 0, 0);
	}

	void SetWrapStartIndent(int indent)
	{
		Call(SCI_SETWRAPSTARTINDENT, (WPARAM)indent, 0);
	}

	int GetWrapStartIndent()
	{
		return Call(SCI_GETWRAPSTARTINDENT, 0, 0);
	}

	void SetWrapIndentMode(int wrapIndentMode)
	{
		Call(SCI_SETWRAPINDENTMODE, (WPARAM)wrapIndentMode, 0);
	}

	int GetWrapIndentMode()
	{
		return Call(SCI_GETWRAPINDENTMODE, 0, 0);
	}

	void SetLayoutCache(int cacheMode)
	{
		Call(SCI_SETLAYOUTCACHE, (WPARAM)cacheMode, 0);
	}

	int GetLayoutCache()
	{
		return Call(SCI_GETLAYOUTCACHE, 0, 0);
	}

	void SetScrollWidth(int pixelWidth)
	{
		Call(SCI_SETSCROLLWIDTH, (WPARAM)pixelWidth, 0);
	}

	int GetScrollWidth()
	{
		return Call(SCI_GETSCROLLWIDTH, 0, 0);
	}

	void SetScrollWidthTracking(bool tracking)
	{
		Call(SCI_SETSCROLLWIDTHTRACKING, (WPARAM)tracking, 0);
	}

	bool GetScrollWidthTracking()
	{
		return Call(SCI_GETSCROLLWIDTHTRACKING, 0, 0);
	}

	int TextWidth(int style, const char* text)
	{
		return Call(SCI_TEXTWIDTH, (WPARAM)style, (LPARAM)text);
	}

	void SetEndAtLastLine(bool endAtLastLine)
	{
		Call(SCI_SETENDATLASTLINE, (WPARAM)endAtLastLine, 0);
	}

	bool GetEndAtLastLine()
	{
		return Call(SCI_GETENDATLASTLINE, 0, 0);
	}

	int TextHeight(int line)
	{
		return Call(SCI_TEXTHEIGHT, (WPARAM)line, 0);
	}

	void SetVScrollBar(bool visible)
	{
		Call(SCI_SETVSCROLLBAR, (WPARAM)visible, 0);
	}

	bool GetVScrollBar()
	{
		return Call(SCI_GETVSCROLLBAR, 0, 0);
	}

	void AppendText(int length, const char* text)
	{
		Call(SCI_APPENDTEXT, (WPARAM)length, (LPARAM)text);
	}

	int GetPhasesDraw()
	{
		return Call(SCI_GETPHASESDRAW, 0, 0);
	}

	void SetPhasesDraw(int phases)
	{
		Call(SCI_SETPHASESDRAW, (WPARAM)phases, 0);
	}

	void SetFontQuality(int fontQuality)
	{
		Call(SCI_SETFONTQUALITY, (WPARAM)fontQuality, 0);
	}

	int GetFontQuality()
	{
		return Call(SCI_GETFONTQUALITY, 0, 0);
	}

	void SetFirstVisibleLine(int displayLine)
	{
		Call(SCI_SETFIRSTVISIBLELINE, (WPARAM)displayLine, 0);
	}

	void SetMultiPaste(int multiPaste)
	{
		Call(SCI_SETMULTIPASTE, (WPARAM)multiPaste, 0);
	}

	int GetMultiPaste()
	{
		return Call(SCI_GETMULTIPASTE, 0, 0);
	}

	int GetTag(int tagNumber, char* tagValue)
	{
		return Call(SCI_GETTAG, (WPARAM)tagNumber, (LPARAM)tagValue);
	}

	void LinesJoin()
	{
		Call(SCI_LINESJOIN, 0, 0);
	}

	void LinesSplit(int pixelWidth)
	{
		Call(SCI_LINESSPLIT, (WPARAM)pixelWidth, 0);
	}

	void SetFoldMarginColour(bool useSetting, int back)
	{
		Call(SCI_SETFOLDMARGINCOLOUR, (WPARAM)useSetting, (LPARAM)back);
	}

	void SetFoldMarginHiColour(bool useSetting, int fore)
	{
		Call(SCI_SETFOLDMARGINHICOLOUR, (WPARAM)useSetting, (LPARAM)fore);
	}

	void SetAccessibility(int accessibility)
	{
		Call(SCI_SETACCESSIBILITY, (WPARAM)accessibility, 0);
	}

	int GetAccessibility()
	{
		return Call(SCI_GETACCESSIBILITY, 0, 0);
	}

	void LineDown()
	{
		Call(SCI_LINEDOWN, 0, 0);
	}

	void LineDownExtend()
	{
		Call(SCI_LINEDOWNEXTEND, 0, 0);
	}

	void LineUp()
	{
		Call(SCI_LINEUP, 0, 0);
	}

	void LineUpExtend()
	{
		Call(SCI_LINEUPEXTEND, 0, 0);
	}

	void CharLeft()
	{
		Call(SCI_CHARLEFT, 0, 0);
	}

	void CharLeftExtend()
	{
		Call(SCI_CHARLEFTEXTEND, 0, 0);
	}

	void CharRight()
	{
		Call(SCI_CHARRIGHT, 0, 0);
	}

	void CharRightExtend()
	{
		Call(SCI_CHARRIGHTEXTEND, 0, 0);
	}

	void WordLeft()
	{
		Call(SCI_WORDLEFT, 0, 0);
	}

	void WordLeftExtend()
	{
		Call(SCI_WORDLEFTEXTEND, 0, 0);
	}

	void WordRight()
	{
		Call(SCI_WORDRIGHT, 0, 0);
	}

	void WordRightExtend()
	{
		Call(SCI_WORDRIGHTEXTEND, 0, 0);
	}

	void Home()
	{
		Call(SCI_HOME, 0, 0);
	}

	void HomeExtend()
	{
		Call(SCI_HOMEEXTEND, 0, 0);
	}

	void LineEnd()
	{
		Call(SCI_LINEEND, 0, 0);
	}

	void LineEndExtend()
	{
		Call(SCI_LINEENDEXTEND, 0, 0);
	}

	void DocumentStart()
	{
		Call(SCI_DOCUMENTSTART, 0, 0);
	}

	void DocumentStartExtend()
	{
		Call(SCI_DOCUMENTSTARTEXTEND, 0, 0);
	}

	void DocumentEnd()
	{
		Call(SCI_DOCUMENTEND, 0, 0);
	}

	void DocumentEndExtend()
	{
		Call(SCI_DOCUMENTENDEXTEND, 0, 0);
	}

	void PageUp()
	{
		Call(SCI_PAGEUP, 0, 0);
	}

	void PageUpExtend()
	{
		Call(SCI_PAGEUPEXTEND, 0, 0);
	}

	void PageDown()
	{
		Call(SCI_PAGEDOWN, 0, 0);
	}

	void PageDownExtend()
	{
		Call(SCI_PAGEDOWNEXTEND, 0, 0);
	}

	void EditToggleOvertype()
	{
		Call(SCI_EDITTOGGLEOVERTYPE, 0, 0);
	}

	void Cancel()
	{
		Call(SCI_CANCEL, 0, 0);
	}

	void DeleteBack()
	{
		Call(SCI_DELETEBACK, 0, 0);
	}

	void Tab()
	{
		Call(SCI_TAB, 0, 0);
	}

	void BackTab()
	{
		Call(SCI_BACKTAB, 0, 0);
	}

	void NewLine()
	{
		Call(SCI_NEWLINE, 0, 0);
	}

	void FormFeed()
	{
		Call(SCI_FORMFEED, 0, 0);
	}

	void VCHome()
	{
		Call(SCI_VCHOME, 0, 0);
	}

	void VCHomeExtend()
	{
		Call(SCI_VCHOMEEXTEND, 0, 0);
	}

	void ZoomIn()
	{
		Call(SCI_ZOOMIN, 0, 0);
	}

	void ZoomOut()
	{
		Call(SCI_ZOOMOUT, 0, 0);
	}

	void DelWordLeft()
	{
		Call(SCI_DELWORDLEFT, 0, 0);
	}

	void DelWordRight()
	{
		Call(SCI_DELWORDRIGHT, 0, 0);
	}

	void DelWordRightEnd()
	{
		Call(SCI_DELWORDRIGHTEND, 0, 0);
	}

	void LineCut()
	{
		Call(SCI_LINECUT, 0, 0);
	}

	void LineDelete()
	{
		Call(SCI_LINEDELETE, 0, 0);
	}

	void LineTranspose()
	{
		Call(SCI_LINETRANSPOSE, 0, 0);
	}

	void LineReverse()
	{
		Call(SCI_LINEREVERSE, 0, 0);
	}

	void LineDuplicate()
	{
		Call(SCI_LINEDUPLICATE, 0, 0);
	}

	void LowerCase()
	{
		Call(SCI_LOWERCASE, 0, 0);
	}

	void UpperCase()
	{
		Call(SCI_UPPERCASE, 0, 0);
	}

	void LineScrollDown()
	{
		Call(SCI_LINESCROLLDOWN, 0, 0);
	}

	void LineScrollUp()
	{
		Call(SCI_LINESCROLLUP, 0, 0);
	}

	void DeleteBackNotLine()
	{
		Call(SCI_DELETEBACKNOTLINE, 0, 0);
	}

	void HomeDisplay()
	{
		Call(SCI_HOMEDISPLAY, 0, 0);
	}

	void HomeDisplayExtend()
	{
		Call(SCI_HOMEDISPLAYEXTEND, 0, 0);
	}

	void LineEndDisplay()
	{
		Call(SCI_LINEENDDISPLAY, 0, 0);
	}

	void LineEndDisplayExtend()
	{
		Call(SCI_LINEENDDISPLAYEXTEND, 0, 0);
	}

	void HomeWrap()
	{
		Call(SCI_HOMEWRAP, 0, 0);
	}

	void HomeWrapExtend()
	{
		Call(SCI_HOMEWRAPEXTEND, 0, 0);
	}

	void LineEndWrap()
	{
		Call(SCI_LINEENDWRAP, 0, 0);
	}

	void LineEndWrapExtend()
	{
		Call(SCI_LINEENDWRAPEXTEND, 0, 0);
	}

	void VCHomeWrap()
	{
		Call(SCI_VCHOMEWRAP, 0, 0);
	}

	void VCHomeWrapExtend()
	{
		Call(SCI_VCHOMEWRAPEXTEND, 0, 0);
	}

	void LineCopy()
	{
		Call(SCI_LINECOPY, 0, 0);
	}

	void MoveCaretInsideView()
	{
		Call(SCI_MOVECARETINSIDEVIEW, 0, 0);
	}

	int LineLength(int line)
	{
		return Call(SCI_LINELENGTH, (WPARAM)line, 0);
	}

	void BraceHighlight(int posA, int posB)
	{
		Call(SCI_BRACEHIGHLIGHT, (WPARAM)posA, (LPARAM)posB);
	}

	void BraceHighlightIndicator(bool useSetting, int indicator)
	{
		Call(SCI_BRACEHIGHLIGHTINDICATOR, (WPARAM)useSetting, (LPARAM)indicator);
	}

	void BraceBadLight(int pos)
	{
		Call(SCI_BRACEBADLIGHT, (WPARAM)pos, 0);
	}

	void BraceBadLightIndicator(bool useSetting, int indicator)
	{
		Call(SCI_BRACEBADLIGHTINDICATOR, (WPARAM)useSetting, (LPARAM)indicator);
	}

	int BraceMatch(int pos, int maxReStyle)
	{
		return Call(SCI_BRACEMATCH, (WPARAM)pos, (LPARAM)maxReStyle);
	}

	bool GetViewEOL()
	{
		return Call(SCI_GETVIEWEOL, 0, 0);
	}

	void SetViewEOL(bool visible)
	{
		Call(SCI_SETVIEWEOL, (WPARAM)visible, 0);
	}

	void* GetDocPointer()
	{
		return Call(SCI_GETDOCPOINTER, 0, 0);
	}

	void SetDocPointer(void* doc)
	{
		Call(SCI_SETDOCPOINTER, 0, (LPARAM)doc);
	}

	void SetModEventMask(int eventMask)
	{
		Call(SCI_SETMODEVENTMASK, (WPARAM)eventMask, 0);
	}

	int GetEdgeColumn()
	{
		return Call(SCI_GETEDGECOLUMN, 0, 0);
	}

	void SetEdgeColumn(int column)
	{
		Call(SCI_SETEDGECOLUMN, (WPARAM)column, 0);
	}

	int GetEdgeMode()
	{
		return Call(SCI_GETEDGEMODE, 0, 0);
	}

	void SetEdgeMode(int edgeMode)
	{
		Call(SCI_SETEDGEMODE, (WPARAM)edgeMode, 0);
	}

	int GetEdgeColour()
	{
		return Call(SCI_GETEDGECOLOUR, 0, 0);
	}

	void SetEdgeColour(int edgeColour)
	{
		Call(SCI_SETEDGECOLOUR, (WPARAM)edgeColour, 0);
	}

	void MultiEdgeAddLine(int column, int edgeColour)
	{
		Call(SCI_MULTIEDGEADDLINE, (WPARAM)column, (LPARAM)edgeColour);
	}

	void MultiEdgeClearAll()
	{
		Call(SCI_MULTIEDGECLEARALL, 0, 0);
	}

	void SearchAnchor()
	{
		Call(SCI_SEARCHANCHOR, 0, 0);
	}

	int SearchNext(int searchFlags, const char* text)
	{
		return Call(SCI_SEARCHNEXT, (WPARAM)searchFlags, (LPARAM)text);
	}

	int SearchPrev(int searchFlags, const char* text)
	{
		return Call(SCI_SEARCHPREV, (WPARAM)searchFlags, (LPARAM)text);
	}

	int LinesOnScreen()
	{
		return Call(SCI_LINESONSCREEN, 0, 0);
	}

	void UsePopUp(int popUpMode)
	{
		Call(SCI_USEPOPUP, (WPARAM)popUpMode, 0);
	}

	bool SelectionIsRectangle()
	{
		return Call(SCI_SELECTIONISRECTANGLE, 0, 0);
	}

	void SetZoom(int zoomInPoints)
	{
		Call(SCI_SETZOOM, (WPARAM)zoomInPoints, 0);
	}

	int GetZoom()
	{
		return Call(SCI_GETZOOM, 0, 0);
	}

	void* CreateDocument(int bytes, int documentOptions)
	{
		return Call(SCI_CREATEDOCUMENT, (WPARAM)bytes, (LPARAM)documentOptions);
	}

	void AddRefDocument(void* doc)
	{
		Call(SCI_ADDREFDOCUMENT, 0, (LPARAM)doc);
	}

	void ReleaseDocument(void* doc)
	{
		Call(SCI_RELEASEDOCUMENT, 0, (LPARAM)doc);
	}

	int GetDocumentOptions()
	{
		return Call(SCI_GETDOCUMENTOPTIONS, 0, 0);
	}

	int GetModEventMask()
	{
		return Call(SCI_GETMODEVENTMASK, 0, 0);
	}

	void SetCommandEvents(bool commandEvents)
	{
		Call(SCI_SETCOMMANDEVENTS, (WPARAM)commandEvents, 0);
	}

	bool GetCommandEvents()
	{
		return Call(SCI_GETCOMMANDEVENTS, 0, 0);
	}

	void SetFocus(bool focus)
	{
		Call(SCI_SETFOCUS, (WPARAM)focus, 0);
	}

	bool GetFocus()
	{
		return Call(SCI_GETFOCUS, 0, 0);
	}

	void SetStatus(int status)
	{
		Call(SCI_SETSTATUS, (WPARAM)status, 0);
	}

	int GetStatus()
	{
		return Call(SCI_GETSTATUS, 0, 0);
	}

	void SetMouseDownCaptures(bool captures)
	{
		Call(SCI_SETMOUSEDOWNCAPTURES, (WPARAM)captures, 0);
	}

	bool GetMouseDownCaptures()
	{
		return Call(SCI_GETMOUSEDOWNCAPTURES, 0, 0);
	}

	void SetMouseWheelCaptures(bool captures)
	{
		Call(SCI_SETMOUSEWHEELCAPTURES, (WPARAM)captures, 0);
	}

	bool GetMouseWheelCaptures()
	{
		return Call(SCI_GETMOUSEWHEELCAPTURES, 0, 0);
	}

	void SetCursor(int cursorType)
	{
		Call(SCI_SETCURSOR, (WPARAM)cursorType, 0);
	}

	int GetCursor()
	{
		return Call(SCI_GETCURSOR, 0, 0);
	}

	void SetControlCharSymbol(int symbol)
	{
		Call(SCI_SETCONTROLCHARSYMBOL, (WPARAM)symbol, 0);
	}

	int GetControlCharSymbol()
	{
		return Call(SCI_GETCONTROLCHARSYMBOL, 0, 0);
	}

	void WordPartLeft()
	{
		Call(SCI_WORDPARTLEFT, 0, 0);
	}

	void WordPartLeftExtend()
	{
		Call(SCI_WORDPARTLEFTEXTEND, 0, 0);
	}

	void WordPartRight()
	{
		Call(SCI_WORDPARTRIGHT, 0, 0);
	}

	void WordPartRightExtend()
	{
		Call(SCI_WORDPARTRIGHTEXTEND, 0, 0);
	}

	void SetVisiblePolicy(int visiblePolicy, int visibleSlop)
	{
		Call(SCI_SETVISIBLEPOLICY, (WPARAM)visiblePolicy, (LPARAM)visibleSlop);
	}

	void DelLineLeft()
	{
		Call(SCI_DELLINELEFT, 0, 0);
	}

	void DelLineRight()
	{
		Call(SCI_DELLINERIGHT, 0, 0);
	}

	void SetXOffset(int xOffset)
	{
		Call(SCI_SETXOFFSET, (WPARAM)xOffset, 0);
	}

	int GetXOffset()
	{
		return Call(SCI_GETXOFFSET, 0, 0);
	}

	void ChooseCaretX()
	{
		Call(SCI_CHOOSECARETX, 0, 0);
	}

	void GrabFocus()
	{
		Call(SCI_GRABFOCUS, 0, 0);
	}

	void SetXCaretPolicy(int caretPolicy, int caretSlop)
	{
		Call(SCI_SETXCARETPOLICY, (WPARAM)caretPolicy, (LPARAM)caretSlop);
	}

	void SetYCaretPolicy(int caretPolicy, int caretSlop)
	{
		Call(SCI_SETYCARETPOLICY, (WPARAM)caretPolicy, (LPARAM)caretSlop);
	}

	void SetPrintWrapMode(int wrapMode)
	{
		Call(SCI_SETPRINTWRAPMODE, (WPARAM)wrapMode, 0);
	}

	int GetPrintWrapMode()
	{
		return Call(SCI_GETPRINTWRAPMODE, 0, 0);
	}

	void SetHotspotActiveFore(bool useSetting, int fore)
	{
		Call(SCI_SETHOTSPOTACTIVEFORE, (WPARAM)useSetting, (LPARAM)fore);
	}

	int GetHotspotActiveFore()
	{
		return Call(SCI_GETHOTSPOTACTIVEFORE, 0, 0);
	}

	void SetHotspotActiveBack(bool useSetting, int back)
	{
		Call(SCI_SETHOTSPOTACTIVEBACK, (WPARAM)useSetting, (LPARAM)back);
	}

	int GetHotspotActiveBack()
	{
		return Call(SCI_GETHOTSPOTACTIVEBACK, 0, 0);
	}

	void SetHotspotActiveUnderline(bool underline)
	{
		Call(SCI_SETHOTSPOTACTIVEUNDERLINE, (WPARAM)underline, 0);
	}

	bool GetHotspotActiveUnderline()
	{
		return Call(SCI_GETHOTSPOTACTIVEUNDERLINE, 0, 0);
	}

	void SetHotspotSingleLine(bool singleLine)
	{
		Call(SCI_SETHOTSPOTSINGLELINE, (WPARAM)singleLine, 0);
	}

	bool GetHotspotSingleLine()
	{
		return Call(SCI_GETHOTSPOTSINGLELINE, 0, 0);
	}

	void ParaDown()
	{
		Call(SCI_PARADOWN, 0, 0);
	}

	void ParaDownExtend()
	{
		Call(SCI_PARADOWNEXTEND, 0, 0);
	}

	void ParaUp()
	{
		Call(SCI_PARAUP, 0, 0);
	}

	void ParaUpExtend()
	{
		Call(SCI_PARAUPEXTEND, 0, 0);
	}

	int PositionBefore(int pos)
	{
		return Call(SCI_POSITIONBEFORE, (WPARAM)pos, 0);
	}

	int PositionAfter(int pos)
	{
		return Call(SCI_POSITIONAFTER, (WPARAM)pos, 0);
	}

	int PositionRelative(int pos, int relative)
	{
		return Call(SCI_POSITIONRELATIVE, (WPARAM)pos, (LPARAM)relative);
	}

	int PositionRelativeCodeUnits(int pos, int relative)
	{
		return Call(SCI_POSITIONRELATIVECODEUNITS, (WPARAM)pos, (LPARAM)relative);
	}

	void CopyRange(int start, int end)
	{
		Call(SCI_COPYRANGE, (WPARAM)start, (LPARAM)end);
	}

	void CopyText(int length, const char* text)
	{
		Call(SCI_COPYTEXT, (WPARAM)length, (LPARAM)text);
	}

	void SetSelectionMode(int selectionMode)
	{
		Call(SCI_SETSELECTIONMODE, (WPARAM)selectionMode, 0);
	}

	int GetSelectionMode()
	{
		return Call(SCI_GETSELECTIONMODE, 0, 0);
	}

	bool GetMoveExtendsSelection()
	{
		return Call(SCI_GETMOVEEXTENDSSELECTION, 0, 0);
	}

	int GetLineSelStartPosition(int line)
	{
		return Call(SCI_GETLINESELSTARTPOSITION, (WPARAM)line, 0);
	}

	int GetLineSelEndPosition(int line)
	{
		return Call(SCI_GETLINESELENDPOSITION, (WPARAM)line, 0);
	}

	void LineDownRectExtend()
	{
		Call(SCI_LINEDOWNRECTEXTEND, 0, 0);
	}

	void LineUpRectExtend()
	{
		Call(SCI_LINEUPRECTEXTEND, 0, 0);
	}

	void CharLeftRectExtend()
	{
		Call(SCI_CHARLEFTRECTEXTEND, 0, 0);
	}

	void CharRightRectExtend()
	{
		Call(SCI_CHARRIGHTRECTEXTEND, 0, 0);
	}

	void HomeRectExtend()
	{
		Call(SCI_HOMERECTEXTEND, 0, 0);
	}

	void VCHomeRectExtend()
	{
		Call(SCI_VCHOMERECTEXTEND, 0, 0);
	}

	void LineEndRectExtend()
	{
		Call(SCI_LINEENDRECTEXTEND, 0, 0);
	}

	void PageUpRectExtend()
	{
		Call(SCI_PAGEUPRECTEXTEND, 0, 0);
	}

	void PageDownRectExtend()
	{
		Call(SCI_PAGEDOWNRECTEXTEND, 0, 0);
	}

	void StutteredPageUp()
	{
		Call(SCI_STUTTEREDPAGEUP, 0, 0);
	}

	void StutteredPageUpExtend()
	{
		Call(SCI_STUTTEREDPAGEUPEXTEND, 0, 0);
	}

	void StutteredPageDown()
	{
		Call(SCI_STUTTEREDPAGEDOWN, 0, 0);
	}

	void StutteredPageDownExtend()
	{
		Call(SCI_STUTTEREDPAGEDOWNEXTEND, 0, 0);
	}

	void WordLeftEnd()
	{
		Call(SCI_WORDLEFTEND, 0, 0);
	}

	void WordLeftEndExtend()
	{
		Call(SCI_WORDLEFTENDEXTEND, 0, 0);
	}

	void WordRightEnd()
	{
		Call(SCI_WORDRIGHTEND, 0, 0);
	}

	void WordRightEndExtend()
	{
		Call(SCI_WORDRIGHTENDEXTEND, 0, 0);
	}

	void SetWhitespaceChars(const char* characters)
	{
		Call(SCI_SETWHITESPACECHARS, 0, (LPARAM)characters);
	}

	int GetWhitespaceChars(char* characters)
	{
		return Call(SCI_GETWHITESPACECHARS, 0, (LPARAM)characters);
	}

	void SetPunctuationChars(const char* characters)
	{
		Call(SCI_SETPUNCTUATIONCHARS, 0, (LPARAM)characters);
	}

	int GetPunctuationChars(char* characters)
	{
		return Call(SCI_GETPUNCTUATIONCHARS, 0, (LPARAM)characters);
	}

	void SetCharsDefault()
	{
		Call(SCI_SETCHARSDEFAULT, 0, 0);
	}

	int AutoCGetCurrent()
	{
		return Call(SCI_AUTOCGETCURRENT, 0, 0);
	}

	int AutoCGetCurrentText(char* text)
	{
		return Call(SCI_AUTOCGETCURRENTTEXT, 0, (LPARAM)text);
	}

	void AutoCSetCaseInsensitiveBehaviour(int behaviour)
	{
		Call(SCI_AUTOCSETCASEINSENSITIVEBEHAVIOUR, (WPARAM)behaviour, 0);
	}

	int AutoCGetCaseInsensitiveBehaviour()
	{
		return Call(SCI_AUTOCGETCASEINSENSITIVEBEHAVIOUR, 0, 0);
	}

	void AutoCSetMulti(int multi)
	{
		Call(SCI_AUTOCSETMULTI, (WPARAM)multi, 0);
	}

	int AutoCGetMulti()
	{
		return Call(SCI_AUTOCGETMULTI, 0, 0);
	}

	void AutoCSetOrder(int order)
	{
		Call(SCI_AUTOCSETORDER, (WPARAM)order, 0);
	}

	int AutoCGetOrder()
	{
		return Call(SCI_AUTOCGETORDER, 0, 0);
	}

	void Allocate(int bytes)
	{
		Call(SCI_ALLOCATE, (WPARAM)bytes, 0);
	}

	int TargetAsUTF8(char* s)
	{
		return Call(SCI_TARGETASUTF8, 0, (LPARAM)s);
	}

	void SetLengthForEncode(int bytes)
	{
		Call(SCI_SETLENGTHFORENCODE, (WPARAM)bytes, 0);
	}

	int EncodedFromUTF8(const char* utf8, char* encoded)
	{
		return Call(SCI_ENCODEDFROMUTF8, (WPARAM)utf8, (LPARAM)encoded);
	}

	int FindColumn(int line, int column)
	{
		return Call(SCI_FINDCOLUMN, (WPARAM)line, (LPARAM)column);
	}

	int GetCaretSticky()
	{
		return Call(SCI_GETCARETSTICKY, 0, 0);
	}

	void SetCaretSticky(int useCaretStickyBehaviour)
	{
		Call(SCI_SETCARETSTICKY, (WPARAM)useCaretStickyBehaviour, 0);
	}

	void ToggleCaretSticky()
	{
		Call(SCI_TOGGLECARETSTICKY, 0, 0);
	}

	void SetPasteConvertEndings(bool convert)
	{
		Call(SCI_SETPASTECONVERTENDINGS, (WPARAM)convert, 0);
	}

	bool GetPasteConvertEndings()
	{
		return Call(SCI_GETPASTECONVERTENDINGS, 0, 0);
	}

	void SelectionDuplicate()
	{
		Call(SCI_SELECTIONDUPLICATE, 0, 0);
	}

	void SetCaretLineBackAlpha(int alpha)
	{
		Call(SCI_SETCARETLINEBACKALPHA, (WPARAM)alpha, 0);
	}

	int GetCaretLineBackAlpha()
	{
		return Call(SCI_GETCARETLINEBACKALPHA, 0, 0);
	}

	void SetCaretStyle(int caretStyle)
	{
		Call(SCI_SETCARETSTYLE, (WPARAM)caretStyle, 0);
	}

	int GetCaretStyle()
	{
		return Call(SCI_GETCARETSTYLE, 0, 0);
	}

	void SetIndicatorCurrent(int indicator)
	{
		Call(SCI_SETINDICATORCURRENT, (WPARAM)indicator, 0);
	}

	int GetIndicatorCurrent()
	{
		return Call(SCI_GETINDICATORCURRENT, 0, 0);
	}

	void SetIndicatorValue(int value)
	{
		Call(SCI_SETINDICATORVALUE, (WPARAM)value, 0);
	}

	int GetIndicatorValue()
	{
		return Call(SCI_GETINDICATORVALUE, 0, 0);
	}

	void IndicatorFillRange(int start, int lengthFill)
	{
		Call(SCI_INDICATORFILLRANGE, (WPARAM)start, (LPARAM)lengthFill);
	}

	void IndicatorClearRange(int start, int lengthClear)
	{
		Call(SCI_INDICATORCLEARRANGE, (WPARAM)start, (LPARAM)lengthClear);
	}

	int IndicatorAllOnFor(int pos)
	{
		return Call(SCI_INDICATORALLONFOR, (WPARAM)pos, 0);
	}

	int IndicatorValueAt(int indicator, int pos)
	{
		return Call(SCI_INDICATORVALUEAT, (WPARAM)indicator, (LPARAM)pos);
	}

	int IndicatorStart(int indicator, int pos)
	{
		return Call(SCI_INDICATORSTART, (WPARAM)indicator, (LPARAM)pos);
	}

	int IndicatorEnd(int indicator, int pos)
	{
		return Call(SCI_INDICATOREND, (WPARAM)indicator, (LPARAM)pos);
	}

	void SetPositionCache(int size)
	{
		Call(SCI_SETPOSITIONCACHE, (WPARAM)size, 0);
	}

	int GetPositionCache()
	{
		return Call(SCI_GETPOSITIONCACHE, 0, 0);
	}

	void CopyAllowLine()
	{
		Call(SCI_COPYALLOWLINE, 0, 0);
	}

	void* GetCharacterPointer()
	{
		return Call(SCI_GETCHARACTERPOINTER, 0, 0);
	}

	void* GetRangePointer(int start, int lengthRange)
	{
		return Call(SCI_GETRANGEPOINTER, (WPARAM)start, (LPARAM)lengthRange);
	}

	int GetGapPosition()
	{
		return Call(SCI_GETGAPPOSITION, 0, 0);
	}

	void IndicSetAlpha(int indicator, int alpha)
	{
		Call(SCI_INDICSETALPHA, (WPARAM)indicator, (LPARAM)alpha);
	}

	int IndicGetAlpha(int indicator)
	{
		return Call(SCI_INDICGETALPHA, (WPARAM)indicator, 0);
	}

	void IndicSetOutlineAlpha(int indicator, int alpha)
	{
		Call(SCI_INDICSETOUTLINEALPHA, (WPARAM)indicator, (LPARAM)alpha);
	}

	int IndicGetOutlineAlpha(int indicator)
	{
		return Call(SCI_INDICGETOUTLINEALPHA, (WPARAM)indicator, 0);
	}

	void SetExtraAscent(int extraAscent)
	{
		Call(SCI_SETEXTRAASCENT, (WPARAM)extraAscent, 0);
	}

	int GetExtraAscent()
	{
		return Call(SCI_GETEXTRAASCENT, 0, 0);
	}

	void SetExtraDescent(int extraDescent)
	{
		Call(SCI_SETEXTRADESCENT, (WPARAM)extraDescent, 0);
	}

	int GetExtraDescent()
	{
		return Call(SCI_GETEXTRADESCENT, 0, 0);
	}

	int MarkerSymbolDefined(int markerNumber)
	{
		return Call(SCI_MARKERSYMBOLDEFINED, (WPARAM)markerNumber, 0);
	}

	void MarginSetText(int line, const char* text)
	{
		Call(SCI_MARGINSETTEXT, (WPARAM)line, (LPARAM)text);
	}

	int MarginGetText(int line, char* text)
	{
		return Call(SCI_MARGINGETTEXT, (WPARAM)line, (LPARAM)text);
	}

	void MarginSetStyle(int line, int style)
	{
		Call(SCI_MARGINSETSTYLE, (WPARAM)line, (LPARAM)style);
	}

	int MarginGetStyle(int line)
	{
		return Call(SCI_MARGINGETSTYLE, (WPARAM)line, 0);
	}

	void MarginSetStyles(int line, const char* styles)
	{
		Call(SCI_MARGINSETSTYLES, (WPARAM)line, (LPARAM)styles);
	}

	int MarginGetStyles(int line, char* styles)
	{
		return Call(SCI_MARGINGETSTYLES, (WPARAM)line, (LPARAM)styles);
	}

	void MarginTextClearAll()
	{
		Call(SCI_MARGINTEXTCLEARALL, 0, 0);
	}

	void MarginSetStyleOffset(int style)
	{
		Call(SCI_MARGINSETSTYLEOFFSET, (WPARAM)style, 0);
	}

	int MarginGetStyleOffset()
	{
		return Call(SCI_MARGINGETSTYLEOFFSET, 0, 0);
	}

	void SetMarginOptions(int marginOptions)
	{
		Call(SCI_SETMARGINOPTIONS, (WPARAM)marginOptions, 0);
	}

	int GetMarginOptions()
	{
		return Call(SCI_GETMARGINOPTIONS, 0, 0);
	}

	void AnnotationSetText(int line, const char* text)
	{
		Call(SCI_ANNOTATIONSETTEXT, (WPARAM)line, (LPARAM)text);
	}

	int AnnotationGetText(int line, char* text)
	{
		return Call(SCI_ANNOTATIONGETTEXT, (WPARAM)line, (LPARAM)text);
	}

	void AnnotationSetStyle(int line, int style)
	{
		Call(SCI_ANNOTATIONSETSTYLE, (WPARAM)line, (LPARAM)style);
	}

	int AnnotationGetStyle(int line)
	{
		return Call(SCI_ANNOTATIONGETSTYLE, (WPARAM)line, 0);
	}

	void AnnotationSetStyles(int line, const char* styles)
	{
		Call(SCI_ANNOTATIONSETSTYLES, (WPARAM)line, (LPARAM)styles);
	}

	int AnnotationGetStyles(int line, char* styles)
	{
		return Call(SCI_ANNOTATIONGETSTYLES, (WPARAM)line, (LPARAM)styles);
	}

	int AnnotationGetLines(int line)
	{
		return Call(SCI_ANNOTATIONGETLINES, (WPARAM)line, 0);
	}

	void AnnotationClearAll()
	{
		Call(SCI_ANNOTATIONCLEARALL, 0, 0);
	}

	void AnnotationSetVisible(int visible)
	{
		Call(SCI_ANNOTATIONSETVISIBLE, (WPARAM)visible, 0);
	}

	int AnnotationGetVisible()
	{
		return Call(SCI_ANNOTATIONGETVISIBLE, 0, 0);
	}

	void AnnotationSetStyleOffset(int style)
	{
		Call(SCI_ANNOTATIONSETSTYLEOFFSET, (WPARAM)style, 0);
	}

	int AnnotationGetStyleOffset()
	{
		return Call(SCI_ANNOTATIONGETSTYLEOFFSET, 0, 0);
	}

	void ReleaseAllExtendedStyles()
	{
		Call(SCI_RELEASEALLEXTENDEDSTYLES, 0, 0);
	}

	int AllocateExtendedStyles(int numberStyles)
	{
		return Call(SCI_ALLOCATEEXTENDEDSTYLES, (WPARAM)numberStyles, 0);
	}

	void AddUndoAction(int token, int flags)
	{
		Call(SCI_ADDUNDOACTION, (WPARAM)token, (LPARAM)flags);
	}

	int CharPositionFromPoint(int x, int y)
	{
		return Call(SCI_CHARPOSITIONFROMPOINT, (WPARAM)x, (LPARAM)y);
	}

	int CharPositionFromPointClose(int x, int y)
	{
		return Call(SCI_CHARPOSITIONFROMPOINTCLOSE, (WPARAM)x, (LPARAM)y);
	}

	void SetMouseSelectionRectangularSwitch(bool mouseSelectionRectangularSwitch)
	{
		Call(SCI_SETMOUSESELECTIONRECTANGULARSWITCH, (WPARAM)mouseSelectionRectangularSwitch, 0);
	}

	bool GetMouseSelectionRectangularSwitch()
	{
		return Call(SCI_GETMOUSESELECTIONRECTANGULARSWITCH, 0, 0);
	}

	void SetMultipleSelection(bool multipleSelection)
	{
		Call(SCI_SETMULTIPLESELECTION, (WPARAM)multipleSelection, 0);
	}

	bool GetMultipleSelection()
	{
		return Call(SCI_GETMULTIPLESELECTION, 0, 0);
	}

	void SetAdditionalSelectionTyping(bool additionalSelectionTyping)
	{
		Call(SCI_SETADDITIONALSELECTIONTYPING, (WPARAM)additionalSelectionTyping, 0);
	}

	bool GetAdditionalSelectionTyping()
	{
		return Call(SCI_GETADDITIONALSELECTIONTYPING, 0, 0);
	}

	void SetAdditionalCaretsBlink(bool additionalCaretsBlink)
	{
		Call(SCI_SETADDITIONALCARETSBLINK, (WPARAM)additionalCaretsBlink, 0);
	}

	bool GetAdditionalCaretsBlink()
	{
		return Call(SCI_GETADDITIONALCARETSBLINK, 0, 0);
	}

	void SetAdditionalCaretsVisible(bool additionalCaretsVisible)
	{
		Call(SCI_SETADDITIONALCARETSVISIBLE, (WPARAM)additionalCaretsVisible, 0);
	}

	bool GetAdditionalCaretsVisible()
	{
		return Call(SCI_GETADDITIONALCARETSVISIBLE, 0, 0);
	}

	int GetSelections()
	{
		return Call(SCI_GETSELECTIONS, 0, 0);
	}

	bool GetSelectionEmpty()
	{
		return Call(SCI_GETSELECTIONEMPTY, 0, 0);
	}

	void ClearSelections()
	{
		Call(SCI_CLEARSELECTIONS, 0, 0);
	}

	void SetSelection(int caret, int anchor)
	{
		Call(SCI_SETSELECTION, (WPARAM)caret, (LPARAM)anchor);
	}

	void AddSelection(int caret, int anchor)
	{
		Call(SCI_ADDSELECTION, (WPARAM)caret, (LPARAM)anchor);
	}

	void DropSelectionN(int selection)
	{
		Call(SCI_DROPSELECTIONN, (WPARAM)selection, 0);
	}

	void SetMainSelection(int selection)
	{
		Call(SCI_SETMAINSELECTION, (WPARAM)selection, 0);
	}

	int GetMainSelection()
	{
		return Call(SCI_GETMAINSELECTION, 0, 0);
	}

	void SetSelectionNCaret(int selection, int caret)
	{
		Call(SCI_SETSELECTIONNCARET, (WPARAM)selection, (LPARAM)caret);
	}

	int GetSelectionNCaret(int selection)
	{
		return Call(SCI_GETSELECTIONNCARET, (WPARAM)selection, 0);
	}

	void SetSelectionNAnchor(int selection, int anchor)
	{
		Call(SCI_SETSELECTIONNANCHOR, (WPARAM)selection, (LPARAM)anchor);
	}

	int GetSelectionNAnchor(int selection)
	{
		return Call(SCI_GETSELECTIONNANCHOR, (WPARAM)selection, 0);
	}

	void SetSelectionNCaretVirtualSpace(int selection, int space)
	{
		Call(SCI_SETSELECTIONNCARETVIRTUALSPACE, (WPARAM)selection, (LPARAM)space);
	}

	int GetSelectionNCaretVirtualSpace(int selection)
	{
		return Call(SCI_GETSELECTIONNCARETVIRTUALSPACE, (WPARAM)selection, 0);
	}

	void SetSelectionNAnchorVirtualSpace(int selection, int space)
	{
		Call(SCI_SETSELECTIONNANCHORVIRTUALSPACE, (WPARAM)selection, (LPARAM)space);
	}

	int GetSelectionNAnchorVirtualSpace(int selection)
	{
		return Call(SCI_GETSELECTIONNANCHORVIRTUALSPACE, (WPARAM)selection, 0);
	}

	void SetSelectionNStart(int selection, int anchor)
	{
		Call(SCI_SETSELECTIONNSTART, (WPARAM)selection, (LPARAM)anchor);
	}

	int GetSelectionNStart(int selection)
	{
		return Call(SCI_GETSELECTIONNSTART, (WPARAM)selection, 0);
	}

	void SetSelectionNEnd(int selection, int caret)
	{
		Call(SCI_SETSELECTIONNEND, (WPARAM)selection, (LPARAM)caret);
	}

	int GetSelectionNEnd(int selection)
	{
		return Call(SCI_GETSELECTIONNEND, (WPARAM)selection, 0);
	}

	void SetRectangularSelectionCaret(int caret)
	{
		Call(SCI_SETRECTANGULARSELECTIONCARET, (WPARAM)caret, 0);
	}

	int GetRectangularSelectionCaret()
	{
		return Call(SCI_GETRECTANGULARSELECTIONCARET, 0, 0);
	}

	void SetRectangularSelectionAnchor(int anchor)
	{
		Call(SCI_SETRECTANGULARSELECTIONANCHOR, (WPARAM)anchor, 0);
	}

	int GetRectangularSelectionAnchor()
	{
		return Call(SCI_GETRECTANGULARSELECTIONANCHOR, 0, 0);
	}

	void SetRectangularSelectionCaretVirtualSpace(int space)
	{
		Call(SCI_SETRECTANGULARSELECTIONCARETVIRTUALSPACE, (WPARAM)space, 0);
	}

	int GetRectangularSelectionCaretVirtualSpace()
	{
		return Call(SCI_GETRECTANGULARSELECTIONCARETVIRTUALSPACE, 0, 0);
	}

	void SetRectangularSelectionAnchorVirtualSpace(int space)
	{
		Call(SCI_SETRECTANGULARSELECTIONANCHORVIRTUALSPACE, (WPARAM)space, 0);
	}

	int GetRectangularSelectionAnchorVirtualSpace()
	{
		return Call(SCI_GETRECTANGULARSELECTIONANCHORVIRTUALSPACE, 0, 0);
	}

	void SetVirtualSpaceOptions(int virtualSpaceOptions)
	{
		Call(SCI_SETVIRTUALSPACEOPTIONS, (WPARAM)virtualSpaceOptions, 0);
	}

	int GetVirtualSpaceOptions()
	{
		return Call(SCI_GETVIRTUALSPACEOPTIONS, 0, 0);
	}

	void SetRectangularSelectionModifier(int modifier)
	{
		Call(SCI_SETRECTANGULARSELECTIONMODIFIER, (WPARAM)modifier, 0);
	}

	int GetRectangularSelectionModifier()
	{
		return Call(SCI_GETRECTANGULARSELECTIONMODIFIER, 0, 0);
	}

	void SetAdditionalSelFore(int fore)
	{
		Call(SCI_SETADDITIONALSELFORE, (WPARAM)fore, 0);
	}

	void SetAdditionalSelBack(int back)
	{
		Call(SCI_SETADDITIONALSELBACK, (WPARAM)back, 0);
	}

	void SetAdditionalSelAlpha(int alpha)
	{
		Call(SCI_SETADDITIONALSELALPHA, (WPARAM)alpha, 0);
	}

	int GetAdditionalSelAlpha()
	{
		return Call(SCI_GETADDITIONALSELALPHA, 0, 0);
	}

	void SetAdditionalCaretFore(int fore)
	{
		Call(SCI_SETADDITIONALCARETFORE, (WPARAM)fore, 0);
	}

	int GetAdditionalCaretFore()
	{
		return Call(SCI_GETADDITIONALCARETFORE, 0, 0);
	}

	void RotateSelection()
	{
		Call(SCI_ROTATESELECTION, 0, 0);
	}

	void SwapMainAnchorCaret()
	{
		Call(SCI_SWAPMAINANCHORCARET, 0, 0);
	}

	void MultipleSelectAddNext()
	{
		Call(SCI_MULTIPLESELECTADDNEXT, 0, 0);
	}

	void MultipleSelectAddEach()
	{
		Call(SCI_MULTIPLESELECTADDEACH, 0, 0);
	}

	int ChangeLexerState(int start, int end)
	{
		return Call(SCI_CHANGELEXERSTATE, (WPARAM)start, (LPARAM)end);
	}

	int ContractedFoldNext(int lineStart)
	{
		return Call(SCI_CONTRACTEDFOLDNEXT, (WPARAM)lineStart, 0);
	}

	void VerticalCentreCaret()
	{
		Call(SCI_VERTICALCENTRECARET, 0, 0);
	}

	void MoveSelectedLinesUp()
	{
		Call(SCI_MOVESELECTEDLINESUP, 0, 0);
	}

	void MoveSelectedLinesDown()
	{
		Call(SCI_MOVESELECTEDLINESDOWN, 0, 0);
	}

	void SetIdentifier(int identifier)
	{
		Call(SCI_SETIDENTIFIER, (WPARAM)identifier, 0);
	}

	int GetIdentifier()
	{
		return Call(SCI_GETIDENTIFIER, 0, 0);
	}

	void RGBAImageSetWidth(int width)
	{
		Call(SCI_RGBAIMAGESETWIDTH, (WPARAM)width, 0);
	}

	void RGBAImageSetHeight(int height)
	{
		Call(SCI_RGBAIMAGESETHEIGHT, (WPARAM)height, 0);
	}

	void RGBAImageSetScale(int scalePercent)
	{
		Call(SCI_RGBAIMAGESETSCALE, (WPARAM)scalePercent, 0);
	}

	void MarkerDefineRGBAImage(int markerNumber, const char* pixels)
	{
		Call(SCI_MARKERDEFINERGBAIMAGE, (WPARAM)markerNumber, (LPARAM)pixels);
	}

	void RegisterRGBAImage(int type, const char* pixels)
	{
		Call(SCI_REGISTERRGBAIMAGE, (WPARAM)type, (LPARAM)pixels);
	}

	void ScrollToStart()
	{
		Call(SCI_SCROLLTOSTART, 0, 0);
	}

	void ScrollToEnd()
	{
		Call(SCI_SCROLLTOEND, 0, 0);
	}

	void SetTechnology(int technology)
	{
		Call(SCI_SETTECHNOLOGY, (WPARAM)technology, 0);
	}

	int GetTechnology()
	{
		return Call(SCI_GETTECHNOLOGY, 0, 0);
	}

	void* CreateLoader(int bytes, int documentOptions)
	{
		return Call(SCI_CREATELOADER, (WPARAM)bytes, (LPARAM)documentOptions);
	}

	void FindIndicatorShow(int start, int end)
	{
		Call(SCI_FINDINDICATORSHOW, (WPARAM)start, (LPARAM)end);
	}

	void FindIndicatorFlash(int start, int end)
	{
		Call(SCI_FINDINDICATORFLASH, (WPARAM)start, (LPARAM)end);
	}

	void FindIndicatorHide()
	{
		Call(SCI_FINDINDICATORHIDE, 0, 0);
	}

	void VCHomeDisplay()
	{
		Call(SCI_VCHOMEDISPLAY, 0, 0);
	}

	void VCHomeDisplayExtend()
	{
		Call(SCI_VCHOMEDISPLAYEXTEND, 0, 0);
	}

	bool GetCaretLineVisibleAlways()
	{
		return Call(SCI_GETCARETLINEVISIBLEALWAYS, 0, 0);
	}

	void SetCaretLineVisibleAlways(bool alwaysVisible)
	{
		Call(SCI_SETCARETLINEVISIBLEALWAYS, (WPARAM)alwaysVisible, 0);
	}

	void SetLineEndTypesAllowed(int lineEndBitSet)
	{
		Call(SCI_SETLINEENDTYPESALLOWED, (WPARAM)lineEndBitSet, 0);
	}

	int GetLineEndTypesAllowed()
	{
		return Call(SCI_GETLINEENDTYPESALLOWED, 0, 0);
	}

	int GetLineEndTypesActive()
	{
		return Call(SCI_GETLINEENDTYPESACTIVE, 0, 0);
	}

	void SetRepresentation(const char* encodedCharacter, const char* representation)
	{
		Call(SCI_SETREPRESENTATION, (WPARAM)encodedCharacter, (LPARAM)representation);
	}

	int GetRepresentation(const char* encodedCharacter, char* representation)
	{
		return Call(SCI_GETREPRESENTATION, (WPARAM)encodedCharacter, (LPARAM)representation);
	}

	void ClearRepresentation(const char* encodedCharacter)
	{
		Call(SCI_CLEARREPRESENTATION, (WPARAM)encodedCharacter, 0);
	}

	void StartRecord()
	{
		Call(SCI_STARTRECORD, 0, 0);
	}

	void StopRecord()
	{
		Call(SCI_STOPRECORD, 0, 0);
	}

	void SetLexer(int lexer)
	{
		Call(SCI_SETLEXER, (WPARAM)lexer, 0);
	}

	int GetLexer()
	{
		return Call(SCI_GETLEXER, 0, 0);
	}

	void Colourise(int start, int end)
	{
		Call(SCI_COLOURISE, (WPARAM)start, (LPARAM)end);
	}

	void SetProperty(const char* key, const char* value)
	{
		Call(SCI_SETPROPERTY, (WPARAM)key, (LPARAM)value);
	}

	void SetKeyWords(int keyWordSet, const char* keyWords)
	{
		Call(SCI_SETKEYWORDS, (WPARAM)keyWordSet, (LPARAM)keyWords);
	}

	void SetLexerLanguage(const char* language)
	{
		Call(SCI_SETLEXERLANGUAGE, 0, (LPARAM)language);
	}

	void LoadLexerLibrary(const char* path)
	{
		Call(SCI_LOADLEXERLIBRARY, 0, (LPARAM)path);
	}

	int GetProperty(const char* key, char* value)
	{
		return Call(SCI_GETPROPERTY, (WPARAM)key, (LPARAM)value);
	}

	int GetPropertyExpanded(const char* key, char* value)
	{
		return Call(SCI_GETPROPERTYEXPANDED, (WPARAM)key, (LPARAM)value);
	}

	int GetPropertyInt(const char* key, int defaultValue)
	{
		return Call(SCI_GETPROPERTYINT, (WPARAM)key, (LPARAM)defaultValue);
	}

	int GetLexerLanguage(char* language)
	{
		return Call(SCI_GETLEXERLANGUAGE, 0, (LPARAM)language);
	}

	void* PrivateLexerCall(int operation, void* pointer)
	{
		return Call(SCI_PRIVATELEXERCALL, (WPARAM)operation, (LPARAM)pointer);
	}

	int PropertyNames(char* names)
	{
		return Call(SCI_PROPERTYNAMES, 0, (LPARAM)names);
	}

	int PropertyType(const char* name)
	{
		return Call(SCI_PROPERTYTYPE, (WPARAM)name, 0);
	}

	int DescribeProperty(const char* name, char* description)
	{
		return Call(SCI_DESCRIBEPROPERTY, (WPARAM)name, (LPARAM)description);
	}

	int DescribeKeyWordSets(char* descriptions)
	{
		return Call(SCI_DESCRIBEKEYWORDSETS, 0, (LPARAM)descriptions);
	}

	int GetLineEndTypesSupported()
	{
		return Call(SCI_GETLINEENDTYPESSUPPORTED, 0, 0);
	}

	int AllocateSubStyles(int styleBase, int numberStyles)
	{
		return Call(SCI_ALLOCATESUBSTYLES, (WPARAM)styleBase, (LPARAM)numberStyles);
	}

	int GetSubStylesStart(int styleBase)
	{
		return Call(SCI_GETSUBSTYLESSTART, (WPARAM)styleBase, 0);
	}

	int GetSubStylesLength(int styleBase)
	{
		return Call(SCI_GETSUBSTYLESLENGTH, (WPARAM)styleBase, 0);
	}

	int GetStyleFromSubStyle(int subStyle)
	{
		return Call(SCI_GETSTYLEFROMSUBSTYLE, (WPARAM)subStyle, 0);
	}

	int GetPrimaryStyleFromStyle(int style)
	{
		return Call(SCI_GETPRIMARYSTYLEFROMSTYLE, (WPARAM)style, 0);
	}

	void FreeSubStyles()
	{
		Call(SCI_FREESUBSTYLES, 0, 0);
	}

	void SetIdentifiers(int style, const char* identifiers)
	{
		Call(SCI_SETIDENTIFIERS, (WPARAM)style, (LPARAM)identifiers);
	}

	int DistanceToSecondaryStyles()
	{
		return Call(SCI_DISTANCETOSECONDARYSTYLES, 0, 0);
	}

	int GetSubStyleBases(char* styles)
	{
		return Call(SCI_GETSUBSTYLEBASES, 0, (LPARAM)styles);
	}

	int GetNamedStyles()
	{
		return Call(SCI_GETNAMEDSTYLES, 0, 0);
	}

	int NameOfStyle(int style, char* name)
	{
		return Call(SCI_NAMEOFSTYLE, (WPARAM)style, (LPARAM)name);
	}

	int TagsOfStyle(int style, char* tags)
	{
		return Call(SCI_TAGSOFSTYLE, (WPARAM)style, (LPARAM)tags);
	}

	int DescriptionOfStyle(int style, char* description)
	{
		return Call(SCI_DESCRIPTIONOFSTYLE, (WPARAM)style, (LPARAM)description);
	}

	int GetBidirectional()
	{
		return Call(SCI_GETBIDIRECTIONAL, 0, 0);
	}

	void SetBidirectional(int bidirectional)
	{
		Call(SCI_SETBIDIRECTIONAL, (WPARAM)bidirectional, 0);
	}

	int GetLineCharacterIndex()
	{
		return Call(SCI_GETLINECHARACTERINDEX, 0, 0);
	}

	void AllocateLineCharacterIndex(int lineCharacterIndex)
	{
		Call(SCI_ALLOCATELINECHARACTERINDEX, (WPARAM)lineCharacterIndex, 0);
	}

	void ReleaseLineCharacterIndex(int lineCharacterIndex)
	{
		Call(SCI_RELEASELINECHARACTERINDEX, (WPARAM)lineCharacterIndex, 0);
	}

	int LineFromIndexPosition(int pos, int lineCharacterIndex)
	{
		return Call(SCI_LINEFROMINDEXPOSITION, (WPARAM)pos, (LPARAM)lineCharacterIndex);
	}

	int IndexPositionFromLine(int line, int lineCharacterIndex)
	{
		return Call(SCI_INDEXPOSITIONFROMLINE, (WPARAM)line, (LPARAM)lineCharacterIndex);
	}

};

class CScintillaCtrl : public CScintillaImpl<CScintillaCtrl>
{
	DECLARE_WND_SUPERCLASS(_T("WTL_ScintillaCtrl"), GetWndClassName())
};
