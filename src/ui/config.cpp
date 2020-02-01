#include "stdafx.h"
#include "config.h"

static const std::vector<config::simple_key_val> init_table =
{
	{"style.default", "font:Courier New,size:10"},
	{"style.comment", "fore:#008000"},
	{"style.keyword", "bold,fore:#0000FF"},
	{"style.indentifier", "$(style.default)"},
	{"style.string", "fore:#FF0000"},
	{"style.number", "fore:#FF0000"},
	{"style.operator", "$(style.default)"},
	{"style.linenumber", "font:Courier New,size:8,fore:#2B91AF"},
	{"style.bracelight", "bold,fore:#000000,back:#FFEE62"},
	{"style.bracebad", "bold,fore:#FF0000"},
	{"style.selection.fore", ""},
	{"style.selection.back", ""},
	{"style.selection.alpha", "256"},
	{"style.caret.fore", ""},
	{"style.caret.width", "1"},
	{"style.caret.line.back", ""},
	{"style.caret.line.back.alpha", "256"}
};

config g_config(jsp::guids::config);

config::config(const GUID& guid) : cfg_var(guid)
{
	init_data();
}

void config::get_data_raw(stream_writer* writer, abort_callback& abort)
{
	try
	{
		writer->write_lendian_t(m_data.size(), abort);
		for (const auto& [key, value] : m_data)
		{
			writer->write_string(key, abort);
			writer->write_string(value, abort);
		}
		writer->write_object(&m_conf_wndpl, sizeof(WINDOWPLACEMENT), abort);
		writer->write_object(&m_property_wndpl, sizeof(WINDOWPLACEMENT), abort);
	}
	catch (...) {}
}

void config::import(pfc::stringp content)
{
	str_vec lines = helpers::split_string(content.get_ptr(), "\r\n");
	simple_map data_map;
	for (const auto& line : lines)
	{
		if (line.empty() || line.at(0) == '#' || line.length() <= 3) continue;
		const size_t pos = line.find('=');
		if (pos == 0 || pos == std::string::npos) continue;
		std::string key = line.substr(0, pos);
		std::string value = line.substr(pos + 1);
		data_map[key.c_str()].set_string(value.c_str());
	}
	merge_data(data_map);
}

void config::init_data()
{
	m_data = init_table;
}

void config::load_preset(int idx)
{
	import(helpers::get_resource_text(idx));
}

void config::merge_data(const simple_map& data_map)
{
	for (auto& [key, value] : m_data)
	{
		if (data_map.count(key))
		{
			value = data_map.at(key);
		}
	}
}

void config::set_data_raw(stream_reader* reader, size_t sizehint, abort_callback& abort)
{
	simple_map data_map;
	pfc::string8_fast key, value;
	size_t count;

	try
	{
		reader->read_lendian_t(count, abort);
		for (size_t i = 0; i < count; ++i)
		{
			reader->read_string(key, abort);
			reader->read_string(value, abort);
			data_map.emplace(key, value);
		}

		try
		{
			// this can fail silently on first run upgrading from old version which doesn't have it set yet
			reader->read_object(&m_conf_wndpl, sizeof(WINDOWPLACEMENT), abort);
			reader->read_object(&m_property_wndpl, sizeof(WINDOWPLACEMENT), abort);
		}
		catch (...) {}
	}
	catch (...)
	{
		init_data();
		return;
	}
	merge_data(data_map);
}
