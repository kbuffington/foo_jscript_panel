#pragma once
#include "com_tools.h"
#include "script_interface_h.h"

#include <memory>

_COM_SMARTPTR_TYPEDEF(IConsole, __uuidof(IConsole));
_COM_SMARTPTR_TYPEDEF(IFb, __uuidof(IFb));
_COM_SMARTPTR_TYPEDEF(IGdi, __uuidof(IGdi));
_COM_SMARTPTR_TYPEDEF(IGdiGraphics, __uuidof(IGdiGraphics));
_COM_SMARTPTR_TYPEDEF(IPlman, __uuidof(IPlman));
_COM_SMARTPTR_TYPEDEF(IPlaylistRecyclerManager, __uuidof(IPlaylistRecyclerManager));
_COM_SMARTPTR_TYPEDEF(IUtils, __uuidof(IUtils));
_COM_SMARTPTR_TYPEDEF(IWindow, __uuidof(IWindow));

struct panel_tooltip_param
{
	panel_tooltip_param() : tooltip_hwnd(0) {}

	BSTR font_name;
	HWND tooltip_hwnd;
	SIZE tooltip_size;
	float font_size;
	int font_style;
};

using panel_tooltip_param_ptr = std::shared_ptr<panel_tooltip_param>;

class ContextMenuManager : public IDisposableImpl4<IContextMenuManager>
{
protected:
	ContextMenuManager();
	virtual ~ContextMenuManager();

	virtual void FinalRelease();

	contextmenu_manager::ptr m_cm;

public:
	STDMETHODIMP BuildMenu(IMenuObj* p, int base_id, int max_id);
	STDMETHODIMP ExecuteByID(UINT id, VARIANT_BOOL* p);
	STDMETHODIMP InitContext(IMetadbHandleList* handles);
	STDMETHODIMP InitContextPlaylist();
	STDMETHODIMP InitNowPlaying();
};

class DropSourceAction : public IDisposableImpl4<IDropSourceAction>
{
protected:
	DropSourceAction();
	virtual ~DropSourceAction();

	virtual void FinalRelease();

public:
	void Reset()
	{
		m_playlist_idx = 0;
		m_base = 0;
		m_to_select = true;
		m_effect = DROPEFFECT_NONE;
	}

	STDMETHODIMP get_Effect(UINT* effect);
	STDMETHODIMP put_Base(UINT base);
	STDMETHODIMP put_Effect(UINT effect);
	STDMETHODIMP put_Playlist(UINT id);
	STDMETHODIMP put_ToSelect(VARIANT_BOOL to_select);

	DWORD m_effect;
	bool m_to_select;
	t_size m_base;
	t_size m_playlist_idx;
};

class FileInfo : public IDisposableImpl4<IFileInfo>
{
protected:
	FileInfo(file_info_impl* p_info_ptr);
	virtual ~FileInfo();

	virtual void FinalRelease();

	file_info_impl* m_info_ptr;

public:
	STDMETHODIMP get__ptr(void** pp);
	STDMETHODIMP InfoFind(BSTR name, int* p);
	STDMETHODIMP InfoName(UINT idx, BSTR* p);
	STDMETHODIMP InfoValue(UINT idx, BSTR* p);
	STDMETHODIMP MetaFind(BSTR name, int* p);
	STDMETHODIMP MetaName(UINT idx, BSTR* p);
	STDMETHODIMP MetaValue(UINT idx, UINT vidx, BSTR* p);
	STDMETHODIMP MetaValueCount(UINT idx, UINT* p);
	STDMETHODIMP get_InfoCount(UINT* p);
	STDMETHODIMP get_MetaCount(UINT* p);
};

class GdiBitmap : public GdiObj<IGdiBitmap, Gdiplus::Bitmap>
{
protected:
	GdiBitmap(Gdiplus::Bitmap* p);

public:
	STDMETHODIMP ApplyAlpha(BYTE alpha, IGdiBitmap** pp);
	STDMETHODIMP ApplyMask(IGdiBitmap* mask, VARIANT_BOOL* p);
	STDMETHODIMP Clone(float x, float y, float w, float h, IGdiBitmap** pp);
	STDMETHODIMP CreateRawBitmap(IGdiRawBitmap** pp);
	STDMETHODIMP GetColourScheme(UINT count, VARIANT* p);
	STDMETHODIMP GetColourSchemeJSON(UINT count, BSTR* p);
	STDMETHODIMP GetGraphics(IGdiGraphics** pp);
	STDMETHODIMP ReleaseGraphics(IGdiGraphics* p);
	STDMETHODIMP Resize(UINT w, UINT h, int interpolationMode, IGdiBitmap** pp);
	STDMETHODIMP RotateFlip(UINT mode);
	STDMETHODIMP SaveAs(BSTR path, BSTR format, VARIANT_BOOL* p);
	STDMETHODIMP StackBlur(BYTE radius);
	STDMETHODIMP get_Height(UINT* p);
	STDMETHODIMP get_Width(UINT* p);
};

class GdiFont : public GdiObj<IGdiFont, Gdiplus::Font>
{
protected:
	GdiFont(Gdiplus::Font* p, HFONT hFont, bool managed = true);
	virtual ~GdiFont();

	virtual void FinalRelease();

	HFONT m_hFont;
	bool m_managed;

public:
	STDMETHODIMP get__HFont(UINT* p);
	STDMETHODIMP get_Height(UINT* p);
	STDMETHODIMP get_Name(BSTR* p);
	STDMETHODIMP get_Size(float* p);
	STDMETHODIMP get_Style(int* p);
};

class GdiGraphics : public GdiObj<IGdiGraphics, Gdiplus::Graphics>
{
protected:
	GdiGraphics();

	static void GetRoundRectPath(Gdiplus::GraphicsPath& gp, Gdiplus::RectF& rect, float arc_width, float arc_height);

public:
	STDMETHODIMP Dispose()
	{
		return E_NOTIMPL;
	}

	STDMETHODIMP CalcTextHeight(BSTR str, IGdiFont* font, UINT* p);
	STDMETHODIMP CalcTextWidth(BSTR str, IGdiFont* font, UINT* p);
	STDMETHODIMP DrawEllipse(float x, float y, float w, float h, float line_width, VARIANT colour);
	STDMETHODIMP DrawImage(IGdiBitmap* image, float dstX, float dstY, float dstW, float dstH, float srcX, float srcY, float srcW, float srcH, float angle, BYTE alpha);
	STDMETHODIMP DrawLine(float x1, float y1, float x2, float y2, float line_width, VARIANT colour);
	STDMETHODIMP DrawPolygon(VARIANT colour, float line_width, VARIANT points);
	STDMETHODIMP DrawRect(float x, float y, float w, float h, float line_width, VARIANT colour);
	STDMETHODIMP DrawRoundRect(float x, float y, float w, float h, float arc_width, float arc_height, float line_width, VARIANT colour);
	STDMETHODIMP DrawString(BSTR str, IGdiFont* font, VARIANT colour, float x, float y, float w, float h, int flags);
	STDMETHODIMP EstimateLineWrap(BSTR str, IGdiFont* font, int max_width, VARIANT* p);
	STDMETHODIMP FillEllipse(float x, float y, float w, float h, VARIANT colour);
	STDMETHODIMP FillGradRect(float x, float y, float w, float h, float angle, VARIANT colour1, VARIANT colour2, float focus);
	STDMETHODIMP FillPolygon(VARIANT colour, int fillmode, VARIANT points);
	STDMETHODIMP FillRoundRect(float x, float y, float w, float h, float arc_width, float arc_height, VARIANT colour);
	STDMETHODIMP FillSolidRect(float x, float y, float w, float h, VARIANT colour);
	STDMETHODIMP GdiAlphaBlend(IGdiRawBitmap* bitmap, int dstX, int dstY, int dstW, int dstH, int srcX, int srcY, int srcW, int srcH, BYTE alpha);
	STDMETHODIMP GdiDrawBitmap(IGdiRawBitmap* bitmap, int dstX, int dstY, int dstW, int dstH, int srcX, int srcY, int srcW, int srcH);
	STDMETHODIMP GdiDrawText(BSTR str, IGdiFont* font, VARIANT colour, int x, int y, int w, int h, int format);
	STDMETHODIMP MeasureString(BSTR str, IGdiFont* font, float x, float y, float w, float h, int flags, IMeasureStringInfo** pp);
	STDMETHODIMP SetInterpolationMode(int mode);
	STDMETHODIMP SetSmoothingMode(int mode);
	STDMETHODIMP SetTextRenderingHint(UINT mode);
	STDMETHODIMP put__ptr(void* p);
};

class GdiRawBitmap : public IDisposableImpl4<IGdiRawBitmap>
{
protected:
	GdiRawBitmap(Gdiplus::Bitmap* p_bmp);
	virtual ~GdiRawBitmap();

	HBITMAP CreateHBITMAP(Gdiplus::Bitmap* bitmap_ptr);
	virtual void FinalRelease();

	HBITMAP m_hbmp, m_hbmpold;
	HDC m_hdc;
	t_size m_width, m_height;

public:
	STDMETHODIMP get__Handle(HDC* p);
	STDMETHODIMP get_Height(UINT* p);
	STDMETHODIMP get_Width(UINT* p);
};

class MainMenuManager : public IDisposableImpl4<IMainMenuManager>
{
protected:
	MainMenuManager();
	virtual ~MainMenuManager();

	virtual void FinalRelease();

	mainmenu_manager::ptr m_mm;

public:
	STDMETHODIMP BuildMenu(IMenuObj* p, int base_id, int count);
	STDMETHODIMP ExecuteByID(UINT id, VARIANT_BOOL* p);
	STDMETHODIMP Init(BSTR root_name);
};

class MeasureStringInfo : public IDispatchImpl3<IMeasureStringInfo>
{
protected:
	MeasureStringInfo(float x, float y, float w, float h, int l, int c);
	virtual ~MeasureStringInfo();

	float m_x, m_y, m_w, m_h;
	int m_l, m_c;

public:
	STDMETHODIMP get_chars(int* p);
	STDMETHODIMP get_height(float* p);
	STDMETHODIMP get_lines(int* p);
	STDMETHODIMP get_width(float* p);
	STDMETHODIMP get_x(float* p);
	STDMETHODIMP get_y(float* p);
};

class MenuObj : public IDisposableImpl4<IMenuObj>
{
protected:
	MenuObj(HWND wnd_parent);
	virtual ~MenuObj();

	virtual void FinalRelease();

	HMENU m_hMenu;
	HWND m_wnd_parent;
	bool m_has_detached;

public:
	STDMETHODIMP get__ID(HMENU* p);
	STDMETHODIMP AppendMenuItem(UINT flags, UINT item_id, BSTR text);
	STDMETHODIMP AppendMenuSeparator();
	STDMETHODIMP AppendTo(IMenuObj* parent, UINT flags, BSTR text);
	STDMETHODIMP CheckMenuItem(UINT item_id, VARIANT_BOOL check);
	STDMETHODIMP CheckMenuRadioItem(UINT first, UINT last, UINT selected);
	STDMETHODIMP TrackPopupMenu(int x, int y, UINT flags, UINT* item_id);
};

class MetadbHandle : public IDisposableImpl4<IMetadbHandle>
{
protected:
	MetadbHandle(const metadb_handle_ptr& src);
	MetadbHandle(metadb_handle* src);
	virtual ~MetadbHandle();

	virtual void FinalRelease();

	metadb_handle_ptr m_handle;

public:
	STDMETHODIMP get__ptr(void** pp);
	STDMETHODIMP ClearStats();
	STDMETHODIMP Compare(IMetadbHandle* handle, VARIANT_BOOL* p);
	STDMETHODIMP GetAlbumArt(UINT art_id, VARIANT_BOOL need_stub, VARIANT* p);
	STDMETHODIMP GetFileInfo(IFileInfo** pp);
	STDMETHODIMP RefreshStats();
	STDMETHODIMP SetFirstPlayed(BSTR first_played);
	STDMETHODIMP SetLastPlayed(BSTR last_played);
	STDMETHODIMP SetLoved(UINT loved);
	STDMETHODIMP SetPlaycount(UINT playcount);
	STDMETHODIMP SetRating(UINT rating);
	STDMETHODIMP get_FileSize(LONGLONG* p);
	STDMETHODIMP get_Length(double* p);
	STDMETHODIMP get_Path(BSTR* p);
	STDMETHODIMP get_RawPath(BSTR* p);
	STDMETHODIMP get_SubSong(UINT* p);
};

class MetadbHandleList : public IDisposableImpl4<IMetadbHandleList>
{
protected:
	MetadbHandleList(metadb_handle_list_cref handles);
	virtual ~MetadbHandleList();

	virtual void FinalRelease();

	metadb_handle_list m_handles;

public:
	STDMETHODIMP get__ptr(void** pp);
	STDMETHODIMP Add(IMetadbHandle* handle);
	STDMETHODIMP AddRange(IMetadbHandleList* handles);
	STDMETHODIMP AttachImage(BSTR image_path, UINT art_id);
	STDMETHODIMP BSearch(IMetadbHandle* handle, int* p);
	STDMETHODIMP CalcTotalDuration(double* p);
	STDMETHODIMP CalcTotalSize(LONGLONG* p);
	STDMETHODIMP Clone(IMetadbHandleList** pp);
	STDMETHODIMP Convert(VARIANT* p);
	STDMETHODIMP Find(IMetadbHandle* handle, int* p);
	STDMETHODIMP GetLibraryRelativePaths(VARIANT* p);
	STDMETHODIMP Insert(UINT index, IMetadbHandle* handle);
	STDMETHODIMP InsertRange(UINT index, IMetadbHandleList* handles);
	STDMETHODIMP MakeDifference(IMetadbHandleList* handles);
	STDMETHODIMP MakeIntersection(IMetadbHandleList* handles);
	STDMETHODIMP MakeUnion(IMetadbHandleList* handles);
	STDMETHODIMP OrderByFormat(__interface ITitleFormat* script, int direction);
	STDMETHODIMP OrderByPath();
	STDMETHODIMP OrderByRelativePath();
	STDMETHODIMP RefreshStats();
	STDMETHODIMP Remove(IMetadbHandle* handle);
	STDMETHODIMP RemoveAll();
	STDMETHODIMP RemoveAttachedImage(UINT art_id);
	STDMETHODIMP RemoveAttachedImages();
	STDMETHODIMP RemoveById(UINT index);
	STDMETHODIMP RemoveRange(UINT from, UINT count);
	STDMETHODIMP Sort();
	STDMETHODIMP UpdateFileInfoFromJSON(BSTR str);
	STDMETHODIMP get_Count(UINT* p);
	STDMETHODIMP get_Item(UINT index, IMetadbHandle** pp);
	STDMETHODIMP put_Item(UINT index, IMetadbHandle* handle);
};

class PlaybackQueueItem : public IDisposableImpl4<IPlaybackQueueItem>
{
protected:
	PlaybackQueueItem();
	PlaybackQueueItem(const t_playback_queue_item& playbackQueueItem);
	virtual ~PlaybackQueueItem();

	virtual void FinalRelease();

	t_playback_queue_item m_playback_queue_item;

public:
	STDMETHODIMP get__ptr(void** pp);
	STDMETHODIMP get_Handle(IMetadbHandle** pp);
	STDMETHODIMP get_PlaylistIndex(int* p);
	STDMETHODIMP get_PlaylistItemIndex(int* p);
};

class PlayingItemLocation : public IDispatchImpl3<IPlayingItemLocation>
{
protected:
	PlayingItemLocation(bool isValid, t_size playlistIndex, t_size playlistItemIndex);

	bool m_isValid;
	t_size m_playlistIndex;
	t_size m_playlistItemIndex;

public:
	STDMETHODIMP get_IsValid(VARIANT_BOOL* p);
	STDMETHODIMP get_PlaylistIndex(int* p);
	STDMETHODIMP get_PlaylistItemIndex(int* p);
};

class PlaylistRecyclerManager : public IDispatchImpl3<IPlaylistRecyclerManager>
{
public:
	STDMETHODIMP Purge(VARIANT affectedItems);
	STDMETHODIMP Restore(UINT index);
	STDMETHODIMP get_Content(UINT index, IMetadbHandleList** pp);
	STDMETHODIMP get_Count(UINT* p);
	STDMETHODIMP get_Name(UINT index, BSTR* p);
};

class Profiler : public IDispatchImpl3<IProfiler>
{
protected:
	Profiler(const char* p_name);
	virtual ~Profiler();

	pfc::hires_timer m_timer;
	pfc::string_simple m_name;

public:
	STDMETHODIMP Print();
	STDMETHODIMP Reset();
	STDMETHODIMP get_Time(int* p);
};

class ThemeManager : public IDisposableImpl4<IThemeManager>
{
protected:
	ThemeManager(HWND hwnd, BSTR classlist);
	virtual ~ThemeManager();

	virtual void FinalRelease();

	HTHEME m_theme;
	int m_partid;
	int m_stateid;

public:
	STDMETHODIMP DrawThemeBackground(IGdiGraphics* gr, int x, int y, int w, int h, int clip_x, int clip_y, int clip_w, int clip_h);
	STDMETHODIMP IsThemePartDefined(int partid, int stateid, VARIANT_BOOL* p);
	STDMETHODIMP SetPartAndStateID(int partid, int stateid);
};

class TitleFormat : public IDisposableImpl4<ITitleFormat>
{
protected:
	TitleFormat(BSTR expr);
	virtual ~TitleFormat();

	virtual void FinalRelease();

	titleformat_object::ptr m_obj;

public:
	STDMETHODIMP get__ptr(void** pp);
	STDMETHODIMP Eval(VARIANT_BOOL force, BSTR* p);
	STDMETHODIMP EvalWithMetadb(IMetadbHandle* handle, BSTR* p);
	STDMETHODIMP EvalWithMetadbs(IMetadbHandleList* handles, VARIANT* p);
};

class Tooltip : public IDisposableImpl4<ITooltip>
{
protected:
	Tooltip(HWND p_wndparent, const panel_tooltip_param_ptr& p_param_ptr);
	virtual ~Tooltip();

	virtual void FinalRelease();

	BSTR m_tip_buffer;
	HWND m_wndtooltip;
	HWND m_wndparent;
	TOOLINFO m_ti;
	panel_tooltip_param_ptr m_panel_tooltip_param_ptr;

public:
	STDMETHODIMP Activate();
	STDMETHODIMP Deactivate();
	STDMETHODIMP GetDelayTime(int type, int* p);
	STDMETHODIMP SetDelayTime(int type, int time);
	STDMETHODIMP SetMaxWidth(int width);
	STDMETHODIMP TrackPosition(int x, int y);
	STDMETHODIMP get_Text(BSTR* p);
	STDMETHODIMP put_Text(BSTR text);
	STDMETHODIMP put_TrackActivate(VARIANT_BOOL activate);
};

class UiSelectionHolder : public IDisposableImpl4<IUiSelectionHolder>
{
protected:
	UiSelectionHolder(const ui_selection_holder::ptr& holder);
	virtual ~UiSelectionHolder();

	virtual void FinalRelease();

	ui_selection_holder::ptr m_holder;

public:
	STDMETHODIMP SetPlaylistSelectionTracking();
	STDMETHODIMP SetPlaylistTracking();
	STDMETHODIMP SetSelection(IMetadbHandleList* handles);
};
