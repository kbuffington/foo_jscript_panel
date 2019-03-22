#pragma once

class properties
{
public:
	using t_map = pfc::map_t<pfc::string_simple, _variant_t, pfc::comparator_stricmp_ascii>;

	bool get_config_item(const char* p_key, VARIANT& p_out);
	static int g_get_sizeof(VARTYPE p_vt);
	static void g_load(t_map& data, stream_reader* reader, abort_callback& abort) throw();
	static void g_save(const t_map& data, stream_writer* writer, abort_callback& abort) throw();
	void load(stream_reader* reader, abort_callback& abort) throw();
	void save(stream_writer* writer, abort_callback& abort) const throw();
	void set_config_item(const char* p_key, const VARIANT& p_val);

	t_map m_map;
};
