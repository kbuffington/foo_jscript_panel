#pragma once
#include "helpers.h"
#include "panel_config.h"
#include "script_info.h"

class panel_base
{
public:
	virtual COLORREF get_colour_ui(size_t type) = 0;
	virtual IGdiFont* get_font_ui(size_t type) = 0;
	virtual void notify_size_limit_changed() = 0;
	virtual void repaint() = 0;
	virtual void repaint_rect(int x, int y, int w, int h) = 0;
	virtual void show_configure_popup(HWND parent) = 0;
	virtual void show_property_popup(HWND parent) = 0;
	virtual void unload_script() = 0;
	virtual void update_script() = 0;

public:
	enum class instance_type
	{
		cui,
		dui,
	};

	struct size
	{
		void reset_min_max()
		{
			min.SetPoint(0, 0);
			max.SetPoint(INT_MAX, INT_MAX);
		}

		CPoint min, max;
		int height = 0;
		int width = 0;
	};

	bool is_dui()
	{
		return m_instance_type == instance_type::dui;
	}

	bool is_transparent()
	{
		return m_supports_transparency && m_panel_config.transparent;
	}

	panel_tooltip_param_ptr& panel_tooltip()
	{
		return m_panel_tooltip_param_ptr;
	}

	CWindow m_hwnd = nullptr;
	bool m_dragdrop = false;
	bool m_grabfocus = false;
	bool m_supports_transparency = false;
	panel_config m_panel_config;
	script_info m_script_info{};
	size m_size{};

protected:
	panel_base() : m_panel_tooltip_param_ptr(new panel_tooltip_param)
	{
		m_panel_config.reset();
		m_size.reset_min_max();
	}

	virtual ~panel_base() {}

	instance_type m_instance_type = instance_type::cui;
	panel_tooltip_param_ptr m_panel_tooltip_param_ptr;
};
