#pragma once
#include "panel_properties.h"
#include "script_info.h"

class host_comm
{
protected:
	host_comm();
	virtual ~host_comm();

	enum class instance_type
	{
		cui,
		dui,
	};

	void load_config(stream_reader* reader, t_size size, abort_callback& abort);
	void reset_config();
	void save_config(stream_writer* writer, abort_callback& abort) const;

	HWND m_hwnd;
	instance_type m_instance_type;
	int m_height;
	int m_width;
	panel_tooltip_param_ptr m_panel_tooltip_param_ptr;
	ui_selection_holder::ptr m_selection_holder;

public:
	virtual DWORD get_colour_ui(t_size type) = 0;
	virtual IGdiFont* get_font_ui(t_size type) = 0;
	virtual bool show_configure_popup(HWND parent) = 0;
	virtual void notify_size_limit_changed() = 0;
	virtual void show_property_popup(HWND parent) = 0;
	virtual void unload_script() = 0;
	virtual void update_script() = 0;
	virtual void repaint() = 0;
	virtual void repaint_rect(int x, int y, int w, int h) = 0;

public:
	enum class edge_style : char
	{
		none,
		sunken,
		grey,
	};

	static pfc::string8_fast get_default_script_code();
	static pfc::string8_fast get_default_script_engine_str();

	DWORD get_edge_style() const;
	HWND get_hwnd();
	bool is_dui();
	int get_height();
	int get_width();
	panel_tooltip_param_ptr& panel_tooltip();

	POINT m_max_size;
	POINT m_min_size;
	WINDOWPLACEMENT m_wndpl;
	bool m_dragdrop;
	bool m_grabfocus;
	bool m_pseudo_transparent;
	edge_style m_edge_style;
	pfc::string8_fast m_script_code;
	pfc::string8_fast m_script_engine_str;
	panel_properties m_panel_properties;
	script_info m_script_info;
};
