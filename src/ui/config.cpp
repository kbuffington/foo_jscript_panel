#include "stdafx.h"
#include "config.h"

static const std::vector<config::simple_key_val> init_table =
{
	{"style.default", "font:Courier New,size:10"},
	{"style.comment", "fore:#008000"},
	{"style.keyword", "bold,fore:#0000ff"},
	{"style.indentifier", "$(style.default)"},
	{"style.string", "fore:#ff0000"},
	{"style.number", "fore:#ff0000"},
	{"style.operator", "$(style.default)"},
	{"style.linenumber", "font:Courier New,size:8,fore:#2b91af"},
	{"style.bracelight", "bold,fore:#000000,back:#ffee62"},
	{"style.bracebad", "bold,fore:#ff0000"},
	{"style.selection.fore", ""},
	{"style.selection.back", ""},
	{"style.selection.alpha", "256"},
	{"style.caret.fore", ""},
	{"style.caret.width", "1"},
	{"style.caret.line.back", ""},
	{"style.caret.line.back.alpha", "256"}
};

config g_config(jsp::guids::config);

config::config(const GUID& p_guid) : cfg_var(p_guid)
{
	init_data();
}

void config::get_data_raw(stream_writer* p_stream, abort_callback& p_abort)
{
	try
	{
		p_stream->write_lendian_t(m_data.size(), p_abort);
		for (const auto& [key, value] : m_data)
		{
			p_stream->write_string(key, p_abort);
			p_stream->write_string(value, p_abort);
		}
		p_stream->write_object(&m_conf_wndpl, sizeof(WINDOWPLACEMENT), p_abort);
		p_stream->write_object(&m_property_wndpl, sizeof(WINDOWPLACEMENT), p_abort);
	}
	catch (...) {}
}

void config::import(const char* content)
{
	str_vec lines = helpers::split_string(content, "\r\n");
	simple_map data_map;
	for (const auto& line : lines)
	{
		if (line.empty() || line.at(0) == '#' || line.length() <= 3) continue;
		const size_t pos = line.find('=');
		if (pos == std::string::npos) continue;
		std::string key = line.substr(0, pos);
		std::string value = line.substr(pos + 1);
		if (key.empty()) continue;
		data_map[key.c_str()].set_string(value.c_str());
	}
	merge_data(data_map);
}

void config::init_data()
{
	m_data = init_table;
}

void config::load_preset(size_t idx)
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

void config::set_data_raw(stream_reader* p_stream, size_t p_sizehint, abort_callback& p_abort)
{
	simple_map data_map;
	pfc::string8_fast key, value;
	size_t count;

	try
	{
		p_stream->read_lendian_t(count, p_abort);
		for (size_t i = 0; i < count; ++i)
		{
			p_stream->read_string(key, p_abort);
			p_stream->read_string(value, p_abort);
			data_map.emplace(key, value);
		}

		try
		{
			// this can fail silently on first run upgrading from old version which doesn't have it set yet
			p_stream->read_object(&m_conf_wndpl, sizeof(WINDOWPLACEMENT), p_abort);
			p_stream->read_object(&m_property_wndpl, sizeof(WINDOWPLACEMENT), p_abort);
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
