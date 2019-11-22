#pragma once
#include "helpers.h"

class config : public cfg_var
{
public:
	config(const GUID& p_guid);

	struct simple_key_val
	{
		pfc::string_simple key, value;
	};

	using simple_map = std::map<pfc::string_simple, pfc::string_simple, helpers::stricmp_ascii>;

	void get_data_raw(stream_writer* p_stream, abort_callback& p_abort) override;
	void import(const char* content);
	void init_data();
	void load_preset(t_size idx);
	void merge_data(const simple_map& data_map);
	void set_data_raw(stream_reader* p_stream, t_size p_sizehint, abort_callback& p_abort) override;

	WINDOWPLACEMENT m_conf_wndpl, m_property_wndpl;
	std::vector<simple_key_val> m_data;
};

extern config g_config;
