#pragma once
#include "helpers.h"
#include "properties.h"
#include "script_interface.h"

struct t_script_info
{
	t_script_info() {}

	struct
	{
		std::string which;
		std::string path;
	} expand_table[3] = {
		{ "%fb2k_path%", helpers::get_fb2k_path().get_ptr() },
		{ "%fb2k_component_path%", helpers::get_fb2k_component_path().get_ptr() },
		{ "%fb2k_profile_path%", helpers::get_profile_path().get_ptr() },
	};

	pfc::string8_fast build_info_string() const
	{
		pfc::string8_fast ret;
		ret << JSP_NAME_VERSION " (";
		if (name.get_length())
		{
			ret << name;
		}
		else
		{
			ret << "id:" << id;
		}
		if (version.get_length())
		{
			ret << " v" << version;
		}
		if (author.get_length())
		{
			ret << " by " << author;
		}
		ret << ")";
		return ret;
	}

	pfc::string8_fast expand_import(t_size idx)
	{
		std::string tmp = imports[idx];
		for (const auto& i : expand_table)
		{
			if (tmp.find(i.which) == 0)
			{
				tmp = i.path + tmp.substr(i.which.length());
				break;
			}
		}
		return tmp.c_str();
	}

	void clear()
	{
		imports.remove_all();
		dragdrop = false;
		name = "";
		version = "";
		author = "";
		id = 0;
	}

	bool dragdrop;
	pfc::string_list_impl imports;
	pfc::string8_fast name;
	pfc::string8_fast version;
	pfc::string8_fast author;
	t_size id;
};

class host_comm
{
protected:
	host_comm();
	virtual ~host_comm();

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

	DWORD get_edge_style() const;
	HDC get_hdc();
	HWND get_hwnd();
	int get_height();
	int get_width();
	panel_tooltip_param_ptr& panel_tooltip();
	static pfc::string8_fast get_default_script_code();
	t_size get_instance_type();
	virtual DWORD get_colour_ui(t_size type) = 0;
	virtual HFONT get_font_ui(t_size type) = 0;
	virtual void notify_size_limit_changed() = 0;
	void load_config(stream_reader* reader, t_size size, abort_callback& abort);
	void redraw();
	void refresh_background(LPRECT lprcUpdate = nullptr);
	void repaint(bool force = false);
	void repaint_rect(int x, int y, int w, int h, bool force = false);
	void reset_config();
	void save_config(stream_writer* writer, abort_callback& abort) const;

	POINT m_max_size;
	POINT m_min_size;
	WINDOWPLACEMENT m_wndpl;
	bool m_grab_focus;
	bool m_pseudo_transparent;
	pfc::string8_fast m_script_code;
	pfc::string8_fast m_script_engine_str;
	properties m_config_prop;
	t_edge_style m_edge_style;
	t_script_info m_script_info;
};
