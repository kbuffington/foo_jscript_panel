#pragma once
#include "helpers.h"

class panel_properties
{
public:
	using property_map = std::map<pfc::string_simple, _variant_t, helpers::stricmp_ascii>;

	static int g_sizeof(VARTYPE vt);
	static void g_get(const property_map& data, stream_writer* writer, abort_callback& abort) throw();
	static void g_set(property_map& data, stream_reader* reader, abort_callback& abort) throw();

	bool get_property(const char* key, VARIANT& out);
	void get(stream_writer* writer, abort_callback& abort) const throw();
	void set(stream_reader* reader, abort_callback& abort) throw();
	void set_property(const char* key, const VARIANT& val);

	property_map m_map;
};
