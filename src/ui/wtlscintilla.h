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
	Colour GetAdditionalCaretFore() { return Call(SCI_GETADDITIONALCARETFORE); }
	Colour GetCaretFore() { return Call(SCI_GETCARETFORE); }
	Colour GetCaretLineBack() { return Call(SCI_GETCARETLINEBACK); }
	Colour GetEdgeColour() { return Call(SCI_GETEDGECOLOUR); }
	Colour GetHotspotActiveBack() { return Call(SCI_GETHOTSPOTACTIVEBACK); }
	Colour GetHotspotActiveFore() { return Call(SCI_GETHOTSPOTACTIVEFORE); }
	Colour GetMarginBackN(int margin) { return Call(SCI_GETMARGINBACKN, margin); }
	Colour IndicGetFore(int indicator) { return Call(SCI_INDICGETFORE, indicator); }
	Colour IndicGetHoverFore(int indicator) { return Call(SCI_INDICGETHOVERFORE, indicator); }
	Colour StyleGetBack(int style) { return Call(SCI_STYLEGETBACK, style); }
	Colour StyleGetFore(int style) { return Call(SCI_STYLEGETFORE, style); }
	Line ContractedFoldNext(Line lineStart) { return Call(SCI_CONTRACTEDFOLDNEXT, lineStart); }
	Line DocLineFromVisible(Line displayLine) { return Call(SCI_DOCLINEFROMVISIBLE, displayLine); }
	Line GetFirstVisibleLine() { return Call(SCI_GETFIRSTVISIBLELINE); }
	Line GetFoldParent(Line line) { return Call(SCI_GETFOLDPARENT, line); }
	Line GetLastChild(Line line, int level) { return Call(SCI_GETLASTCHILD, line, level); }
	Line GetLineCount() { return Call(SCI_GETLINECOUNT); }
	Line LineFromIndexPosition(Position pos, int lineCharacterIndex) { return Call(SCI_LINEFROMINDEXPOSITION, pos, lineCharacterIndex); }
	Line LineFromPosition(Position pos) { return Call(SCI_LINEFROMPOSITION, pos); }
	Line LinesOnScreen() { return Call(SCI_LINESONSCREEN); }
	Line MarkerNext(Line lineStart, int markerMask) { return Call(SCI_MARKERNEXT, lineStart, markerMask); }
	Line MarkerPrevious(Line lineStart, int markerMask) { return Call(SCI_MARKERPREVIOUS, lineStart, markerMask); }
	Line VisibleFromDocLine(Line docLine) { return Call(SCI_VISIBLEFROMDOCLINE, docLine); }
	Line WrapCount(Line docLine) { return Call(SCI_WRAPCOUNT, docLine); }
	Position AutoCPosStart() { return Call(SCI_AUTOCPOSSTART); }
	Position BraceMatch(Position pos, int maxReStyle) { return Call(SCI_BRACEMATCH, pos, maxReStyle); }
	Position CallTipPosStart() { return Call(SCI_CALLTIPPOSSTART); }
	Position CharPositionFromPoint(int x, int y) { return Call(SCI_CHARPOSITIONFROMPOINT, x, y); }
	Position CharPositionFromPointClose(int x, int y) { return Call(SCI_CHARPOSITIONFROMPOINTCLOSE, x, y); }
	Position CountCharacters(Position start, Position end) { return Call(SCI_COUNTCHARACTERS, start, end); }
	Position CountCodeUnits(Position start, Position end) { return Call(SCI_COUNTCODEUNITS, start, end); }
	Position EncodedFromUTF8(const char* utf8, char* encoded) { return Call(SCI_ENCODEDFROMUTF8, reinterpret_cast<uintptr_t>(utf8), reinterpret_cast<intptr_t>(encoded)); }
	Position FindColumn(Line line, Position column) { return Call(SCI_FINDCOLUMN, line, column); }
	Position FindText(int searchFlags, void* ft) { return Call(SCI_FINDTEXT, searchFlags, reinterpret_cast<intptr_t>(ft)); }
	Position FormatRange(bool draw, void* fr) { return Call(SCI_FORMATRANGE, draw, reinterpret_cast<intptr_t>(fr)); }
	Position GetAnchor() { return Call(SCI_GETANCHOR); }
	Position GetColumn(Position pos) { return Call(SCI_GETCOLUMN, pos); }
	Position GetCurLine(Position length, char* text) { return Call(SCI_GETCURLINE, length, reinterpret_cast<intptr_t>(text)); }
	Position GetCurrentPos() { return Call(SCI_GETCURRENTPOS); }
	Position GetEdgeColumn() { return Call(SCI_GETEDGECOLUMN); }
	Position GetEndStyled() { return Call(SCI_GETENDSTYLED); }
	Position GetGapPosition() { return Call(SCI_GETGAPPOSITION); }
	Position GetHighlightGuide() { return Call(SCI_GETHIGHLIGHTGUIDE); }
	Position GetLength() { return Call(SCI_GETLENGTH); }
	Position GetLine(Line line, char* text) { return Call(SCI_GETLINE, line, reinterpret_cast<intptr_t>(text)); }
	Position GetLineEndPosition(Line line) { return Call(SCI_GETLINEENDPOSITION, line); }
	Position GetLineIndentPosition(Line line) { return Call(SCI_GETLINEINDENTPOSITION, line); }
	Position GetLineSelEndPosition(Line line) { return Call(SCI_GETLINESELENDPOSITION, line); }
	Position GetLineSelStartPosition(Line line) { return Call(SCI_GETLINESELSTARTPOSITION, line); }
	Position GetRectangularSelectionAnchor() { return Call(SCI_GETRECTANGULARSELECTIONANCHOR); }
	Position GetRectangularSelectionAnchorVirtualSpace() { return Call(SCI_GETRECTANGULARSELECTIONANCHORVIRTUALSPACE); }
	Position GetRectangularSelectionCaret() { return Call(SCI_GETRECTANGULARSELECTIONCARET); }
	Position GetRectangularSelectionCaretVirtualSpace() { return Call(SCI_GETRECTANGULARSELECTIONCARETVIRTUALSPACE); }
	Position GetSelText(char* text) { return Call(SCI_GETSELTEXT, 0, reinterpret_cast<intptr_t>(text)); }
	Position GetSelectionEnd() { return Call(SCI_GETSELECTIONEND); }
	Position GetSelectionNAnchor(int selection) { return Call(SCI_GETSELECTIONNANCHOR, selection); }
	Position GetSelectionNAnchorVirtualSpace(int selection) { return Call(SCI_GETSELECTIONNANCHORVIRTUALSPACE, selection); }
	Position GetSelectionNCaret(int selection) { return Call(SCI_GETSELECTIONNCARET, selection); }
	Position GetSelectionNCaretVirtualSpace(int selection) { return Call(SCI_GETSELECTIONNCARETVIRTUALSPACE, selection); }
	Position GetSelectionNEnd(int selection) { return Call(SCI_GETSELECTIONNEND, selection); }
	Position GetSelectionNStart(int selection) { return Call(SCI_GETSELECTIONNSTART, selection); }
	Position GetSelectionStart() { return Call(SCI_GETSELECTIONSTART); }
	Position GetStyledText(void* tr) { return Call(SCI_GETSTYLEDTEXT, 0, reinterpret_cast<intptr_t>(tr)); }
	Position GetTargetEnd() { return Call(SCI_GETTARGETEND); }
	Position GetTargetStart() { return Call(SCI_GETTARGETSTART); }
	Position GetTargetText(char* text) { return Call(SCI_GETTARGETTEXT, 0, reinterpret_cast<intptr_t>(text)); }
	Position GetText(Position length, char* text) { return Call(SCI_GETTEXT, length, reinterpret_cast<intptr_t>(text)); }
	Position GetTextLength() { return Call(SCI_GETTEXTLENGTH); }
	Position GetTextRange(void* tr) { return Call(SCI_GETTEXTRANGE, 0, reinterpret_cast<intptr_t>(tr)); }
	Position IndexPositionFromLine(Line line, int lineCharacterIndex) { return Call(SCI_INDEXPOSITIONFROMLINE, line, lineCharacterIndex); }
	Position LineLength(Line line) { return Call(SCI_LINELENGTH, line); }
	Position PositionAfter(Position pos) { return Call(SCI_POSITIONAFTER, pos); }
	Position PositionBefore(Position pos) { return Call(SCI_POSITIONBEFORE, pos); }
	Position PositionFromLine(Line line) { return Call(SCI_POSITIONFROMLINE, line); }
	Position PositionFromPoint(int x, int y) { return Call(SCI_POSITIONFROMPOINT, x, y); }
	Position PositionFromPointClose(int x, int y) { return Call(SCI_POSITIONFROMPOINTCLOSE, x, y); }
	Position PositionRelative(Position pos, Position relative) { return Call(SCI_POSITIONRELATIVE, pos, relative); }
	Position PositionRelativeCodeUnits(Position pos, Position relative) { return Call(SCI_POSITIONRELATIVECODEUNITS, pos, relative); }
	Position ReplaceTarget(Position length, const char* text) { return Call(SCI_REPLACETARGET, length, reinterpret_cast<intptr_t>(text)); }
	Position ReplaceTargetRE(Position length, const char* text) { return Call(SCI_REPLACETARGETRE, length, reinterpret_cast<intptr_t>(text)); }
	Position SearchInTarget(Position length, const char* text) { return Call(SCI_SEARCHINTARGET, length, reinterpret_cast<intptr_t>(text)); }
	Position SearchNext(int searchFlags, const char* text) { return Call(SCI_SEARCHNEXT, searchFlags, reinterpret_cast<intptr_t>(text)); }
	Position SearchPrev(int searchFlags, const char* text) { return Call(SCI_SEARCHPREV, searchFlags, reinterpret_cast<intptr_t>(text)); }
	Position TargetAsUTF8(char* s) { return Call(SCI_TARGETASUTF8, 0, reinterpret_cast<intptr_t>(s)); }
	Position WordEndPosition(Position pos, bool onlyWordCharacters) { return Call(SCI_WORDENDPOSITION, pos, onlyWordCharacters); }
	Position WordStartPosition(Position pos, bool onlyWordCharacters) { return Call(SCI_WORDSTARTPOSITION, pos, onlyWordCharacters); }
	bool AutoCActive() { return Call(SCI_AUTOCACTIVE); }
	bool AutoCGetAutoHide() { return Call(SCI_AUTOCGETAUTOHIDE); }
	bool AutoCGetCancelAtStart() { return Call(SCI_AUTOCGETCANCELATSTART); }
	bool AutoCGetChooseSingle() { return Call(SCI_AUTOCGETCHOOSESINGLE); }
	bool AutoCGetDropRestOfWord() { return Call(SCI_AUTOCGETDROPRESTOFWORD); }
	bool AutoCGetIgnoreCase() { return Call(SCI_AUTOCGETIGNORECASE); }
	bool CallTipActive() { return Call(SCI_CALLTIPACTIVE); }
	bool CanPaste() { return Call(SCI_CANPASTE); }
	bool CanRedo() { return Call(SCI_CANREDO); }
	bool CanUndo() { return Call(SCI_CANUNDO); }
	bool GetAdditionalCaretsBlink() { return Call(SCI_GETADDITIONALCARETSBLINK); }
	bool GetAdditionalCaretsVisible() { return Call(SCI_GETADDITIONALCARETSVISIBLE); }
	bool GetAdditionalSelectionTyping() { return Call(SCI_GETADDITIONALSELECTIONTYPING); }
	bool GetAllLinesVisible() { return Call(SCI_GETALLLINESVISIBLE); }
	bool GetBackSpaceUnIndents() { return Call(SCI_GETBACKSPACEUNINDENTS); }
	bool GetBufferedDraw() { return Call(SCI_GETBUFFEREDDRAW); }
	bool GetCaretLineVisible() { return Call(SCI_GETCARETLINEVISIBLE); }
	bool GetCaretLineVisibleAlways() { return Call(SCI_GETCARETLINEVISIBLEALWAYS); }
	bool GetCommandEvents() { return Call(SCI_GETCOMMANDEVENTS); }
	bool GetEndAtLastLine() { return Call(SCI_GETENDATLASTLINE); }
	bool GetFocus() { return Call(SCI_GETFOCUS); }
	bool GetFoldExpanded(Line line) { return Call(SCI_GETFOLDEXPANDED, line); }
	bool GetHScrollBar() { return Call(SCI_GETHSCROLLBAR); }
	bool GetHotspotActiveUnderline() { return Call(SCI_GETHOTSPOTACTIVEUNDERLINE); }
	bool GetHotspotSingleLine() { return Call(SCI_GETHOTSPOTSINGLELINE); }
	bool GetLineVisible(Line line) { return Call(SCI_GETLINEVISIBLE, line); }
	bool GetMarginSensitiveN(int margin) { return Call(SCI_GETMARGINSENSITIVEN, margin); }
	bool GetModify() { return Call(SCI_GETMODIFY); }
	bool GetMouseDownCaptures() { return Call(SCI_GETMOUSEDOWNCAPTURES); }
	bool GetMouseSelectionRectangularSwitch() { return Call(SCI_GETMOUSESELECTIONRECTANGULARSWITCH); }
	bool GetMouseWheelCaptures() { return Call(SCI_GETMOUSEWHEELCAPTURES); }
	bool GetMoveExtendsSelection() { return Call(SCI_GETMOVEEXTENDSSELECTION); }
	bool GetMultipleSelection() { return Call(SCI_GETMULTIPLESELECTION); }
	bool GetOvertype() { return Call(SCI_GETOVERTYPE); }
	bool GetPasteConvertEndings() { return Call(SCI_GETPASTECONVERTENDINGS); }
	bool GetReadOnly() { return Call(SCI_GETREADONLY); }
	bool GetScrollWidthTracking() { return Call(SCI_GETSCROLLWIDTHTRACKING); }
	bool GetSelEOLFilled() { return Call(SCI_GETSELEOLFILLED); }
	bool GetSelectionEmpty() { return Call(SCI_GETSELECTIONEMPTY); }
	bool GetTabIndents() { return Call(SCI_GETTABINDENTS); }
	bool GetUndoCollection() { return Call(SCI_GETUNDOCOLLECTION); }
	bool GetUseTabs() { return Call(SCI_GETUSETABS); }
	bool GetVScrollBar() { return Call(SCI_GETVSCROLLBAR); }
	bool GetViewEOL() { return Call(SCI_GETVIEWEOL); }
	bool IndicGetUnder(int indicator) { return Call(SCI_INDICGETUNDER, indicator); }
	bool IsRangeWord(Position start, Position end) { return Call(SCI_ISRANGEWORD, start, end); }
	bool SelectionIsRectangle() { return Call(SCI_SELECTIONISRECTANGLE); }
	bool StyleGetBold(int style) { return Call(SCI_STYLEGETBOLD, style); }
	bool StyleGetChangeable(int style) { return Call(SCI_STYLEGETCHANGEABLE, style); }
	bool StyleGetEOLFilled(int style) { return Call(SCI_STYLEGETEOLFILLED, style); }
	bool StyleGetHotSpot(int style) { return Call(SCI_STYLEGETHOTSPOT, style); }
	bool StyleGetItalic(int style) { return Call(SCI_STYLEGETITALIC, style); }
	bool StyleGetUnderline(int style) { return Call(SCI_STYLEGETUNDERLINE, style); }
	bool StyleGetVisible(int style) { return Call(SCI_STYLEGETVISIBLE, style); }
	int AllocateExtendedStyles(int numberStyles) { return Call(SCI_ALLOCATEEXTENDEDSTYLES, numberStyles); }
	int AllocateSubStyles(int styleBase, int numberStyles) { return Call(SCI_ALLOCATESUBSTYLES, styleBase, numberStyles); }
	int AnnotationGetLines(Line line) { return Call(SCI_ANNOTATIONGETLINES, line); }
	int AnnotationGetStyle(Line line) { return Call(SCI_ANNOTATIONGETSTYLE, line); }
	int AnnotationGetStyleOffset() { return Call(SCI_ANNOTATIONGETSTYLEOFFSET); }
	int AnnotationGetStyles(Line line, char* styles) { return Call(SCI_ANNOTATIONGETSTYLES, line, reinterpret_cast<intptr_t>(styles)); }
	int AnnotationGetText(Line line, char* text) { return Call(SCI_ANNOTATIONGETTEXT, line, reinterpret_cast<intptr_t>(text)); }
	int AnnotationGetVisible() { return Call(SCI_ANNOTATIONGETVISIBLE); }
	int AutoCGetCaseInsensitiveBehaviour() { return Call(SCI_AUTOCGETCASEINSENSITIVEBEHAVIOUR); }
	int AutoCGetCurrent() { return Call(SCI_AUTOCGETCURRENT); }
	int AutoCGetCurrentText(char* text) { return Call(SCI_AUTOCGETCURRENTTEXT, 0, reinterpret_cast<intptr_t>(text)); }
	int AutoCGetMaxHeight() { return Call(SCI_AUTOCGETMAXHEIGHT); }
	int AutoCGetMaxWidth() { return Call(SCI_AUTOCGETMAXWIDTH); }
	int AutoCGetMulti() { return Call(SCI_AUTOCGETMULTI); }
	int AutoCGetOrder() { return Call(SCI_AUTOCGETORDER); }
	int AutoCGetSeparator() { return Call(SCI_AUTOCGETSEPARATOR); }
	int AutoCGetTypeSeparator() { return Call(SCI_AUTOCGETTYPESEPARATOR); }
	int ChangeLexerState(Position start, Position end) { return Call(SCI_CHANGELEXERSTATE, start, end); }
	int DescribeKeyWordSets(char* descriptions) { return Call(SCI_DESCRIBEKEYWORDSETS, 0, reinterpret_cast<intptr_t>(descriptions)); }
	int DescribeProperty(const char* name, char* description) { return Call(SCI_DESCRIBEPROPERTY, reinterpret_cast<uintptr_t>(name), reinterpret_cast<intptr_t>(description)); }
	int DescriptionOfStyle(int style, char* description) { return Call(SCI_DESCRIPTIONOFSTYLE, style, reinterpret_cast<intptr_t>(description)); }
	int DistanceToSecondaryStyles() { return Call(SCI_DISTANCETOSECONDARYSTYLES); }
	int FoldDisplayTextGetStyle() { return Call(SCI_FOLDDISPLAYTEXTGETSTYLE); }
	int GetAccessibility() { return Call(SCI_GETACCESSIBILITY); }
	int GetAdditionalSelAlpha() { return Call(SCI_GETADDITIONALSELALPHA); }
	int GetAutomaticFold() { return Call(SCI_GETAUTOMATICFOLD); }
	int GetBidirectional() { return Call(SCI_GETBIDIRECTIONAL); }
	int GetCaretLineBackAlpha() { return Call(SCI_GETCARETLINEBACKALPHA); }
	int GetCaretLineFrame() { return Call(SCI_GETCARETLINEFRAME); }
	int GetCaretPeriod() { return Call(SCI_GETCARETPERIOD); }
	int GetCaretSticky() { return Call(SCI_GETCARETSTICKY); }
	int GetCaretStyle() { return Call(SCI_GETCARETSTYLE); }
	int GetCaretWidth() { return Call(SCI_GETCARETWIDTH); }
	int GetCharAt(Position pos) { return Call(SCI_GETCHARAT, pos); }
	int GetCharacterCategoryOptimization() { return Call(SCI_GETCHARACTERCATEGORYOPTIMIZATION); }
	int GetCodePage() { return Call(SCI_GETCODEPAGE); }
	int GetControlCharSymbol() { return Call(SCI_GETCONTROLCHARSYMBOL); }
	int GetCursor() { return Call(SCI_GETCURSOR); }
	int GetDefaultFoldDisplayText(char* text) { return Call(SCI_GETDEFAULTFOLDDISPLAYTEXT, 0, reinterpret_cast<intptr_t>(text)); }
	int GetDocumentOptions() { return Call(SCI_GETDOCUMENTOPTIONS); }
	int GetEOLMode() { return Call(SCI_GETEOLMODE); }
	int GetEdgeMode() { return Call(SCI_GETEDGEMODE); }
	int GetExtraAscent() { return Call(SCI_GETEXTRAASCENT); }
	int GetExtraDescent() { return Call(SCI_GETEXTRADESCENT); }
	int GetFoldLevel(Line line) { return Call(SCI_GETFOLDLEVEL, line); }
	int GetFontQuality() { return Call(SCI_GETFONTQUALITY); }
	int GetIMEInteraction() { return Call(SCI_GETIMEINTERACTION); }
	int GetIdentifier() { return Call(SCI_GETIDENTIFIER); }
	int GetIdleStyling() { return Call(SCI_GETIDLESTYLING); }
	int GetIndent() { return Call(SCI_GETINDENT); }
	int GetIndentationGuides() { return Call(SCI_GETINDENTATIONGUIDES); }
	int GetIndicatorCurrent() { return Call(SCI_GETINDICATORCURRENT); }
	int GetIndicatorValue() { return Call(SCI_GETINDICATORVALUE); }
	int GetLayoutCache() { return Call(SCI_GETLAYOUTCACHE); }
	int GetLexer() { return Call(SCI_GETLEXER); }
	int GetLexerLanguage(char* language) { return Call(SCI_GETLEXERLANGUAGE, 0, reinterpret_cast<intptr_t>(language)); }
	int GetLineCharacterIndex() { return Call(SCI_GETLINECHARACTERINDEX); }
	int GetLineEndTypesActive() { return Call(SCI_GETLINEENDTYPESACTIVE); }
	int GetLineEndTypesAllowed() { return Call(SCI_GETLINEENDTYPESALLOWED); }
	int GetLineEndTypesSupported() { return Call(SCI_GETLINEENDTYPESSUPPORTED); }
	int GetLineIndentation(Line line) { return Call(SCI_GETLINEINDENTATION, line); }
	int GetLineState(Line line) { return Call(SCI_GETLINESTATE, line); }
	int GetMainSelection() { return Call(SCI_GETMAINSELECTION); }
	int GetMarginCursorN(int margin) { return Call(SCI_GETMARGINCURSORN, margin); }
	int GetMarginLeft() { return Call(SCI_GETMARGINLEFT); }
	int GetMarginMaskN(int margin) { return Call(SCI_GETMARGINMASKN, margin); }
	int GetMarginOptions() { return Call(SCI_GETMARGINOPTIONS); }
	int GetMarginRight() { return Call(SCI_GETMARGINRIGHT); }
	int GetMarginTypeN(int margin) { return Call(SCI_GETMARGINTYPEN, margin); }
	int GetMarginWidthN(int margin) { return Call(SCI_GETMARGINWIDTHN, margin); }
	int GetMargins() { return Call(SCI_GETMARGINS); }
	int GetMaxLineState() { return Call(SCI_GETMAXLINESTATE); }
	int GetModEventMask() { return Call(SCI_GETMODEVENTMASK); }
	int GetMouseDwellTime() { return Call(SCI_GETMOUSEDWELLTIME); }
	int GetMultiPaste() { return Call(SCI_GETMULTIPASTE); }
	int GetNamedStyles() { return Call(SCI_GETNAMEDSTYLES); }
	int GetNextTabStop(Line line, int x) { return Call(SCI_GETNEXTTABSTOP, line, x); }
	int GetPhasesDraw() { return Call(SCI_GETPHASESDRAW); }
	int GetPositionCache() { return Call(SCI_GETPOSITIONCACHE); }
	int GetPrimaryStyleFromStyle(int style) { return Call(SCI_GETPRIMARYSTYLEFROMSTYLE, style); }
	int GetPrintColourMode() { return Call(SCI_GETPRINTCOLOURMODE); }
	int GetPrintMagnification() { return Call(SCI_GETPRINTMAGNIFICATION); }
	int GetPrintWrapMode() { return Call(SCI_GETPRINTWRAPMODE); }
	int GetProperty(const char* key, char* value) { return Call(SCI_GETPROPERTY, reinterpret_cast<uintptr_t>(key), reinterpret_cast<intptr_t>(value)); }
	int GetPropertyExpanded(const char* key, char* value) { return Call(SCI_GETPROPERTYEXPANDED, reinterpret_cast<uintptr_t>(key), reinterpret_cast<intptr_t>(value)); }
	int GetPropertyInt(const char* key, int defaultValue) { return Call(SCI_GETPROPERTYINT, reinterpret_cast<uintptr_t>(key), defaultValue); }
	int GetPunctuationChars(char* characters) { return Call(SCI_GETPUNCTUATIONCHARS, 0, reinterpret_cast<intptr_t>(characters)); }
	int GetRectangularSelectionModifier() { return Call(SCI_GETRECTANGULARSELECTIONMODIFIER); }
	int GetRepresentation(const char* encodedCharacter, char* representation) { return Call(SCI_GETREPRESENTATION, reinterpret_cast<uintptr_t>(encodedCharacter), reinterpret_cast<intptr_t>(representation)); }
	int GetScrollWidth() { return Call(SCI_GETSCROLLWIDTH); }
	int GetSearchFlags() { return Call(SCI_GETSEARCHFLAGS); }
	int GetSelAlpha() { return Call(SCI_GETSELALPHA); }
	int GetSelectionMode() { return Call(SCI_GETSELECTIONMODE); }
	int GetSelections() { return Call(SCI_GETSELECTIONS); }
	int GetStatus() { return Call(SCI_GETSTATUS); }
	int GetStyleAt(Position pos) { return Call(SCI_GETSTYLEAT, pos); }
	int GetStyleFromSubStyle(int subStyle) { return Call(SCI_GETSTYLEFROMSUBSTYLE, subStyle); }
	int GetSubStyleBases(char* styles) { return Call(SCI_GETSUBSTYLEBASES, 0, reinterpret_cast<intptr_t>(styles)); }
	int GetSubStylesLength(int styleBase) { return Call(SCI_GETSUBSTYLESLENGTH, styleBase); }
	int GetSubStylesStart(int styleBase) { return Call(SCI_GETSUBSTYLESSTART, styleBase); }
	int GetTabDrawMode() { return Call(SCI_GETTABDRAWMODE); }
	int GetTabMinimumWidth() { return Call(SCI_GETTABMINIMUMWIDTH); }
	int GetTabWidth() { return Call(SCI_GETTABWIDTH); }
	int GetTag(int tagNumber, char* tagValue) { return Call(SCI_GETTAG, tagNumber, reinterpret_cast<intptr_t>(tagValue)); }
	int GetTechnology() { return Call(SCI_GETTECHNOLOGY); }
	int GetViewWS() { return Call(SCI_GETVIEWWS); }
	int GetVirtualSpaceOptions() { return Call(SCI_GETVIRTUALSPACEOPTIONS); }
	int GetWhitespaceChars(char* characters) { return Call(SCI_GETWHITESPACECHARS, 0, reinterpret_cast<intptr_t>(characters)); }
	int GetWhitespaceSize() { return Call(SCI_GETWHITESPACESIZE); }
	int GetWordChars(char* characters) { return Call(SCI_GETWORDCHARS, 0, reinterpret_cast<intptr_t>(characters)); }
	int GetWrapIndentMode() { return Call(SCI_GETWRAPINDENTMODE); }
	int GetWrapMode() { return Call(SCI_GETWRAPMODE); }
	int GetWrapStartIndent() { return Call(SCI_GETWRAPSTARTINDENT); }
	int GetWrapVisualFlags() { return Call(SCI_GETWRAPVISUALFLAGS); }
	int GetWrapVisualFlagsLocation() { return Call(SCI_GETWRAPVISUALFLAGSLOCATION); }
	int GetXOffset() { return Call(SCI_GETXOFFSET); }
	int GetZoom() { return Call(SCI_GETZOOM); }
	int IndicGetAlpha(int indicator) { return Call(SCI_INDICGETALPHA, indicator); }
	int IndicGetFlags(int indicator) { return Call(SCI_INDICGETFLAGS, indicator); }
	int IndicGetHoverStyle(int indicator) { return Call(SCI_INDICGETHOVERSTYLE, indicator); }
	int IndicGetOutlineAlpha(int indicator) { return Call(SCI_INDICGETOUTLINEALPHA, indicator); }
	int IndicGetStyle(int indicator) { return Call(SCI_INDICGETSTYLE, indicator); }
	int IndicatorAllOnFor(Position pos) { return Call(SCI_INDICATORALLONFOR, pos); }
	int IndicatorEnd(int indicator, Position pos) { return Call(SCI_INDICATOREND, indicator, pos); }
	int IndicatorStart(int indicator, Position pos) { return Call(SCI_INDICATORSTART, indicator, pos); }
	int IndicatorValueAt(int indicator, Position pos) { return Call(SCI_INDICATORVALUEAT, indicator, pos); }
	int MarginGetStyle(Line line) { return Call(SCI_MARGINGETSTYLE, line); }
	int MarginGetStyleOffset() { return Call(SCI_MARGINGETSTYLEOFFSET); }
	int MarginGetStyles(Line line, char* styles) { return Call(SCI_MARGINGETSTYLES, line, reinterpret_cast<intptr_t>(styles)); }
	int MarginGetText(Line line, char* text) { return Call(SCI_MARGINGETTEXT, line, reinterpret_cast<intptr_t>(text)); }
	int MarkerAdd(Line line, int markerNumber) { return Call(SCI_MARKERADD, line, markerNumber); }
	int MarkerGet(Line line) { return Call(SCI_MARKERGET, line); }
	int MarkerLineFromHandle(int markerHandle) { return Call(SCI_MARKERLINEFROMHANDLE, markerHandle); }
	int MarkerSymbolDefined(int markerNumber) { return Call(SCI_MARKERSYMBOLDEFINED, markerNumber); }
	int NameOfStyle(int style, char* name) { return Call(SCI_NAMEOFSTYLE, style, reinterpret_cast<intptr_t>(name)); }
	int PointXFromPosition(Position pos) { return Call(SCI_POINTXFROMPOSITION, 0, pos); }
	int PointYFromPosition(Position pos) { return Call(SCI_POINTYFROMPOSITION, 0, pos); }
	int PropertyNames(char* names) { return Call(SCI_PROPERTYNAMES, 0, reinterpret_cast<intptr_t>(names)); }
	int PropertyType(const char* name) { return Call(SCI_PROPERTYTYPE, reinterpret_cast<uintptr_t>(name)); }
	int StyleGetCase(int style) { return Call(SCI_STYLEGETCASE, style); }
	int StyleGetCharacterSet(int style) { return Call(SCI_STYLEGETCHARACTERSET, style); }
	int StyleGetFont(int style, char* fontName) { return Call(SCI_STYLEGETFONT, style, reinterpret_cast<intptr_t>(fontName)); }
	int StyleGetSize(int style) { return Call(SCI_STYLEGETSIZE, style); }
	int StyleGetSizeFractional(int style) { return Call(SCI_STYLEGETSIZEFRACTIONAL, style); }
	int StyleGetWeight(int style) { return Call(SCI_STYLEGETWEIGHT, style); }
	int TagsOfStyle(int style, char* tags) { return Call(SCI_TAGSOFSTYLE, style, reinterpret_cast<intptr_t>(tags)); }
	int TextHeight(Line line) { return Call(SCI_TEXTHEIGHT, line); }
	int TextWidth(int style, const char* text) { return Call(SCI_TEXTWIDTH, style, reinterpret_cast<intptr_t>(text)); }
	void AddRefDocument(void* doc) { Call(SCI_ADDREFDOCUMENT, 0, reinterpret_cast<intptr_t>(doc)); }
	void AddSelection(Position caret, Position anchor) { Call(SCI_ADDSELECTION, caret, anchor); }
	void AddStyledText(Position length, const char* c) { Call(SCI_ADDSTYLEDTEXT, length, reinterpret_cast<intptr_t>(c)); }
	void AddTabStop(Line line, int x) { Call(SCI_ADDTABSTOP, line, x); }
	void AddText(Position length, const char* text) { Call(SCI_ADDTEXT, length, reinterpret_cast<intptr_t>(text)); }
	void AddUndoAction(int token, int flags) { Call(SCI_ADDUNDOACTION, token, flags); }
	void Allocate(Position bytes) { Call(SCI_ALLOCATE, bytes); }
	void AllocateLineCharacterIndex(int lineCharacterIndex) { Call(SCI_ALLOCATELINECHARACTERINDEX, lineCharacterIndex); }
	void AnnotationClearAll() { Call(SCI_ANNOTATIONCLEARALL); }
	void AnnotationSetStyle(Line line, int style) { Call(SCI_ANNOTATIONSETSTYLE, line, style); }
	void AnnotationSetStyleOffset(int style) { Call(SCI_ANNOTATIONSETSTYLEOFFSET, style); }
	void AnnotationSetStyles(Line line, const char* styles) { Call(SCI_ANNOTATIONSETSTYLES, line, reinterpret_cast<intptr_t>(styles)); }
	void AnnotationSetText(Line line, const char* text) { Call(SCI_ANNOTATIONSETTEXT, line, reinterpret_cast<intptr_t>(text)); }
	void AnnotationSetVisible(int visible) { Call(SCI_ANNOTATIONSETVISIBLE, visible); }
	void AppendText(Position length, const char* text) { Call(SCI_APPENDTEXT, length, reinterpret_cast<intptr_t>(text)); }
	void AssignCmdKey(int keyDefinition, int sciCommand) { Call(SCI_ASSIGNCMDKEY, keyDefinition, sciCommand); }
	void AutoCCancel() { Call(SCI_AUTOCCANCEL); }
	void AutoCComplete() { Call(SCI_AUTOCCOMPLETE); }
	void AutoCSelect(const char* select) { Call(SCI_AUTOCSELECT, 0, reinterpret_cast<intptr_t>(select)); }
	void AutoCSetAutoHide(bool autoHide) { Call(SCI_AUTOCSETAUTOHIDE, autoHide); }
	void AutoCSetCancelAtStart(bool cancel) { Call(SCI_AUTOCSETCANCELATSTART, cancel); }
	void AutoCSetCaseInsensitiveBehaviour(int behaviour) { Call(SCI_AUTOCSETCASEINSENSITIVEBEHAVIOUR, behaviour); }
	void AutoCSetChooseSingle(bool chooseSingle) { Call(SCI_AUTOCSETCHOOSESINGLE, chooseSingle); }
	void AutoCSetDropRestOfWord(bool dropRestOfWord) { Call(SCI_AUTOCSETDROPRESTOFWORD, dropRestOfWord); }
	void AutoCSetFillUps(const char* characterSet) { Call(SCI_AUTOCSETFILLUPS, 0, reinterpret_cast<intptr_t>(characterSet)); }
	void AutoCSetIgnoreCase(bool ignoreCase) { Call(SCI_AUTOCSETIGNORECASE, ignoreCase); }
	void AutoCSetMaxHeight(int rowCount) { Call(SCI_AUTOCSETMAXHEIGHT, rowCount); }
	void AutoCSetMaxWidth(int characterCount) { Call(SCI_AUTOCSETMAXWIDTH, characterCount); }
	void AutoCSetMulti(int multi) { Call(SCI_AUTOCSETMULTI, multi); }
	void AutoCSetOrder(int order) { Call(SCI_AUTOCSETORDER, order); }
	void AutoCSetSeparator(int separatorCharacter) { Call(SCI_AUTOCSETSEPARATOR, separatorCharacter); }
	void AutoCSetTypeSeparator(int separatorCharacter) { Call(SCI_AUTOCSETTYPESEPARATOR, separatorCharacter); }
	void AutoCShow(Position lengthEntered, const char* itemList) { Call(SCI_AUTOCSHOW, lengthEntered, reinterpret_cast<intptr_t>(itemList)); }
	void AutoCStops(const char* characterSet) { Call(SCI_AUTOCSTOPS, 0, reinterpret_cast<intptr_t>(characterSet)); }
	void BackTab() { Call(SCI_BACKTAB); }
	void BeginUndoAction() { Call(SCI_BEGINUNDOACTION); }
	void BraceBadLight(Position pos) { Call(SCI_BRACEBADLIGHT, pos); }
	void BraceBadLightIndicator(bool useSetting, int indicator) { Call(SCI_BRACEBADLIGHTINDICATOR, useSetting, indicator); }
	void BraceHighlight(Position posA, Position posB) { Call(SCI_BRACEHIGHLIGHT, posA, posB); }
	void BraceHighlightIndicator(bool useSetting, int indicator) { Call(SCI_BRACEHIGHLIGHTINDICATOR, useSetting, indicator); }
	void CallTipCancel() { Call(SCI_CALLTIPCANCEL); }
	void CallTipSetBack(Colour back) { Call(SCI_CALLTIPSETBACK, back); }
	void CallTipSetFore(Colour fore) { Call(SCI_CALLTIPSETFORE, fore); }
	void CallTipSetForeHlt(Colour fore) { Call(SCI_CALLTIPSETFOREHLT, fore); }
	void CallTipSetHlt(int highlightStart, int highlightEnd) { Call(SCI_CALLTIPSETHLT, highlightStart, highlightEnd); }
	void CallTipSetPosStart(Position posStart) { Call(SCI_CALLTIPSETPOSSTART, posStart); }
	void CallTipSetPosition(bool above) { Call(SCI_CALLTIPSETPOSITION, above); }
	void CallTipShow(Position pos, const char* definition) { Call(SCI_CALLTIPSHOW, pos, reinterpret_cast<intptr_t>(definition)); }
	void CallTipUseStyle(int tabSize) { Call(SCI_CALLTIPUSESTYLE, tabSize); }
	void Cancel() { Call(SCI_CANCEL); }
	void ChangeInsertion(Position length, const char* text) { Call(SCI_CHANGEINSERTION, length, reinterpret_cast<intptr_t>(text)); }
	void CharLeft() { Call(SCI_CHARLEFT); }
	void CharLeftExtend() { Call(SCI_CHARLEFTEXTEND); }
	void CharLeftRectExtend() { Call(SCI_CHARLEFTRECTEXTEND); }
	void CharRight() { Call(SCI_CHARRIGHT); }
	void CharRightExtend() { Call(SCI_CHARRIGHTEXTEND); }
	void CharRightRectExtend() { Call(SCI_CHARRIGHTRECTEXTEND); }
	void ChooseCaretX() { Call(SCI_CHOOSECARETX); }
	void Clear() { Call(SCI_CLEAR); }
	void ClearAll() { Call(SCI_CLEARALL); }
	void ClearAllCmdKeys() { Call(SCI_CLEARALLCMDKEYS); }
	void ClearCmdKey(int keyDefinition) { Call(SCI_CLEARCMDKEY, keyDefinition); }
	void ClearDocumentStyle() { Call(SCI_CLEARDOCUMENTSTYLE); }
	void ClearRegisteredImages() { Call(SCI_CLEARREGISTEREDIMAGES); }
	void ClearRepresentation(const char* encodedCharacter) { Call(SCI_CLEARREPRESENTATION, reinterpret_cast<uintptr_t>(encodedCharacter)); }
	void ClearSelections() { Call(SCI_CLEARSELECTIONS); }
	void ClearTabStops(Line line) { Call(SCI_CLEARTABSTOPS, line); }
	void Colourise(Position start, Position end) { Call(SCI_COLOURISE, start, end); }
	void ConvertEOLs(int eolMode) { Call(SCI_CONVERTEOLS, eolMode); }
	void Copy() { Call(SCI_COPY); }
	void CopyAllowLine() { Call(SCI_COPYALLOWLINE); }
	void CopyRange(Position start, Position end) { Call(SCI_COPYRANGE, start, end); }
	void CopyText(Position length, const char* text) { Call(SCI_COPYTEXT, length, reinterpret_cast<intptr_t>(text)); }
	void Cut() { Call(SCI_CUT); }
	void DelLineLeft() { Call(SCI_DELLINELEFT); }
	void DelLineRight() { Call(SCI_DELLINERIGHT); }
	void DelWordLeft() { Call(SCI_DELWORDLEFT); }
	void DelWordRight() { Call(SCI_DELWORDRIGHT); }
	void DelWordRightEnd() { Call(SCI_DELWORDRIGHTEND); }
	void DeleteBack() { Call(SCI_DELETEBACK); }
	void DeleteBackNotLine() { Call(SCI_DELETEBACKNOTLINE); }
	void DeleteRange(Position start, Position lengthDelete) { Call(SCI_DELETERANGE, start, lengthDelete); }
	void DocumentEnd() { Call(SCI_DOCUMENTEND); }
	void DocumentEndExtend() { Call(SCI_DOCUMENTENDEXTEND); }
	void DocumentStart() { Call(SCI_DOCUMENTSTART); }
	void DocumentStartExtend() { Call(SCI_DOCUMENTSTARTEXTEND); }
	void DropSelectionN(int selection) { Call(SCI_DROPSELECTIONN, selection); }
	void EditToggleOvertype() { Call(SCI_EDITTOGGLEOVERTYPE); }
	void EmptyUndoBuffer() { Call(SCI_EMPTYUNDOBUFFER); }
	void EndUndoAction() { Call(SCI_ENDUNDOACTION); }
	void EnsureVisible(Line line) { Call(SCI_ENSUREVISIBLE, line); }
	void EnsureVisibleEnforcePolicy(Line line) { Call(SCI_ENSUREVISIBLEENFORCEPOLICY, line); }
	void ExpandChildren(Line line, int level) { Call(SCI_EXPANDCHILDREN, line, level); }
	void FindIndicatorFlash(Position start, Position end) { Call(SCI_FINDINDICATORFLASH, start, end); }
	void FindIndicatorHide() { Call(SCI_FINDINDICATORHIDE); }
	void FindIndicatorShow(Position start, Position end) { Call(SCI_FINDINDICATORSHOW, start, end); }
	void FoldAll(int action) { Call(SCI_FOLDALL, action); }
	void FoldChildren(Line line, int action) { Call(SCI_FOLDCHILDREN, line, action); }
	void FoldDisplayTextSetStyle(int style) { Call(SCI_FOLDDISPLAYTEXTSETSTYLE, style); }
	void FoldLine(Line line, int action) { Call(SCI_FOLDLINE, line, action); }
	void FormFeed() { Call(SCI_FORMFEED); }
	void FreeSubStyles() { Call(SCI_FREESUBSTYLES); }
	void GotoLine(Line line) { Call(SCI_GOTOLINE, line); }
	void GotoPos(Position caret) { Call(SCI_GOTOPOS, caret); }
	void GrabFocus() { Call(SCI_GRABFOCUS); }
	void HideLines(Line lineStart, Line lineEnd) { Call(SCI_HIDELINES, lineStart, lineEnd); }
	void HideSelection(bool hide) { Call(SCI_HIDESELECTION, hide); }
	void Home() { Call(SCI_HOME); }
	void HomeDisplay() { Call(SCI_HOMEDISPLAY); }
	void HomeDisplayExtend() { Call(SCI_HOMEDISPLAYEXTEND); }
	void HomeExtend() { Call(SCI_HOMEEXTEND); }
	void HomeRectExtend() { Call(SCI_HOMERECTEXTEND); }
	void HomeWrap() { Call(SCI_HOMEWRAP); }
	void HomeWrapExtend() { Call(SCI_HOMEWRAPEXTEND); }
	void IndicSetAlpha(int indicator, int alpha) { Call(SCI_INDICSETALPHA, indicator, alpha); }
	void IndicSetFlags(int indicator, int flags) { Call(SCI_INDICSETFLAGS, indicator, flags); }
	void IndicSetFore(int indicator, Colour fore) { Call(SCI_INDICSETFORE, indicator, fore); }
	void IndicSetHoverFore(int indicator, Colour fore) { Call(SCI_INDICSETHOVERFORE, indicator, fore); }
	void IndicSetHoverStyle(int indicator, int indicatorStyle) { Call(SCI_INDICSETHOVERSTYLE, indicator, indicatorStyle); }
	void IndicSetOutlineAlpha(int indicator, int alpha) { Call(SCI_INDICSETOUTLINEALPHA, indicator, alpha); }
	void IndicSetStyle(int indicator, int indicatorStyle) { Call(SCI_INDICSETSTYLE, indicator, indicatorStyle); }
	void IndicSetUnder(int indicator, bool under) { Call(SCI_INDICSETUNDER, indicator, under); }
	void IndicatorClearRange(Position start, Position lengthClear) { Call(SCI_INDICATORCLEARRANGE, start, lengthClear); }
	void IndicatorFillRange(Position start, Position lengthFill) { Call(SCI_INDICATORFILLRANGE, start, lengthFill); }
	void InsertText(Position pos, const char* text) { Call(SCI_INSERTTEXT, pos, reinterpret_cast<intptr_t>(text)); }
	void LineCopy() { Call(SCI_LINECOPY); }
	void LineCut() { Call(SCI_LINECUT); }
	void LineDelete() { Call(SCI_LINEDELETE); }
	void LineDown() { Call(SCI_LINEDOWN); }
	void LineDownExtend() { Call(SCI_LINEDOWNEXTEND); }
	void LineDownRectExtend() { Call(SCI_LINEDOWNRECTEXTEND); }
	void LineDuplicate() { Call(SCI_LINEDUPLICATE); }
	void LineEnd() { Call(SCI_LINEEND); }
	void LineEndDisplay() { Call(SCI_LINEENDDISPLAY); }
	void LineEndDisplayExtend() { Call(SCI_LINEENDDISPLAYEXTEND); }
	void LineEndExtend() { Call(SCI_LINEENDEXTEND); }
	void LineEndRectExtend() { Call(SCI_LINEENDRECTEXTEND); }
	void LineEndWrap() { Call(SCI_LINEENDWRAP); }
	void LineEndWrapExtend() { Call(SCI_LINEENDWRAPEXTEND); }
	void LineReverse() { Call(SCI_LINEREVERSE); }
	void LineScroll(Position columns, Line lines) { Call(SCI_LINESCROLL, columns, lines); }
	void LineScrollDown() { Call(SCI_LINESCROLLDOWN); }
	void LineScrollUp() { Call(SCI_LINESCROLLUP); }
	void LineTranspose() { Call(SCI_LINETRANSPOSE); }
	void LineUp() { Call(SCI_LINEUP); }
	void LineUpExtend() { Call(SCI_LINEUPEXTEND); }
	void LineUpRectExtend() { Call(SCI_LINEUPRECTEXTEND); }
	void LinesJoin() { Call(SCI_LINESJOIN); }
	void LinesSplit(int pixelWidth) { Call(SCI_LINESSPLIT, pixelWidth); }
	void LoadLexerLibrary(const char* path) { Call(SCI_LOADLEXERLIBRARY, 0, reinterpret_cast<intptr_t>(path)); }
	void LowerCase() { Call(SCI_LOWERCASE); }
	void MarginSetStyle(Line line, int style) { Call(SCI_MARGINSETSTYLE, line, style); }
	void MarginSetStyleOffset(int style) { Call(SCI_MARGINSETSTYLEOFFSET, style); }
	void MarginSetStyles(Line line, const char* styles) { Call(SCI_MARGINSETSTYLES, line, reinterpret_cast<intptr_t>(styles)); }
	void MarginSetText(Line line, const char* text) { Call(SCI_MARGINSETTEXT, line, reinterpret_cast<intptr_t>(text)); }
	void MarginTextClearAll() { Call(SCI_MARGINTEXTCLEARALL); }
	void MarkerAddSet(Line line, int markerSet) { Call(SCI_MARKERADDSET, line, markerSet); }
	void MarkerDefine(int markerNumber, int markerSymbol) { Call(SCI_MARKERDEFINE, markerNumber, markerSymbol); }
	void MarkerDefinePixmap(int markerNumber, const char* pixmap) { Call(SCI_MARKERDEFINEPIXMAP, markerNumber, reinterpret_cast<intptr_t>(pixmap)); }
	void MarkerDefineRGBAImage(int markerNumber, const char* pixels) { Call(SCI_MARKERDEFINERGBAIMAGE, markerNumber, reinterpret_cast<intptr_t>(pixels)); }
	void MarkerDelete(Line line, int markerNumber) { Call(SCI_MARKERDELETE, line, markerNumber); }
	void MarkerDeleteAll(int markerNumber) { Call(SCI_MARKERDELETEALL, markerNumber); }
	void MarkerDeleteHandle(int markerHandle) { Call(SCI_MARKERDELETEHANDLE, markerHandle); }
	void MarkerEnableHighlight(bool enabled) { Call(SCI_MARKERENABLEHIGHLIGHT, enabled); }
	void MarkerSetAlpha(int markerNumber, int alpha) { Call(SCI_MARKERSETALPHA, markerNumber, alpha); }
	void MarkerSetBack(int markerNumber, Colour back) { Call(SCI_MARKERSETBACK, markerNumber, back); }
	void MarkerSetBackSelected(int markerNumber, Colour back) { Call(SCI_MARKERSETBACKSELECTED, markerNumber, back); }
	void MarkerSetFore(int markerNumber, Colour fore) { Call(SCI_MARKERSETFORE, markerNumber, fore); }
	void MoveCaretInsideView() { Call(SCI_MOVECARETINSIDEVIEW); }
	void MoveSelectedLinesDown() { Call(SCI_MOVESELECTEDLINESDOWN); }
	void MoveSelectedLinesUp() { Call(SCI_MOVESELECTEDLINESUP); }
	void MultiEdgeAddLine(Position column, Colour edgeColour) { Call(SCI_MULTIEDGEADDLINE, column, edgeColour); }
	void MultiEdgeClearAll() { Call(SCI_MULTIEDGECLEARALL); }
	void MultipleSelectAddEach() { Call(SCI_MULTIPLESELECTADDEACH); }
	void MultipleSelectAddNext() { Call(SCI_MULTIPLESELECTADDNEXT); }
	void NewLine() { Call(SCI_NEWLINE); }
	void Null() { Call(SCI_NULL); }
	void PageDown() { Call(SCI_PAGEDOWN); }
	void PageDownExtend() { Call(SCI_PAGEDOWNEXTEND); }
	void PageDownRectExtend() { Call(SCI_PAGEDOWNRECTEXTEND); }
	void PageUp() { Call(SCI_PAGEUP); }
	void PageUpExtend() { Call(SCI_PAGEUPEXTEND); }
	void PageUpRectExtend() { Call(SCI_PAGEUPRECTEXTEND); }
	void ParaDown() { Call(SCI_PARADOWN); }
	void ParaDownExtend() { Call(SCI_PARADOWNEXTEND); }
	void ParaUp() { Call(SCI_PARAUP); }
	void ParaUpExtend() { Call(SCI_PARAUPEXTEND); }
	void Paste() { Call(SCI_PASTE); }
	void RGBAImageSetHeight(int height) { Call(SCI_RGBAIMAGESETHEIGHT, height); }
	void RGBAImageSetScale(int scalePercent) { Call(SCI_RGBAIMAGESETSCALE, scalePercent); }
	void RGBAImageSetWidth(int width) { Call(SCI_RGBAIMAGESETWIDTH, width); }
	void Redo() { Call(SCI_REDO); }
	void RegisterImage(int type, const char* xpmData) { Call(SCI_REGISTERIMAGE, type, reinterpret_cast<intptr_t>(xpmData)); }
	void RegisterRGBAImage(int type, const char* pixels) { Call(SCI_REGISTERRGBAIMAGE, type, reinterpret_cast<intptr_t>(pixels)); }
	void ReleaseAllExtendedStyles() { Call(SCI_RELEASEALLEXTENDEDSTYLES); }
	void ReleaseDocument(void* doc) { Call(SCI_RELEASEDOCUMENT, 0, reinterpret_cast<intptr_t>(doc)); }
	void ReleaseLineCharacterIndex(int lineCharacterIndex) { Call(SCI_RELEASELINECHARACTERINDEX, lineCharacterIndex); }
	void ReplaceSel(const char* text) { Call(SCI_REPLACESEL, 0, reinterpret_cast<intptr_t>(text)); }
	void RotateSelection() { Call(SCI_ROTATESELECTION); }
	void ScrollCaret() { Call(SCI_SCROLLCARET); }
	void ScrollRange(Position secondary, Position primary) { Call(SCI_SCROLLRANGE, secondary, primary); }
	void ScrollToEnd() { Call(SCI_SCROLLTOEND); }
	void ScrollToStart() { Call(SCI_SCROLLTOSTART); }
	void SearchAnchor() { Call(SCI_SEARCHANCHOR); }
	void SelectAll() { Call(SCI_SELECTALL); }
	void SelectionDuplicate() { Call(SCI_SELECTIONDUPLICATE); }
	void SetAccessibility(int accessibility) { Call(SCI_SETACCESSIBILITY, accessibility); }
	void SetAdditionalCaretFore(Colour fore) { Call(SCI_SETADDITIONALCARETFORE, fore); }
	void SetAdditionalCaretsBlink(bool additionalCaretsBlink) { Call(SCI_SETADDITIONALCARETSBLINK, additionalCaretsBlink); }
	void SetAdditionalCaretsVisible(bool additionalCaretsVisible) { Call(SCI_SETADDITIONALCARETSVISIBLE, additionalCaretsVisible); }
	void SetAdditionalSelAlpha(int alpha) { Call(SCI_SETADDITIONALSELALPHA, alpha); }
	void SetAdditionalSelBack(Colour back) { Call(SCI_SETADDITIONALSELBACK, back); }
	void SetAdditionalSelFore(Colour fore) { Call(SCI_SETADDITIONALSELFORE, fore); }
	void SetAdditionalSelectionTyping(bool additionalSelectionTyping) { Call(SCI_SETADDITIONALSELECTIONTYPING, additionalSelectionTyping); }
	void SetAnchor(Position anchor) { Call(SCI_SETANCHOR, anchor); }
	void SetAutomaticFold(int automaticFold) { Call(SCI_SETAUTOMATICFOLD, automaticFold); }
	void SetBackSpaceUnIndents(bool bsUnIndents) { Call(SCI_SETBACKSPACEUNINDENTS, bsUnIndents); }
	void SetBidirectional(int bidirectional) { Call(SCI_SETBIDIRECTIONAL, bidirectional); }
	void SetBufferedDraw(bool buffered) { Call(SCI_SETBUFFEREDDRAW, buffered); }
	void SetCaretFore(Colour fore) { Call(SCI_SETCARETFORE, fore); }
	void SetCaretLineBack(Colour back) { Call(SCI_SETCARETLINEBACK, back); }
	void SetCaretLineBackAlpha(int alpha) { Call(SCI_SETCARETLINEBACKALPHA, alpha); }
	void SetCaretLineFrame(int width) { Call(SCI_SETCARETLINEFRAME, width); }
	void SetCaretLineVisible(bool show) { Call(SCI_SETCARETLINEVISIBLE, show); }
	void SetCaretLineVisibleAlways(bool alwaysVisible) { Call(SCI_SETCARETLINEVISIBLEALWAYS, alwaysVisible); }
	void SetCaretPeriod(int periodMilliseconds) { Call(SCI_SETCARETPERIOD, periodMilliseconds); }
	void SetCaretSticky(int useCaretStickyBehaviour) { Call(SCI_SETCARETSTICKY, useCaretStickyBehaviour); }
	void SetCaretStyle(int caretStyle) { Call(SCI_SETCARETSTYLE, caretStyle); }
	void SetCaretWidth(int pixelWidth) { Call(SCI_SETCARETWIDTH, pixelWidth); }
	void SetCharacterCategoryOptimization(int countCharacters) { Call(SCI_SETCHARACTERCATEGORYOPTIMIZATION, countCharacters); }
	void SetCharsDefault() { Call(SCI_SETCHARSDEFAULT); }
	void SetCodePage(int codePage) { Call(SCI_SETCODEPAGE, codePage); }
	void SetCommandEvents(bool commandEvents) { Call(SCI_SETCOMMANDEVENTS, commandEvents); }
	void SetControlCharSymbol(int symbol) { Call(SCI_SETCONTROLCHARSYMBOL, symbol); }
	void SetCurrentPos(Position caret) { Call(SCI_SETCURRENTPOS, caret); }
	void SetCursor(int cursorType) { Call(SCI_SETCURSOR, cursorType); }
	void SetDefaultFoldDisplayText(const char* text) { Call(SCI_SETDEFAULTFOLDDISPLAYTEXT, 0, reinterpret_cast<intptr_t>(text)); }
	void SetDocPointer(void* doc) { Call(SCI_SETDOCPOINTER, 0, reinterpret_cast<intptr_t>(doc)); }
	void SetEOLMode(int eolMode) { Call(SCI_SETEOLMODE, eolMode); }
	void SetEdgeColour(Colour edgeColour) { Call(SCI_SETEDGECOLOUR, edgeColour); }
	void SetEdgeColumn(Position column) { Call(SCI_SETEDGECOLUMN, column); }
	void SetEdgeMode(int edgeMode) { Call(SCI_SETEDGEMODE, edgeMode); }
	void SetEmptySelection(Position caret) { Call(SCI_SETEMPTYSELECTION, caret); }
	void SetEndAtLastLine(bool endAtLastLine) { Call(SCI_SETENDATLASTLINE, endAtLastLine); }
	void SetExtraAscent(int extraAscent) { Call(SCI_SETEXTRAASCENT, extraAscent); }
	void SetExtraDescent(int extraDescent) { Call(SCI_SETEXTRADESCENT, extraDescent); }
	void SetFirstVisibleLine(Line displayLine) { Call(SCI_SETFIRSTVISIBLELINE, displayLine); }
	void SetFocus(bool focus) { Call(SCI_SETFOCUS, focus); }
	void SetFoldExpanded(Line line, bool expanded) { Call(SCI_SETFOLDEXPANDED, line, expanded); }
	void SetFoldFlags(int flags) { Call(SCI_SETFOLDFLAGS, flags); }
	void SetFoldLevel(Line line, int level) { Call(SCI_SETFOLDLEVEL, line, level); }
	void SetFoldMarginColour(bool useSetting, Colour back) { Call(SCI_SETFOLDMARGINCOLOUR, useSetting, back); }
	void SetFoldMarginHiColour(bool useSetting, Colour fore) { Call(SCI_SETFOLDMARGINHICOLOUR, useSetting, fore); }
	void SetFontQuality(int fontQuality) { Call(SCI_SETFONTQUALITY, fontQuality); }
	void SetHScrollBar(bool visible) { Call(SCI_SETHSCROLLBAR, visible); }
	void SetHighlightGuide(Position column) { Call(SCI_SETHIGHLIGHTGUIDE, column); }
	void SetHotspotActiveBack(bool useSetting, Colour back) { Call(SCI_SETHOTSPOTACTIVEBACK, useSetting, back); }
	void SetHotspotActiveFore(bool useSetting, Colour fore) { Call(SCI_SETHOTSPOTACTIVEFORE, useSetting, fore); }
	void SetHotspotActiveUnderline(bool underline) { Call(SCI_SETHOTSPOTACTIVEUNDERLINE, underline); }
	void SetHotspotSingleLine(bool singleLine) { Call(SCI_SETHOTSPOTSINGLELINE, singleLine); }
	void SetIMEInteraction(int imeInteraction) { Call(SCI_SETIMEINTERACTION, imeInteraction); }
	void SetIdentifier(int identifier) { Call(SCI_SETIDENTIFIER, identifier); }
	void SetIdentifiers(int style, const char* identifiers) { Call(SCI_SETIDENTIFIERS, style, reinterpret_cast<intptr_t>(identifiers)); }
	void SetIdleStyling(int idleStyling) { Call(SCI_SETIDLESTYLING, idleStyling); }
	void SetIndent(int indentSize) { Call(SCI_SETINDENT, indentSize); }
	void SetIndentationGuides(int indentView) { Call(SCI_SETINDENTATIONGUIDES, indentView); }
	void SetIndicatorCurrent(int indicator) { Call(SCI_SETINDICATORCURRENT, indicator); }
	void SetIndicatorValue(int value) { Call(SCI_SETINDICATORVALUE, value); }
	void SetKeyWords(int keyWordSet, const char* keyWords) { Call(SCI_SETKEYWORDS, keyWordSet, reinterpret_cast<intptr_t>(keyWords)); }
	void SetLayoutCache(int cacheMode) { Call(SCI_SETLAYOUTCACHE, cacheMode); }
	void SetLengthForEncode(Position bytes) { Call(SCI_SETLENGTHFORENCODE, bytes); }
	void SetLexer(int lexer) { Call(SCI_SETLEXER, lexer); }
	void SetLexerLanguage(const char* language) { Call(SCI_SETLEXERLANGUAGE, 0, reinterpret_cast<intptr_t>(language)); }
	void SetLineEndTypesAllowed(int lineEndBitSet) { Call(SCI_SETLINEENDTYPESALLOWED, lineEndBitSet); }
	void SetLineIndentation(Line line, int indentation) { Call(SCI_SETLINEINDENTATION, line, indentation); }
	void SetLineState(Line line, int state) { Call(SCI_SETLINESTATE, line, state); }
	void SetMainSelection(int selection) { Call(SCI_SETMAINSELECTION, selection); }
	void SetMarginBackN(int margin, Colour back) { Call(SCI_SETMARGINBACKN, margin, back); }
	void SetMarginCursorN(int margin, int cursor) { Call(SCI_SETMARGINCURSORN, margin, cursor); }
	void SetMarginLeft(int pixelWidth) { Call(SCI_SETMARGINLEFT, 0, pixelWidth); }
	void SetMarginMaskN(int margin, int mask) { Call(SCI_SETMARGINMASKN, margin, mask); }
	void SetMarginOptions(int marginOptions) { Call(SCI_SETMARGINOPTIONS, marginOptions); }
	void SetMarginRight(int pixelWidth) { Call(SCI_SETMARGINRIGHT, 0, pixelWidth); }
	void SetMarginSensitiveN(int margin, bool sensitive) { Call(SCI_SETMARGINSENSITIVEN, margin, sensitive); }
	void SetMarginTypeN(int margin, int marginType) { Call(SCI_SETMARGINTYPEN, margin, marginType); }
	void SetMarginWidthN(int margin, int pixelWidth) { Call(SCI_SETMARGINWIDTHN, margin, pixelWidth); }
	void SetMargins(int margins) { Call(SCI_SETMARGINS, margins); }
	void SetModEventMask(int eventMask) { Call(SCI_SETMODEVENTMASK, eventMask); }
	void SetMouseDownCaptures(bool captures) { Call(SCI_SETMOUSEDOWNCAPTURES, captures); }
	void SetMouseDwellTime(int periodMilliseconds) { Call(SCI_SETMOUSEDWELLTIME, periodMilliseconds); }
	void SetMouseSelectionRectangularSwitch(bool mouseSelectionRectangularSwitch) { Call(SCI_SETMOUSESELECTIONRECTANGULARSWITCH, mouseSelectionRectangularSwitch); }
	void SetMouseWheelCaptures(bool captures) { Call(SCI_SETMOUSEWHEELCAPTURES, captures); }
	void SetMultiPaste(int multiPaste) { Call(SCI_SETMULTIPASTE, multiPaste); }
	void SetMultipleSelection(bool multipleSelection) { Call(SCI_SETMULTIPLESELECTION, multipleSelection); }
	void SetOvertype(bool overType) { Call(SCI_SETOVERTYPE, overType); }
	void SetPasteConvertEndings(bool convert) { Call(SCI_SETPASTECONVERTENDINGS, convert); }
	void SetPhasesDraw(int phases) { Call(SCI_SETPHASESDRAW, phases); }
	void SetPositionCache(int size) { Call(SCI_SETPOSITIONCACHE, size); }
	void SetPrintColourMode(int mode) { Call(SCI_SETPRINTCOLOURMODE, mode); }
	void SetPrintMagnification(int magnification) { Call(SCI_SETPRINTMAGNIFICATION, magnification); }
	void SetPrintWrapMode(int wrapMode) { Call(SCI_SETPRINTWRAPMODE, wrapMode); }
	void SetProperty(const char* key, const char* value) { Call(SCI_SETPROPERTY, reinterpret_cast<uintptr_t>(key), reinterpret_cast<intptr_t>(value)); }
	void SetPunctuationChars(const char* characters) { Call(SCI_SETPUNCTUATIONCHARS, 0, reinterpret_cast<intptr_t>(characters)); }
	void SetReadOnly(bool readOnly) { Call(SCI_SETREADONLY, readOnly); }
	void SetRectangularSelectionAnchor(Position anchor) { Call(SCI_SETRECTANGULARSELECTIONANCHOR, anchor); }
	void SetRectangularSelectionAnchorVirtualSpace(Position space) { Call(SCI_SETRECTANGULARSELECTIONANCHORVIRTUALSPACE, space); }
	void SetRectangularSelectionCaret(Position caret) { Call(SCI_SETRECTANGULARSELECTIONCARET, caret); }
	void SetRectangularSelectionCaretVirtualSpace(Position space) { Call(SCI_SETRECTANGULARSELECTIONCARETVIRTUALSPACE, space); }
	void SetRectangularSelectionModifier(int modifier) { Call(SCI_SETRECTANGULARSELECTIONMODIFIER, modifier); }
	void SetRepresentation(const char* encodedCharacter, const char* representation) { Call(SCI_SETREPRESENTATION, reinterpret_cast<uintptr_t>(encodedCharacter), reinterpret_cast<intptr_t>(representation)); }
	void SetSavePoint() { Call(SCI_SETSAVEPOINT); }
	void SetScrollWidth(int pixelWidth) { Call(SCI_SETSCROLLWIDTH, pixelWidth); }
	void SetScrollWidthTracking(bool tracking) { Call(SCI_SETSCROLLWIDTHTRACKING, tracking); }
	void SetSearchFlags(int searchFlags) { Call(SCI_SETSEARCHFLAGS, searchFlags); }
	void SetSel(Position anchor, Position caret) { Call(SCI_SETSEL, anchor, caret); }
	void SetSelAlpha(int alpha) { Call(SCI_SETSELALPHA, alpha); }
	void SetSelBack(bool useSetting, Colour back) { Call(SCI_SETSELBACK, useSetting, back); }
	void SetSelEOLFilled(bool filled) { Call(SCI_SETSELEOLFILLED, filled); }
	void SetSelFore(bool useSetting, Colour fore) { Call(SCI_SETSELFORE, useSetting, fore); }
	void SetSelection(Position caret, Position anchor) { Call(SCI_SETSELECTION, caret, anchor); }
	void SetSelectionEnd(Position caret) { Call(SCI_SETSELECTIONEND, caret); }
	void SetSelectionMode(int selectionMode) { Call(SCI_SETSELECTIONMODE, selectionMode); }
	void SetSelectionNAnchor(int selection, Position anchor) { Call(SCI_SETSELECTIONNANCHOR, selection, anchor); }
	void SetSelectionNAnchorVirtualSpace(int selection, Position space) { Call(SCI_SETSELECTIONNANCHORVIRTUALSPACE, selection, space); }
	void SetSelectionNCaret(int selection, Position caret) { Call(SCI_SETSELECTIONNCARET, selection, caret); }
	void SetSelectionNCaretVirtualSpace(int selection, Position space) { Call(SCI_SETSELECTIONNCARETVIRTUALSPACE, selection, space); }
	void SetSelectionNEnd(int selection, Position caret) { Call(SCI_SETSELECTIONNEND, selection, caret); }
	void SetSelectionNStart(int selection, Position anchor) { Call(SCI_SETSELECTIONNSTART, selection, anchor); }
	void SetSelectionStart(Position anchor) { Call(SCI_SETSELECTIONSTART, anchor); }
	void SetStatus(int status) { Call(SCI_SETSTATUS, status); }
	void SetStyling(Position length, int style) { Call(SCI_SETSTYLING, length, style); }
	void SetStylingEx(Position length, const char* styles) { Call(SCI_SETSTYLINGEX, length, reinterpret_cast<intptr_t>(styles)); }
	void SetTabDrawMode(int tabDrawMode) { Call(SCI_SETTABDRAWMODE, tabDrawMode); }
	void SetTabIndents(bool tabIndents) { Call(SCI_SETTABINDENTS, tabIndents); }
	void SetTabMinimumWidth(int pixels) { Call(SCI_SETTABMINIMUMWIDTH, pixels); }
	void SetTabWidth(int tabWidth) { Call(SCI_SETTABWIDTH, tabWidth); }
	void SetTargetEnd(Position end) { Call(SCI_SETTARGETEND, end); }
	void SetTargetRange(Position start, Position end) { Call(SCI_SETTARGETRANGE, start, end); }
	void SetTargetStart(Position start) { Call(SCI_SETTARGETSTART, start); }
	void SetTechnology(int technology) { Call(SCI_SETTECHNOLOGY, technology); }
	void SetText(const char* text) { Call(SCI_SETTEXT, 0, reinterpret_cast<intptr_t>(text)); }
	void SetUndoCollection(bool collectUndo) { Call(SCI_SETUNDOCOLLECTION, collectUndo); }
	void SetUseTabs(bool useTabs) { Call(SCI_SETUSETABS, useTabs); }
	void SetVScrollBar(bool visible) { Call(SCI_SETVSCROLLBAR, visible); }
	void SetViewEOL(bool visible) { Call(SCI_SETVIEWEOL, visible); }
	void SetViewWS(int viewWS) { Call(SCI_SETVIEWWS, viewWS); }
	void SetVirtualSpaceOptions(int virtualSpaceOptions) { Call(SCI_SETVIRTUALSPACEOPTIONS, virtualSpaceOptions); }
	void SetVisiblePolicy(int visiblePolicy, int visibleSlop) { Call(SCI_SETVISIBLEPOLICY, visiblePolicy, visibleSlop); }
	void SetWhitespaceBack(bool useSetting, Colour back) { Call(SCI_SETWHITESPACEBACK, useSetting, back); }
	void SetWhitespaceChars(const char* characters) { Call(SCI_SETWHITESPACECHARS, 0, reinterpret_cast<intptr_t>(characters)); }
	void SetWhitespaceFore(bool useSetting, Colour fore) { Call(SCI_SETWHITESPACEFORE, useSetting, fore); }
	void SetWhitespaceSize(int size) { Call(SCI_SETWHITESPACESIZE, size); }
	void SetWordChars(const char* characters) { Call(SCI_SETWORDCHARS, 0, reinterpret_cast<intptr_t>(characters)); }
	void SetWrapIndentMode(int wrapIndentMode) { Call(SCI_SETWRAPINDENTMODE, wrapIndentMode); }
	void SetWrapMode(int wrapMode) { Call(SCI_SETWRAPMODE, wrapMode); }
	void SetWrapStartIndent(int indent) { Call(SCI_SETWRAPSTARTINDENT, indent); }
	void SetWrapVisualFlags(int wrapVisualFlags) { Call(SCI_SETWRAPVISUALFLAGS, wrapVisualFlags); }
	void SetWrapVisualFlagsLocation(int wrapVisualFlagsLocation) { Call(SCI_SETWRAPVISUALFLAGSLOCATION, wrapVisualFlagsLocation); }
	void SetXCaretPolicy(int caretPolicy, int caretSlop) { Call(SCI_SETXCARETPOLICY, caretPolicy, caretSlop); }
	void SetXOffset(int xOffset) { Call(SCI_SETXOFFSET, xOffset); }
	void SetYCaretPolicy(int caretPolicy, int caretSlop) { Call(SCI_SETYCARETPOLICY, caretPolicy, caretSlop); }
	void SetZoom(int zoomInPoints) { Call(SCI_SETZOOM, zoomInPoints); }
	void ShowLines(Line lineStart, Line lineEnd) { Call(SCI_SHOWLINES, lineStart, lineEnd); }
	void StartRecord() { Call(SCI_STARTRECORD); }
	void StartStyling(Position start, int unused) { Call(SCI_STARTSTYLING, start, unused); }
	void StopRecord() { Call(SCI_STOPRECORD); }
	void StutteredPageDown() { Call(SCI_STUTTEREDPAGEDOWN); }
	void StutteredPageDownExtend() { Call(SCI_STUTTEREDPAGEDOWNEXTEND); }
	void StutteredPageUp() { Call(SCI_STUTTEREDPAGEUP); }
	void StutteredPageUpExtend() { Call(SCI_STUTTEREDPAGEUPEXTEND); }
	void StyleClearAll() { Call(SCI_STYLECLEARALL); }
	void StyleResetDefault() { Call(SCI_STYLERESETDEFAULT); }
	void StyleSetBack(int style, Colour back) { Call(SCI_STYLESETBACK, style, back); }
	void StyleSetBold(int style, bool bold) { Call(SCI_STYLESETBOLD, style, bold); }
	void StyleSetCase(int style, int caseVisible) { Call(SCI_STYLESETCASE, style, caseVisible); }
	void StyleSetChangeable(int style, bool changeable) { Call(SCI_STYLESETCHANGEABLE, style, changeable); }
	void StyleSetCharacterSet(int style, int characterSet) { Call(SCI_STYLESETCHARACTERSET, style, characterSet); }
	void StyleSetEOLFilled(int style, bool eolFilled) { Call(SCI_STYLESETEOLFILLED, style, eolFilled); }
	void StyleSetFont(int style, const char* fontName) { Call(SCI_STYLESETFONT, style, reinterpret_cast<intptr_t>(fontName)); }
	void StyleSetFore(int style, Colour fore) { Call(SCI_STYLESETFORE, style, fore); }
	void StyleSetHotSpot(int style, bool hotspot) { Call(SCI_STYLESETHOTSPOT, style, hotspot); }
	void StyleSetItalic(int style, bool italic) { Call(SCI_STYLESETITALIC, style, italic); }
	void StyleSetSize(int style, int sizePoints) { Call(SCI_STYLESETSIZE, style, sizePoints); }
	void StyleSetSizeFractional(int style, int sizeHundredthPoints) { Call(SCI_STYLESETSIZEFRACTIONAL, style, sizeHundredthPoints); }
	void StyleSetUnderline(int style, bool underline) { Call(SCI_STYLESETUNDERLINE, style, underline); }
	void StyleSetVisible(int style, bool visible) { Call(SCI_STYLESETVISIBLE, style, visible); }
	void StyleSetWeight(int style, int weight) { Call(SCI_STYLESETWEIGHT, style, weight); }
	void SwapMainAnchorCaret() { Call(SCI_SWAPMAINANCHORCARET); }
	void Tab() { Call(SCI_TAB); }
	void TargetFromSelection() { Call(SCI_TARGETFROMSELECTION); }
	void TargetWholeDocument() { Call(SCI_TARGETWHOLEDOCUMENT); }
	void ToggleCaretSticky() { Call(SCI_TOGGLECARETSTICKY); }
	void ToggleFold(Line line) { Call(SCI_TOGGLEFOLD, line); }
	void ToggleFoldShowText(Line line, const char* text) { Call(SCI_TOGGLEFOLDSHOWTEXT, line, reinterpret_cast<intptr_t>(text)); }
	void Undo() { Call(SCI_UNDO); }
	void UpperCase() { Call(SCI_UPPERCASE); }
	void UsePopUp(int popUpMode) { Call(SCI_USEPOPUP, popUpMode); }
	void UserListShow(int listType, const char* itemList) { Call(SCI_USERLISTSHOW, listType, reinterpret_cast<intptr_t>(itemList)); }
	void VCHome() { Call(SCI_VCHOME); }
	void VCHomeDisplay() { Call(SCI_VCHOMEDISPLAY); }
	void VCHomeDisplayExtend() { Call(SCI_VCHOMEDISPLAYEXTEND); }
	void VCHomeExtend() { Call(SCI_VCHOMEEXTEND); }
	void VCHomeRectExtend() { Call(SCI_VCHOMERECTEXTEND); }
	void VCHomeWrap() { Call(SCI_VCHOMEWRAP); }
	void VCHomeWrapExtend() { Call(SCI_VCHOMEWRAPEXTEND); }
	void VerticalCentreCaret() { Call(SCI_VERTICALCENTRECARET); }
	void WordLeft() { Call(SCI_WORDLEFT); }
	void WordLeftEnd() { Call(SCI_WORDLEFTEND); }
	void WordLeftEndExtend() { Call(SCI_WORDLEFTENDEXTEND); }
	void WordLeftExtend() { Call(SCI_WORDLEFTEXTEND); }
	void WordPartLeft() { Call(SCI_WORDPARTLEFT); }
	void WordPartLeftExtend() { Call(SCI_WORDPARTLEFTEXTEND); }
	void WordPartRight() { Call(SCI_WORDPARTRIGHT); }
	void WordPartRightExtend() { Call(SCI_WORDPARTRIGHTEXTEND); }
	void WordRight() { Call(SCI_WORDRIGHT); }
	void WordRightEnd() { Call(SCI_WORDRIGHTEND); }
	void WordRightEndExtend() { Call(SCI_WORDRIGHTENDEXTEND); }
	void WordRightExtend() { Call(SCI_WORDRIGHTEXTEND); }
	void ZoomIn() { Call(SCI_ZOOMIN); }
	void ZoomOut() { Call(SCI_ZOOMOUT); }
	void* CreateDocument(Position bytes, int documentOptions) { return reinterpret_cast<void*>(Call(SCI_CREATEDOCUMENT, bytes, documentOptions)); }
	void* CreateLoader(Position bytes, int documentOptions) { return reinterpret_cast<void*>(Call(SCI_CREATELOADER, bytes, documentOptions)); }
	void* GetCharacterPointer() { return reinterpret_cast<void*>(Call(SCI_GETCHARACTERPOINTER)); }
	void* GetDirectFunction() { return reinterpret_cast<void*>(Call(SCI_GETDIRECTFUNCTION)); }
	void* GetDirectPointer() { return reinterpret_cast<void*>(Call(SCI_GETDIRECTPOINTER)); }
	void* GetDocPointer() { return reinterpret_cast<void*>(Call(SCI_GETDOCPOINTER)); }
	void* GetRangePointer(Position start, Position lengthRange) { return reinterpret_cast<void*>(Call(SCI_GETRANGEPOINTER, start, lengthRange)); }
	void* PrivateLexerCall(int operation, void* pointer) { return reinterpret_cast<void*>(Call(SCI_PRIVATELEXERCALL, operation, reinterpret_cast<intptr_t>(pointer))); }

private:
	using FunctionDirect = intptr_t(*)(intptr_t ptr, unsigned int msg, uintptr_t wParam, intptr_t lParam);

	intptr_t Call(unsigned int msg, uintptr_t wParam = 0, intptr_t lParam = 0)
	{
		return fn(ptr, msg, wParam, lParam);
	}

	FunctionDirect fn;
	intptr_t ptr;
};
