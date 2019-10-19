#include "stdafx.h"
#include "helpers.h"
#include "scintilla_properties.h"
#include "ui_pref.h"

CDialogPref::CDialogPref(preferences_page_callback::ptr callback) : m_callback(callback), m_props(this) {}

BOOL CDialogPref::OnInitDialog(HWND, LPARAM)
{
	m_props.CreateInDialog(*this, IDC_LIST_PREFERENCES);

	const auto DPI = m_props.GetDPI();

	m_props.AddColumn("Name", MulDiv(150, DPI.cx, 96));
	m_props.AddColumn("Value", MulDiv(310, DPI.cx, 96));

	return FALSE;
}

HWND CDialogPref::get_wnd()
{
	return m_hWnd;
}

bool CDialogPref::listCanSelectItem(ctx_t, t_size)
{
	return false;
}

bool CDialogPref::listIsColumnEditable(ctx_t, t_size sub_item)
{
	return sub_item == 1;
}

pfc::string8 CDialogPref::listGetSubItemText(ctx_t, t_size item, t_size sub_item)
{
	switch (sub_item)
	{
	case 0:
		return g_scintilla_properties.m_data[item].key.get_ptr();
	case 1:
		return g_scintilla_properties.m_data[item].val.get_ptr();
	}
	return "";
}

t_size CDialogPref::get_state()
{
	return preferences_state::resettable;
}

t_size CDialogPref::listGetItemCount(ctx_t)
{
	return g_scintilla_properties.m_data.size();
}

void CDialogPref::LoadProps(bool reset)
{
	if (reset)
	{
		g_scintilla_properties.init_data();
	}

	m_props.ReloadData();
}

void CDialogPref::OnExportBnClicked(UINT, int, HWND)
{
	pfc::string8_fast filename;
	if (uGetOpenFileName(m_hWnd, "Configuration files|*.cfg", 0, "cfg", "Save as", nullptr, filename, TRUE))
	{
		pfc::string8_fast content = "# Generated by " JSP_NAME "\r\n";
		for (const auto& [key, val] : g_scintilla_properties.m_data)
		{
			content << key << "=" << val << "\r\n";
		}
		helpers::write_file(filename, content);
	}
}

void CDialogPref::OnImportBnClicked(UINT, int, HWND)
{
	pfc::string8_fast filename;
	if (uGetOpenFileName(m_hWnd, "Configuration files|*.cfg|All files|*.*", 0, "cfg", "Import from", nullptr, filename, FALSE))
	{
		g_scintilla_properties.import(helpers::read_file(filename));
		LoadProps();
	}
}

void CDialogPref::OnPresetsBnClicked(UINT, int, HWND)
{
	CMenu menu = CreatePopupMenu();

	uAppendMenu(menu, MF_STRING, IDR_BRIGHT, "Bright");
	uAppendMenu(menu, MF_STRING, IDR_DARK, "Dark");
	uAppendMenu(menu, MF_STRING, IDR_RUBY, "Ruby Blue");

	RECT rc;
	::GetWindowRect(::GetDlgItem(m_hWnd, IDC_PRESETS), &rc);
	const int idx = TrackPopupMenu(menu, TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD, rc.left, rc.bottom, 0, m_hWnd, nullptr);
	if (idx > 0)
	{
		g_scintilla_properties.load_preset(idx);
		LoadProps();
	}
}

void CDialogPref::apply()
{
	m_callback->on_state_changed();
}

void CDialogPref::listSetEditField(ctx_t, t_size item, t_size sub_item, const char* value)
{
	if (sub_item == 1)
	{
		g_scintilla_properties.m_data[item].val = value;
	}
}

void CDialogPref::listSubItemClicked(ctx_t, t_size item, t_size sub_item)
{
	if (sub_item == 1)
	{
		m_props.TableEdit_Start(item, sub_item);
	}
}

void CDialogPref::reset()
{
	LoadProps(true);
}
