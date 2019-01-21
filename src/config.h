#pragma once

enum t_edge_style : char
{
	NO_EDGE = 0,
	SUNKEN_EDGE,
	GREY_EDGE,
};

inline DWORD edge_style_from_config(t_edge_style edge_style)
{
	switch (edge_style)
	{
	case SUNKEN_EDGE:
		return WS_EX_CLIENTEDGE;

	case GREY_EDGE:
		return WS_EX_STATICEDGE;

	default:
		return 0;
	}
}

class prop_kv_config
{
public:
	using t_map = pfc::map_t<pfc::string_simple, _variant_t, pfc::comparator_stricmp_ascii>;

	bool get_config_item(const char* p_key, VARIANT& p_out);
	static bool g_is_allowed_type(VARTYPE p_vt);
	static void g_load(t_map& data, stream_reader* reader, abort_callback& abort) throw();
	static void g_save(const t_map& data, stream_writer* writer, abort_callback& abort) throw();
	void load(stream_reader* reader, abort_callback& abort) throw();
	void save(stream_writer* writer, abort_callback& abort) const throw();
	void set_config_item(const char* p_key, const VARIANT& p_val);

	t_map m_map;
};

class js_panel_vars
{
public:
	js_panel_vars();

	WINDOWPLACEMENT& get_windowplacement();
	bool& get_grab_focus();
	bool& get_pseudo_transparent();
	const bool& get_pseudo_transparent() const;
	const t_edge_style& get_edge_style() const;
	pfc::string_base& get_script_code();
	pfc::string_base& get_script_engine();
	prop_kv_config& get_config_prop();
	static void get_default_script_code(pfc::string_base& out);
	t_edge_style& get_edge_style();
	void load_config(stream_reader* reader, t_size size, abort_callback& abort);
	void reset_config();
	void save_config(stream_writer* writer, abort_callback& abort) const;

private:
	WINDOWPLACEMENT m_wndpl;
	bool m_grab_focus;
	bool m_pseudo_transparent;
	pfc::string8 m_script_engine_str;
	pfc::string8 m_script_code;
	prop_kv_config m_config_prop;
	t_edge_style m_edge_style;
};
