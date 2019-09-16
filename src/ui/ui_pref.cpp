#include "stdafx.h"
#include "helpers.h"
#include "scintilla_prop_sets.h"
#include "ui_pref.h"

CDialogPref::CDialogPref(preferences_page_callback::ptr callback) : m_callback(callback), m_props(this) {}

BOOL CDialogPref::OnInitDialog(HWND hwndFocus, LPARAM lParam)
{
	m_props.CreateInDialog(*this, IDC_LIST_PREFERENCES);

	auto DPI = m_props.GetDPI();

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

t_size CDialogPref::get_state()
{
	return preferences_state::resettable;
}

t_size CDialogPref::listGetItemCount(ctx_t)
{
	return g_sci_prop_sets.m_data.size();
}

pfc::string8 CDialogPref::listGetSubItemText(ctx_t, t_size item, t_size sub_item)
{
	switch (sub_item)
	{
	case 0:
		return g_sci_prop_sets.m_data[item].key.get_ptr();
	case 1:
		return g_sci_prop_sets.m_data[item].val.get_ptr();
	}
	return "";
}

void CDialogPref::LoadProps(bool reset)
{
	if (reset)
	{
		g_sci_prop_sets.reset();
	}

	m_props.ReloadData();
}

void CDialogPref::OnExportBnClicked(UINT uNotifyCode, int nID, HWND wndCtl)
{
	pfc::string8_fast filename;
	if (uGetOpenFileName(m_hWnd, "Configuration files|*.cfg", 0, "cfg", "Save as", nullptr, filename, TRUE))
	{
		g_sci_prop_sets.export_to_file(filename);
	}
}

void CDialogPref::OnImportBnClicked(UINT uNotifyCode, int nID, HWND wndCtl)
{
	pfc::string8_fast filename;
	if (uGetOpenFileName(m_hWnd, "Configuration files|*.cfg|All files|*.*", 0, "cfg", "Import from", nullptr, filename, FALSE))
	{
		g_sci_prop_sets.import(helpers::read_file(filename));
		LoadProps();
	}
}

void CDialogPref::OnPresetsBnClicked(UINT uNotifyCode, int nID, HWND wndCtl)
{
	CMenu menu = CreatePopupMenu();

	uAppendMenu(menu, MF_STRING, 1, "Bright");
	uAppendMenu(menu, MF_STRING, 2, "Dark");
	uAppendMenu(menu, MF_STRING, 3, "Ruby Blue");

	RECT rc;
	::GetWindowRect(::GetDlgItem(m_hWnd, IDC_PRESETS), &rc);
	int idx = TrackPopupMenu(menu, TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD, rc.left, rc.bottom, 0, m_hWnd, 0);
	if (idx > 0)
	{
		puResource pures = uLoadResource(core_api::get_my_instance(), uMAKEINTRESOURCE(idx == 1 ? IDR_BRIGHT : idx == 2 ? IDR_DARK : IDR_RUBY), "TEXT");
		pfc::string8_fast content(static_cast<const char*>(pures->GetPointer()), pures->GetSize());
		g_sci_prop_sets.import(content);
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
		g_sci_prop_sets.m_data[item].val = value;
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
