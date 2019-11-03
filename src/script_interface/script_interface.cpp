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

STDMETHODIMP ContextMenuManager::BuildMenu(IMenuObj* p, int base_id)
{
	if (m_cm.is_empty()) return E_POINTER;

	HMENU menuid;
	p->get__ID(&menuid);
	contextmenu_node* parent = m_cm->get_root();
	m_cm->win32_build_menu(menuid, parent, base_id, -1);
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
	metadb_handle_list* handles_ptr = nullptr;
	GET_PTR(handles, handles_ptr)

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

DropSourceAction::DropSourceAction() : m_base(0), m_effect(DROPEFFECT_NONE), m_playlist_idx(0), m_to_select(true) {}

DropSourceAction::~DropSourceAction() {}

void DropSourceAction::FinalRelease() {}

STDMETHODIMP DropSourceAction::get_Effect(UINT* p)
{
	if (!p) return E_POINTER;

	*p = m_effect;
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
		m_info_ptr = nullptr;
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
		*p = TO_BSTR(m_info_ptr->info_enum_name(idx));
		return S_OK;
	}
	return E_INVALIDARG;
}

STDMETHODIMP FileInfo::InfoValue(UINT idx, BSTR* p)
{
	if (!m_info_ptr || !p) return E_POINTER;

	if (idx < m_info_ptr->info_get_count())
	{
		*p = TO_BSTR(m_info_ptr->info_enum_value(idx));
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
		*p = TO_BSTR(m_info_ptr->meta_enum_name(idx));
		return S_OK;
	}
	return E_INVALIDARG;
}

STDMETHODIMP FileInfo::MetaValue(UINT idx, UINT vidx, BSTR* p)
{
	if (!m_info_ptr || !p) return E_POINTER;

	if (idx < m_info_ptr->meta_get_count() && vidx < m_info_ptr->meta_enum_value_count(idx))
	{
		*p = TO_BSTR(m_info_ptr->meta_enum_value(idx, vidx));
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

	const auto width = m_ptr->GetWidth();
	const auto height = m_ptr->GetHeight();
	auto out = new Gdiplus::Bitmap(width, height, PixelFormat32bppPARGB);
	Gdiplus::Graphics g(out);
	Gdiplus::ImageAttributes ia;
	Gdiplus::ColorMatrix cm = { 0.0 };
	const Gdiplus::Rect rect(0, 0, width, height);

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
	Gdiplus::Bitmap* bitmap_mask = nullptr;
	GET_PTR(mask, bitmap_mask)

	if (bitmap_mask->GetHeight() != m_ptr->GetHeight() || bitmap_mask->GetWidth() != m_ptr->GetWidth())
	{
		return E_INVALIDARG;
	}

	const Gdiplus::Rect rect(0, 0, m_ptr->GetWidth(), m_ptr->GetHeight());

	Gdiplus::BitmapData bmpdata_mask = { 0 };
	if (bitmap_mask->LockBits(&rect, Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, &bmpdata_mask) != Gdiplus::Ok)
	{
		return S_OK;
	}

	Gdiplus::BitmapData bmpdata_dst = { 0 };
	if (m_ptr->LockBits(&rect, Gdiplus::ImageLockModeRead | Gdiplus::ImageLockModeWrite, PixelFormat32bppARGB, &bmpdata_dst) != Gdiplus::Ok)
	{
		bitmap_mask->UnlockBits(&bmpdata_mask);
		return S_OK;
	}

	t_size* p_mask = reinterpret_cast<t_size*>(bmpdata_mask.Scan0);
	t_size* p_dst = reinterpret_cast<t_size*>(bmpdata_dst.Scan0);
	const t_size* p_mask_end = p_mask + (rect.Width * rect.Height);
	t_size alpha;

	while (p_mask < p_mask_end)
	{
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

	*pp = nullptr;
	Gdiplus::Bitmap* img = m_ptr->Clone(x, y, w, h, PixelFormat32bppPARGB);
	if (helpers::ensure_gdiplus_object(img))
	{
		*pp = new com_object_impl_t<GdiBitmap>(img);
	}
	else
	{
		if (img) delete img;
		img = nullptr;
	}

	return S_OK;
}

STDMETHODIMP GdiBitmap::CreateRawBitmap(IGdiRawBitmap** pp)
{
	if (!m_ptr || !pp) return E_POINTER;

	*pp = new com_object_impl_t<GdiRawBitmap>(m_ptr);
	return S_OK;
}

STDMETHODIMP GdiBitmap::GetColourSchemeJSON(UINT count, BSTR* p)
{
	if (!m_ptr || !p) return E_POINTER;

	Gdiplus::BitmapData bmpdata;
	const int w = std::min(static_cast<int>(m_ptr->GetWidth()), 220);
	const int h = std::min(static_cast<int>(m_ptr->GetHeight()), 220);
	auto bitmap = new Gdiplus::Bitmap(w, h, PixelFormat32bppPARGB);
	Gdiplus::Graphics gb(bitmap);
	const Gdiplus::Rect rect(0, 0, w, h);
	gb.SetInterpolationMode((Gdiplus::InterpolationMode)6);
	gb.DrawImage(m_ptr, 0, 0, w, h);

	if (bitmap->LockBits(&rect, Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, &bmpdata) != Gdiplus::Ok)
		return E_POINTER;

	std::map<t_size, int> colour_counters;
	const t_size colours_length = bmpdata.Width * bmpdata.Height;
	const t_size* colours = (const t_size*)bmpdata.Scan0;

	for (t_size i = 0; i < colours_length; ++i)
	{
		BYTE r = (colours[i] >> 16) & 0xff;
		BYTE g = (colours[i] >> 8) & 0xff;
		BYTE b = (colours[i] & 0xff);

		// We're reducing total colors from 2^24 to 2^15 by rounding each color component value to multiples of 8.
		// First we need to check if the byte will overflow, and if so pin to 0xff, otherwise add 4 and round down.
		r = (r > 251) ? 0xff : (r + 4) & 0xf8;
		g = (g > 251) ? 0xff : (g + 4) & 0xf8;
		b = (b > 251) ? 0xff : (b + 4) & 0xf8;

		++colour_counters[r << 16 | g << 8 | b];
	}
	bitmap->UnlockBits(&bmpdata);

	std::map<t_size, int>::iterator it;
	std::vector<Point> points;
	int idx = 0;

	for (it = colour_counters.begin(); it != colour_counters.end(); it++, idx++)
	{
		const BYTE r = (it->first >> 16) & 0xff;
		const BYTE g = (it->first >> 8) & 0xff;
		const BYTE b = (it->first & 0xff);

		std::vector<t_size> values = { r, g, b };
		Point pt(idx, values, it->second);
		points.push_back(pt);
	}

	KMeans kmeans(count, colour_counters.size(), 12);
	std::vector<Cluster> clusters = kmeans.run(points);

	std::sort(
		clusters.begin(),
		clusters.end(),
		[](Cluster & a, Cluster & b) {
			return a.getTotalPoints() > b.getTotalPoints();
		});

	json j = json::array();
	const t_size outCount = std::min(count, colour_counters.size());
	for (t_size i = 0; i < outCount; ++i)
	{
		int colour = 0xff000000 | static_cast<int>(clusters[i].getCentralValue(0)) << 16 | static_cast<int>(clusters[i].getCentralValue(1)) << 8 | static_cast<int>(clusters[i].getCentralValue(2));
		double frequency = clusters[i].getTotalPoints() / (double)colours_length;

		j.push_back({
			{ "col", colour },
			{ "freq", frequency }
		});
	}
	*p = TO_BSTR((j.dump()).c_str());
	return S_OK;
}

STDMETHODIMP GdiBitmap::GetGraphics(IGdiGraphics** pp)
{
	if (!m_ptr || !pp) return E_POINTER;

	auto g = new Gdiplus::Graphics(m_ptr);
	*pp = new com_object_impl_t<GdiGraphics>();
	(*pp)->put__ptr(g);
	return S_OK;
}

STDMETHODIMP GdiBitmap::ReleaseGraphics(IGdiGraphics* p)
{
	if (p)
	{
		Gdiplus::Graphics* g = nullptr;
		GET_PTR(p, g)
		p->put__ptr(nullptr);
		if (g) delete g;
	}

	return S_OK;
}

STDMETHODIMP GdiBitmap::Resize(UINT w, UINT h, int interpolationMode, IGdiBitmap** pp)
{
	if (!m_ptr || !pp) return E_POINTER;

	auto bitmap = new Gdiplus::Bitmap(w, h, PixelFormat32bppPARGB);
	Gdiplus::Graphics g(bitmap);
	g.SetInterpolationMode(static_cast<Gdiplus::InterpolationMode>(interpolationMode));
	g.DrawImage(m_ptr, 0, 0, w, h);
	*pp = new com_object_impl_t<GdiBitmap>(bitmap);
	return S_OK;
}

STDMETHODIMP GdiBitmap::RotateFlip(UINT mode)
{
	if (!m_ptr) return E_POINTER;

	m_ptr->RotateFlip(static_cast<Gdiplus::RotateFlipType>(mode));
	return S_OK;
}

STDMETHODIMP GdiBitmap::SaveAs(BSTR path, BSTR format, VARIANT_BOOL* p)
{
	if (!m_ptr || !p) return E_POINTER;

	*p = VARIANT_FALSE;
	CLSID clsid_encoder;
	if (helpers::get_encoder_clsid(format, &clsid_encoder))
	{
		m_ptr->Save(path, &clsid_encoder);
		*p = TO_VARIANT_BOOL(m_ptr->GetLastStatus() == Gdiplus::Ok);
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
GdiFont::~GdiFont() {}

void GdiFont::FinalRelease()
{
	if (m_hFont && m_managed)
	{
		DeleteFont(m_hFont);
		m_hFont = nullptr;
	}

	GdiObj<IGdiFont, Gdiplus::Font>::FinalRelease();
}

STDMETHODIMP GdiFont::get__HFont(UINT* p)
{
	if (!m_ptr || !p) return E_POINTER;

	*p = reinterpret_cast<UINT>(m_hFont);
	return S_OK;
}

STDMETHODIMP GdiFont::get_Height(UINT* p)
{
	if (!m_ptr || !p) return E_POINTER;

	Gdiplus::Bitmap img(1, 1, PixelFormat32bppPARGB);
	Gdiplus::Graphics g(&img);
	*p = static_cast<UINT>(m_ptr->GetHeight(&g));
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

GdiGraphics::GdiGraphics() : GdiObj<IGdiGraphics, Gdiplus::Graphics>(nullptr) {}

void GdiGraphics::GetRoundRectPath(Gdiplus::GraphicsPath& gp, const Gdiplus::RectF& rect, float arc_width, float arc_height)
{
	const float arc_dia_w = arc_width * 2;
	const float arc_dia_h = arc_height * 2;
	Gdiplus::RectF corner(rect.X, rect.Y, arc_dia_w, arc_dia_h);

	gp.Reset();

	gp.AddArc(corner, 180, 90);

	corner.X += (rect.Width - arc_dia_w);
	gp.AddArc(corner, 270, 90);

	corner.Y += (rect.Height - arc_dia_h);
	gp.AddArc(corner, 0, 90);

	corner.X -= (rect.Width - arc_dia_w);
	gp.AddArc(corner, 90, 90);

	gp.CloseFigure();
}

void GdiGraphics::SetFormat(int flags, Gdiplus::StringFormat& fmt)
{
	if (flags != 0)
	{
		fmt.SetAlignment(static_cast<Gdiplus::StringAlignment>((flags >> 28) & 0x3));
		fmt.SetLineAlignment(static_cast<Gdiplus::StringAlignment>((flags >> 24) & 0x3));
		fmt.SetTrimming(static_cast<Gdiplus::StringTrimming>((flags >> 20) & 0x7));
		fmt.SetFormatFlags(static_cast<Gdiplus::StringFormatFlags>(flags & 0x7FFF));
	}
}

STDMETHODIMP GdiGraphics::put__ptr(void* p)
{
	m_ptr = (Gdiplus::Graphics*)p;
	return S_OK;
}

STDMETHODIMP GdiGraphics::CalcTextHeight(BSTR str, IGdiFont* font, UINT* p)
{
	if (!m_ptr || !p) return E_POINTER;

	HFONT hFont = nullptr;
	font->get__HFont(reinterpret_cast<UINT*>(&hFont));
	HDC dc = m_ptr->GetHDC();

	{
		SelectObjectScope scope(dc, hFont);
		*p = helpers::get_text_height(dc, str, SysStringLen(str));
	}

	m_ptr->ReleaseHDC(dc);
	return S_OK;
}

STDMETHODIMP GdiGraphics::CalcTextWidth(BSTR str, IGdiFont* font, UINT* p)
{
	if (!m_ptr || !p) return E_POINTER;

	HFONT hFont = nullptr;
	font->get__HFont(reinterpret_cast<UINT*>(&hFont));
	HDC dc = m_ptr->GetHDC();

	{
		SelectObjectScope scope(dc, hFont);
		*p = helpers::get_text_width(dc, str, SysStringLen(str));
	}

	m_ptr->ReleaseHDC(dc);
	return S_OK;
}

STDMETHODIMP GdiGraphics::DrawEllipse(float x, float y, float w, float h, float line_width, LONGLONG colour)
{
	if (!m_ptr) return E_POINTER;

	Gdiplus::Pen pen(static_cast<unsigned int>(colour), line_width);
	m_ptr->DrawEllipse(&pen, x, y, w, h);
	return S_OK;
}

STDMETHODIMP GdiGraphics::DrawImage(IGdiBitmap* image, float dstX, float dstY, float dstW, float dstH, float srcX, float srcY, float srcW, float srcH, float angle, BYTE alpha)
{
	if (!m_ptr) return E_POINTER;

	Gdiplus::Bitmap* img = nullptr;
	GET_PTR(image, img)

	Gdiplus::Matrix old_m;

	if (angle != 0.0)
	{
		const Gdiplus::PointF pt(dstX + dstW / 2, dstY + dstH / 2);
		Gdiplus::Matrix m;
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

STDMETHODIMP GdiGraphics::DrawLine(float x1, float y1, float x2, float y2, float line_width, LONGLONG colour)
{
	if (!m_ptr) return E_POINTER;

	Gdiplus::Pen pen(static_cast<unsigned int>(colour), line_width);
	m_ptr->DrawLine(&pen, x1, y1, x2, y2);
	return S_OK;
}

STDMETHODIMP GdiGraphics::DrawPolygon(LONGLONG colour, float line_width, VARIANT points)
{
	if (!m_ptr) return E_POINTER;

	helpers::com_array helper;
	std::vector<Gdiplus::PointF> point_array;
	if (!helper.convert(points, point_array)) return E_INVALIDARG;

	Gdiplus::Pen pen(static_cast<unsigned int>(colour), line_width);
	m_ptr->DrawPolygon(&pen, point_array.data(), point_array.size());
	return S_OK;
}

STDMETHODIMP GdiGraphics::DrawRect(float x, float y, float w, float h, float line_width, LONGLONG colour)
{
	if (!m_ptr) return E_POINTER;

	Gdiplus::Pen pen(static_cast<unsigned int>(colour), line_width);
	m_ptr->DrawRectangle(&pen, x, y, w, h);
	return S_OK;
}

STDMETHODIMP GdiGraphics::DrawRoundRect(float x, float y, float w, float h, float arc_width, float arc_height, float line_width, LONGLONG colour)
{
	if (!m_ptr) return E_POINTER;

	if (2 * arc_width > w || 2 * arc_height > h) return E_INVALIDARG;

	Gdiplus::Pen pen(static_cast<unsigned int>(colour), line_width);
	Gdiplus::GraphicsPath gp;
	const Gdiplus::RectF rect(x, y, w, h);
	GetRoundRectPath(gp, rect, arc_width, arc_height);
	pen.SetStartCap(Gdiplus::LineCapRound);
	pen.SetEndCap(Gdiplus::LineCapRound);
	m_ptr->DrawPath(&pen, &gp);
	return S_OK;
}

STDMETHODIMP GdiGraphics::DrawString(BSTR str, IGdiFont* font, LONGLONG colour, float x, float y, float w, float h, int flags)
{
	if (!m_ptr) return E_POINTER;

	Gdiplus::Font* fn = nullptr;
	GET_PTR(font, fn)

	Gdiplus::StringFormat fmt(Gdiplus::StringFormat::GenericTypographic());
	SetFormat(flags, fmt);

	Gdiplus::SolidBrush br(static_cast<unsigned int>(colour));
	m_ptr->DrawString(str, -1, fn, Gdiplus::RectF(x, y, w, h), &fmt, &br);
	return S_OK;
}

STDMETHODIMP GdiGraphics::EstimateLineWrap(BSTR str, IGdiFont* font, int max_width, VARIANT* p)
{
	if (!m_ptr || !p) return E_POINTER;

	helpers::wrapped_item_list result;

	HFONT hFont = nullptr;
	font->get__HFont(reinterpret_cast<UINT*>(&hFont));
	HDC dc = m_ptr->GetHDC();

	{
		SelectObjectScope scope(dc, hFont);
		helpers::estimate_line_wrap(dc, str, max_width, result);
	}

	m_ptr->ReleaseHDC(dc);

	const LONG count = result.size();
	helpers::com_array helper;
	if (!helper.create(count * 2)) return E_OUTOFMEMORY;
	for (LONG i = 0; i < count; ++i)
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

STDMETHODIMP GdiGraphics::FillEllipse(float x, float y, float w, float h, LONGLONG colour)
{
	if (!m_ptr) return E_POINTER;

	Gdiplus::SolidBrush br(static_cast<unsigned int>(colour));
	m_ptr->FillEllipse(&br, x, y, w, h);
	return S_OK;
}

STDMETHODIMP GdiGraphics::FillGradRect(float x, float y, float w, float h, float angle, LONGLONG colour1, LONGLONG colour2, float focus)
{
	if (!m_ptr) return E_POINTER;

	const Gdiplus::RectF rect(x, y, w, h);
	Gdiplus::LinearGradientBrush brush(rect, static_cast<unsigned int>(colour1), static_cast<unsigned int>(colour2), angle, TRUE);
	brush.SetBlendTriangularShape(focus);
	m_ptr->FillRectangle(&brush, rect);
	return S_OK;
}

STDMETHODIMP GdiGraphics::FillPolygon(LONGLONG colour, int fillmode, VARIANT points)
{
	if (!m_ptr) return E_POINTER;

	helpers::com_array helper;
	std::vector<Gdiplus::PointF> point_array;
	if (!helper.convert(points, point_array)) return E_INVALIDARG;

	Gdiplus::SolidBrush br(static_cast<unsigned int>(colour));
	m_ptr->FillPolygon(&br, point_array.data(), point_array.size(), (Gdiplus::FillMode)fillmode);
	return S_OK;
}

STDMETHODIMP GdiGraphics::FillRoundRect(float x, float y, float w, float h, float arc_width, float arc_height, LONGLONG colour)
{
	if (!m_ptr) return E_POINTER;

	if (2 * arc_width > w || 2 * arc_height > h) return E_INVALIDARG;

	Gdiplus::SolidBrush br(static_cast<unsigned int>(colour));
	Gdiplus::GraphicsPath gp;
	const Gdiplus::RectF rect(x, y, w, h);
	GetRoundRectPath(gp, rect, arc_width, arc_height);
	m_ptr->FillPath(&br, &gp);
	return S_OK;
}

STDMETHODIMP GdiGraphics::FillSolidRect(float x, float y, float w, float h, LONGLONG colour)
{
	if (!m_ptr) return E_POINTER;

	Gdiplus::SolidBrush brush(static_cast<unsigned int>(colour));
	m_ptr->FillRectangle(&brush, x, y, w, h);
	return S_OK;
}

STDMETHODIMP GdiGraphics::GdiAlphaBlend(IGdiRawBitmap* bitmap, int dstX, int dstY, int dstW, int dstH, int srcX, int srcY, int srcW, int srcH, BYTE alpha)
{
	if (!m_ptr) return E_POINTER;

	HDC src_dc = nullptr;
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

	HDC src_dc = nullptr;
	bitmap->get__Handle(&src_dc);
	HDC dc = m_ptr->GetHDC();

	if (dstW == srcW && dstH == srcH)
	{
		BitBlt(dc, dstX, dstY, dstW, dstH, src_dc, srcX, srcY, SRCCOPY);
	}
	else
	{
		SetStretchBltMode(dc, HALFTONE);
		SetBrushOrgEx(dc, 0, 0, nullptr);
		StretchBlt(dc, dstX, dstY, dstW, dstH, src_dc, srcX, srcY, srcW, srcH, SRCCOPY);
	}

	m_ptr->ReleaseHDC(dc);
	return S_OK;
}

STDMETHODIMP GdiGraphics::GdiDrawText(BSTR str, IGdiFont* font, LONGLONG colour, int x, int y, int w, int h, int format)
{
	if (!m_ptr) return E_POINTER;
	if (format & DT_MODIFYSTRING) return E_INVALIDARG;

	HFONT hFont = nullptr;
	font->get__HFont(reinterpret_cast<UINT*>(&hFont));
	HDC dc = m_ptr->GetHDC();

	{
		RECT rc = { x, y, x + w, y + h };
		SelectObjectScope scope(dc, hFont);

		SetTextColor(dc, helpers::convert_argb_to_colorref(static_cast<unsigned int>(colour)));
		SetBkMode(dc, TRANSPARENT);
		SetTextAlign(dc, TA_LEFT | TA_TOP | TA_NOUPDATECP);

		if (format & DT_CALCRECT)
		{
			RECT rc_calc = { 0 }, rc_old = { 0 };

			memcpy(&rc_calc, &rc, sizeof(RECT));
			memcpy(&rc_old, &rc, sizeof(RECT));

			DrawText(dc, str, -1, &rc_calc, format);

			format &= ~DT_CALCRECT;

			if (format & DT_VCENTER)
			{
				rc.top = rc_old.top + ((RECT_CY(rc_old) - RECT_CY(rc_calc)) >> 1);
				rc.bottom = rc.top + RECT_CY(rc_calc);
			}
			else if (format & DT_BOTTOM)
			{
				rc.top = rc_old.bottom - RECT_CY(rc_calc);
			}
		}

		DRAWTEXTPARAMS dpt = { sizeof(DRAWTEXTPARAMS), 4, 0, 0, 0 };
		DrawTextEx(dc, str, -1, &rc, format, &dpt);
	}

	m_ptr->ReleaseHDC(dc);
	return S_OK;
}

STDMETHODIMP GdiGraphics::MeasureString(BSTR str, IGdiFont* font, float x, float y, float w, float h, int flags, IMeasureStringInfo** pp)
{
	if (!m_ptr || !pp) return E_POINTER;

	Gdiplus::Font* fn = nullptr;
	GET_PTR(font, fn)

	Gdiplus::StringFormat fmt(Gdiplus::StringFormat::GenericTypographic());
	SetFormat(flags, fmt);

	Gdiplus::RectF bound;
	int chars, lines;

	m_ptr->MeasureString(str, -1, fn, Gdiplus::RectF(x, y, w, h), &fmt, &bound, &chars, &lines);

	*pp = new com_object_impl_t<MeasureStringInfo>(bound.X, bound.Y, bound.Width, bound.Height, lines, chars);
	return S_OK;
}

STDMETHODIMP GdiGraphics::SetInterpolationMode(int mode)
{
	if (!m_ptr) return E_POINTER;

	m_ptr->SetInterpolationMode(static_cast<Gdiplus::InterpolationMode>(mode));
	return S_OK;
}

STDMETHODIMP GdiGraphics::SetSmoothingMode(int mode)
{
	if (!m_ptr) return E_POINTER;

	m_ptr->SetSmoothingMode(static_cast<Gdiplus::SmoothingMode>(mode));
	return S_OK;
}

STDMETHODIMP GdiGraphics::SetTextRenderingHint(UINT mode)
{
	if (!m_ptr) return E_POINTER;

	m_ptr->SetTextRenderingHint(static_cast<Gdiplus::TextRenderingHint>(mode));
	return S_OK;
}

GdiRawBitmap::GdiRawBitmap(Gdiplus::Bitmap* p_bmp)
{
	PFC_ASSERT(p_bmp != nullptr);
	m_width = p_bmp->GetWidth();
	m_height = p_bmp->GetHeight();

	m_hdc = CreateCompatibleDC(nullptr);
	m_hbmp = CreateHBITMAP(p_bmp);
	m_hbmpold = static_cast<HBITMAP>(SelectObject(m_hdc, m_hbmp));
}

GdiRawBitmap::~GdiRawBitmap() {}

HBITMAP GdiRawBitmap::CreateHBITMAP(Gdiplus::Bitmap* bitmap_ptr)
{
	const Gdiplus::Rect rect(0, 0, bitmap_ptr->GetWidth(), bitmap_ptr->GetHeight());
	Gdiplus::BitmapData bmpdata;

	if (bitmap_ptr->LockBits(&rect, Gdiplus::ImageLockModeRead, PixelFormat32bppPARGB, &bmpdata) != Gdiplus::Ok)
	{
		return nullptr;
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
		SelectObject(m_hdc, m_hbmpold);
		DeleteDC(m_hdc);
		m_hdc = nullptr;
	}

	if (m_hbmp)
	{
		DeleteBitmap(m_hbmp);
		m_hbmp = nullptr;
	}
}

STDMETHODIMP GdiRawBitmap::get__Handle(HDC* p)
{
	if (!m_hdc || !p) return E_POINTER;

	*p = m_hdc;
	return S_OK;
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

MainMenuManager::MainMenuManager() {}
MainMenuManager::~MainMenuManager() {}

void MainMenuManager::FinalRelease()
{
	m_mm.release();
}

STDMETHODIMP MainMenuManager::BuildMenu(IMenuObj* p, UINT base_id)
{
	if (m_mm.is_empty()) return E_POINTER;

	HMENU menuid;
	p->get__ID(&menuid);

	// HACK: workaround for foo_menu_addons
	try
	{
		m_mm->generate_menu_win32(menuid, base_id, SIZE_MAX, mainmenu_manager::flag_show_shortcuts);
	}
	catch (...) {}

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
	using t_valid_root_name = std::pair<const wchar_t*, const GUID*>;

	static const std::vector<t_valid_root_name> valid_root_names =
	{
		{ L"file", &mainmenu_groups::file },
		{ L"edit", &mainmenu_groups::edit },
		{ L"view", &mainmenu_groups::view },
		{ L"playback", &mainmenu_groups::playback },
		{ L"library", &mainmenu_groups::library },
		{ L"help", &mainmenu_groups::help }
	};

	const auto it = std::find_if(valid_root_names.begin(), valid_root_names.end(), [&](const t_valid_root_name& item) { return _wcsicmp(root_name, item.first) == 0; });
	if (it != valid_root_names.end())
	{
		m_mm = standard_api_create_t<mainmenu_manager>();
		m_mm->instantiate(*it->second);
		return S_OK;
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
		m_hMenu = nullptr;
	}
}

STDMETHODIMP MenuObj::get__ID(HMENU* p)
{
	if (!m_hMenu || !p) return E_POINTER;

	*p = m_hMenu;
	return S_OK;
}

STDMETHODIMP MenuObj::AppendMenuItem(UINT flags, UINT item_id, BSTR text)
{
	if (!m_hMenu) return E_POINTER;
	if (flags & MF_POPUP || flags & MF_OWNERDRAW || flags & MF_BITMAP) return E_INVALIDARG;

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

STDMETHODIMP MenuObj::TrackPopupMenu(int x, int y, UINT flags, UINT* p)
{
	if (!m_hMenu || !p) return E_POINTER;

	flags |= TPM_NONOTIFY | TPM_RETURNCMD | TPM_RIGHTBUTTON;
	flags &= ~TPM_RECURSE;

	POINT pt = { x, y };
	ClientToScreen(m_wnd_parent, &pt);
	*p = ::TrackPopupMenu(m_hMenu, flags, pt.x, pt.y, 0, m_wnd_parent, 0);
	return S_OK;
}

MetadbHandle::MetadbHandle(const metadb_handle_ptr& src) : m_handle(src) {}
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

	metadb_handle* ptr = nullptr;
	GET_PTR(handle, ptr)

	*p = TO_VARIANT_BOOL(ptr == m_handle.get_ptr());
	return S_OK;
}

STDMETHODIMP MetadbHandle::GetAlbumArt(UINT art_id, VARIANT_BOOL need_stub, VARIANT* p)
{
	if (m_handle.is_empty() || !p) return E_POINTER;

	pfc::string8_fast image_path;
	IGdiBitmap* bitmap = helpers::get_album_art(m_handle, art_id, need_stub != VARIANT_FALSE, false, image_path);

	_variant_t var1, var2;
	var1.vt = VT_DISPATCH;
	var1.pdispVal = bitmap;
	var2.vt = VT_BSTR;
	var2.bstrVal = TO_BSTR(image_path);

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
		stats::theAPI()->dispatch_refresh(jsp_guids::metadb_index, hash);
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
		auto fp = string_utf8_from_wide(first_played);
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
		auto lp = string_utf8_from_wide(last_played);
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

	*p = TO_BSTR(file_path_display(m_handle->get_path()));
	return S_OK;
}

STDMETHODIMP MetadbHandle::get_RawPath(BSTR* p)
{
	if (m_handle.is_empty() || !p) return E_POINTER;

	*p = TO_BSTR(m_handle->get_path());
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
	metadb_handle* ptr = nullptr;
	GET_PTR(handle, ptr)

	m_handles.add_item(ptr);
	return S_OK;
}

STDMETHODIMP MetadbHandleList::AddRange(IMetadbHandleList* handles)
{
	metadb_handle_list* handles_ptr = nullptr;
	GET_PTR(handles, handles_ptr)

	m_handles.add_items(*handles_ptr);
	return S_OK;
}

STDMETHODIMP MetadbHandleList::AttachImage(BSTR path, UINT art_id)
{
	if (m_handles.get_count() == 0) return E_POINTER;

	album_art_data_ptr data;

	try
	{
		auto upath = string_utf8_from_wide(path);
		if (!filesystem::g_is_remote_or_unrecognized(upath))
		{
			file::ptr file;
			filesystem::g_open(file, upath, filesystem::open_mode_read, fb2k::noAbort);
			if (file.is_valid())
			{
				auto tmp = fb2k::service_new<album_art_data_impl>();
				tmp->from_stream(file.get_ptr(), t_size(file->get_size_ex(fb2k::noAbort)), fb2k::noAbort);
				data = tmp;
			}
		}
	}
	catch (...) {}

	if (data.is_valid())
	{
		auto cb = fb2k::service_new<helpers::embed_thread>(helpers::embed_thread::actions::attach, data, m_handles, art_id);
		threaded_process::get()->run_modeless(cb, helpers::embed_thread::flags, core_api::get_main_window(), "Embedding image...");
	}
	return S_OK;
}

STDMETHODIMP MetadbHandleList::BSearch(IMetadbHandle* handle, int* p)
{
	if (!p) return E_POINTER;

	metadb_handle* ptr = nullptr;
	GET_PTR(handle, ptr)

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

	const LONG count = m_handles.get_count();
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

	metadb_handle* ptr = nullptr;
	GET_PTR(handle, ptr)

	*p = m_handles.find_item(ptr);
	return S_OK;
}

STDMETHODIMP MetadbHandleList::GetLibraryRelativePaths(VARIANT* p)
{
	if (!p) return E_POINTER;

	const LONG count = m_handles.get_count();
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
		var.bstrVal = TO_BSTR(temp);
		if (!helper.put_item(i, var)) return E_OUTOFMEMORY;
	}
	p->vt = VT_ARRAY | VT_VARIANT;
	p->parray = helper.get_ptr();
	return S_OK;
}

STDMETHODIMP MetadbHandleList::Insert(UINT index, IMetadbHandle* handle)
{
	metadb_handle* ptr = nullptr;
	GET_PTR(handle, ptr)

	m_handles.insert_item(ptr, index);
	return S_OK;
}

STDMETHODIMP MetadbHandleList::InsertRange(UINT index, IMetadbHandleList* handles)
{
	metadb_handle_list* handles_ptr = nullptr;
	GET_PTR(handles, handles_ptr)

	m_handles.insert_items(*handles_ptr, index);
	return S_OK;
}

STDMETHODIMP MetadbHandleList::MakeDifference(IMetadbHandleList* handles)
{
	metadb_handle_list* handles_ptr = nullptr;
	GET_PTR(handles, handles_ptr)

	metadb_handle_list r1, r2;
	metadb_handle_list_helper::sorted_by_pointer_extract_difference(m_handles, *handles_ptr, r1, r2);
	m_handles = r1;
	return S_OK;
}

STDMETHODIMP MetadbHandleList::MakeIntersection(IMetadbHandleList* handles)
{
	metadb_handle_list* handles_ptr = nullptr;
	GET_PTR(handles, handles_ptr)

	const metadb_handle_list_ref handles_ref = *handles_ptr;
	metadb_handle_list result;
	t_size walk1 = 0;
	t_size walk2 = 0;
	const t_size last1 = m_handles.get_count();
	const t_size last2 = handles_ptr->get_count();

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
	metadb_handle_list* handles_ptr = nullptr;
	GET_PTR(handles, handles_ptr)

	m_handles.add_items(*handles_ptr);
	m_handles.sort_by_pointer_remove_duplicates();
	return S_OK;
}

STDMETHODIMP MetadbHandleList::OrderByFormat(__interface ITitleFormat* script, int direction)
{
	titleformat_object* obj = nullptr;
	GET_PTR(script, obj)

	m_handles.sort_by_format(obj, nullptr, direction);
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
	const t_size count = m_handles.get_count();
	t_size i;

	std::vector<helpers::custom_sort_data> data(count);

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
	stats::hash_set hashes;
	stats::get_hashes(m_handles, hashes);

	pfc::list_t<metadb_index_hash> to_refresh;
	for (const auto& hash : hashes)
	{
		to_refresh += hash;
	}
	stats::theAPI()->dispatch_refresh(jsp_guids::metadb_index, to_refresh);
	return S_OK;
}

STDMETHODIMP MetadbHandleList::Remove(IMetadbHandle* handle)
{
	metadb_handle* ptr = nullptr;
	GET_PTR(handle, ptr)

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

	auto cb = fb2k::service_new<helpers::embed_thread>(helpers::embed_thread::actions::remove, album_art_data_ptr(), m_handles, art_id);
	threaded_process::get()->run_modeless(cb, helpers::embed_thread::flags, core_api::get_main_window(), "Removing images...");
	return S_OK;
}

STDMETHODIMP MetadbHandleList::RemoveAttachedImages()
{
	if (m_handles.get_count() == 0) return E_POINTER;

	auto cb = fb2k::service_new<helpers::embed_thread>(helpers::embed_thread::actions::remove_all, album_art_data_ptr(), m_handles, 0);
	threaded_process::get()->run_modeless(cb, helpers::embed_thread::flags, core_api::get_main_window(), "Removing images...");
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
	const t_size count = m_handles.get_count();
	if (count == 0) return E_POINTER;

	json j = json::parse(string_utf8_from_wide(str).get_ptr(), nullptr, false);

	if (j.is_array() && j.size() == count)
	{
		std::vector<file_info_impl> info(count);

		for (t_size i = 0; i < count; ++i)
		{
			if (!j[i].is_object() || j[i].size() == 0) return E_INVALIDARG;

			info[i] = m_handles[i]->get_info_ref()->info();

			for (auto& [name, value] : j[i].items())
			{
				if (name.empty()) return E_INVALIDARG;

				info[i].meta_remove_field(name.c_str());

				for (auto& v : helpers::js_file_info_filter::get_values(value))
				{
					info[i].meta_add(name.c_str(), v.c_str());
				}
			}
		}

		metadb_io_v2::get()->update_info_async_simple(
			m_handles,
			pfc::ptr_list_const_array_t<const file_info, file_info_impl*>(info.data(), info.size()),
			core_api::get_main_window(),
			metadb_io_v2::op_flag_delay_ui,
			nullptr
		);
		return S_OK;
	}
	else if (j.is_object() && j.size() > 0)
	{
		std::vector<helpers::js_file_info_filter::tag> tags;

		for (auto& [name, value] : j.items())
		{
			if (name.empty()) return E_INVALIDARG;

			helpers::js_file_info_filter::tag t;
			t.first = name;
			t.second = helpers::js_file_info_filter::get_values(value);
			tags.emplace_back(t);
		}

		metadb_io_v2::get()->update_info_async(
			m_handles,
			fb2k::service_new<helpers::js_file_info_filter>(tags),
			core_api::get_main_window(),
			metadb_io_v2::op_flag_delay_ui,
			nullptr
		);
		return S_OK;
	}
	else
	{
		return E_INVALIDARG;
	}
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
		metadb_handle* ptr = nullptr;
		GET_PTR(handle, ptr)

		m_handles.replace_item(index, ptr);
		return S_OK;
	}
	return E_INVALIDARG;
}

PlaybackQueueItem::PlaybackQueueItem() {}

PlaybackQueueItem::PlaybackQueueItem(const t_playback_queue_item& playbackQueueItem)
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

Profiler::Profiler(const char* p_name) : m_name(p_name)
{
	m_timer.start();
}

Profiler::~Profiler() {}

STDMETHODIMP Profiler::Print()
{
	FB2K_console_formatter() << "FbProfiler (" << m_name << "): " << static_cast<int>(m_timer.query() * 1000) << " ms";
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

	*p = static_cast<int>(m_timer.query() * 1000);
	return S_OK;
}

ThemeManager::ThemeManager(HWND hwnd, BSTR classlist) : m_theme(nullptr), m_partid(0), m_stateid(0)
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
		m_theme = nullptr;
	}
}

STDMETHODIMP ThemeManager::DrawThemeBackground(IGdiGraphics* gr, int x, int y, int w, int h, int clip_x, int clip_y, int clip_w, int clip_h)
{
	if (!m_theme) return E_POINTER;

	Gdiplus::Graphics* graphics = nullptr;
	GET_PTR(gr, graphics)

	RECT rc = { x, y, x + w, y + h };
	RECT rc_clip = { clip_x, clip_y, clip_x + clip_w, clip_y + clip_h };
	LPCRECT rc_pclip = &rc_clip;
	HDC dc = graphics->GetHDC();

	if (clip_x == 0 && clip_y == 0 && clip_w == 0 && clip_h == 0)
	{
		rc_pclip = nullptr;
	}

	const HRESULT hr = ::DrawThemeBackground(m_theme, dc, m_partid, m_stateid, &rc, rc_pclip);

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

TitleFormat::TitleFormat(BSTR pattern)
{
	titleformat_compiler::get()->compile_safe(m_obj, string_utf8_from_wide(pattern));
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

STDMETHODIMP TitleFormat::Eval(BSTR* p)
{
	if (m_obj.is_empty() || !p) return E_POINTER;

	pfc::string8_fast str;
	playback_control::get()->playback_format_title(nullptr, str, m_obj, nullptr, playback_control::display_level_all);
	*p = TO_BSTR(str);
	return S_OK;
}

STDMETHODIMP TitleFormat::EvalWithMetadb(IMetadbHandle* handle, BSTR* p)
{
	if (m_obj.is_empty() || !p) return E_POINTER;

	metadb_handle* ptr = nullptr;
	GET_PTR(handle, ptr)

	pfc::string8_fast str;
	ptr->format_title(nullptr, str, m_obj, nullptr);
	*p = TO_BSTR(str);
	return S_OK;
}

STDMETHODIMP TitleFormat::EvalWithMetadbs(IMetadbHandleList* handles, VARIANT* p)
{
	if (m_obj.is_empty() || !p) return E_POINTER;

	metadb_handle_list* handles_ptr = nullptr;
	GET_PTR(handles, handles_ptr)

	const LONG count = handles_ptr->get_count();
	helpers::com_array helper;
	if (!helper.create(count)) return E_OUTOFMEMORY;

	for (LONG i = 0; i < count; ++i)
	{
		pfc::string8_fast str;
		handles_ptr->get_item(i)->format_title(nullptr, str, m_obj, nullptr);
		_variant_t var;
		var.vt = VT_BSTR;
		var.bstrVal = TO_BSTR(str);
		if (!helper.put_item(i, var)) return E_OUTOFMEMORY;
	}
	p->vt = VT_ARRAY | VT_VARIANT;
	p->parray = helper.get_ptr();
	return S_OK;
}

Tooltip::Tooltip(HWND p_wndparent, const panel_tooltip_param_ptr& p_param_ptr) : m_wndparent(p_wndparent), m_panel_tooltip_param_ptr(p_param_ptr), m_tip_buffer(SysAllocString(PFC_WIDESTRING(JSP_NAME)))
{
	m_wndtooltip = CreateWindowEx(
		WS_EX_TOPMOST,
		TOOLTIPS_CLASS,
		nullptr,
		WS_POPUP | TTS_ALWAYSTIP | TTS_NOPREFIX,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		p_wndparent,
		nullptr,
		core_api::get_my_instance(),
		nullptr);

	SetWindowPos(m_wndtooltip, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	memset(&m_ti, 0, sizeof(m_ti));
	m_ti.cbSize = sizeof(m_ti);
	m_ti.uFlags = TTF_IDISHWND | TTF_SUBCLASS | TTF_TRANSPARENT;
	m_ti.hinst = core_api::get_my_instance();
	m_ti.hwnd = p_wndparent;
	m_ti.uId = reinterpret_cast<UINT_PTR>(p_wndparent);
	m_ti.lpszText = m_tip_buffer;

	const HFONT hFont = CreateFont(
		-static_cast<int>(m_panel_tooltip_param_ptr->font_size),
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

	SendMessage(m_wndtooltip, TTM_ADDTOOL, 0, reinterpret_cast<LPARAM>(&m_ti));
	SendMessage(m_wndtooltip, TTM_ACTIVATE, FALSE, 0);
	SendMessage(m_wndtooltip, WM_SETFONT, reinterpret_cast<WPARAM>(hFont), MAKELPARAM(FALSE, 0));

	m_panel_tooltip_param_ptr->tooltip_hwnd = m_wndtooltip;
}

Tooltip::~Tooltip() {}

void Tooltip::FinalRelease()
{
	if (m_wndtooltip && IsWindow(m_wndtooltip))
	{
		DestroyWindow(m_wndtooltip);
		m_wndtooltip = nullptr;
	}

	if (m_tip_buffer)
	{
		SysFreeString(m_tip_buffer);
		m_tip_buffer = nullptr;
	}
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

STDMETHODIMP Tooltip::SetMaxWidth(int width)
{
	SendMessage(m_wndtooltip, TTM_SETMAXTIPWIDTH, 0, width);
	return S_OK;
}

STDMETHODIMP Tooltip::TrackPosition(int x, int y)
{
	POINT pt = { x, y };
	ClientToScreen(m_wndparent, &pt);
	SendMessage(m_wndtooltip, TTM_TRACKPOSITION, 0, MAKELONG(pt.x, pt.y));
	return S_OK;
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
	SendMessage(m_wndtooltip, TTM_SETTOOLINFO, 0, reinterpret_cast<LPARAM>(&m_ti));
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

	SendMessage(m_wndtooltip, TTM_TRACKACTIVATE, activate != VARIANT_FALSE ? TRUE : FALSE, reinterpret_cast<LPARAM>(&m_ti));
	return S_OK;
}

UiSelectionHolder::UiSelectionHolder(const ui_selection_holder::ptr& holder) : m_holder(holder) {}
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
	metadb_handle_list* handles_ptr = nullptr;
	GET_PTR(handles, handles_ptr)

	m_holder->set_selection(*handles_ptr);
	return S_OK;
}
