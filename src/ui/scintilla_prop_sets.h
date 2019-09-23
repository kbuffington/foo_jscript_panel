#pragma once

struct t_sci_prop_set
{
	pfc::string_simple key, val;
};

using t_str_map = pfc::map_t<pfc::string_simple, pfc::string_simple, pfc::comparator_stricmp_ascii>;

class cfg_sci_prop_sets : public cfg_var
{
public:
	cfg_sci_prop_sets(const GUID& p_guid);

	void get_data_raw(stream_writer* p_stream, abort_callback& p_abort) override;
	void import(const char* content);
	void init_data();
	void load_preset(t_size idx);
	void merge_data(const t_str_map& data_map);
	void set_data_raw(stream_reader* p_stream, t_size p_sizehint, abort_callback& p_abort) override;

	std::vector<t_sci_prop_set> m_data;
};

extern cfg_sci_prop_sets g_sci_prop_sets;
