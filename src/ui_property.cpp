#include "stdafx.h"
#include "ui_property.h"

CDialogProperty::CDialogProperty(js_panel_window* p_parent) : m_parent(p_parent) {}
CDialogProperty::~CDialogProperty() {}

BOOL CDialogProperty::OnInitDialog(HWND hwndFocus, LPARAM lParam)
{
	DlgResize_Init();

	// Subclassing
	m_properties.SubclassWindow(GetDlgItem(IDC_LIST_PROPERTIES));
	m_properties.ModifyStyle(0, LBS_SORT | LBS_HASSTRINGS);
	m_properties.SetExtendedListStyle(PLS_EX_SORTED | PLS_EX_XPLOOK);

	LoadProperties();

	return FALSE;
}

LRESULT CDialogProperty::OnClearBnClicked(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{
	m_dup_prop_map.remove_all();
	m_properties.ResetContent();

	return 0;
}

LRESULT CDialogProperty::OnCloseCmd(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{
	switch (wID)
	{
	case IDOK:
		Apply();
		break;

	case IDC_APPLY:
		Apply();
		return 0;
	}

	EndDialog(wID);
	return 0;
}

LRESULT CDialogProperty::OnDelBnClicked(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{
	int idx = m_properties.GetCurSel();

	if (idx >= 0)
	{
		HPROPERTY hproperty = m_properties.GetProperty(idx);
		string_utf8_from_wide uname = hproperty->GetName();

		m_properties.DeleteItem(hproperty);
		m_dup_prop_map.remove(uname);
	}

	return 0;
}

LRESULT CDialogProperty::OnExportBnClicked(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{
	pfc::string8_fast path;

	if (uGetOpenFileName(m_hWnd, "Property files|*.wsp", 0, "wsp", "Save as", NULL, path, TRUE))
	{
		file_ptr io;
		abort_callback_dummy abort;

		try
		{
			filesystem::g_open_write_new(io, path, abort);
			prop_kv_config::g_save(m_dup_prop_map, io.get_ptr(), abort);
		}
		catch (...) {}
	}
	return 0;
}

LRESULT CDialogProperty::OnImportBnClicked(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{
	pfc::string8_fast path;

	if (uGetOpenFileName(m_hWnd, "Property files|*.wsp|All files|*.*", 0, "wsp", "Import from", NULL, path, FALSE))
	{
		file_ptr io;
		abort_callback_dummy abort;

		try
		{
			filesystem::g_open_read(io, path, abort);
			prop_kv_config::g_load(m_dup_prop_map, io.get_ptr(), abort);
			LoadProperties(false);
		}
		catch (...) {}
	}
	return 0;
}

LRESULT CDialogProperty::OnPinItemChanged(LPNMHDR pnmh)
{
	LPNMPROPERTYITEM pnpi = (LPNMPROPERTYITEM)pnmh;

	string_utf8_from_wide uname = pnpi->prop->GetName();

	if (m_dup_prop_map.have_item(uname))
	{
		_variant_t& val = m_dup_prop_map[uname];
		_variant_t var;

		if (pnpi->prop->GetValue(&var))
		{
			val.ChangeType(val.vt, &var);
		}
	}

	return 0;
}

void CDialogProperty::Apply()
{
	// Copy back
	m_parent->get_config_prop().m_map = m_dup_prop_map;
	m_parent->update_script();
	LoadProperties();
}

void CDialogProperty::LoadProperties(bool reload)
{
	m_properties.ResetContent();

	if (reload)
	{
		m_dup_prop_map = m_parent->get_config_prop().m_map;
	}

	for (prop_kv_config::t_map::const_iterator iter = m_dup_prop_map.first(); iter.is_valid(); ++iter)
	{
		string_wide_from_utf8_fast wname = iter->m_key;
		HPROPERTY hProp = NULL;
		const _variant_t& v = iter->m_value;
		_variant_t var;

		switch (v.vt)
		{
		case VT_BOOL:
			hProp = PropCreateSimple(wname, v.boolVal ? true : false);
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
			hProp = PropCreateSimple(wname, var.lVal);
			break;

		case VT_BSTR:
		default:
			var.ChangeType(VT_BSTR, &v);
			hProp = PropCreateSimple(wname, var.bstrVal);
			break;
		}

		m_properties.AddItem(hProp);
	}
}
