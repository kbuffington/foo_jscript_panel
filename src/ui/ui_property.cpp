#include "stdafx.h"
#include "ui_property.h"

CDialogProperty::CDialogProperty(panel_window* p_parent) : m_parent(p_parent)
{
	m_caption << JSP_NAME " Properties (id:" << m_parent->m_script_info.id << ")";
}

CDialogProperty::~CDialogProperty() {}

BOOL CDialogProperty::OnInitDialog(HWND hwndFocus, LPARAM lParam)
{
	// Set caption text
	uSetWindowText(m_hWnd, m_caption);

	DlgResize_Init();

	m_properties.CreateInDialog(*this, IDC_LIST_PROPERTIES);
	LoadProperties();
	return TRUE;
}

void CDialogProperty::Apply()
{
	m_dup_prop_map.remove_all();

	for (const MyCList::data_t& d : m_properties.m_data)
	{
		variant_t source, dest;

		if (d.is_bool)
		{
			source.vt = VT_BOOL;
			source.boolVal = TO_VARIANT_BOOL(d.bool_value);
			m_dup_prop_map[d.key] = source;
		}
		else
		{
			source.vt = VT_BSTR;
			source.bstrVal = TO_BSTR(d.value);

			if (SUCCEEDED(VariantChangeType(&dest, &source, 0, VT_R8)))
			{
				m_dup_prop_map[d.key] = dest;
			}
			else
			{
				m_dup_prop_map[d.key] = source;
			}
		}
	}

	m_parent->m_config_prop.m_map = m_dup_prop_map;
	m_parent->update_script();
	LoadProperties();
}

void CDialogProperty::LoadProperties(bool reload)
{
	m_properties.m_data.clear();

	if (reload)
	{
		m_dup_prop_map = m_parent->m_config_prop.m_map;
	}

	for (properties::t_map::const_iterator iter = m_dup_prop_map.first(); iter.is_valid(); ++iter)
	{
		MyCList::data_t d;
		d.key = iter->m_key;
		d.is_bool = false;

		const _variant_t& v = iter->m_value;
		_variant_t var;

		switch (v.vt)
		{
		case VT_BOOL:
			d.is_bool = true;
			d.bool_value = !!v.boolVal;
			break;

		case VT_I1:
		case VT_UI1:
		case VT_I2:
		case VT_UI2:
		case VT_I4:
		case VT_UI4:
		case VT_I8:
		case VT_UI8:
			var.ChangeType(VT_I4, &v);
			d.value = PFC_string_formatter() << var.lVal;
			break;

		case VT_BSTR:
		default:
			var.ChangeType(VT_BSTR, &v);
			d.value = PFC_string_formatter() << string_utf8_from_wide(var.bstrVal);
			break;
		}

		m_properties.m_data.emplace_back(d);
	}

	m_properties.ReloadData();
}

void CDialogProperty::OnCloseCmd(UINT uNotifyCode, int nID, HWND wndCtl)
{
	switch (nID)
	{
	case IDOK:
		Apply();
		break;

	case IDC_APPLY:
		Apply();
		return;
	}
	EndDialog(nID);
}

void CDialogProperty::OnExportBnClicked(UINT uNotifyCode, int nID, HWND wndCtl)
{
	pfc::string8_fast path;

	if (uGetOpenFileName(m_hWnd, "Property files|*.wsp", 0, "wsp", "Save as", nullptr, path, TRUE))
	{
		file_ptr io;

		try
		{
			filesystem::g_open_write_new(io, path, fb2k::noAbort);
			properties::g_save(m_dup_prop_map, io.get_ptr(), fb2k::noAbort);
		}
		catch (...) {}
	}
}

void CDialogProperty::OnImportBnClicked(UINT uNotifyCode, int nID, HWND wndCtl)
{
	pfc::string8_fast path;

	if (uGetOpenFileName(m_hWnd, "Property files|*.wsp|All files|*.*", 0, "wsp", "Import from", nullptr, path, FALSE))
	{
		file_ptr io;

		try
		{
			filesystem::g_open_read(io, path, fb2k::noAbort);
			properties::g_load(m_dup_prop_map, io.get_ptr(), fb2k::noAbort);
			LoadProperties(false);
		}
		catch (...) {}
	}
}
