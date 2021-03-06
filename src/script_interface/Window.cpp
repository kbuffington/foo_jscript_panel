#include "stdafx.h"
#include "Window.h"
#include "host_timer_dispatcher.h"
#include "panel_manager.h"
#include "panel_window.h"

Window::Window(panel_window* p) : m_host(p) {}
Window::~Window() {}

STDMETHODIMP Window::ClearInterval(UINT intervalID)
{
	host_timer_dispatcher::instance().kill_timer(intervalID);
	return S_OK;
}

STDMETHODIMP Window::ClearTimeout(UINT timeoutID)
{
	host_timer_dispatcher::instance().kill_timer(timeoutID);
	return S_OK;
}

STDMETHODIMP Window::CreatePopupMenu(IMenuObj** pp)
{
	if (!pp) return E_POINTER;

	*pp = new com_object_impl_t<MenuObj>(m_host->m_hwnd);
	return S_OK;
}

STDMETHODIMP Window::CreateThemeManager(BSTR classid, IThemeManager** pp)
{
	if (!pp) return E_POINTER;

	IThemeManager* ptheme = nullptr;

	try
	{
		ptheme = new com_object_impl_t<ThemeManager>(m_host->m_hwnd, classid);
	}
	catch (...)
	{
		if (ptheme)
		{
			ptheme->Dispose();
			delete ptheme;
			ptheme = nullptr;
		}
	}

	*pp = ptheme;
	return S_OK;
}

STDMETHODIMP Window::CreateTooltip(BSTR name, float pxSize, int style, ITooltip** pp)
{
	if (!pp) return E_POINTER;

	const auto& tooltip_param = m_host->panel_tooltip();
	tooltip_param->font_name = name;
	tooltip_param->font_size = pxSize;
	tooltip_param->font_style = style;
	*pp = new com_object_impl_t<Tooltip>(m_host->m_hwnd, tooltip_param);
	return S_OK;
}

STDMETHODIMP Window::GetColourCUI(UINT type, int* p)
{
	if (!p) return E_POINTER;
	if (m_host->is_dui()) return E_NOTIMPL;

	*p = helpers::convert_colorref_to_argb(m_host->get_colour_ui(type));
	return S_OK;
}

STDMETHODIMP Window::GetColourDUI(UINT type, int* p)
{
	if (!p) return E_POINTER;
	if (!m_host->is_dui()) return E_NOTIMPL;

	*p = helpers::convert_colorref_to_argb(m_host->get_colour_ui(type));
	return S_OK;
}

STDMETHODIMP Window::GetFontCUI(UINT type, IGdiFont** pp)
{
	if (!pp) return E_POINTER;
	if (m_host->is_dui()) return E_NOTIMPL;

	*pp = m_host->get_font_ui(type);
	return S_OK;
}

STDMETHODIMP Window::GetFontDUI(UINT type, IGdiFont** pp)
{
	if (!pp) return E_POINTER;
	if (!m_host->is_dui()) return E_NOTIMPL;

	*pp = m_host->get_font_ui(type);
	return S_OK;
}

STDMETHODIMP Window::GetProperty(BSTR name, VARIANT defaultval, VARIANT* p)
{
	if (!p) return E_POINTER;

	HRESULT hr;
	_variant_t var;
	auto uname = string_utf8_from_wide(name);

	if (m_host->m_panel_config.properties.get_property(uname, var))
	{
		hr = VariantCopy(p, &var);
	}
	else
	{
		m_host->m_panel_config.properties.set_property(uname, defaultval);
		hr = VariantCopy(p, &defaultval);
	}

	if (FAILED(hr))
	{
		p = nullptr;
	}

	return S_OK;
}

STDMETHODIMP Window::NotifyOthers(BSTR name, VARIANT info)
{
	if (info.vt & VT_BYREF) return E_INVALIDARG;

	_variant_t var;
	if (FAILED(VariantCopy(&var, &info))) return E_INVALIDARG;
	auto d = var.Detach();

	auto data = new callback_data<_bstr_t, _variant_t>(name, 0);
	data->m_item2.Attach(d);
	panel_manager::instance().notify_others(m_host->m_hwnd, data);
	return S_OK;
}

STDMETHODIMP Window::Reload(VARIANT_BOOL clear_properties)
{
	if (clear_properties != VARIANT_FALSE)
	{
		m_host->m_panel_config.properties.m_map.clear();
	}
	m_host->update_script();
	return S_OK;
}

STDMETHODIMP Window::Repaint()
{
	m_host->repaint();
	return S_OK;
}

STDMETHODIMP Window::RepaintRect(int x, int y, int w, int h)
{
	m_host->repaint_rect(x, y, w, h);
	return S_OK;
}

STDMETHODIMP Window::SetCursor(UINT id)
{
	::SetCursor(LoadCursor(nullptr, MAKEINTRESOURCE(id)));
	return S_OK;
}

STDMETHODIMP Window::SetInterval(IDispatch* func, int delay, UINT* p)
{
	if (!p) return E_POINTER;

	*p = host_timer_dispatcher::instance().set_interval(m_host->m_hwnd, delay, func);
	return S_OK;
}

STDMETHODIMP Window::SetProperty(BSTR name, VARIANT val)
{
	m_host->m_panel_config.properties.set_property(string_utf8_from_wide(name), val);
	return S_OK;
}

STDMETHODIMP Window::SetTimeout(IDispatch* func, int delay, UINT* p)
{
	if (!p) return E_POINTER;

	*p = host_timer_dispatcher::instance().set_timeout(m_host->m_hwnd, delay, func);
	return S_OK;
}

STDMETHODIMP Window::ShowConfigure()
{
	fb2k::inMainThread([&]()
		{
			m_host->show_configure_popup(m_host->m_hwnd);
		});
	return S_OK;
}

STDMETHODIMP Window::ShowProperties()
{
	fb2k::inMainThread([&]()
		{
			m_host->show_property_popup(m_host->m_hwnd);
		});
	return S_OK;
}

STDMETHODIMP Window::get_Height(int* p)
{
	if (!p) return E_POINTER;

	*p = m_host->m_size.height;
	return S_OK;
}

STDMETHODIMP Window::get_ID(UINT* p)
{
	if (!p) return E_POINTER;

	*p = m_host->m_script_host->m_info.id;
	return S_OK;
}

STDMETHODIMP Window::get_InstanceType(UINT* p)
{
	if (!p) return E_POINTER;

	*p = m_host->is_dui() ? 1 : 0;
	return S_OK;
}

STDMETHODIMP Window::get_IsTransparent(VARIANT_BOOL* p)
{
	if (!p) return E_POINTER;

	*p = TO_VARIANT_BOOL(m_host->is_transparent());
	return S_OK;
}

STDMETHODIMP Window::get_IsVisible(VARIANT_BOOL* p)
{
	if (!p) return E_POINTER;

	*p = TO_VARIANT_BOOL(IsWindowVisible(m_host->m_hwnd));
	return S_OK;
}

STDMETHODIMP Window::get_MaxHeight(UINT* p)
{
	if (!p) return E_POINTER;

	*p = m_host->m_size.max.y;
	return S_OK;
}

STDMETHODIMP Window::get_MaxWidth(UINT* p)
{
	if (!p) return E_POINTER;

	*p = m_host->m_size.max.x;
	return S_OK;
}

STDMETHODIMP Window::get_MinHeight(UINT* p)
{
	if (!p) return E_POINTER;

	*p = m_host->m_size.min.y;
	return S_OK;
}

STDMETHODIMP Window::get_MinWidth(UINT* p)
{
	if (!p) return E_POINTER;

	*p = m_host->m_size.min.x;
	return S_OK;
}

STDMETHODIMP Window::get_Name(BSTR* p)
{
	if (!p) return E_POINTER;

	pfc::string8_fast name = m_host->m_script_host->m_info.name.c_str();
	if (name.is_empty())
	{
		name << "id:" << m_host->m_script_host->m_info.id;
	}
	*p = TO_BSTR(name);
	return S_OK;
}

STDMETHODIMP Window::get_Width(int* p)
{
	if (!p) return E_POINTER;

	*p = m_host->m_size.width;
	return S_OK;
}

STDMETHODIMP Window::put_MaxHeight(UINT height)
{
	m_host->m_size.max.y = height;
	m_host->notify_size_limit_changed();
	return S_OK;
}

STDMETHODIMP Window::put_MaxWidth(UINT width)
{
	m_host->m_size.max.x = width;
	m_host->notify_size_limit_changed();
	return S_OK;
}

STDMETHODIMP Window::put_MinHeight(UINT height)
{
	m_host->m_size.min.y = height;
	m_host->notify_size_limit_changed();
	return S_OK;
}

STDMETHODIMP Window::put_MinWidth(UINT width)
{
	m_host->m_size.min.x = width;
	m_host->notify_size_limit_changed();
	return S_OK;
}
