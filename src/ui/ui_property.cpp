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

	m_properties.SubclassWindow(GetDlgItem(IDC_LIST_PROPERTIES));
	m_properties.ModifyStyle(0, LBS_SORT | LBS_HASSTRINGS);
	m_properties.SetExtendedListStyle(PLS_EX_SORTED | PLS_EX_XPLOOK);

	LoadProperties();

	return FALSE;
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
	m_parent->m_config_prop.m_map = m_dup_prop_map;
	m_parent->update_script();
	LoadProperties();
}

void CDialogProperty::LoadProperties(bool reload)
{
	m_properties.ResetContent();

	if (reload)
	{
		m_dup_prop_map = m_parent->m_config_prop.m_map;
	}

	for (properties::t_map::const_iterator iter = m_dup_prop_map.first(); iter.is_valid(); ++iter)
	{
		string_wide_from_utf8_fast wname(iter->m_key);
		HPROPERTY hProp = nullptr;
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

void CDialogProperty::OnClearBnClicked(UINT uNotifyCode, int nID, HWND wndCtl)
{
	m_dup_prop_map.remove_all();
	m_properties.ResetContent();
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

void CDialogProperty::OnDelBnClicked(UINT uNotifyCode, int nID, HWND wndCtl)
{
	int idx = m_properties.GetCurSel();

	if (idx >= 0)
	{
		HPROPERTY hproperty = m_properties.GetProperty(idx);
		string_utf8_from_wide uname = hproperty->GetName();

		m_properties.DeleteItem(hproperty);
		m_dup_prop_map.remove(uname);
	}
}

void CDialogProperty::OnExportBnClicked(UINT uNotifyCode, int nID, HWND wndCtl)
{
	pfc::string8_fast path;

	if (uGetOpenFileName(m_hWnd, "Property files|*.wsp", 0, "wsp", "Save as", nullptr, path, TRUE))
	{
		file_ptr io;
		abort_callback_dummy abort;

		try
		{
			filesystem::g_open_write_new(io, path, abort);
			properties::g_save(m_dup_prop_map, io.get_ptr(), abort);
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
		abort_callback_dummy abort;

		try
		{
			filesystem::g_open_read(io, path, abort);
			properties::g_load(m_dup_prop_map, io.get_ptr(), abort);
			LoadProperties(false);
		}
		catch (...) {}
	}
}
