#pragma once
#include "com_tools.h"
#include "script_interface_h.h"

struct panel_tooltip_param
{
	panel_tooltip_param() : tooltip_hwnd(nullptr) {}

	BSTR font_name;
	HWND tooltip_hwnd;
	float font_size;
	int font_style;
};

using panel_tooltip_param_ptr = std::shared_ptr<panel_tooltip_param>;

class ContextMenuManager : public IDisposableImpl4<IContextMenuManager>
{
protected:
	ContextMenuManager();
	~ContextMenuManager();

	void FinalRelease() override;

	contextmenu_manager::ptr m_cm;

public:
	STDMETHODIMP BuildMenu(IMenuObj* p, int base_id, int max_id) override;
	STDMETHODIMP ExecuteByID(UINT id, VARIANT_BOOL* p) override;
	STDMETHODIMP InitContext(IMetadbHandleList* handles) override;
	STDMETHODIMP InitContextPlaylist() override;
	STDMETHODIMP InitNowPlaying() override;
};

class DropSourceAction : public IDisposableImpl4<IDropSourceAction>
{
protected:
	DropSourceAction();
	~DropSourceAction();

	void FinalRelease() override;

public:
	void Reset()
	{
		m_playlist_idx = 0;
		m_base = 0;
		m_to_select = true;
		m_effect = DROPEFFECT_NONE;
	}

	STDMETHODIMP get_Effect(UINT* p) override;
	STDMETHODIMP put_Base(UINT base) override;
	STDMETHODIMP put_Effect(UINT effect) override;
	STDMETHODIMP put_Playlist(UINT id) override;
	STDMETHODIMP put_ToSelect(VARIANT_BOOL to_select) override;

	DWORD m_effect;
	bool m_to_select;
	t_size m_base;
	t_size m_playlist_idx;
};

class FileInfo : public IDisposableImpl4<IFileInfo>
{
protected:
	FileInfo(file_info_impl* p_info_ptr);
	~FileInfo();

	void FinalRelease() override;

	file_info_impl* m_info_ptr;

public:
	STDMETHODIMP get__ptr(void** pp) override;
	STDMETHODIMP InfoFind(BSTR name, int* p) override;
	STDMETHODIMP InfoName(UINT idx, BSTR* p) override;
	STDMETHODIMP InfoValue(UINT idx, BSTR* p) override;
	STDMETHODIMP MetaFind(BSTR name, int* p) override;
	STDMETHODIMP MetaName(UINT idx, BSTR* p) override;
	STDMETHODIMP MetaValue(UINT idx, UINT vidx, BSTR* p) override;
	STDMETHODIMP MetaValueCount(UINT idx, UINT* p) override;
	STDMETHODIMP get_InfoCount(UINT* p) override;
	STDMETHODIMP get_MetaCount(UINT* p) override;
};

class GdiBitmap : public GdiObj<IGdiBitmap, Gdiplus::Bitmap>
{
protected:
	GdiBitmap(Gdiplus::Bitmap* p);

public:
	STDMETHODIMP ApplyAlpha(BYTE alpha, IGdiBitmap** pp) override;
	STDMETHODIMP ApplyMask(IGdiBitmap* mask, VARIANT_BOOL* p) override;
	STDMETHODIMP Clone(float x, float y, float w, float h, IGdiBitmap** pp) override;
	STDMETHODIMP CreateRawBitmap(IGdiRawBitmap** pp) override;
	STDMETHODIMP GetColourSchemeJSON(UINT count, BSTR* p) override;
	STDMETHODIMP GetGraphics(IGdiGraphics** pp) override;
	STDMETHODIMP ReleaseGraphics(IGdiGraphics* p) override;
	STDMETHODIMP Resize(UINT w, UINT h, int interpolationMode, IGdiBitmap** pp) override;
	STDMETHODIMP RotateFlip(UINT mode) override;
	STDMETHODIMP SaveAs(BSTR path, BSTR format, VARIANT_BOOL* p) override;
	STDMETHODIMP StackBlur(BYTE radius) override;
	STDMETHODIMP get_Height(UINT* p) override;
	STDMETHODIMP get_Width(UINT* p) override;
};

class GdiFont : public GdiObj<IGdiFont, Gdiplus::Font>
{
protected:
	GdiFont(Gdiplus::Font* p, HFONT hFont, bool managed = true);
	~GdiFont();

	void FinalRelease() override;

	HFONT m_hFont;
	bool m_managed;

public:
	STDMETHODIMP get__HFont(UINT* p) override;
	STDMETHODIMP get_Height(UINT* p) override;
	STDMETHODIMP get_Name(BSTR* p) override;
	STDMETHODIMP get_Size(float* p) override;
	STDMETHODIMP get_Style(int* p) override;
};

class GdiGraphics : public GdiObj<IGdiGraphics, Gdiplus::Graphics>
{
protected:
	GdiGraphics();

	static void GetRoundRectPath(Gdiplus::GraphicsPath& gp, Gdiplus::RectF& rect, float arc_width, float arc_height);

public:
	STDMETHODIMP Dispose() override
	{
		return E_NOTIMPL;
	}

	STDMETHODIMP put__ptr(void* p) override;
	STDMETHODIMP CalcTextHeight(BSTR str, IGdiFont* font, UINT* p) override;
	STDMETHODIMP CalcTextWidth(BSTR str, IGdiFont* font, UINT* p) override;
	STDMETHODIMP DrawEllipse(float x, float y, float w, float h, float line_width, LONGLONG colour) override;
	STDMETHODIMP DrawImage(IGdiBitmap* image, float dstX, float dstY, float dstW, float dstH, float srcX, float srcY, float srcW, float srcH, float angle, BYTE alpha) override;
	STDMETHODIMP DrawLine(float x1, float y1, float x2, float y2, float line_width, LONGLONG colour) override;
	STDMETHODIMP DrawPolygon(LONGLONG colour, float line_width, VARIANT points) override;
	STDMETHODIMP DrawRect(float x, float y, float w, float h, float line_width, LONGLONG colour) override;
	STDMETHODIMP DrawRoundRect(float x, float y, float w, float h, float arc_width, float arc_height, float line_width, LONGLONG colour) override;
	STDMETHODIMP DrawString(BSTR str, IGdiFont* font, LONGLONG colour, float x, float y, float w, float h, int flags) override;
	STDMETHODIMP EstimateLineWrap(BSTR str, IGdiFont* font, int max_width, VARIANT* p) override;
	STDMETHODIMP FillEllipse(float x, float y, float w, float h, LONGLONG colour) override;
	STDMETHODIMP FillGradRect(float x, float y, float w, float h, float angle, LONGLONG colour1, LONGLONG colour2, float focus) override;
	STDMETHODIMP FillPolygon(LONGLONG colour, int fillmode, VARIANT points) override;
	STDMETHODIMP FillRoundRect(float x, float y, float w, float h, float arc_width, float arc_height, LONGLONG colour) override;
	STDMETHODIMP FillSolidRect(float x, float y, float w, float h, LONGLONG colour) override;
	STDMETHODIMP GdiAlphaBlend(IGdiRawBitmap* bitmap, int dstX, int dstY, int dstW, int dstH, int srcX, int srcY, int srcW, int srcH, BYTE alpha) override;
	STDMETHODIMP GdiDrawBitmap(IGdiRawBitmap* bitmap, int dstX, int dstY, int dstW, int dstH, int srcX, int srcY, int srcW, int srcH) override;
	STDMETHODIMP GdiDrawText(BSTR str, IGdiFont* font, LONGLONG colour, int x, int y, int w, int h, int format) override;
	STDMETHODIMP MeasureString(BSTR str, IGdiFont* font, float x, float y, float w, float h, int flags, IMeasureStringInfo** pp) override;
	STDMETHODIMP SetInterpolationMode(int mode) override;
	STDMETHODIMP SetSmoothingMode(int mode) override;
	STDMETHODIMP SetTextRenderingHint(UINT mode) override;
};

class GdiRawBitmap : public IDisposableImpl4<IGdiRawBitmap>
{
protected:
	GdiRawBitmap(Gdiplus::Bitmap* p_bmp);
	~GdiRawBitmap();

	HBITMAP CreateHBITMAP(Gdiplus::Bitmap* bitmap_ptr);
	void FinalRelease() override;

	HBITMAP m_hbmp, m_hbmpold;
	HDC m_hdc;
	t_size m_height, m_width;

public:
	STDMETHODIMP get__Handle(HDC* p) override;
	STDMETHODIMP get_Height(UINT* p) override;
	STDMETHODIMP get_Width(UINT* p) override;
};

class MainMenuManager : public IDisposableImpl4<IMainMenuManager>
{
protected:
	MainMenuManager();
	~MainMenuManager();

	void FinalRelease() override;

	mainmenu_manager::ptr m_mm;

public:
	STDMETHODIMP BuildMenu(IMenuObj* p, UINT base_id, int count) override;
	STDMETHODIMP ExecuteByID(UINT id, VARIANT_BOOL* p) override;
	STDMETHODIMP Init(BSTR root_name) override;
};

class MeasureStringInfo : public IDispatchImpl3<IMeasureStringInfo>
{
protected:
	MeasureStringInfo(float x, float y, float w, float h, int l, int c);
	~MeasureStringInfo();

	float m_x, m_y, m_w, m_h;
	int m_l, m_c;

public:
	STDMETHODIMP get_chars(int* p) override;
	STDMETHODIMP get_height(float* p) override;
	STDMETHODIMP get_lines(int* p) override;
	STDMETHODIMP get_width(float* p) override;
	STDMETHODIMP get_x(float* p) override;
	STDMETHODIMP get_y(float* p) override;
};

class MenuObj : public IDisposableImpl4<IMenuObj>
{
protected:
	MenuObj(HWND wnd_parent);
	~MenuObj();

	void FinalRelease() override;

	HMENU m_hMenu;
	HWND m_wnd_parent;
	bool m_has_detached;

public:
	STDMETHODIMP get__ID(HMENU* p) override;
	STDMETHODIMP AppendMenuItem(UINT flags, UINT item_id, BSTR text) override;
	STDMETHODIMP AppendMenuSeparator() override;
	STDMETHODIMP AppendTo(IMenuObj* parent, UINT flags, BSTR text) override;
	STDMETHODIMP CheckMenuItem(UINT item_id, VARIANT_BOOL check) override;
	STDMETHODIMP CheckMenuRadioItem(UINT first, UINT last, UINT selected) override;
	STDMETHODIMP TrackPopupMenu(int x, int y, UINT flags, UINT* p) override;
};

class MetadbHandle : public IDisposableImpl4<IMetadbHandle>
{
protected:
	MetadbHandle(const metadb_handle_ptr& src);
	~MetadbHandle();

	void FinalRelease() override;

	metadb_handle_ptr m_handle;

public:
	STDMETHODIMP get__ptr(void** pp) override;
	STDMETHODIMP ClearStats() override;
	STDMETHODIMP Compare(IMetadbHandle* handle, VARIANT_BOOL* p) override;
	STDMETHODIMP GetAlbumArt(UINT art_id, VARIANT_BOOL need_stub, VARIANT* p) override;
	STDMETHODIMP GetFileInfo(IFileInfo** pp) override;
	STDMETHODIMP RefreshStats() override;
	STDMETHODIMP SetFirstPlayed(BSTR first_played) override;
	STDMETHODIMP SetLastPlayed(BSTR last_played) override;
	STDMETHODIMP SetLoved(UINT loved) override;
	STDMETHODIMP SetPlaycount(UINT playcount) override;
	STDMETHODIMP SetRating(UINT rating) override;
	STDMETHODIMP get_FileSize(LONGLONG* p) override;
	STDMETHODIMP get_Length(double* p) override;
	STDMETHODIMP get_Path(BSTR* p) override;
	STDMETHODIMP get_RawPath(BSTR* p) override;
	STDMETHODIMP get_SubSong(UINT* p) override;
};

class MetadbHandleList : public IDisposableImpl4<IMetadbHandleList>
{
protected:
	MetadbHandleList(metadb_handle_list_cref handles);
	~MetadbHandleList();

	void FinalRelease() override;

	metadb_handle_list m_handles;

public:
	STDMETHODIMP get__ptr(void** pp) override;
	STDMETHODIMP Add(IMetadbHandle* handle) override;
	STDMETHODIMP AddRange(IMetadbHandleList* handles) override;
	STDMETHODIMP AttachImage(BSTR path, UINT art_id) override;
	STDMETHODIMP BSearch(IMetadbHandle* handle, int* p) override;
	STDMETHODIMP CalcTotalDuration(double* p) override;
	STDMETHODIMP CalcTotalSize(LONGLONG* p) override;
	STDMETHODIMP Clone(IMetadbHandleList** pp) override;
	STDMETHODIMP Convert(VARIANT* p) override;
	STDMETHODIMP Find(IMetadbHandle* handle, int* p) override;
	STDMETHODIMP GetLibraryRelativePaths(VARIANT* p) override;
	STDMETHODIMP Insert(UINT index, IMetadbHandle* handle) override;
	STDMETHODIMP InsertRange(UINT index, IMetadbHandleList* handles) override;
	STDMETHODIMP MakeDifference(IMetadbHandleList* handles) override;
	STDMETHODIMP MakeIntersection(IMetadbHandleList* handles) override;
	STDMETHODIMP MakeUnion(IMetadbHandleList* handles) override;
	STDMETHODIMP OrderByFormat(__interface ITitleFormat* script, int direction) override;
	STDMETHODIMP OrderByPath() override;
	STDMETHODIMP OrderByRelativePath() override;
	STDMETHODIMP RefreshStats() override;
	STDMETHODIMP Remove(IMetadbHandle* handle) override;
	STDMETHODIMP RemoveAll() override;
	STDMETHODIMP RemoveAttachedImage(UINT art_id) override;
	STDMETHODIMP RemoveAttachedImages() override;
	STDMETHODIMP RemoveById(UINT index) override;
	STDMETHODIMP RemoveRange(UINT from, UINT count) override;
	STDMETHODIMP Sort() override;
	STDMETHODIMP UpdateFileInfoFromJSON(BSTR str) override;
	STDMETHODIMP get_Count(UINT* p) override;
	STDMETHODIMP get_Item(UINT index, IMetadbHandle** pp) override;
	STDMETHODIMP put_Item(UINT index, IMetadbHandle* handle) override;
};

class PlaybackQueueItem : public IDisposableImpl4<IPlaybackQueueItem>
{
protected:
	PlaybackQueueItem();
	PlaybackQueueItem(const t_playback_queue_item& playbackQueueItem);
	~PlaybackQueueItem();

	void FinalRelease() override;

	t_playback_queue_item m_playback_queue_item;

public:
	STDMETHODIMP get__ptr(void** pp) override;
	STDMETHODIMP get_Handle(IMetadbHandle** pp) override;
	STDMETHODIMP get_PlaylistIndex(int* p) override;
	STDMETHODIMP get_PlaylistItemIndex(int* p) override;
};

class PlayingItemLocation : public IDispatchImpl3<IPlayingItemLocation>
{
protected:
	PlayingItemLocation(bool isValid, t_size playlistIndex, t_size playlistItemIndex);

	bool m_isValid;
	t_size m_playlistIndex;
	t_size m_playlistItemIndex;

public:
	STDMETHODIMP get_IsValid(VARIANT_BOOL* p) override;
	STDMETHODIMP get_PlaylistIndex(int* p) override;
	STDMETHODIMP get_PlaylistItemIndex(int* p) override;
};

class PlaylistRecyclerManager : public IDispatchImpl3<IPlaylistRecyclerManager>
{
public:
	STDMETHODIMP Purge(VARIANT affectedItems) override;
	STDMETHODIMP Restore(UINT index) override;
	STDMETHODIMP get_Content(UINT index, IMetadbHandleList** pp) override;
	STDMETHODIMP get_Count(UINT* p) override;
	STDMETHODIMP get_Name(UINT index, BSTR* p) override;
};

class Profiler : public IDispatchImpl3<IProfiler>
{
protected:
	Profiler(const char* p_name);
	~Profiler();

	pfc::hires_timer m_timer;
	pfc::string_simple m_name;

public:
	STDMETHODIMP Print() override;
	STDMETHODIMP Reset() override;
	STDMETHODIMP get_Time(int* p) override;
};

class ThemeManager : public IDisposableImpl4<IThemeManager>
{
protected:
	ThemeManager(HWND hwnd, BSTR classlist);
	~ThemeManager();

	void FinalRelease() override;

	HTHEME m_theme;
	int m_partid;
	int m_stateid;

public:
	STDMETHODIMP DrawThemeBackground(IGdiGraphics* gr, int x, int y, int w, int h, int clip_x, int clip_y, int clip_w, int clip_h) override;
	STDMETHODIMP IsThemePartDefined(int partid, int stateid, VARIANT_BOOL* p) override;
	STDMETHODIMP SetPartAndStateID(int partid, int stateid) override;
};

class TitleFormat : public IDisposableImpl4<ITitleFormat>
{
protected:
	TitleFormat(BSTR expr);
	~TitleFormat();

	void FinalRelease() override;

	titleformat_object::ptr m_obj;

public:
	STDMETHODIMP get__ptr(void** pp) override;
	STDMETHODIMP Eval(VARIANT_BOOL force, BSTR* p) override;
	STDMETHODIMP EvalWithMetadb(IMetadbHandle* handle, BSTR* p) override;
	STDMETHODIMP EvalWithMetadbs(IMetadbHandleList* handles, VARIANT* p) override;
};

class Tooltip : public IDisposableImpl4<ITooltip>
{
protected:
	Tooltip(HWND p_wndparent, const panel_tooltip_param_ptr& p_param_ptr);
	~Tooltip();

	void FinalRelease() override;

	BSTR m_tip_buffer;
	HWND m_wndparent;
	HWND m_wndtooltip;
	TOOLINFO m_ti;
	panel_tooltip_param_ptr m_panel_tooltip_param_ptr;

public:
	STDMETHODIMP Activate() override;
	STDMETHODIMP Deactivate() override;
	STDMETHODIMP GetDelayTime(int type, int* p) override;
	STDMETHODIMP SetDelayTime(int type, int time) override;
	STDMETHODIMP SetMaxWidth(int width) override;
	STDMETHODIMP TrackPosition(int x, int y) override;
	STDMETHODIMP get_Text(BSTR* p) override;
	STDMETHODIMP put_Text(BSTR text) override;
	STDMETHODIMP put_TrackActivate(VARIANT_BOOL activate) override;
};

class UiSelectionHolder : public IDisposableImpl4<IUiSelectionHolder>
{
protected:
	UiSelectionHolder(const ui_selection_holder::ptr& holder);
	~UiSelectionHolder();

	void FinalRelease() override;

	ui_selection_holder::ptr m_holder;

public:
	STDMETHODIMP SetPlaylistSelectionTracking() override;
	STDMETHODIMP SetPlaylistTracking() override;
	STDMETHODIMP SetSelection(IMetadbHandleList* handles) override;
};
