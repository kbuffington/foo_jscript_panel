#pragma once
#include "helpers.h"
#include "properties.h"
#include "script_interface.h"

struct t_script_info
{
	t_script_info() : id(0) {}

	std::vector<std::pair<std::string, std::string>> expand_table =
	{
		{ "%fb2k_profile_path%", helpers::get_profile_path().get_ptr() },
		{ "%fb2k_component_path%", helpers::get_fb2k_component_path().get_ptr() },
		{ "%fb2k_path%", helpers::get_fb2k_path().get_ptr() },
	};

	pfc::string8_fast build_info_string() const
	{
		pfc::string8_fast ret;
		ret << JSP_NAME_VERSION " (";
		if (name.length())
		{
			ret << name.c_str();
		}
		else
		{
			ret << "id:" << id;
		}
		if (version.length())
		{
			ret << " v" << version.c_str();
		}
		if (author.length())
		{
			ret << " by " << author.c_str();
		}
		ret << ")";
		return ret;
	}

	std::string expand_import(t_size idx)
	{
		std::string tmp = imports[idx];
		for (const auto& [key, value] : expand_table)
		{
			if (tmp.find(key) == 0)
			{
				tmp = value + tmp.substr(key.length());
				break;
			}
		}
		return tmp;
	}

	void clear()
	{
		imports.clear();
		name = "";
		version = "";
		author = "";
		id = 0;
	}

	slist imports;
	std::string name;
	std::string version;
	std::string author;
	t_size id;
};

class host_comm
{
protected:
	host_comm();
	virtual ~host_comm();

	void create_context();
	void delete_context();
	void load_config(stream_reader* reader, t_size size, abort_callback& abort);
	void redraw();
	void refresh_background(LPRECT lprcUpdate);
	void reset_config();
	void save_config(stream_writer* writer, abort_callback& abort) const;

	HBITMAP m_gr_bmp;
	HBITMAP m_gr_bmp_bk;
	HDC m_hdc;
	HWND m_hwnd;
	bool m_paint_pending;
	bool m_suppress_drawing;
	int m_height;
	int m_width;
	panel_tooltip_param_ptr m_panel_tooltip_param_ptr;
	t_size m_instance_type;
	ui_selection_holder::ptr m_selection_holder;

public:
	enum
	{
		KInstanceTypeCUI = 0,
		KInstanceTypeDUI,
	};

	enum t_edge_style : char
	{
		NO_EDGE = 0,
		SUNKEN_EDGE,
		GREY_EDGE,
	};

	static pfc::string8_fast get_default_script_code();
	static pfc::string8_fast get_default_script_engine_str();

	DWORD get_edge_style() const;
	HDC get_hdc();
	HWND get_hwnd();
	int get_height();
	int get_width();
	panel_tooltip_param_ptr& panel_tooltip();
	t_size get_instance_type();
	virtual DWORD get_colour_ui(t_size type) = 0;
	virtual IGdiFont* get_font_ui(t_size type) = 0;
	virtual bool show_configure_popup(HWND parent) = 0;
	virtual void notify_size_limit_changed() = 0;
	virtual void show_property_popup(HWND parent) = 0;
	virtual void unload_script() = 0;
	virtual void update_script() = 0;
	void repaint();
	void repaint_rect(int x, int y, int w, int h);

	POINT m_max_size;
	POINT m_min_size;
	WINDOWPLACEMENT m_wndpl;
	bool m_dragdrop;
	bool m_grabfocus;
	bool m_pseudo_transparent;
	pfc::string8_fast m_script_code;
	pfc::string8_fast m_script_engine_str;
	properties m_config_prop;
	t_edge_style m_edge_style;
	t_script_info m_script_info;
};
