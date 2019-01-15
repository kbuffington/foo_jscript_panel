#pragma once
#include "config.h"
#include "helpers.h"
#include "script_interface_impl.h"

struct t_script_info
{
	t_script_info(GUID& guid_ref) : m_guid_ref(guid_ref) {}

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
			ret << "{" << pfc::print_guid(m_guid_ref) << "}";
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
		for (t_size i = 0; i < _countof(expand_table); ++i)
		{
			if (tmp.find(expand_table[i].which) == 0)
			{
				tmp = expand_table[i].path + tmp.substr(expand_table[i].which.length());
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
	}

	bool dragdrop;
	pfc::string_list_impl imports;
	pfc::string8_fast name;
	pfc::string8_fast version;
	pfc::string8_fast author;

private:
	GUID& m_guid_ref;
};

class host_comm : public js_panel_vars
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
	int m_instance_type;
	int m_width;
	panel_tooltip_param_ptr m_panel_tooltip_param_ptr;
	ui_selection_holder::ptr m_selection_holder;

public:
	enum
	{
		KInstanceTypeCUI = 0,
		KInstanceTypeDUI,
	};

	HDC GetHDC();
	HWND GetHWND();
	int GetHeight();
	int GetWidth();
	panel_tooltip_param_ptr& PanelTooltipParam();
	t_size GetInstanceType();
	virtual DWORD GetColourUI(t_size type) = 0;
	virtual HFONT GetFontUI(t_size type) = 0;
	void Redraw();
	void RefreshBackground(LPRECT lprcUpdate = NULL);
	void Repaint(bool force = false);
	void RepaintRect(LONG x, LONG y, LONG w, LONG h, bool force = false);

	POINT m_max_size;
	POINT m_min_size;
	t_script_info m_script_info;
};
