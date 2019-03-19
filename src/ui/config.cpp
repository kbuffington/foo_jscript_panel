#include "stdafx.h"
#include "config.h"

bool prop_kv_config::get_config_item(const char* p_key, VARIANT& p_out)
{
	_variant_t val;

	if (m_map.query(p_key, val))
	{
		if (g_is_allowed_type(val.vt) && SUCCEEDED(VariantCopy(&p_out, &val)))
		{
			return true;
		}
		else
		{
			m_map.remove(p_key);
		}
	}

	return false;
}

bool prop_kv_config::g_is_allowed_type(VARTYPE p_vt)
{
	switch (p_vt)
	{
	case VT_UI1:
	case VT_I1:
	case VT_I2:
	case VT_UI2:
	case VT_BOOL:
	case VT_I4:
	case VT_UI4:
	case VT_R4:
	case VT_INT:
	case VT_UINT:
	case VT_I8:
	case VT_UI8:
	case VT_R8:
	case VT_CY:
	case VT_DATE:
	case VT_BSTR:
		return true;
	}

	return false;
}

void prop_kv_config::g_load(t_map& data, stream_reader* reader, abort_callback& abort) throw()
{
	t_size count;

	data.remove_all();

	try
	{
		reader->read_lendian_t(count, abort);

		for (t_size i = 0; i < count; ++i)
		{
			pfc::string8_fast key;
			_variant_t val;
			VARTYPE vt;
			int cbRead = 0;

			reader->read_string(key, abort);
			reader->read_lendian_t(vt, abort);

			switch (vt)
			{
			case VT_UI1:
			case VT_I1:
				cbRead = sizeof(BYTE);
				break;

			case VT_I2:
			case VT_UI2:
			case VT_BOOL:
				cbRead = sizeof(short);
				break;

			case VT_I4:
			case VT_UI4:
			case VT_R4:
			case VT_INT:
			case VT_UINT:
				cbRead = sizeof(LONG);
				break;

			case VT_I8:
			case VT_UI8:
				cbRead = sizeof(LONGLONG);
				break;

			case VT_R8:
			case VT_CY:
			case VT_DATE:
				cbRead = sizeof(double);
				break;
			}

			val.vt = vt;

			if (cbRead != 0)
			{
				reader->read(&val.bVal, cbRead, abort);
			}
			else
			{
				pfc::string8_fast str;
				reader->read_string(str, abort);
				val.bstrVal = SysAllocString(string_wide_from_utf8_fast(str));
			}

			data[key] = val;
		}
	}
	catch (...) {}
}

void prop_kv_config::g_save(const t_map& data, stream_writer* writer, abort_callback& abort) throw()
{
	try
	{
		writer->write_lendian_t(data.get_count(), abort);

		for (t_map::const_iterator iter = data.first(); iter.is_valid(); ++iter)
		{
			writer->write_string(iter->m_key, abort);
			writer->write_lendian_t(iter->m_value.vt, abort);
			int cbWrite = 0;

			switch (iter->m_value.vt)
			{
			case VT_UI1:
			case VT_I1:
				cbWrite = sizeof(BYTE);
				break;

			case VT_I2:
			case VT_UI2:
			case VT_BOOL:
				cbWrite = sizeof(short);
				break;

			case VT_I4:
			case VT_UI4:
			case VT_R4:
			case VT_INT:
			case VT_UINT:
				cbWrite = sizeof(LONG);
				break;

			case VT_I8:
			case VT_UI8:
				cbWrite = sizeof(LONGLONG);
				break;

			case VT_R8:
			case VT_CY:
			case VT_DATE:
				cbWrite = sizeof(double);
				break;
			}

			if (cbWrite != 0)
			{
				writer->write(&iter->m_value.bVal, cbWrite, abort);
			}
			else if (iter->m_value.vt == VT_BSTR)
			{
				string_utf8_from_wide conv = iter->m_value.bstrVal;
				writer->write_string(conv, abort);
			}
		}
	}
	catch (...) {}
}

void prop_kv_config::load(stream_reader* reader, abort_callback& abort) throw()
{
	g_load(m_map, reader, abort);
}

void prop_kv_config::save(stream_writer* writer, abort_callback& abort) const throw()
{
	g_save(m_map, writer, abort);
}

void prop_kv_config::set_config_item(const char* p_key, const VARIANT& p_val)
{
	if (g_is_allowed_type(p_val.vt))
	{
		m_map[p_key] = p_val;
	}
	else
	{
		m_map.remove(p_key);
	}
}

panel_vars::panel_vars()
{
	reset_config();
}

pfc::string8_fast panel_vars::get_default_script_code()
{
	puResource pures = uLoadResource(core_api::get_my_instance(), uMAKEINTRESOURCE(IDR_SCRIPT), "TEXT");
	pfc::string8_fast content(static_cast<const char*>(pures->GetPointer()), pures->GetSize());
	return content;
}

void panel_vars::load_config(stream_reader* reader, t_size size, abort_callback& abort)
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
			reader->read_object_t(m_grab_focus, abort);
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

void panel_vars::reset_config()
{
	m_script_engine_str = "Chakra";
	m_script_code = get_default_script_code();
	m_pseudo_transparent = false;
	m_wndpl.length = 0;
	m_grab_focus = true;
	m_edge_style = NO_EDGE;
}

void panel_vars::save_config(stream_writer* writer, abort_callback& abort) const
{
	const t_size ver = 123;

	try
	{
		writer->write_object_t(ver, abort);
		writer->write_object_t(false, abort); // HACK: write this in place of "delay load"
		writer->write_object_t(pfc::guid_null, abort); // HACK: write this in place of "GUID"
		writer->write_object(&m_edge_style, sizeof(m_edge_style), abort);
		m_config_prop.save(writer, abort);
		writer->write_object_t(false, abort); // HACK: write this in place of "disable before"
		writer->write_object_t(m_grab_focus, abort);
		writer->write_object(&m_wndpl, sizeof(m_wndpl), abort);
		writer->write_string(m_script_engine_str, abort);
		writer->write_string(m_script_code, abort);
		writer->write_object_t(m_pseudo_transparent, abort);
	}
	catch (...) {}
}
