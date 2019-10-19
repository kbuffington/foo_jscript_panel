#pragma once

struct simple_key_val
{
	pfc::string_simple key, val;
};

using simple_map = pfc::map_t<pfc::string_simple, pfc::string_simple, pfc::comparator_stricmp_ascii>;

class scintilla_properties : public cfg_var
{
public:
	scintilla_properties(const GUID& p_guid);

	void get_data_raw(stream_writer* p_stream, abort_callback& p_abort) override;
	void import(const char* content);
	void init_data();
	void load_preset(t_size idx);
	void merge_data(const simple_map& data_map);
	void set_data_raw(stream_reader* p_stream, t_size p_sizehint, abort_callback& p_abort) override;

	std::vector<simple_key_val> m_data;
};

extern scintilla_properties g_scintilla_properties;
