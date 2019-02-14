#pragma once

enum t_sci_editor_style_flag
{
	ESF_NONE = 0,
	ESF_FONT = 1 << 0,
	ESF_SIZE = 1 << 1,
	ESF_FORE = 1 << 2,
	ESF_BACK = 1 << 3,
	ESF_BOLD = 1 << 4,
	ESF_ITALICS = 1 << 5,
	ESF_UNDERLINED = 1 << 6,
	ESF_CASEFORCE = 1 << 7,
};

struct t_sci_editor_style
{
	t_sci_editor_style()
	{
		flags = 0;
	}

	DWORD back, fore;
	bool bold, italics, underlined;
	int case_force;
	pfc::string8_fast font;
	t_size flags, size;
};

struct t_sci_prop_set
{
	pfc::string_simple key, defaultval, val;
};

struct t_prop_set_init_table
{
	const char* key;
	const char* defaultval;
};

using t_sci_prop_set_list = pfc::list_t<t_sci_prop_set>;
using t_str_to_str_map = pfc::map_t<pfc::string_simple, pfc::string_simple, pfc::comparator_stricmp_ascii>;

class cfg_sci_prop_sets : public cfg_var
{
public:
	cfg_sci_prop_sets(const GUID& p_guid, const t_prop_set_init_table* p_default);

	void export_to_file(const char* filename);
	void get_data_raw(stream_writer* p_stream, abort_callback& p_abort) override;
	void import(const char* content);
	void reset();
	void set_data_raw(stream_reader* p_stream, t_size p_sizehint, abort_callback& p_abort) override;

	t_sci_prop_set_list m_data;

private:
	void init_data(const t_prop_set_init_table* p_default);
	void merge_data(const t_str_to_str_map& data_map);
};

extern cfg_sci_prop_sets g_sci_prop_sets;
