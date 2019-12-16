#pragma once
#include "helpers.h"
#include "panel_config.h"
#include "panel_properties.h"
#include "script_info.h"

class panel_base
{
protected:
	panel_base();
	virtual ~panel_base();

	enum class instance_type
	{
		cui,
		dui,
	};

	struct size
	{
		CPoint min, max;
		int width, height;
	};

	void load_config(stream_reader* reader, size_t size, abort_callback& abort);
	void save_config(stream_writer* writer, abort_callback& abort) const;

	CWindow m_hwnd = nullptr;
	instance_type m_instance_type = instance_type::cui;
	panel_tooltip_param_ptr m_panel_tooltip_param_ptr;
	ui_selection_holder::ptr m_selection_holder;

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
	CWindow get_hwnd();
	bool is_dui();
	panel_tooltip_param_ptr& panel_tooltip();

	bool m_dragdrop = false;
	bool m_grabfocus = false;
	panel_config m_panel_config;
	panel_properties m_panel_properties;
	script_info m_script_info{};
	size m_size{};
};
