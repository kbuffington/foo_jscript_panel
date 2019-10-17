#include "stdafx.h"
#include "host_comm.h"

host_comm::host_comm()
	: m_hwnd(nullptr)
	, m_width(0)
	, m_height(0)
	, m_dragdrop(false)
	, m_grabfocus(false)
	, m_pseudo_transparent(false)
	, m_edge_style(NO_EDGE)
	, m_instance_type(KInstanceTypeCUI)
	, m_script_info()
	, m_panel_tooltip_param_ptr(new panel_tooltip_param)
	, m_max_size { INT_MAX, INT_MAX }
	, m_min_size { 0, 0 }
	, m_wndpl {}
{
	reset_config();
}

host_comm::~host_comm() {}

DWORD host_comm::get_edge_style() const
{
	switch (m_edge_style)
	{
	case SUNKEN_EDGE:
		return WS_EX_CLIENTEDGE;

	case GREY_EDGE:
		return WS_EX_STATICEDGE;

	default:
		return 0;
	}
}

HWND host_comm::get_hwnd()
{
	return m_hwnd;
}

int host_comm::get_height()
{
	return m_height;
}

int host_comm::get_width()
{
	return m_width;
}

panel_tooltip_param_ptr& host_comm::panel_tooltip()
{
	return m_panel_tooltip_param_ptr;
}

pfc::string8_fast host_comm::get_default_script_code()
{
	puResource pures = uLoadResource(core_api::get_my_instance(), uMAKEINTRESOURCE(IDR_SCRIPT), "TEXT");
	pfc::string8_fast content(static_cast<const char*>(pures->GetPointer()), pures->GetSize());
	return content;
}

pfc::string8_fast host_comm::get_default_script_engine_str()
{
	return helpers::supports_chakra() ? "Chakra" : "JScript";
}

t_size host_comm::get_instance_type()
{
	return m_instance_type;
}

void host_comm::load_config(stream_reader* reader, t_size size, abort_callback& abort)
{
	reset_config();

	if (size > sizeof(t_size))
	{
		t_size ver = 0;
		try
		{
			reader->read_object_t(ver, abort);
			reader->skip_object(sizeof(false), abort); // HACK: skip over "delay load"
			reader->skip_object(sizeof(GUID), abort); // HACK: skip over "GUID"
			reader->read_object(&m_edge_style, sizeof(m_edge_style), abort);
			m_config_prop.load(reader, abort);
			reader->skip_object(sizeof(false), abort); // HACK: skip over "disable before"
			reader->skip_object(sizeof(false), abort); // HACK: skip over "grab focus"
			reader->read_object(&m_wndpl, sizeof(m_wndpl), abort);
			reader->read_string(m_script_engine_str, abort);
			reader->read_string(m_script_code, abort);
			reader->read_object_t(m_pseudo_transparent, abort);
		}
		catch (...)
		{
			reset_config();
			FB2K_console_formatter() << JSP_NAME_VERSION ": Configuration has been corrupted. All settings have been reset.";
		}
	}
}

void host_comm::reset_config()
{
	m_script_engine_str = get_default_script_engine_str();
	m_script_code = get_default_script_code();
	m_pseudo_transparent = false;
	m_wndpl.length = 0;
	m_edge_style = NO_EDGE;
}

void host_comm::save_config(stream_writer* writer, abort_callback& abort) const
{
	constexpr t_size ver = 123;

	try
	{
		writer->write_object_t(ver, abort);
		writer->write_object_t(false, abort); // HACK: write this in place of "delay load"
		writer->write_object_t(pfc::guid_null, abort); // HACK: write this in place of "GUID"
		writer->write_object(&m_edge_style, sizeof(m_edge_style), abort);
		m_config_prop.save(writer, abort);
		writer->write_object_t(false, abort); // HACK: write this in place of "disable before"
		writer->write_object_t(true, abort); // HACK: write this in place of "grab focus"
		writer->write_object(&m_wndpl, sizeof(m_wndpl), abort);
		writer->write_string(m_script_engine_str, abort);
		writer->write_string(m_script_code, abort);
		writer->write_object_t(m_pseudo_transparent, abort);
	}
	catch (...) {}
}
