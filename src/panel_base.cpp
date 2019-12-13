#include "stdafx.h"
#include "panel_base.h"

panel_base::panel_base() : m_panel_tooltip_param_ptr(new panel_tooltip_param)
{
	reset_config();
	m_size.min = { 0, 0 };
	m_size.max = { INT_MAX, INT_MAX };
	m_size.width = 0;
	m_size.height = 0;
}

panel_base::~panel_base() {}

DWORD panel_base::get_edge_style() const
{
	switch (m_edge_style)
	{
	case edge_style::sunken:
		return WS_EX_CLIENTEDGE;

	case edge_style::grey:
		return WS_EX_STATICEDGE;

	default:
		return 0;
	}
}

HWND panel_base::get_hwnd()
{
	return m_hwnd;
}

bool panel_base::is_dui()
{
	return m_instance_type == instance_type::dui;
}

panel_tooltip_param_ptr& panel_base::panel_tooltip()
{
	return m_panel_tooltip_param_ptr;
}

pfc::string8_fast panel_base::g_get_default_script_code()
{
	return helpers::get_resource_text(IDR_SCRIPT);
}

pfc::string8_fast panel_base::g_get_default_script_engine_str()
{
	return helpers::supports_chakra() ? "Chakra" : "JScript";
}

void panel_base::load_config(stream_reader* reader, size_t size, abort_callback& abort)
{
	reset_config();

	if (size > sizeof(size_t))
	{
		size_t ver = 0;
		try
		{
			reader->read_object_t(ver, abort);
			reader->skip_object(sizeof(false), abort); // HACK: skip over "delay load"
			reader->skip_object(sizeof(GUID), abort); // HACK: skip over "GUID"
			reader->read_object(&m_edge_style, sizeof(m_edge_style), abort);
			m_panel_properties.load(reader, abort);
			reader->skip_object(sizeof(false), abort); // HACK: skip over "disable before"
			reader->skip_object(sizeof(false), abort); // HACK: skip over "grab focus"
			reader->skip_object(sizeof(WINDOWPLACEMENT), abort); // HACK: skip over window placement
			reader->read_string(m_script_engine_str, abort);
			reader->read_string(m_script_code, abort);
			reader->read_object_t(m_pseudo_transparent, abort);
		}
		catch (...)
		{
			reset_config();
			FB2K_console_formatter() << jsp::component_name << ": Configuration has been corrupted. All settings have been reset.";
		}
	}
}

void panel_base::reset_config()
{
	m_script_engine_str = g_get_default_script_engine_str();
	m_script_code = g_get_default_script_code();
	m_pseudo_transparent = false;
	m_edge_style = edge_style::none;
}

void panel_base::save_config(stream_writer* writer, abort_callback& abort) const
{
	constexpr size_t ver = 123;

	try
	{
		WINDOWPLACEMENT wndpl;
		writer->write_object_t(ver, abort);
		writer->write_object_t(false, abort); // HACK: write this in place of "delay load"
		writer->write_object_t(pfc::guid_null, abort); // HACK: write this in place of "GUID"
		writer->write_object(&m_edge_style, sizeof(m_edge_style), abort);
		m_panel_properties.save(writer, abort);
		writer->write_object_t(false, abort); // HACK: write this in place of "disable before"
		writer->write_object_t(true, abort); // HACK: write this in place of "grab focus"
		writer->write_object(&wndpl, sizeof(WINDOWPLACEMENT), abort); // HACK: write this in place of window placement
		writer->write_string(m_script_engine_str, abort);
		writer->write_string(m_script_code, abort);
		writer->write_object_t(m_pseudo_transparent, abort);
	}
	catch (...) {}
}
