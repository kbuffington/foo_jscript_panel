#include "stdafx.h"
#include "helpers.h"
#include "script_interface.h"
#include "stats.h"

#include <kmeans.h>
#include <stackblur.h>

ContextMenuManager::ContextMenuManager() {}
ContextMenuManager::~ContextMenuManager() {}
void ContextMenuManager::FinalRelease()
{
	m_cm.release();
}

STDMETHODIMP ContextMenuManager::BuildMenu(IMenuObj* p, int base_id, int max_id)
{
	if (m_cm.is_empty()) return E_POINTER;

	HMENU menuid;
	p->get__ID(&menuid);
	contextmenu_node* parent = m_cm->get_root();
	m_cm->win32_build_menu(menuid, parent, base_id, max_id);
	return S_OK;
}

STDMETHODIMP ContextMenuManager::ExecuteByID(UINT id, VARIANT_BOOL* p)
{
	if (m_cm.is_empty() || !p) return E_POINTER;

	*p = TO_VARIANT_BOOL(m_cm->execute_by_id(id));
	return S_OK;
}

STDMETHODIMP ContextMenuManager::InitContext(IMetadbHandleList* handles)
{
	metadb_handle_list* handles_ptr = NULL;
	handles->get__ptr((void**)&handles_ptr);
	contextmenu_manager::g_create(m_cm);
	m_cm->init_context(*handles_ptr, contextmenu_manager::flag_show_shortcuts);
	return S_OK;
}

STDMETHODIMP ContextMenuManager::InitContextPlaylist()
{
	contextmenu_manager::g_create(m_cm);
	m_cm->init_context_playlist(contextmenu_manager::flag_show_shortcuts);
	return S_OK;
}

STDMETHODIMP ContextMenuManager::InitNowPlaying()
{
	contextmenu_manager::g_create(m_cm);
	m_cm->init_context_now_playing(contextmenu_manager::flag_show_shortcuts);
	return S_OK;
}

DropSourceAction::DropSourceAction()
{
	Reset();
}

DropSourceAction::~DropSourceAction() {}
void DropSourceAction::FinalRelease() {}


STDMETHODIMP DropSourceAction::get_Effect(UINT* effect)
{
	if (!effect) return E_POINTER;

	*effect = m_effect;
	return S_OK;
}

STDMETHODIMP DropSourceAction::put_Base(UINT base)
{
	m_base = base;
	return S_OK;
}

STDMETHODIMP DropSourceAction::put_Effect(UINT effect)
{
	m_effect = effect;
	return S_OK;
}

STDMETHODIMP DropSourceAction::put_Playlist(UINT id)
{
	m_playlist_idx = id;
	return S_OK;
}

STDMETHODIMP DropSourceAction::put_ToSelect(VARIANT_BOOL select)
{
	m_to_select = select != VARIANT_FALSE;
	return S_OK;
}

FileInfo::FileInfo(file_info_impl* p_info_ptr) : m_info_ptr(p_info_ptr) {}
FileInfo::~FileInfo() {}

void FileInfo::FinalRelease()
{
	if (m_info_ptr)
	{
		delete m_info_ptr;
		m_info_ptr = NULL;
	}
}

STDMETHODIMP FileInfo::get__ptr(void** pp)
{
	if (!pp) return E_POINTER;

	*pp = m_info_ptr;
	return S_OK;
}

STDMETHODIMP FileInfo::InfoFind(BSTR name, int* p)
{
	if (!m_info_ptr || !p) return E_POINTER;

	*p = m_info_ptr->info_find(string_utf8_from_wide(name));
	return S_OK;
}

STDMETHODIMP FileInfo::InfoName(UINT idx, BSTR* p)
{
	if (!m_info_ptr || !p) return E_POINTER;

	if (idx < m_info_ptr->info_get_count())
	{
		*p = SysAllocString(string_wide_from_utf8_fast(m_info_ptr->info_enum_name(idx)));
		return S_OK;
	}
	return E_INVALIDARG;
}

STDMETHODIMP FileInfo::InfoValue(UINT idx, BSTR* p)
{
	if (!m_info_ptr || !p) return E_POINTER;

	if (idx < m_info_ptr->info_get_count())
	{
		*p = SysAllocString(string_wide_from_utf8_fast(m_info_ptr->info_enum_value(idx)));
		return S_OK;
	}
	return E_INVALIDARG;
}

STDMETHODIMP FileInfo::MetaFind(BSTR name, int* p)
{
	if (!m_info_ptr || !p) return E_POINTER;

	*p = m_info_ptr->meta_find(string_utf8_from_wide(name));
	return S_OK;
}

STDMETHODIMP FileInfo::MetaName(UINT idx, BSTR* p)
{
	if (!m_info_ptr || !p) return E_POINTER;

	if (idx < m_info_ptr->meta_get_count())
	{
		*p = SysAllocString(string_wide_from_utf8_fast(m_info_ptr->meta_enum_name(idx)));
		return S_OK;
	}
	return E_INVALIDARG;
}

STDMETHODIMP FileInfo::MetaValue(UINT idx, UINT vidx, BSTR* p)
{
	if (!m_info_ptr || !p) return E_POINTER;

	if (idx < m_info_ptr->meta_get_count() && vidx < m_info_ptr->meta_enum_value_count(idx))
	{
		*p = SysAllocString(string_wide_from_utf8_fast(m_info_ptr->meta_enum_value(idx, vidx)));
		return S_OK;
	}
	return E_INVALIDARG;
}

STDMETHODIMP FileInfo::MetaValueCount(UINT idx, UINT* p)
{
	if (!m_info_ptr || !p) return E_POINTER;

	if (idx < m_info_ptr->meta_get_count())
	{
		*p = m_info_ptr->meta_enum_value_count(idx);
		return S_OK;
	}
	return E_INVALIDARG;
}

STDMETHODIMP FileInfo::get_InfoCount(UINT* p)
{
	if (!m_info_ptr || !p) return E_POINTER;

	*p = m_info_ptr->info_get_count();
	return S_OK;
}

STDMETHODIMP FileInfo::get_MetaCount(UINT* p)
{
	if (!m_info_ptr || !p) return E_POINTER;

	*p = m_info_ptr->meta_get_count();
	return S_OK;
}

GdiBitmap::GdiBitmap(Gdiplus::Bitmap* p) : GdiObj<IGdiBitmap, Gdiplus::Bitmap>(p) {}

STDMETHODIMP GdiBitmap::ApplyAlpha(BYTE alpha, IGdiBitmap** pp)
{
	if (!m_ptr || !pp) return E_POINTER;

	t_size width = m_ptr->GetWidth();
	t_size height = m_ptr->GetHeight();
	Gdiplus::Bitmap* out = new Gdiplus::Bitmap(width, height, PixelFormat32bppPARGB);
	Gdiplus::Graphics g(out);
	Gdiplus::ImageAttributes ia;
	Gdiplus::ColorMatrix cm = { 0.0 };
	Gdiplus::Rect rect(0, 0, width, height);

	cm.m[0][0] = cm.m[1][1] = cm.m[2][2] = cm.m[4][4] = 1.0;
	cm.m[3][3] = static_cast<float>(alpha) / 255;
	ia.SetColorMatrix(&cm);

	g.DrawImage(m_ptr, rect, 0, 0, width, height, Gdiplus::UnitPixel, &ia);

	*pp = new com_object_impl_t<GdiBitmap>(out);
	return S_OK;
}

STDMETHODIMP GdiBitmap::ApplyMask(IGdiBitmap* mask, VARIANT_BOOL* p)
{
	if (!m_ptr || !p) return E_POINTER;

	*p = VARIANT_FALSE;
	Gdiplus::Bitmap* bitmap_mask = NULL;
	mask->get__ptr((void**)&bitmap_mask);

	if (!bitmap_mask || bitmap_mask->GetHeight() != m_ptr->GetHeight() || bitmap_mask->GetWidth() != m_ptr->GetWidth())
	{
		return E_INVALIDARG;
	}

	Gdiplus::Rect rect(0, 0, m_ptr->GetWidth(), m_ptr->GetHeight());
	Gdiplus::BitmapData bmpdata_mask = { 0 }, bmpdata_dst = { 0 };

	if (bitmap_mask->LockBits(&rect, Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, &bmpdata_mask) != Gdiplus::Ok)
	{
		return S_OK;
	}

	if (m_ptr->LockBits(&rect, Gdiplus::ImageLockModeRead | Gdiplus::ImageLockModeWrite, PixelFormat32bppARGB, &bmpdata_dst) != Gdiplus::Ok)
	{
		bitmap_mask->UnlockBits(&bmpdata_mask);
		return S_OK;
	}

	const int width = rect.Width;
	const int height = rect.Height;
	const int size = width * height;
	//const int size_threshold = 512;
	t_size* p_mask = reinterpret_cast<t_size*>(bmpdata_mask.Scan0);
	t_size* p_dst = reinterpret_cast<t_size*>(bmpdata_dst.Scan0);
	const t_size* p_mask_end = p_mask + rect.Width * rect.Height;
	t_size alpha;

	while (p_mask < p_mask_end)
	{
		// Method 1:
		//alpha = (~*p_mask & 0xff) * (*p_dst >> 24) + 0x80;
		//*p_dst = ((((alpha >> 8) + alpha) & 0xff00) << 16) | (*p_dst & 0xffffff);
		// Method 2
		alpha = (((~*p_mask & 0xff) * (*p_dst >> 24)) << 16) & 0xff000000;
		*p_dst = alpha | (*p_dst & 0xffffff);

		++p_mask;
		++p_dst;
	}

	m_ptr->UnlockBits(&bmpdata_dst);
	bitmap_mask->UnlockBits(&bmpdata_mask);

	*p = VARIANT_TRUE;
	return S_OK;
}

STDMETHODIMP GdiBitmap::Clone(float x, float y, float w, float h, IGdiBitmap** pp)
{
	if (!m_ptr || !pp) return E_POINTER;

	Gdiplus::Bitmap* img = m_ptr->Clone(x, y, w, h, PixelFormat32bppPARGB);
	if (helpers::ensure_gdiplus_object(img))
	{
		*pp = new com_object_impl_t<GdiBitmap>(img);
	}
	else
	{
		if (img) delete img;
		*pp = NULL;
	}

	return S_OK;
}

STDMETHODIMP GdiBitmap::CreateRawBitmap(IGdiRawBitmap** pp)
{
	if (!m_ptr || !pp) return E_POINTER;

	*pp = new com_object_impl_t<GdiRawBitmap>(m_ptr);
	return S_OK;
}

STDMETHODIMP GdiBitmap::GetColourScheme(UINT count, VARIANT* p)
{
	if (!m_ptr || !p) return E_POINTER;

	Gdiplus::BitmapData bmpdata;
	Gdiplus::Rect rect(0, 0, m_ptr->GetWidth(), m_ptr->GetHeight());

	if (m_ptr->LockBits(&rect, Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, &bmpdata) != Gdiplus::Ok) return E_POINTER;

	std::map<t_size, int> color_counters;
	const t_size colors_length = bmpdata.Width * bmpdata.Height;
	const t_size* colors = (const t_size*)bmpdata.Scan0;

	for (t_size i = 0; i < colors_length; ++i)
	{
		// format: 0xaarrggbb
		t_size color = colors[i];
		BYTE r = (color >> 16) & 0xff;
		BYTE g = (color >> 8) & 0xff;
		BYTE b = (color) & 0xff;

		// Round colors
		r = (r + 16) & 0xffffffe0;
		g = (g + 16) & 0xffffffe0;
		b = (b + 16) & 0xffffffe0;

		if (r > 0xff) r = 0xff;
		if (g > 0xff) g = 0xff;
		if (b > 0xff) b = 0xff;

		++color_counters[Gdiplus::Color::MakeARGB(0xff, r, g, b)];
	}

	m_ptr->UnlockBits(&bmpdata);

	// Sorting
	using sort_vec_pair_t = std::pair<t_size, int>;
	std::vector<sort_vec_pair_t> sort_vec(color_counters.begin(), color_counters.end());
	count = min(count, sort_vec.size());
	std::partial_sort(
		sort_vec.begin(),
		sort_vec.begin() + count,
		sort_vec.end(),
		[](const sort_vec_pair_t & a, const sort_vec_pair_t & b)
		{
			return a.second > b.second;
		});

	helpers::com_array helper;
	if (!helper.create(count)) return E_OUTOFMEMORY;

	for (LONG i = 0; i < helper.get_count(); ++i)
	{
		_variant_t var;
		var.vt = VT_UI4;
		var.ulVal = sort_vec[i].first;

		if (!helper.put_item(i, var)) return E_OUTOFMEMORY;
	}
	p->vt = VT_ARRAY | VT_VARIANT;
	p->parray = helper.get_ptr();
	return S_OK;
}

STDMETHODIMP GdiBitmap::GetColourSchemeJSON(UINT count, BSTR* p)
{
	if (!m_ptr || !p) return E_POINTER;

	Gdiplus::BitmapData bmpdata;

	// rescaled image will have max of ~48k pixels
	int w = min(m_ptr->GetWidth(), 220), h = min(m_ptr->GetHeight(), 220);

	Gdiplus::Bitmap* bitmap = new Gdiplus::Bitmap(w, h, PixelFormat32bppPARGB);
	Gdiplus::Graphics g(bitmap);
	Gdiplus::Rect rect(0, 0, w, h);
	g.SetInterpolationMode((Gdiplus::InterpolationMode)6); // InterpolationModeHighQualityBilinear
	g.DrawImage(m_ptr, 0, 0, w, h); // scale image down

	if (bitmap->LockBits(&rect, Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, &bmpdata) != Gdiplus::Ok)
		return E_POINTER;

	std::map<t_size, int> colour_counters;
	const t_size colours_length = bmpdata.Width * bmpdata.Height;
	const t_size* colours = (const t_size*)bmpdata.Scan0;

	// reduce color set to pass to k-means by rounding colour components to multiples of 8
	for (t_size i = 0; i < colours_length; ++i)
	{
		BYTE r = (colours[i] >> 16) & 0xff;
		BYTE g = (colours[i] >> 8) & 0xff;
		BYTE b = (colours[i] & 0xff);

		// round colours
		r = (r + 4) & 0xfffffff8;
		g = (g + 4) & 0xfffffff8;
		b = (b + 4) & 0xfffffff8;

		if (r > 255) r = 0xff;
		if (g > 255) g = 0xff;
		if (b > 255) b = 0xff;

		++colour_counters[r << 16 | g << 8 | b];
	}
	bitmap->UnlockBits(&bmpdata);

	std::map<t_size, int>::iterator it;
	std::vector<Point> points;
	int idx = 0;

	for (it = colour_counters.begin(); it != colour_counters.end(); it++, idx++)
	{
		BYTE r = (it->first >> 16) & 0xff;
		BYTE g = (it->first >> 8) & 0xff;
		BYTE b = (it->first & 0xff);

		std::vector<t_size> values = { r, g, b };
		Point p(idx, values, it->second);
		points.push_back(p);
	}

	KMeans kmeans(count, colour_counters.size(), 12); // 12 iterations max
	std::vector<Cluster> clusters = kmeans.run(points);

	// sort by largest clusters
	std::sort(
		clusters.begin(),
		clusters.end(),
		[](Cluster & a, Cluster & b) {
			return a.getTotalPoints() > b.getTotalPoints();
		});

	json j = json::array();
	t_size outCount = min(count, colour_counters.size());
	for (t_size i = 0; i < outCount; ++i)
	{
		int colour = 0xff000000 | (int)clusters[i].getCentralValue(0) << 16 | (int)clusters[i].getCentralValue(1) << 8 | (int)clusters[i].getCentralValue(2);
		double frequency = clusters[i].getTotalPoints() / (double)colours_length;

		j.push_back({
			{ "col", colour },
			{ "freq", frequency }
			});
	}
	*p = SysAllocString(string_wide_from_utf8_fast((j.dump()).c_str()));
	return S_OK;
}

STDMETHODIMP GdiBitmap::GetGraphics(IGdiGraphics** pp)
{
	if (!m_ptr || !pp) return E_POINTER;

	Gdiplus::Graphics* g = new Gdiplus::Graphics(m_ptr);
	*pp = new com_object_impl_t<GdiGraphics>();
	(*pp)->put__ptr(g);
	return S_OK;
}

STDMETHODIMP GdiBitmap::ReleaseGraphics(IGdiGraphics* p)
{
	if (p)
	{
		Gdiplus::Graphics* g = NULL;
		p->get__ptr((void**)&g);
		p->put__ptr(NULL);
		if (g) delete g;
	}

	return S_OK;
}

STDMETHODIMP GdiBitmap::Resize(UINT w, UINT h, int interpolationMode, IGdiBitmap** pp)
{
	if (!m_ptr || !pp) return E_POINTER;

	Gdiplus::Bitmap* bitmap = new Gdiplus::Bitmap(w, h, PixelFormat32bppPARGB);
	Gdiplus::Graphics g(bitmap);
	g.SetInterpolationMode((Gdiplus::InterpolationMode)interpolationMode);
	g.DrawImage(m_ptr, 0, 0, w, h);
	*pp = new com_object_impl_t<GdiBitmap>(bitmap);
	return S_OK;
}

STDMETHODIMP GdiBitmap::RotateFlip(UINT mode)
{
	if (!m_ptr) return E_POINTER;

	m_ptr->RotateFlip((Gdiplus::RotateFlipType)mode);
	return S_OK;
}

STDMETHODIMP GdiBitmap::SaveAs(BSTR path, BSTR format, VARIANT_BOOL* p)
{
	if (!m_ptr || !p) return E_POINTER;

	CLSID clsid_encoder;
	int ret = helpers::get_encoder_clsid(format, &clsid_encoder);

	if (ret > -1)
	{
		m_ptr->Save(path, &clsid_encoder);
		*p = TO_VARIANT_BOOL(m_ptr->GetLastStatus() == Gdiplus::Ok);
	}
	else
	{
		*p = VARIANT_FALSE;
	}

	return S_OK;
}

STDMETHODIMP GdiBitmap::StackBlur(BYTE radius)
{
	if (!m_ptr) return E_POINTER;

	stack_blur_filter(*m_ptr, radius);
	return S_OK;
}

STDMETHODIMP GdiBitmap::get_Height(UINT* p)
{
	if (!m_ptr || !p) return E_POINTER;

	*p = m_ptr->GetHeight();
	return S_OK;
}

STDMETHODIMP GdiBitmap::get_Width(UINT* p)
{
	if (!m_ptr || !p) return E_POINTER;

	*p = m_ptr->GetWidth();
	return S_OK;
}

GdiFont::GdiFont(Gdiplus::Font* p, HFONT hFont, bool managed) : GdiObj<IGdiFont, Gdiplus::Font>(p), m_hFont(hFont), m_managed(managed) {}
GdiFont:: ~GdiFont() {}

void GdiFont::FinalRelease()
{
	if (m_hFont&& m_managed)
	{
		DeleteFont(m_hFont);
		m_hFont = NULL;
	}

	// call parent
	GdiObj<IGdiFont, Gdiplus::Font>::FinalRelease();
}

STDMETHODIMP GdiFont::get__HFont(UINT* p)
{
	if (!m_ptr || !p) return E_POINTER;

	*p = (UINT)m_hFont;
	return S_OK;
}

STDMETHODIMP GdiFont::get_Height(UINT* p)
{
	if (!m_ptr || !p) return E_POINTER;

	Gdiplus::Bitmap img(1, 1, PixelFormat32bppPARGB);
	Gdiplus::Graphics g(&img);
	*p = (UINT)m_ptr->GetHeight(&g);
	return S_OK;
}

STDMETHODIMP GdiFont::get_Name(BSTR* p)
{
	if (!m_ptr || !p) return E_POINTER;

	wchar_t name[LF_FACESIZE] = { 0 };
	Gdiplus::FontFamily fontFamily;
	m_ptr->GetFamily(&fontFamily);
	fontFamily.GetFamilyName(name, LANG_NEUTRAL);
	*p = SysAllocString(name);
	return S_OK;
}

STDMETHODIMP GdiFont::get_Size(float* p)
{
	if (!m_ptr || !p) return E_POINTER;

	*p = m_ptr->GetSize();
	return S_OK;
}

STDMETHODIMP GdiFont::get_Style(int* p)
{
	if (!m_ptr || !p) return E_POINTER;

	*p = m_ptr->GetStyle();
	return S_OK;
}

GdiGraphics::GdiGraphics() : GdiObj<IGdiGraphics, Gdiplus::Graphics>(NULL) {}

void GdiGraphics::GetRoundRectPath(Gdiplus::GraphicsPath & gp, Gdiplus::RectF & rect, float arc_width, float arc_height)
{
	float arc_dia_w = arc_width * 2;
	float arc_dia_h = arc_height * 2;
	Gdiplus::RectF corner(rect.X, rect.Y, arc_dia_w, arc_dia_h);

	gp.Reset();

	// top left
	gp.AddArc(corner, 180, 90);

	// top right
	corner.X += (rect.Width - arc_dia_w);
	gp.AddArc(corner, 270, 90);

	// bottom right
	corner.Y += (rect.Height - arc_dia_h);
	gp.AddArc(corner, 0, 90);

	// bottom left
	corner.X -= (rect.Width - arc_dia_w);
	gp.AddArc(corner, 90, 90);

	gp.CloseFigure();
}

STDMETHODIMP GdiGraphics::CalcTextHeight(BSTR str, IGdiFont* font, UINT* p)
{
	if (!m_ptr || !p) return E_POINTER;

	HFONT hFont = NULL;
	font->get__HFont((UINT*)&hFont);
	HFONT oldfont;
	HDC dc = m_ptr->GetHDC();
	oldfont = SelectFont(dc, hFont);
	*p = helpers::get_text_height(dc, str, SysStringLen(str));
	SelectFont(dc, oldfont);
	m_ptr->ReleaseHDC(dc);
	return S_OK;
}

STDMETHODIMP GdiGraphics::CalcTextWidth(BSTR str, IGdiFont* font, UINT* p)
{
	if (!m_ptr || !p) return E_POINTER;

	HFONT hFont = NULL;
	font->get__HFont((UINT*)&hFont);
	HFONT oldfont;
	HDC dc = m_ptr->GetHDC();
	oldfont = SelectFont(dc, hFont);
	*p = helpers::get_text_width(dc, str, SysStringLen(str));
	SelectFont(dc, oldfont);
	m_ptr->ReleaseHDC(dc);
	return S_OK;
}

STDMETHODIMP GdiGraphics::DrawEllipse(float x, float y, float w, float h, float line_width, VARIANT colour)
{
	if (!m_ptr) return E_POINTER;

	Gdiplus::Pen pen(helpers::get_colour_from_variant(colour), line_width);
	m_ptr->DrawEllipse(&pen, x, y, w, h);
	return S_OK;
}

STDMETHODIMP GdiGraphics::DrawImage(IGdiBitmap* image, float dstX, float dstY, float dstW, float dstH, float srcX, float srcY, float srcW, float srcH, float angle, BYTE alpha)
{
	if (!m_ptr) return E_POINTER;

	Gdiplus::Bitmap* img = NULL;
	image->get__ptr((void**)&img);
	Gdiplus::Matrix old_m;

	if (angle != 0.0)
	{
		Gdiplus::Matrix m;
		Gdiplus::RectF rect;
		Gdiplus::PointF pt;

		pt.X = dstX + dstW / 2;
		pt.Y = dstY + dstH / 2;
		m.RotateAt(angle, pt);
		m_ptr->GetTransform(&old_m);
		m_ptr->SetTransform(&m);
	}

	if (alpha != (BYTE)~0)
	{
		Gdiplus::ImageAttributes ia;
		Gdiplus::ColorMatrix cm = { 0.0f };

		cm.m[0][0] = cm.m[1][1] = cm.m[2][2] = cm.m[4][4] = 1.0f;
		cm.m[3][3] = static_cast<float>(alpha) / 255;

		ia.SetColorMatrix(&cm);

		m_ptr->DrawImage(img, Gdiplus::RectF(dstX, dstY, dstW, dstH), srcX, srcY, srcW, srcH, Gdiplus::UnitPixel, &ia);
	}
	else
	{
		m_ptr->DrawImage(img, Gdiplus::RectF(dstX, dstY, dstW, dstH), srcX, srcY, srcW, srcH, Gdiplus::UnitPixel);
	}

	if (angle != 0.0)
	{
		m_ptr->SetTransform(&old_m);
	}

	return S_OK;
}

STDMETHODIMP GdiGraphics::DrawLine(float x1, float y1, float x2, float y2, float line_width, VARIANT colour)
{
	if (!m_ptr) return E_POINTER;

	Gdiplus::Pen pen(helpers::get_colour_from_variant(colour), line_width);
	m_ptr->DrawLine(&pen, x1, y1, x2, y2);
	return S_OK;
}

STDMETHODIMP GdiGraphics::DrawPolygon(VARIANT colour, float line_width, VARIANT points)
{
	if (!m_ptr) return E_POINTER;

	helpers::com_array helper;
	if (!helper.convert(&points)) return E_INVALIDARG;
	LONG count = helper.get_count();
	if (count % 2 != 0) return E_INVALIDARG;

	pfc::array_t<Gdiplus::PointF> point_array;
	point_array.set_size(count / 2);

	for (LONG i = 0; i < count / 2; ++i)
	{
		_variant_t varX, varY;

		if (!helper.get_item(i * 2, varX, VT_R4)) return E_INVALIDARG;
		if (!helper.get_item((i * 2) + 1, varY, VT_R4)) return E_INVALIDARG;

		point_array[i] = { varX.fltVal, varY.fltVal };
	}

	Gdiplus::Pen pen(helpers::get_colour_from_variant(colour), line_width);
	m_ptr->DrawPolygon(&pen, point_array.get_ptr(), point_array.get_count());
	return S_OK;
}

STDMETHODIMP GdiGraphics::DrawRect(float x, float y, float w, float h, float line_width, VARIANT colour)
{
	if (!m_ptr) return E_POINTER;

	Gdiplus::Pen pen(helpers::get_colour_from_variant(colour), line_width);
	m_ptr->DrawRectangle(&pen, x, y, w, h);
	return S_OK;
}

STDMETHODIMP GdiGraphics::DrawRoundRect(float x, float y, float w, float h, float arc_width, float arc_height, float line_width, VARIANT colour)
{
	if (!m_ptr) return E_POINTER;

	if (2 * arc_width > w || 2 * arc_height > h) return E_INVALIDARG;

	Gdiplus::Pen pen(helpers::get_colour_from_variant(colour), line_width);
	Gdiplus::GraphicsPath gp;
	Gdiplus::RectF rect(x, y, w, h);
	GetRoundRectPath(gp, rect, arc_width, arc_height);
	pen.SetStartCap(Gdiplus::LineCapRound);
	pen.SetEndCap(Gdiplus::LineCapRound);
	m_ptr->DrawPath(&pen, &gp);
	return S_OK;
}

STDMETHODIMP GdiGraphics::DrawString(BSTR str, IGdiFont* font, VARIANT colour, float x, float y, float w, float h, int flags)
{
	if (!m_ptr) return E_POINTER;

	Gdiplus::Font* fn = NULL;
	font->get__ptr((void**)&fn);
	Gdiplus::SolidBrush br(helpers::get_colour_from_variant(colour));
	Gdiplus::StringFormat fmt(Gdiplus::StringFormat::GenericTypographic());

	if (flags != 0)
	{
		fmt.SetAlignment((Gdiplus::StringAlignment)((flags >> 28) & 0x3)); //0xf0000000
		fmt.SetLineAlignment((Gdiplus::StringAlignment)((flags >> 24) & 0x3)); //0x0f000000
		fmt.SetTrimming((Gdiplus::StringTrimming)((flags >> 20) & 0x7)); //0x00f00000
		fmt.SetFormatFlags((Gdiplus::StringFormatFlags)(flags & 0x7FFF)); //0x0000ffff
	}

	m_ptr->DrawString(str, -1, fn, Gdiplus::RectF(x, y, w, h), &fmt, &br);
	return S_OK;
}

STDMETHODIMP GdiGraphics::EstimateLineWrap(BSTR str, IGdiFont* font, int max_width, VARIANT* p)
{
	if (!m_ptr || !p) return E_POINTER;

	HFONT hFont = NULL;
	font->get__HFont((UINT*)&hFont);
	HDC dc = m_ptr->GetHDC();
	HFONT oldfont = SelectFont(dc, hFont);

	pfc::list_t<helpers::wrapped_item> result;
	estimate_line_wrap(dc, str, SysStringLen(str), max_width, result);
	SelectFont(dc, oldfont);
	m_ptr->ReleaseHDC(dc);

	LONG count = result.get_count() * 2;
	helpers::com_array helper;
	if (!helper.create(count)) return E_OUTOFMEMORY;
	for (LONG i = 0; i < count / 2; ++i)
	{
		_variant_t var1, var2;
		var1.vt = VT_BSTR;
		var1.bstrVal = result[i].text;
		var2.vt = VT_I4;
		var2.lVal = result[i].width;
		if (!helper.put_item(i * 2, var1)) return E_OUTOFMEMORY;
		if (!helper.put_item((i * 2) + 1, var2)) return E_OUTOFMEMORY;
	}
	p->vt = VT_ARRAY | VT_VARIANT;
	p->parray = helper.get_ptr();
	return S_OK;
}

STDMETHODIMP GdiGraphics::FillEllipse(float x, float y, float w, float h, VARIANT colour)
{
	if (!m_ptr) return E_POINTER;

	Gdiplus::SolidBrush br(helpers::get_colour_from_variant(colour));
	m_ptr->FillEllipse(&br, x, y, w, h);
	return S_OK;
}

STDMETHODIMP GdiGraphics::FillGradRect(float x, float y, float w, float h, float angle, VARIANT colour1, VARIANT colour2, float focus)
{
	if (!m_ptr) return E_POINTER;

	Gdiplus::RectF rect(x, y, w, h);
	Gdiplus::LinearGradientBrush brush(rect, helpers::get_colour_from_variant(colour1), helpers::get_colour_from_variant(colour2), angle, TRUE);
	brush.SetBlendTriangularShape(focus);
	m_ptr->FillRectangle(&brush, rect);
	return S_OK;
}

STDMETHODIMP GdiGraphics::FillPolygon(VARIANT colour, int fillmode, VARIANT points)
{
	if (!m_ptr) return E_POINTER;

	helpers::com_array helper;
	if (!helper.convert(&points)) return E_INVALIDARG;
	LONG count = helper.get_count();
	if (count % 2 != 0) return E_INVALIDARG;

	pfc::array_t<Gdiplus::PointF> point_array;
	point_array.set_size(count / 2);

	for (LONG i = 0; i < count / 2; ++i)
	{
		_variant_t varX, varY;

		if (!helper.get_item(i * 2, varX, VT_R4)) return E_INVALIDARG;
		if (!helper.get_item((i * 2) + 1, varY, VT_R4)) return E_INVALIDARG;

		point_array[i] = { varX.fltVal, varY.fltVal };
	}

	Gdiplus::SolidBrush br(helpers::get_colour_from_variant(colour));
	m_ptr->FillPolygon(&br, point_array.get_ptr(), point_array.get_count(), (Gdiplus::FillMode)fillmode);
	return S_OK;
}

STDMETHODIMP GdiGraphics::FillRoundRect(float x, float y, float w, float h, float arc_width, float arc_height, VARIANT colour)
{
	if (!m_ptr) return E_POINTER;

	if (2 * arc_width > w || 2 * arc_height > h) return E_INVALIDARG;

	Gdiplus::SolidBrush br(helpers::get_colour_from_variant(colour));
	Gdiplus::GraphicsPath gp;
	Gdiplus::RectF rect(x, y, w, h);
	GetRoundRectPath(gp, rect, arc_width, arc_height);
	m_ptr->FillPath(&br, &gp);
	return S_OK;
}

STDMETHODIMP GdiGraphics::FillSolidRect(float x, float y, float w, float h, VARIANT colour)
{
	if (!m_ptr) return E_POINTER;

	Gdiplus::SolidBrush brush(helpers::get_colour_from_variant(colour));
	m_ptr->FillRectangle(&brush, x, y, w, h);
	return S_OK;
}

STDMETHODIMP GdiGraphics::GdiAlphaBlend(IGdiRawBitmap* bitmap, int dstX, int dstY, int dstW, int dstH, int srcX, int srcY, int srcW, int srcH, BYTE alpha)
{
	if (!m_ptr) return E_POINTER;

	HDC src_dc = NULL;
	bitmap->get__Handle(&src_dc);
	HDC dc = m_ptr->GetHDC();
	BLENDFUNCTION bf = { AC_SRC_OVER, 0, alpha, AC_SRC_ALPHA };

	::GdiAlphaBlend(dc, dstX, dstY, dstW, dstH, src_dc, srcX, srcY, srcW, srcH, bf);
	m_ptr->ReleaseHDC(dc);
	return S_OK;
}

STDMETHODIMP GdiGraphics::GdiDrawBitmap(IGdiRawBitmap* bitmap, int dstX, int dstY, int dstW, int dstH, int srcX, int srcY, int srcW, int srcH)
{
	if (!m_ptr) return E_POINTER;

	HDC src_dc = NULL;
	bitmap->get__Handle(&src_dc);
	HDC dc = m_ptr->GetHDC();

	if (dstW == srcW && dstH == srcH)
	{
		BitBlt(dc, dstX, dstY, dstW, dstH, src_dc, srcX, srcY, SRCCOPY);
	}
	else
	{
		SetStretchBltMode(dc, HALFTONE);
		SetBrushOrgEx(dc, 0, 0, NULL);
		StretchBlt(dc, dstX, dstY, dstW, dstH, src_dc, srcX, srcY, srcW, srcH, SRCCOPY);
	}

	m_ptr->ReleaseHDC(dc);
	return S_OK;
}

STDMETHODIMP GdiGraphics::GdiDrawText(BSTR str, IGdiFont* font, VARIANT colour, int x, int y, int w, int h, int format)
{
	if (!m_ptr) return E_POINTER;

	HFONT hFont = NULL;
	font->get__HFont((UINT*)&hFont);
	HFONT oldfont;
	HDC dc = m_ptr->GetHDC();
	RECT rc = { x, y, x + w, y + h };
	DRAWTEXTPARAMS dpt = { sizeof(DRAWTEXTPARAMS), 4, 0, 0, 0 };

	oldfont = SelectFont(dc, hFont);
	SetTextColor(dc, helpers::convert_argb_to_colorref(helpers::get_colour_from_variant(colour)));
	SetBkMode(dc, TRANSPARENT);
	SetTextAlign(dc, TA_LEFT | TA_TOP | TA_NOUPDATECP);

	// Remove DT_MODIFYSTRING flag
	if (format& DT_MODIFYSTRING)
		format &= ~DT_MODIFYSTRING;

	// Well, magic :P
	if (format & DT_CALCRECT)
	{
		RECT rc_calc = { 0 }, rc_old = { 0 };

		memcpy(&rc_calc, &rc, sizeof(RECT));
		memcpy(&rc_old, &rc, sizeof(RECT));

		DrawText(dc, str, -1, &rc_calc, format);

		format &= ~DT_CALCRECT;

		// adjust vertical align
		if (format& DT_VCENTER)
		{
			rc.top = rc_old.top + (((rc_old.bottom - rc_old.top) - (rc_calc.bottom - rc_calc.top)) >> 1);
			rc.bottom = rc.top + (rc_calc.bottom - rc_calc.top);
		}
		else if (format& DT_BOTTOM)
		{
			rc.top = rc_old.bottom - (rc_calc.bottom - rc_calc.top);
		}
	}

	DrawTextEx(dc, str, -1, &rc, format, &dpt);
	SelectFont(dc, oldfont);
	m_ptr->ReleaseHDC(dc);
	return S_OK;
}

STDMETHODIMP GdiGraphics::MeasureString(BSTR str, IGdiFont* font, float x, float y, float w, float h, int flags, IMeasureStringInfo** pp)
{
	if (!m_ptr || !pp) return E_POINTER;

	Gdiplus::Font* fn = NULL;
	font->get__ptr((void**)&fn);

	Gdiplus::StringFormat fmt = Gdiplus::StringFormat::GenericTypographic();

	if (flags != 0)
	{
		fmt.SetAlignment((Gdiplus::StringAlignment)((flags >> 28) & 0x3)); //0xf0000000
		fmt.SetLineAlignment((Gdiplus::StringAlignment)((flags >> 24) & 0x3)); //0x0f000000
		fmt.SetTrimming((Gdiplus::StringTrimming)((flags >> 20) & 0x7)); //0x00f00000
		fmt.SetFormatFlags((Gdiplus::StringFormatFlags)(flags & 0x7FFF)); //0x0000ffff
	}

	Gdiplus::RectF bound;
	int chars, lines;

	m_ptr->MeasureString(str, -1, fn, Gdiplus::RectF(x, y, w, h), &fmt, &bound, &chars, &lines);

	*pp = new com_object_impl_t<MeasureStringInfo>(bound.X, bound.Y, bound.Width, bound.Height, lines, chars);
	return S_OK;
}

STDMETHODIMP GdiGraphics::SetInterpolationMode(int mode)
{
	if (!m_ptr) return E_POINTER;

	m_ptr->SetInterpolationMode((Gdiplus::InterpolationMode)mode);
	return S_OK;
}

STDMETHODIMP GdiGraphics::SetSmoothingMode(int mode)
{
	if (!m_ptr) return E_POINTER;

	m_ptr->SetSmoothingMode((Gdiplus::SmoothingMode)mode);
	return S_OK;
}

STDMETHODIMP GdiGraphics::SetTextRenderingHint(UINT mode)
{
	if (!m_ptr) return E_POINTER;

	m_ptr->SetTextRenderingHint((Gdiplus::TextRenderingHint)mode);
	return S_OK;
}

STDMETHODIMP GdiGraphics::put__ptr(void* p)
{
	m_ptr = (Gdiplus::Graphics*)p;
	return S_OK;
}

GdiRawBitmap::GdiRawBitmap(Gdiplus::Bitmap* p_bmp)
{
	PFC_ASSERT(p_bmp != NULL);
	m_width = p_bmp->GetWidth();
	m_height = p_bmp->GetHeight();

	m_hdc = CreateCompatibleDC(NULL);
	m_hbmp = CreateHBITMAP(p_bmp);
	m_hbmpold = SelectBitmap(m_hdc, m_hbmp);
}

GdiRawBitmap::~GdiRawBitmap() {}

HBITMAP GdiRawBitmap::CreateHBITMAP(Gdiplus::Bitmap* bitmap_ptr)
{
	Gdiplus::Rect rect(0, 0, bitmap_ptr->GetWidth(), bitmap_ptr->GetHeight());
	Gdiplus::BitmapData bmpdata;

	if (bitmap_ptr->LockBits(&rect, Gdiplus::ImageLockModeRead, PixelFormat32bppPARGB, &bmpdata) != Gdiplus::Ok)
	{
		// Error
		return NULL;
	}

	BITMAP bm;
	bm.bmType = 0;
	bm.bmWidth = bmpdata.Width;
	bm.bmHeight = bmpdata.Height;
	bm.bmWidthBytes = bmpdata.Stride;
	bm.bmPlanes = 1;
	bm.bmBitsPixel = 32;
	bm.bmBits = bmpdata.Scan0;

	HBITMAP hBitmap = CreateBitmapIndirect(&bm);
	bitmap_ptr->UnlockBits(&bmpdata);
	return hBitmap;
}

void GdiRawBitmap::FinalRelease()
{
	if (m_hdc)
	{
		SelectBitmap(m_hdc, m_hbmpold);
		DeleteDC(m_hdc);
		m_hdc = NULL;
	}

	if (m_hbmp)
	{
		DeleteBitmap(m_hbmp);
		m_hbmp = NULL;
	}
}

STDMETHODIMP GdiRawBitmap::get_Height(UINT* p)
{
	if (!m_hdc || !p) return E_POINTER;

	*p = m_height;
	return S_OK;
}

STDMETHODIMP GdiRawBitmap::get_Width(UINT* p)
{
	if (!m_hdc || !p) return E_POINTER;

	*p = m_width;
	return S_OK;
}

STDMETHODIMP GdiRawBitmap::get__Handle(HDC* p)
{
	if (!m_hdc || !p) return E_POINTER;

	*p = m_hdc;
	return S_OK;
}

MainMenuManager::MainMenuManager() {}
MainMenuManager::~MainMenuManager() {}

void MainMenuManager::FinalRelease()
{
	m_mm.release();
}

STDMETHODIMP MainMenuManager::BuildMenu(IMenuObj* p, int base_id, int count)
{
	if (m_mm.is_empty()) return E_POINTER;

	HMENU menuid;
	p->get__ID(&menuid);

	// HACK: workaround for foo_menu_addons
	try
	{
		m_mm->generate_menu_win32(menuid, base_id, count, mainmenu_manager::flag_show_shortcuts);
	}
	catch (...)
	{
	}

	return S_OK;
}

STDMETHODIMP MainMenuManager::ExecuteByID(UINT id, VARIANT_BOOL* p)
{
	if (m_mm.is_empty() || !p) return E_POINTER;

	*p = TO_VARIANT_BOOL(m_mm->execute_command(id));
	return S_OK;
}

STDMETHODIMP MainMenuManager::Init(BSTR root_name)
{
	struct t_valid_root_name
	{
		const wchar_t* name;
		const GUID* guid;
	};

	// In mainmenu_groups:
	// static const GUID file,view,edit,playback,library,help;
	const t_valid_root_name valid_root_names[] =
	{
		{L"file", &mainmenu_groups::file},
		{L"view", &mainmenu_groups::view},
		{L"edit", &mainmenu_groups::edit},
		{L"playback", &mainmenu_groups::playback},
		{L"library", &mainmenu_groups::library},
		{L"help", &mainmenu_groups::help},
	};

	// Find
	for (int i = 0; i < _countof(valid_root_names); ++i)
	{
		if (_wcsicmp(root_name, valid_root_names[i].name) == 0)
		{
			// found
			m_mm = standard_api_create_t<mainmenu_manager>();
			m_mm->instantiate(*valid_root_names[i].guid);
			return S_OK;
		}
	}
	return E_INVALIDARG;
}

MeasureStringInfo::MeasureStringInfo(float x, float y, float w, float h, int l, int c) : m_x(x), m_y(y), m_w(w), m_h(h), m_l(l), m_c(c) {}
MeasureStringInfo::~MeasureStringInfo() {}

STDMETHODIMP MeasureStringInfo::get_chars(int* p)
{
	if (!p) return E_POINTER;

	*p = m_c;
	return S_OK;
}

STDMETHODIMP MeasureStringInfo::get_height(float* p)
{
	if (!p) return E_POINTER;

	*p = m_h;
	return S_OK;
}

STDMETHODIMP MeasureStringInfo::get_lines(int* p)
{
	if (!p) return E_POINTER;

	*p = m_l;
	return S_OK;
}

STDMETHODIMP MeasureStringInfo::get_width(float* p)
{
	if (!p) return E_POINTER;

	*p = m_w;
	return S_OK;
}

STDMETHODIMP MeasureStringInfo::get_x(float* p)
{
	if (!p) return E_POINTER;

	*p = m_x;
	return S_OK;
}

STDMETHODIMP MeasureStringInfo::get_y(float* p)
{
	if (!p) return E_POINTER;

	*p = m_y;
	return S_OK;
}

MenuObj::MenuObj(HWND wnd_parent) : m_wnd_parent(wnd_parent), m_has_detached(false)
{
	m_hMenu = ::CreatePopupMenu();
}

MenuObj::~MenuObj() {}

void MenuObj::FinalRelease()
{
	if (!m_has_detached && m_hMenu && IsMenu(m_hMenu))
	{
		DestroyMenu(m_hMenu);
		m_hMenu = NULL;
	}
}

STDMETHODIMP MenuObj::AppendMenuItem(UINT flags, UINT item_id, BSTR text)
{
	if (!m_hMenu) return E_POINTER;
	if (flags& MF_POPUP) return E_INVALIDARG;

	::AppendMenu(m_hMenu, flags, item_id, text);
	return S_OK;
}

STDMETHODIMP MenuObj::AppendMenuSeparator()
{
	if (!m_hMenu) return E_POINTER;

	::AppendMenu(m_hMenu, MF_SEPARATOR, 0, 0);
	return S_OK;
}

STDMETHODIMP MenuObj::AppendTo(IMenuObj* parent, UINT flags, BSTR text)
{
	if (!m_hMenu) return E_POINTER;

	MenuObj* pMenuParent = static_cast<MenuObj*>(parent);
	if (::AppendMenu(pMenuParent->m_hMenu, flags | MF_STRING | MF_POPUP, UINT_PTR(m_hMenu), text))
	{
		m_has_detached = true;
	}
	return S_OK;
}

STDMETHODIMP MenuObj::CheckMenuItem(UINT item_id, VARIANT_BOOL check)
{
	if (!m_hMenu) return E_POINTER;

	::CheckMenuItem(m_hMenu, item_id, check != VARIANT_FALSE ? MF_CHECKED : MF_UNCHECKED);
	return S_OK;
}

STDMETHODIMP MenuObj::CheckMenuRadioItem(UINT first, UINT last, UINT selected)
{
	if (!m_hMenu) return E_POINTER;

	::CheckMenuRadioItem(m_hMenu, first, last, selected, MF_BYCOMMAND);
	return S_OK;
}

STDMETHODIMP MenuObj::TrackPopupMenu(int x, int y, UINT flags, UINT* item_id)
{
	if (!m_hMenu || !item_id) return E_POINTER;

	// Only include specified flags
	flags |= TPM_NONOTIFY | TPM_RETURNCMD | TPM_RIGHTBUTTON;
	flags &= ~TPM_RECURSE;

	POINT pt = { x, y };
	ClientToScreen(m_wnd_parent, &pt);
	*item_id = ::TrackPopupMenu(m_hMenu, flags, pt.x, pt.y, 0, m_wnd_parent, 0);
	return S_OK;
}

STDMETHODIMP MenuObj::get__ID(HMENU* p)
{
	if (!m_hMenu || !p) return E_POINTER;

	*p = m_hMenu;
	return S_OK;
}

MetadbHandle::MetadbHandle(const metadb_handle_ptr & src) : m_handle(src) {}
MetadbHandle::MetadbHandle(metadb_handle* src) : m_handle(src) {}
MetadbHandle::~MetadbHandle() {}

void MetadbHandle::FinalRelease()
{
	m_handle.release();
}

STDMETHODIMP MetadbHandle::get__ptr(void** pp)
{
	if (!pp) return E_POINTER;

	*pp = m_handle.get_ptr();
	return S_OK;
}

STDMETHODIMP MetadbHandle::ClearStats()
{
	if (m_handle.is_empty()) return E_POINTER;

	metadb_index_hash hash;
	if (stats::hashHandle(m_handle, hash))
	{
		stats::set(hash, stats::fields());
	}
	return S_OK;
}

STDMETHODIMP MetadbHandle::Compare(IMetadbHandle* handle, VARIANT_BOOL* p)
{
	if (m_handle.is_empty() || !p) return E_POINTER;

	metadb_handle* ptr = NULL;
	handle->get__ptr((void**)&ptr);
	if (!ptr) return E_INVALIDARG;

	*p = TO_VARIANT_BOOL(ptr == m_handle.get_ptr());
	return S_OK;
}

STDMETHODIMP MetadbHandle::GetAlbumArt(UINT art_id, VARIANT_BOOL need_stub, VARIANT* p)
{
	if (m_handle.is_empty() || !p) return E_POINTER;

	pfc::string8_fast image_path;
	IGdiBitmap* bitmap = helpers::get_album_art(m_handle, art_id, need_stub != VARIANT_FALSE, image_path);

	_variant_t var1, var2;
	var1.vt = VT_DISPATCH;
	var1.pdispVal = bitmap;
	var2.vt = VT_BSTR;
	var2.bstrVal = SysAllocString(string_wide_from_utf8_fast(image_path));

	helpers::com_array helper;
	if (!helper.create(2)) return E_OUTOFMEMORY;
	if (!helper.put_item(0, var1)) return E_OUTOFMEMORY;
	if (!helper.put_item(1, var2)) return E_OUTOFMEMORY;

	p->vt = VT_ARRAY | VT_VARIANT;
	p->parray = helper.get_ptr();
	return S_OK;
}

STDMETHODIMP MetadbHandle::GetFileInfo(IFileInfo** pp)
{
	if (m_handle.is_empty() || !pp) return E_POINTER;

	file_info_impl* info_ptr = new file_info_impl;
	m_handle->get_info(*info_ptr);
	*pp = new com_object_impl_t<FileInfo>(info_ptr);
	return S_OK;
}

STDMETHODIMP MetadbHandle::RefreshStats()
{
	if (m_handle.is_empty()) return E_POINTER;

	metadb_index_hash hash;
	if (stats::hashHandle(m_handle, hash))
	{
		stats::theAPI()->dispatch_refresh(g_guid_jsp_metadb_index, hash);
	}
	return S_OK;
}

STDMETHODIMP MetadbHandle::SetFirstPlayed(BSTR first_played)
{
	if (m_handle.is_empty()) return E_POINTER;

	metadb_index_hash hash;
	if (stats::hashHandle(m_handle, hash))
	{
		stats::fields tmp = stats::get(hash);
		string_utf8_from_wide fp(first_played);
		if (!tmp.first_played.equals(fp))
		{
			tmp.first_played = fp;
			stats::set(hash, tmp);
		}
	}
	return S_OK;
}

STDMETHODIMP MetadbHandle::SetLastPlayed(BSTR last_played)
{
	if (m_handle.is_empty()) return E_POINTER;

	metadb_index_hash hash;
	if (stats::hashHandle(m_handle, hash))
	{
		stats::fields tmp = stats::get(hash);
		string_utf8_from_wide lp(last_played);
		if (!tmp.last_played.equals(lp))
		{
			tmp.last_played = lp;
			stats::set(hash, tmp);
		}
	}
	return S_OK;
}

STDMETHODIMP MetadbHandle::SetLoved(UINT loved)
{
	if (m_handle.is_empty()) return E_POINTER;

	metadb_index_hash hash;
	if (stats::hashHandle(m_handle, hash))
	{
		stats::fields tmp = stats::get(hash);
		if (tmp.loved != loved)
		{
			tmp.loved = loved;
			stats::set(hash, tmp);
		}
	}
	return S_OK;
}

STDMETHODIMP MetadbHandle::SetPlaycount(UINT playcount)
{
	if (m_handle.is_empty()) return E_POINTER;

	metadb_index_hash hash;
	if (stats::hashHandle(m_handle, hash))
	{
		stats::fields tmp = stats::get(hash);
		if (tmp.playcount != playcount)
		{
			tmp.playcount = playcount;
			stats::set(hash, tmp);
		}
	}
	return S_OK;
}

STDMETHODIMP MetadbHandle::SetRating(UINT rating)
{
	if (m_handle.is_empty()) return E_POINTER;

	metadb_index_hash hash;
	if (stats::hashHandle(m_handle, hash))
	{
		stats::fields tmp = stats::get(hash);
		if (tmp.rating != rating)
		{
			tmp.rating = rating;
			stats::set(hash, tmp);
		}
	}
	return S_OK;
}

STDMETHODIMP MetadbHandle::get_FileSize(LONGLONG* p)
{
	if (m_handle.is_empty() || !p) return E_POINTER;

	*p = m_handle->get_filesize();
	return S_OK;
}

STDMETHODIMP MetadbHandle::get_Length(double* p)
{
	if (m_handle.is_empty() || !p) return E_POINTER;

	*p = m_handle->get_length();
	return S_OK;
}

STDMETHODIMP MetadbHandle::get_Path(BSTR* p)
{
	if (m_handle.is_empty() || !p) return E_POINTER;

	*p = SysAllocString(string_wide_from_utf8_fast(file_path_display(m_handle->get_path())));
	return S_OK;
}

STDMETHODIMP MetadbHandle::get_RawPath(BSTR* p)
{
	if (m_handle.is_empty() || !p) return E_POINTER;

	*p = SysAllocString(string_wide_from_utf8_fast(m_handle->get_path()));
	return S_OK;
}

STDMETHODIMP MetadbHandle::get_SubSong(UINT* p)
{
	if (m_handle.is_empty() || !p) return E_POINTER;

	*p = m_handle->get_subsong_index();
	return S_OK;
}

MetadbHandleList::MetadbHandleList(metadb_handle_list_cref handles) : m_handles(handles) {}
MetadbHandleList::~MetadbHandleList() {}

void MetadbHandleList::FinalRelease()
{
	m_handles.remove_all();
}

STDMETHODIMP MetadbHandleList::get__ptr(void** pp)
{
	if (!pp) return E_POINTER;

	*pp = &m_handles;
	return S_OK;
}

STDMETHODIMP MetadbHandleList::Add(IMetadbHandle* handle)
{
	metadb_handle* ptr = NULL;
	handle->get__ptr((void**)&ptr);
	if (!ptr) return E_INVALIDARG;

	m_handles.add_item(ptr);
	return S_OK;
}

STDMETHODIMP MetadbHandleList::AddRange(IMetadbHandleList* handles)
{
	metadb_handle_list* handles_ptr = NULL;
	handles->get__ptr((void**)&handles_ptr);
	m_handles.add_items(*handles_ptr);
	return S_OK;
}

STDMETHODIMP MetadbHandleList::AttachImage(BSTR image_path, UINT art_id)
{
	if (m_handles.get_count() == 0) return E_POINTER;

	album_art_data_ptr data;

	try
	{
		string_utf8_from_wide path(image_path);
		if (!filesystem::g_is_remote_or_unrecognized(path))
		{
			file::ptr file;
			abort_callback_dummy abort;
			filesystem::g_open(file, path, filesystem::open_mode_read, abort);
			if (file.is_valid())
			{
				auto tmp = fb2k::service_new<album_art_data_impl>();
				tmp->from_stream(file.get_ptr(), t_size(file->get_size_ex(abort)), abort);
				data = tmp;
			}
		}
	}
	catch (...) {}

	if (data.is_valid())
	{
		auto cb = fb2k::service_new<helpers::embed_thread>(helpers::embed_thread::attach, data, m_handles, art_id);
		threaded_process::get()->run_modeless(cb, threaded_process::flag_show_progress | threaded_process::flag_show_delayed | threaded_process::flag_show_item, core_api::get_main_window(), "Embedding image...");
	}
	return S_OK;
}

STDMETHODIMP MetadbHandleList::BSearch(IMetadbHandle* handle, int* p)
{
	if (!p) return E_POINTER;

	metadb_handle* ptr = NULL;
	handle->get__ptr((void**)&ptr);
	if (!ptr) return E_INVALIDARG;

	*p = m_handles.bsearch_by_pointer(ptr);
	return S_OK;
}

STDMETHODIMP MetadbHandleList::CalcTotalDuration(double* p)
{
	if (!p) return E_POINTER;

	*p = m_handles.calc_total_duration();
	return S_OK;
}

STDMETHODIMP MetadbHandleList::CalcTotalSize(LONGLONG* p)
{
	if (!p) return E_POINTER;

	*p = metadb_handle_list_helper::calc_total_size(m_handles, true);
	return S_OK;
}

STDMETHODIMP MetadbHandleList::Clone(IMetadbHandleList** pp)
{
	if (!pp) return E_POINTER;

	*pp = new com_object_impl_t<MetadbHandleList>(m_handles);
	return S_OK;
}

STDMETHODIMP MetadbHandleList::Convert(VARIANT* p)
{
	if (!p) return E_POINTER;

	LONG count = m_handles.get_count();
	helpers::com_array helper;
	if (!helper.create(count)) return E_OUTOFMEMORY;

	for (LONG i = 0; i < count; ++i)
	{
		_variant_t var;
		var.vt = VT_DISPATCH;
		var.pdispVal = new com_object_impl_t<MetadbHandle>(m_handles[i]);
		if (!helper.put_item(i, var)) return E_OUTOFMEMORY;
	}
	p->vt = VT_ARRAY | VT_VARIANT;
	p->parray = helper.get_ptr();
	return S_OK;
}

STDMETHODIMP MetadbHandleList::Find(IMetadbHandle* handle, int* p)
{
	if (!p) return E_POINTER;

	metadb_handle* ptr = NULL;
	handle->get__ptr((void**)&ptr);
	if (!ptr) return E_INVALIDARG;

	*p = m_handles.find_item(ptr);
	return S_OK;
}

STDMETHODIMP MetadbHandleList::GetLibraryRelativePaths(VARIANT* p)
{
	if (!p) return E_POINTER;

	LONG count = m_handles.get_count();
	helpers::com_array helper;
	if (!helper.create(count)) return E_OUTOFMEMORY;

	pfc::string8_fastalloc temp;
	temp.prealloc(512);

	auto api = library_manager::get();

	for (LONG i = 0; i < count; ++i)
	{
		metadb_handle_ptr item = m_handles[i];
		if (!api->get_relative_path(item, temp)) temp = "";
		_variant_t var;
		var.vt = VT_BSTR;
		var.bstrVal = SysAllocString(string_wide_from_utf8_fast(temp));
		if (!helper.put_item(i, var)) return E_OUTOFMEMORY;
	}
	p->vt = VT_ARRAY | VT_VARIANT;
	p->parray = helper.get_ptr();
	return S_OK;
}

STDMETHODIMP MetadbHandleList::Insert(UINT index, IMetadbHandle* handle)
{
	metadb_handle* ptr = NULL;
	handle->get__ptr((void**)&ptr);
	if (!ptr) return E_INVALIDARG;

	m_handles.insert_item(ptr, index);
	return S_OK;
}

STDMETHODIMP MetadbHandleList::InsertRange(UINT index, IMetadbHandleList* handles)
{
	metadb_handle_list* handles_ptr = NULL;
	handles->get__ptr((void**)&handles_ptr);
	m_handles.insert_items(*handles_ptr, index);
	return S_OK;
}

STDMETHODIMP MetadbHandleList::MakeDifference(IMetadbHandleList* handles)
{
	metadb_handle_list* handles_ptr = NULL;
	handles->get__ptr((void**)&handles_ptr);

	metadb_handle_list_ref handles_ref = *handles_ptr;
	metadb_handle_list result;
	t_size walk1 = 0;
	t_size walk2 = 0;
	t_size last1 = m_handles.get_count();
	t_size last2 = handles_ptr->get_count();

	while (walk1 != last1 && walk2 != last2)
	{
		if (m_handles[walk1] < handles_ref[walk2])
		{
			result.add_item(m_handles[walk1]);
			++walk1;
		}
		else if (handles_ref[walk2] < m_handles[walk1])
		{
			++walk2;
		}
		else
		{
			++walk1;
			++walk2;
		}
	}

	m_handles = result;
	return S_OK;
}

STDMETHODIMP MetadbHandleList::MakeIntersection(IMetadbHandleList* handles)
{
	metadb_handle_list* handles_ptr = NULL;
	handles->get__ptr((void**)&handles_ptr);

	metadb_handle_list_ref handles_ref = *handles_ptr;
	metadb_handle_list result;
	t_size walk1 = 0;
	t_size walk2 = 0;
	t_size last1 = m_handles.get_count();
	t_size last2 = handles_ptr->get_count();

	while (walk1 != last1 && walk2 != last2)
	{
		if (m_handles[walk1] < handles_ref[walk2])
			++walk1;
		else if (handles_ref[walk2] < m_handles[walk1])
			++walk2;
		else
		{
			result.add_item(m_handles[walk1]);
			++walk1;
			++walk2;
		}
	}

	m_handles = result;
	return S_OK;
}

STDMETHODIMP MetadbHandleList::MakeUnion(IMetadbHandleList* handles)
{
	metadb_handle_list* handles_ptr = NULL;
	handles->get__ptr((void**)&handles_ptr);

	m_handles.add_items(*handles_ptr);
	m_handles.sort_by_pointer_remove_duplicates();
	return S_OK;
}

STDMETHODIMP MetadbHandleList::OrderByFormat(__interface ITitleFormat* script, int direction)
{
	titleformat_object* obj = NULL;
	script->get__ptr((void**)&obj);
	m_handles.sort_by_format(obj, NULL, direction);
	return S_OK;
}

STDMETHODIMP MetadbHandleList::OrderByPath()
{
	m_handles.sort_by_path();
	return S_OK;
}

STDMETHODIMP MetadbHandleList::OrderByRelativePath()
{
	// lifted from metadb_handle_list.cpp - adds subsong index for better sorting. github issue #16
	auto api = library_manager::get();
	t_size i, count = m_handles.get_count();

	pfc::array_t<helpers::custom_sort_data> data;
	data.set_size(count);

	pfc::string8_fastalloc temp;
	temp.prealloc(512);

	for (i = 0; i < count; ++i)
	{
		metadb_handle_ptr item;
		m_handles.get_item_ex(item, i);
		if (!api->get_relative_path(item, temp)) temp = "";
		temp << item->get_subsong_index();
		data[i].index = i;
		data[i].text = helpers::make_sort_string(temp);
	}

	pfc::sort_t(data, helpers::custom_sort_compare<1>, count);
	order_helper order(count);

	for (i = 0; i < count; ++i)
	{
		order[i] = data[i].index;
		delete[] data[i].text;
	}

	m_handles.reorder(order.get_ptr());
	return S_OK;
}

STDMETHODIMP MetadbHandleList::RefreshStats()
{
	const t_size count = m_handles.get_count();
	pfc::avltree_t<metadb_index_hash> tmp;
	for (t_size i = 0; i < count; ++i)
	{
		metadb_index_hash hash;
		if (stats::hashHandle(m_handles[i], hash))
		{
			tmp += hash;
		}
	}
	pfc::list_t<metadb_index_hash> hashes;
	for (auto iter = tmp.first(); iter.is_valid(); ++iter)
	{
		const metadb_index_hash hash = *iter;
		hashes += hash;
	}
	stats::theAPI()->dispatch_refresh(g_guid_jsp_metadb_index, hashes);
	return S_OK;
}

STDMETHODIMP MetadbHandleList::Remove(IMetadbHandle* handle)
{
	metadb_handle* ptr = NULL;
	handle->get__ptr((void**)&ptr);
	if (!ptr) return E_INVALIDARG;

	m_handles.remove_item(ptr);
	return S_OK;
}

STDMETHODIMP MetadbHandleList::RemoveAll()
{
	m_handles.remove_all();
	return S_OK;
}

STDMETHODIMP MetadbHandleList::RemoveAttachedImage(UINT art_id)
{
	if (m_handles.get_count() == 0) return E_POINTER;

	auto cb = fb2k::service_new<helpers::embed_thread>(helpers::embed_thread::remove, album_art_data_ptr(), m_handles, art_id);
	threaded_process::get()->run_modeless(cb, threaded_process::flag_show_progress | threaded_process::flag_show_delayed | threaded_process::flag_show_item, core_api::get_main_window(), "Removing images...");
	return S_OK;
}

STDMETHODIMP MetadbHandleList::RemoveAttachedImages()
{
	if (m_handles.get_count() == 0) return E_POINTER;

	auto cb = fb2k::service_new<helpers::embed_thread>(helpers::embed_thread::remove_all, album_art_data_ptr(), m_handles, 0);
	threaded_process::get()->run_modeless(cb, threaded_process::flag_show_progress | threaded_process::flag_show_delayed | threaded_process::flag_show_item, core_api::get_main_window(), "Removing images...");
	return S_OK;
}

STDMETHODIMP MetadbHandleList::RemoveById(UINT index)
{
	if (index < m_handles.get_count())
	{
		m_handles.remove_by_idx(index);
		return S_OK;
	}
	return E_INVALIDARG;
}

STDMETHODIMP MetadbHandleList::RemoveRange(UINT from, UINT count)
{
	m_handles.remove_from_idx(from, count);
	return S_OK;
}

STDMETHODIMP MetadbHandleList::Sort()
{
	m_handles.sort_by_pointer_remove_duplicates();
	return S_OK;
}

STDMETHODIMP MetadbHandleList::UpdateFileInfoFromJSON(BSTR str)
{
	t_size count = m_handles.get_count();
	if (count == 0) return E_POINTER;

	json j;
	bool is_array;

	try
	{
		string_utf8_from_wide ustr(str);
		j = json::parse(ustr.get_ptr());
	}
	catch (...)
	{
		return E_INVALIDARG;
	}

	if (j.is_array() && j.size() == count)
	{
		is_array = true;
	}
	else if (j.is_object() && j.size() > 0)
	{
		is_array = false;
	}
	else
	{
		return E_INVALIDARG;
	}

	pfc::list_t<file_info_impl> info;
	info.set_size(count);

	for (t_size i = 0; i < count; ++i)
	{
		json obj = is_array ? j[i] : j;
		if (!obj.is_object() || obj.size() == 0) return E_INVALIDARG;

		metadb_handle_ptr item = m_handles.get_item(i);
		item->get_info(info[i]);

		for (json::iterator it = obj.begin(); it != obj.end(); ++it)
		{
			pfc::string8 key = (it.key()).c_str();
			if (key.is_empty()) return E_INVALIDARG;

			info[i].meta_remove_field(key);

			if (it.value().is_array())
			{
				for (json::iterator ita = it.value().begin(); ita != it.value().end(); ++ita)
				{
					pfc::string8 value = helpers::iterator_to_string(ita);
					if (value.get_length())
						info[i].meta_add(key, value);
				}
			}
			else
			{
				pfc::string8 value = helpers::iterator_to_string(it);
				if (value.get_length())
					info[i].meta_set(key, value);
			}
		}
	}

	metadb_io_v2::get()->update_info_async_simple(
		m_handles,
		pfc::ptr_list_const_array_t<const file_info, file_info_impl*>(info.get_ptr(), info.get_count()),
		core_api::get_main_window(),
		metadb_io_v2::op_flag_delay_ui,
		NULL
	);

	return S_OK;
}

STDMETHODIMP MetadbHandleList::get_Count(UINT* p)
{
	if (!p) return E_POINTER;

	*p = m_handles.get_count();
	return S_OK;
}

STDMETHODIMP MetadbHandleList::get_Item(UINT index, IMetadbHandle** pp)
{
	if (!pp) return E_POINTER;

	if (index < m_handles.get_count())
	{
		*pp = new com_object_impl_t<MetadbHandle>(m_handles.get_item_ref(index));
		return S_OK;
	}
	return E_INVALIDARG;
}

STDMETHODIMP MetadbHandleList::put_Item(UINT index, IMetadbHandle* handle)
{
	if (index < m_handles.get_count())
	{
		metadb_handle* ptr = NULL;
		handle->get__ptr((void**)&ptr);
		if (!ptr) return E_INVALIDARG;

		m_handles.replace_item(index, ptr);
		return S_OK;
	}
	return E_INVALIDARG;
}

PlaybackQueueItem::PlaybackQueueItem() {}

PlaybackQueueItem::PlaybackQueueItem(const t_playback_queue_item & playbackQueueItem)
{
	m_playback_queue_item.m_handle = playbackQueueItem.m_handle;
	m_playback_queue_item.m_playlist = playbackQueueItem.m_playlist;
	m_playback_queue_item.m_item = playbackQueueItem.m_item;
}

PlaybackQueueItem::~PlaybackQueueItem() {}

void PlaybackQueueItem::FinalRelease()
{
	m_playback_queue_item.m_handle.release();
	m_playback_queue_item.m_playlist = 0;
	m_playback_queue_item.m_item = 0;
}

STDMETHODIMP PlaybackQueueItem::get__ptr(void** pp)
{
	if (!pp) return E_POINTER;

	*pp = &m_playback_queue_item;
	return S_OK;
}

STDMETHODIMP PlaybackQueueItem::get_Handle(IMetadbHandle** pp)
{
	if (!pp) return E_POINTER;

	*pp = new com_object_impl_t<MetadbHandle>(m_playback_queue_item.m_handle);
	return S_OK;
}

STDMETHODIMP PlaybackQueueItem::get_PlaylistIndex(int* p)
{
	if (!p) return E_POINTER;

	*p = m_playback_queue_item.m_playlist;
	return S_OK;
}

STDMETHODIMP PlaybackQueueItem::get_PlaylistItemIndex(int* p)
{
	if (!p) return E_POINTER;

	*p = m_playback_queue_item.m_item;
	return S_OK;
}

PlayingItemLocation::PlayingItemLocation(bool isValid, t_size playlistIndex, t_size playlistItemIndex) : m_isValid(isValid), m_playlistIndex(playlistIndex), m_playlistItemIndex(playlistItemIndex) {}

STDMETHODIMP PlayingItemLocation::get_IsValid(VARIANT_BOOL* p)
{
	if (!p) return E_POINTER;

	*p = TO_VARIANT_BOOL(m_isValid);
	return S_OK;
}

STDMETHODIMP PlayingItemLocation::get_PlaylistIndex(int* p)
{
	if (!p) return E_POINTER;

	*p = m_isValid ? m_playlistIndex : -1;
	return S_OK;
}

STDMETHODIMP PlayingItemLocation::get_PlaylistItemIndex(int* p)
{
	if (!p) return E_POINTER;

	*p = m_isValid ? m_playlistItemIndex : -1;
	return S_OK;
}

STDMETHODIMP PlaylistRecyclerManager::Purge(VARIANT affectedItems)
{
	auto api = playlist_manager_v3::get();
	pfc::bit_array_bittable affected(api->recycler_get_count());
	helpers::com_array helper;
	if (!helper.convert_to_bit_array(affectedItems, affected)) return E_INVALIDARG;
	if (helper.get_count())
	{
		api->recycler_purge(affected);
	}
	return S_OK;
}

STDMETHODIMP PlaylistRecyclerManager::Restore(UINT index)
{
	auto api = playlist_manager_v3::get();
	if (index < api->recycler_get_count())
	{
		api->recycler_restore(index);
		return S_OK;
	}
	return E_INVALIDARG;
}

STDMETHODIMP PlaylistRecyclerManager::get_Content(UINT index, IMetadbHandleList** pp)
{
	if (!pp) return E_POINTER;

	auto api = playlist_manager_v3::get();
	if (index < api->recycler_get_count())
	{
		metadb_handle_list handles;
		api->recycler_get_content(index, handles);
		*pp = new com_object_impl_t<MetadbHandleList>(handles);
		return S_OK;
	}
	return E_INVALIDARG;
}

STDMETHODIMP PlaylistRecyclerManager::get_Count(UINT* p)
{
	if (!p) return E_POINTER;

	*p = playlist_manager_v3::get()->recycler_get_count();
	return S_OK;
}

STDMETHODIMP PlaylistRecyclerManager::get_Name(UINT index, BSTR* p)
{
	if (!p) return E_POINTER;

	auto api = playlist_manager_v3::get();
	t_size count = api->recycler_get_count();
	if (index < count)
	{
		pfc::string8_fast name;
		api->recycler_get_name(index, name);
		*p = SysAllocString(string_wide_from_utf8_fast(name));
		return S_OK;
	}
	return E_INVALIDARG;
}

Profiler::Profiler(const char* p_name) : m_name(p_name)
{
	m_timer.start();
}

Profiler::~Profiler() {}

STDMETHODIMP Profiler::Print()
{
	FB2K_console_formatter() << JSP_NAME_VERSION ": FbProfiler (" << m_name << "): " << (int)(m_timer.query() * 1000) << " ms";
	return S_OK;
}

STDMETHODIMP Profiler::Reset()
{
	m_timer.start();
	return S_OK;
}

STDMETHODIMP Profiler::get_Time(int* p)
{
	if (!p) return E_POINTER;

	*p = (int)(m_timer.query() * 1000);
	return S_OK;
}

ThemeManager::ThemeManager(HWND hwnd, BSTR classlist) : m_theme(NULL), m_partid(0), m_stateid(0)
{
	m_theme = OpenThemeData(hwnd, classlist);

	if (!m_theme) throw pfc::exception_invalid_params();
}

ThemeManager::~ThemeManager() {}

void ThemeManager::FinalRelease()
{
	if (m_theme)
	{
		CloseThemeData(m_theme);
		m_theme = NULL;
	}
}

STDMETHODIMP ThemeManager::DrawThemeBackground(IGdiGraphics* gr, int x, int y, int w, int h, int clip_x, int clip_y, int clip_w, int clip_h)
{
	if (!m_theme) return E_POINTER;

	Gdiplus::Graphics* graphics = NULL;
	gr->get__ptr((void**)&graphics);

	RECT rc = { x, y, x + w, y + h };
	RECT clip_rc = { clip_x, clip_y, clip_x + clip_w, clip_y + clip_h };
	LPCRECT pclip_rc = &clip_rc;
	HDC dc = graphics->GetHDC();

	if (clip_x == 0 && clip_y == 0 && clip_w == 0 && clip_h == 0)
	{
		pclip_rc = NULL;
	}

	HRESULT hr = ::DrawThemeBackground(m_theme, dc, m_partid, m_stateid, &rc, pclip_rc);

	graphics->ReleaseHDC(dc);
	return hr;
}

STDMETHODIMP ThemeManager::IsThemePartDefined(int partid, int stateid, VARIANT_BOOL* p)
{
	if (!m_theme || !p) return E_POINTER;

	*p = TO_VARIANT_BOOL(::IsThemePartDefined(m_theme, partid, stateid));
	return S_OK;
}

STDMETHODIMP ThemeManager::SetPartAndStateID(int partid, int stateid)
{
	if (!m_theme) return E_POINTER;

	m_partid = partid;
	m_stateid = stateid;
	return S_OK;
}

TitleFormat::TitleFormat(BSTR expr)
{
	string_utf8_from_wide uexpr(expr);
	titleformat_compiler::get()->compile_safe(m_obj, uexpr);
}

TitleFormat::~TitleFormat() {}

void TitleFormat::FinalRelease()
{
	m_obj.release();
}

STDMETHODIMP TitleFormat::get__ptr(void** pp)
{
	if (!pp) return E_POINTER;

	*pp = m_obj.get_ptr();
	return S_OK;
}

STDMETHODIMP TitleFormat::Eval(VARIANT_BOOL force, BSTR* p)
{
	if (m_obj.is_empty() || !p) return E_POINTER;

	pfc::string8_fast text;

	if (!playback_control::get()->playback_format_title(NULL, text, m_obj, NULL, playback_control::display_level_all) && force != VARIANT_FALSE)
	{
		metadb_handle_ptr handle;
		metadb::get()->handle_create(handle, make_playable_location("file://C:\\________.ogg", 0));
		handle->format_title(NULL, text, m_obj, NULL);
	}

	*p = SysAllocString(string_wide_from_utf8_fast(text));
	return S_OK;
}

STDMETHODIMP TitleFormat::EvalWithMetadb(IMetadbHandle* handle, BSTR* p)
{
	if (m_obj.is_empty() || !p) return E_POINTER;

	metadb_handle* ptr = NULL;
	handle->get__ptr((void**)&ptr);
	if (!ptr) return E_INVALIDARG;

	pfc::string8_fast text;
	ptr->format_title(NULL, text, m_obj, NULL);

	*p = SysAllocString(string_wide_from_utf8_fast(text));
	return S_OK;
}

STDMETHODIMP TitleFormat::EvalWithMetadbs(IMetadbHandleList* handles, VARIANT* p)
{
	if (m_obj.is_empty() || !p) return E_POINTER;

	metadb_handle_list* handles_ptr = NULL;
	handles->get__ptr((void**)&handles_ptr);

	metadb_handle_list_ref handles_ref = *handles_ptr;
	LONG count = handles_ref.get_count();
	helpers::com_array helper;
	if (!helper.create(count)) return E_OUTOFMEMORY;

	for (LONG i = 0; i < count; ++i)
	{
		pfc::string8_fast text;
		handles_ref[i]->format_title(NULL, text, m_obj, NULL);
		_variant_t var;
		var.vt = VT_BSTR;
		var.bstrVal = SysAllocString(string_wide_from_utf8_fast(text));
		if (!helper.put_item(i, var)) return E_OUTOFMEMORY;
	}
	p->vt = VT_ARRAY | VT_VARIANT;
	p->parray = helper.get_ptr();
	return S_OK;
}

Tooltip::Tooltip(HWND p_wndparent, const panel_tooltip_param_ptr & p_param_ptr) : m_wndparent(p_wndparent), m_panel_tooltip_param_ptr(p_param_ptr), m_tip_buffer(SysAllocString(PFC_WIDESTRING(JSP_NAME)))
{
	m_wndtooltip = CreateWindowEx(
		WS_EX_TOPMOST,
		TOOLTIPS_CLASS,
		NULL,
		WS_POPUP | TTS_ALWAYSTIP | TTS_NOPREFIX,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		p_wndparent,
		NULL,
		core_api::get_my_instance(),
		NULL);

	// Original position
	SetWindowPos(m_wndtooltip, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

	// Set up tooltip information.
	memset(&m_ti, 0, sizeof(m_ti));

	m_ti.cbSize = sizeof(m_ti);
	m_ti.uFlags = TTF_IDISHWND | TTF_SUBCLASS | TTF_TRANSPARENT;
	m_ti.hinst = core_api::get_my_instance();
	m_ti.hwnd = p_wndparent;
	m_ti.uId = (UINT_PTR)p_wndparent;
	m_ti.lpszText = m_tip_buffer;

	HFONT font = CreateFont(
		-(int)m_panel_tooltip_param_ptr->font_size,
		0,
		0,
		0,
		(m_panel_tooltip_param_ptr->font_style & Gdiplus::FontStyleBold) ? FW_BOLD : FW_NORMAL,
		(m_panel_tooltip_param_ptr->font_style & Gdiplus::FontStyleItalic) ? TRUE : FALSE,
		(m_panel_tooltip_param_ptr->font_style & Gdiplus::FontStyleUnderline) ? TRUE : FALSE,
		(m_panel_tooltip_param_ptr->font_style & Gdiplus::FontStyleStrikeout) ? TRUE : FALSE,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		m_panel_tooltip_param_ptr->font_name);

	SendMessage(m_wndtooltip, TTM_ADDTOOL, 0, (LPARAM)&m_ti);
	SendMessage(m_wndtooltip, TTM_ACTIVATE, FALSE, 0);
	SendMessage(m_wndtooltip, WM_SETFONT, (WPARAM)font, MAKELPARAM(FALSE, 0));

	m_panel_tooltip_param_ptr->tooltip_hwnd = m_wndtooltip;
	m_panel_tooltip_param_ptr->tooltip_size.cx = -1;
	m_panel_tooltip_param_ptr->tooltip_size.cy = -1;
}

Tooltip::~Tooltip() {}

void Tooltip::FinalRelease()
{
	if (m_wndtooltip && IsWindow(m_wndtooltip))
	{
		DestroyWindow(m_wndtooltip);
		m_wndtooltip = NULL;
	}

	if (m_tip_buffer)
	{
		SysFreeString(m_tip_buffer);
		m_tip_buffer = NULL;
	}
}

STDMETHODIMP Tooltip::get_Text(BSTR* p)
{
	if (!p) return E_POINTER;

	*p = SysAllocString(m_tip_buffer);
	return S_OK;
}

STDMETHODIMP Tooltip::put_Text(BSTR text)
{
	SysReAllocString(&m_tip_buffer, text);
	m_ti.lpszText = m_tip_buffer;
	SendMessage(m_wndtooltip, TTM_SETTOOLINFO, 0, (LPARAM)&m_ti);
	return S_OK;
}

STDMETHODIMP Tooltip::put_TrackActivate(VARIANT_BOOL activate)
{
	if (activate)
	{
		m_ti.uFlags |= TTF_TRACK | TTF_ABSOLUTE;
	}
	else
	{
		m_ti.uFlags &= ~(TTF_TRACK | TTF_ABSOLUTE);
	}

	SendMessage(m_wndtooltip, TTM_TRACKACTIVATE, activate != VARIANT_FALSE ? TRUE : FALSE, (LPARAM)&m_ti);
	return S_OK;
}

STDMETHODIMP Tooltip::Activate()
{
	SendMessage(m_wndtooltip, TTM_ACTIVATE, TRUE, 0);
	return S_OK;
}

STDMETHODIMP Tooltip::Deactivate()
{
	SendMessage(m_wndtooltip, TTM_ACTIVATE, FALSE, 0);
	return S_OK;
}

STDMETHODIMP Tooltip::SetMaxWidth(int width)
{
	SendMessage(m_wndtooltip, TTM_SETMAXTIPWIDTH, 0, width);
	return S_OK;
}

STDMETHODIMP Tooltip::GetDelayTime(int type, int* p)
{
	if (!p) return E_POINTER;
	if (type < TTDT_AUTOMATIC || type > TTDT_INITIAL) return E_INVALIDARG;

	*p = SendMessage(m_wndtooltip, TTM_GETDELAYTIME, type, 0);
	return S_OK;
}

STDMETHODIMP Tooltip::SetDelayTime(int type, int time)
{
	if (type < TTDT_AUTOMATIC || type > TTDT_INITIAL) return E_INVALIDARG;

	SendMessage(m_wndtooltip, TTM_SETDELAYTIME, type, time);
	return S_OK;
}

STDMETHODIMP Tooltip::TrackPosition(int x, int y)
{
	POINT pt = { x, y };
	ClientToScreen(m_wndparent, &pt);
	SendMessage(m_wndtooltip, TTM_TRACKPOSITION, 0, MAKELONG(pt.x, pt.y));
	return S_OK;
}

UiSelectionHolder::UiSelectionHolder(const ui_selection_holder::ptr & holder) : m_holder(holder) {}
UiSelectionHolder::~UiSelectionHolder() {}

void UiSelectionHolder::FinalRelease()
{
	m_holder.release();
}

STDMETHODIMP UiSelectionHolder::SetPlaylistSelectionTracking()
{
	m_holder->set_playlist_selection_tracking();
	return S_OK;
}

STDMETHODIMP UiSelectionHolder::SetPlaylistTracking()
{
	m_holder->set_playlist_tracking();
	return S_OK;
}

STDMETHODIMP UiSelectionHolder::SetSelection(IMetadbHandleList* handles)
{
	metadb_handle_list* handles_ptr = NULL;
	handles->get__ptr((void**)&handles_ptr);
	m_holder->set_selection(*handles_ptr);
	return S_OK;
}
