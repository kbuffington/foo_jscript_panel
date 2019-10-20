#include "stdafx.h"
#include "panel_properties.h"

bool panel_properties::get_config_item(const char* p_key, VARIANT& p_out)
{
	if (m_map.count(p_key))
	{
		_variant_t val = m_map.at(p_key);
		if (g_get_sizeof(val.vt) != 0 && SUCCEEDED(VariantCopy(&p_out, &val)))
		{
			return true;
		}
		else
		{
			m_map.erase(p_key);
		}
	}

	return false;
}

int panel_properties::g_get_sizeof(VARTYPE p_vt)
{
	switch (p_vt)
	{
	case VT_BSTR:
		return -1;
	case VT_UI1:
	case VT_I1:
		return sizeof(BYTE);
	case VT_I2:
	case VT_UI2:
	case VT_BOOL:
		return sizeof(short);
	case VT_I4:
	case VT_UI4:
	case VT_R4:
	case VT_INT:
	case VT_UINT:
		return sizeof(LONG);
	case VT_I8:
	case VT_UI8:
		return sizeof(LONGLONG);
	case VT_R8:
	case VT_CY:
	case VT_DATE:
		return sizeof(double);
	default:
		return 0;
	}
}

void panel_properties::g_load(property_map& data, stream_reader* reader, abort_callback& abort) throw()
{
	t_size count;
	data.clear();

	try
	{
		reader->read_lendian_t(count, abort);

		for (t_size i = 0; i < count; ++i)
		{
			pfc::string8_fast key;
			VARTYPE vt;
			reader->read_string(key, abort);
			reader->read_lendian_t(vt, abort);

			const int cbRead = g_get_sizeof(vt);

			_variant_t val;
			val.vt = vt;

			if (cbRead > 0)
			{
				reader->read(&val.bVal, cbRead, abort);
			}
			else
			{
				pfc::string8_fast str;
				reader->read_string(str, abort);
				val.bstrVal = TO_BSTR(str);
			}

			data.emplace(key, val);
		}
	}
	catch (...) {}
}

void panel_properties::g_save(const property_map& data, stream_writer* writer, abort_callback& abort) throw()
{
	try
	{
		writer->write_lendian_t(data.size(), abort);

		for (const auto& [key, value] : data)
		{
			writer->write_string(key, abort);
			writer->write_lendian_t(value.vt, abort);
			const int cbWrite = g_get_sizeof(value.vt);

			if (cbWrite > 0)
			{
				writer->write(&value.bVal, cbWrite, abort);
			}
			else if (cbWrite == -1)
			{
				writer->write_string(string_utf8_from_wide(value.bstrVal), abort);
			}
		}
	}
	catch (...) {}
}

void panel_properties::load(stream_reader* reader, abort_callback& abort) throw()
{
	g_load(m_map, reader, abort);
}

void panel_properties::save(stream_writer* writer, abort_callback& abort) const throw()
{
	g_save(m_map, writer, abort);
}

void panel_properties::set_config_item(const char* p_key, const VARIANT& p_val)
{
	if (g_get_sizeof(p_val.vt) != 0)
	{
		m_map[p_key] = p_val;
	}
	else
	{
		m_map.erase(p_key);
	}
}
