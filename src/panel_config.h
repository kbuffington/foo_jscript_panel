#pragma once
#include "panel_properties.h"

struct panel_config
{
	enum class edge_style : char
	{
		none,
		sunken,
		grey,
	};

	DWORD get_edge_style()
	{
		switch (style)
		{
		case edge_style::sunken: return WS_EX_CLIENTEDGE;
		case edge_style::grey: return WS_EX_STATICEDGE;
		default: return 0;
		}
	}

	void set(stream_reader* reader, size_t size, abort_callback& abort)
	{
		reset();

		if (size > sizeof(size_t))
		{
			size_t ver = 0;
			try
			{
				reader->read_object_t(ver, abort);

				if (ver < 2400)
				{
					set_legacy(reader, abort);
				}
				else
				{
					properties.set(reader, abort);
					reader->read_string(code, abort);
					reader->read_object_t(transparent, abort);
					reader->read_object(&style, sizeof(char), abort);
				}
			}
			catch (...)
			{
				reset();
				FB2K_console_formatter() << jsp::component_name << ": Configuration has been corrupted. All settings have been reset.";
			}
		}
	}

	void set_legacy(stream_reader* reader, abort_callback& abort)
	{
		pfc::string8_fast tmp;
		reader->skip_object(sizeof(bool), abort); // HACK: skip over "delay load"
		reader->skip_object(sizeof(GUID), abort); // HACK: skip over "GUID"
		reader->read_object(&style, sizeof(char), abort);
		properties.set(reader, abort);
		reader->skip_object(sizeof(bool), abort); // HACK: skip over "disable before"
		reader->skip_object(sizeof(bool), abort); // HACK: skip over "grab focus"
		reader->skip_object(sizeof(WINDOWPLACEMENT), abort); // HACK: skip over window placement
		reader->read_string(tmp, abort); // HACK: skip over "engine"
		reader->read_string(code, abort);
		reader->read_object_t(transparent, abort);
	}

	void reset()
	{
		code = helpers::get_resource_text(IDR_SCRIPT);
		style = edge_style::none;
		transparent = false;
	}

	void get(stream_writer* writer, abort_callback& abort) const
	{
		try
		{
			writer->write_object_t(jsp::version, abort);
			properties.get(writer, abort);
			writer->write_string(code, abort);
			writer->write_object_t(transparent, abort);
			writer->write_object(&style, sizeof(char), abort);
		}
		catch (...) {}
	}

	bool transparent = false;
	edge_style style = edge_style::none;
	panel_properties properties;
	pfc::string8_fast code;
};
