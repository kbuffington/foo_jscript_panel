#include "stdafx.h"
#include "ui_property.h"

static const CDialogResizeHelper::Param resize_data[] =
{
	{ IDC_LIST_PROPERTIES, 0, 0, 1, 1 },
	{ IDC_BTN_IMPORT, 0, 1, 0, 1 },
	{ IDC_BTN_EXPORT, 0, 1, 0, 1 },
	{ IDC_BTN_CLEAR, 0, 1, 0, 1 },
	{ IDOK, 1, 1, 1, 1 },
	{ IDCANCEL, 1, 1, 1, 1 },
	{ IDC_BTN_APPLY, 1, 1, 1, 1 },
};

static const CRect resize_min_max(620, 381, 0, 0);

CDialogProperty::CDialogProperty(panel_window* p_parent) : m_parent(p_parent), m_resizer(resize_data, resize_min_max)
{
	m_caption << JSP_NAME " Properties (id:" << m_parent->m_script_info.id << ")";
}

BOOL CDialogProperty::OnInitDialog(HWND, LPARAM)
{
	// Set caption text
	uSetWindowText(m_hWnd, m_caption);

	m_clear_btn = GetDlgItem(IDC_BTN_CLEAR);
	m_export_btn = GetDlgItem(IDC_BTN_EXPORT);

	m_properties.CreateInDialog(*this, IDC_LIST_PROPERTIES);
	LoadProperties();
	return TRUE;
}

LRESULT CDialogProperty::OnPropertiesCleared(UINT, WPARAM, LPARAM, BOOL&)
{
	m_clear_btn.EnableWindow(false);
	m_export_btn.EnableWindow(false);
	return 0;
}

void CDialogProperty::Apply()
{
	m_dup_prop_map.clear();

	for (const MyCList::data_t& d : m_properties.m_data)
	{
		variant_t source, dest;

		if (d.is_bool)
		{
			source.vt = VT_BOOL;
			source.boolVal = TO_VARIANT_BOOL(d.bool_value);
			m_dup_prop_map.emplace(d.key, source);
		}
		else
		{
			source.vt = VT_BSTR;
			source.bstrVal = TO_BSTR(d.value);

			if (d.is_string)
			{
				m_dup_prop_map.emplace(d.key, source);
			}
			else
			{
				if (SUCCEEDED(VariantChangeType(&dest, &source, 0, VT_R8)))
				{
					m_dup_prop_map.emplace(d.key, dest);
				}
				else
				{
					m_dup_prop_map.emplace(d.key, source);
				}
			}
		}
	}

	m_parent->m_panel_properties.m_map = m_dup_prop_map;
	m_parent->update_script();
	LoadProperties();
}

void CDialogProperty::LoadProperties(bool reload)
{
	m_properties.m_data.clear();

	if (reload)
	{
		m_dup_prop_map = m_parent->m_panel_properties.m_map;
	}

	for (const auto& [key, value] : m_dup_prop_map)
	{
		MyCList::data_t d;
		d.key = key;
		d.is_bool = false;
		d.is_string = false;

		const _variant_t& v = value;
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
			d.value = std::to_string(var.lVal).c_str();
			break;

		case VT_BSTR:
			d.is_string = true;
			var.ChangeType(VT_BSTR, &v);
			d.value = string_utf8_from_wide(var.bstrVal);
			break;

		default:
			var.ChangeType(VT_BSTR, &v);
			d.value = string_utf8_from_wide(var.bstrVal);
			break;
		}

		m_properties.m_data.emplace_back(d);
	}

	m_properties.ReloadData();

	m_clear_btn.EnableWindow(m_properties.m_data.size());
	m_export_btn.EnableWindow(m_properties.m_data.size());
}

void CDialogProperty::OnClearBnClicked(UINT, int, HWND)
{
	m_properties.SelectAll();
	m_properties.RequestRemoveSelection();
}

void CDialogProperty::OnCloseCmd(UINT, int nID, HWND)
{
	switch (nID)
	{
	case IDOK:
		Apply();
		break;

	case IDC_BTN_APPLY:
		Apply();
		return;
	}
	EndDialog(nID);
}

void CDialogProperty::OnExportBnClicked(UINT, int, HWND)
{
	pfc::string8_fast path;

	if (uGetOpenFileName(m_hWnd, "Property files|*.wsp", 0, "wsp", "Save as", nullptr, path, TRUE))
	{
		try
		{
			file_ptr io;
			filesystem::g_open_write_new(io, path, fb2k::noAbort);
			panel_properties::g_save(m_dup_prop_map, io.get_ptr(), fb2k::noAbort);
		}
		catch (...) {}
	}
}

void CDialogProperty::OnImportBnClicked(UINT, int, HWND)
{
	pfc::string8_fast path;

	if (uGetOpenFileName(m_hWnd, "Property files|*.wsp|All files|*.*", 0, "wsp", "Import from", nullptr, path, FALSE))
	{
		try
		{
			file_ptr io;
			filesystem::g_open_read(io, path, fb2k::noAbort);
			panel_properties::g_load(m_dup_prop_map, io.get_ptr(), fb2k::noAbort);
			LoadProperties(false);
		}
		catch (...) {}
	}
}
