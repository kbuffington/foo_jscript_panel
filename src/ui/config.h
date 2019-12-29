#pragma once
#include "helpers.h"

class config : public cfg_var
{
public:
	config(const GUID& guid);

	struct simple_key_val
	{
		pfc::string_simple key, value;
	};

	using simple_map = std::map<pfc::string_simple, pfc::string_simple, helpers::stricmp_ascii>;

	void get_data_raw(stream_writer* writer, abort_callback& abort) override;
	void import(pfc::stringp content);
	void init_data();
	void load_preset(size_t idx);
	void merge_data(const simple_map& data_map);
	void set_data_raw(stream_reader* reader, size_t sizehint, abort_callback& abort) override;

	WINDOWPLACEMENT m_conf_wndpl{}, m_property_wndpl{};
	std::vector<simple_key_val> m_data;
};

extern config g_config;
