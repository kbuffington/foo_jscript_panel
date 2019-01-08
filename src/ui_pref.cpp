#include "stdafx.h"
#include "ui_name_value_edit.h"
#include "ui_pref.h"
#include "scintilla_prop_sets.h"

CDialogPref::CDialogPref(preferences_page_callback::ptr callback) : m_callback(callback) {}

BOOL CDialogPref::OnInitDialog(HWND hwndFocus, LPARAM lParam)
{
	DoDataExchange();

	SetWindowTheme(m_props.m_hWnd, _T("explorer"), NULL);

	m_props.SetExtendedListViewStyle(LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER);
	m_props.AddColumn(_T("Name"), 0);
	m_props.SetColumnWidth(0, 150);
	m_props.AddColumn(_T("Value"), 1);
	m_props.SetColumnWidth(1, 310);
	LoadProps();

	return TRUE; // set focus to default control
}

HWND CDialogPref::get_wnd()
{
	return m_hWnd;
}

LRESULT CDialogPref::OnPropNMDblClk(LPNMHDR pnmh)
{
	//for ListView - (LPNMITEMACTIVATE)pnmh
	//for StatusBar	- (LPNMMOUSE)pnmh
	LPNMITEMACTIVATE pniv = (LPNMITEMACTIVATE)pnmh;

	if (pniv->iItem >= 0)
	{
		t_sci_prop_set_list& prop_sets = g_sci_prop_sets.m_data;
		pfc::string8_fast key, val;

		uGetItemText(pniv->iItem, 0, key);
		uGetItemText(pniv->iItem, 1, val);

		modal_dialog_scope scope;
		if (scope.can_create())
		{
			scope.initialize(m_hWnd);
			CNameValueEdit dlg(key, val);

			if (dlg.DoModal(m_hWnd) == IDOK)
			{
				dlg.GetValue(val);

				for (t_size i = 0; i < prop_sets.get_count(); ++i)
				{
					if (strcmp(prop_sets[i].key, key) == 0)
					{
						prop_sets[i].val = val;
						break;
					}
				}

				m_props.SetItemText(pniv->iItem, 1, string_wide_from_utf8_fast(val));
				DoDataExchange();
			}
		}
	}

	return 0;
}

t_size CDialogPref::get_state()
{
	return preferences_state::resettable;
}

void CDialogPref::LoadProps(bool reset)
{
	if (reset)
	{
		g_sci_prop_sets.reset();
	}

	string_wide_from_utf8_fast conv;
	t_sci_prop_set_list& prop_sets = g_sci_prop_sets.m_data;

	m_props.DeleteAllItems();

	for (t_size i = 0; i < prop_sets.get_count(); ++i)
	{
		conv.convert(prop_sets[i].key);
		m_props.AddItem(i, 0, conv);

		conv.convert(prop_sets[i].val);
		m_props.AddItem(i, 1, conv);
	}

	OnChanged();
}

void CDialogPref::OnButtonExportBnClicked(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{
	pfc::string8_fast filename;
	if (uGetOpenFileName(m_hWnd, "Configuration files|*.cfg", 0, "cfg", "Save as", NULL, filename, TRUE))
	{
		g_sci_prop_sets.export_to_file(filename);
	}
}

void CDialogPref::OnButtonImportBnClicked(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{
	pfc::string8_fast filename;
	if (uGetOpenFileName(m_hWnd, "Configuration files|*.cfg|All files|*.*", 0, "cfg", "Import from", NULL, filename, FALSE))
	{
		g_sci_prop_sets.import_from_file(filename);
	}

	LoadProps();
}

void CDialogPref::OnChanged()
{
	m_callback->on_state_changed();
}

void CDialogPref::OnEditChange(WORD, WORD, HWND)
{
	OnChanged();
}

void CDialogPref::uGetItemText(int nItem, int nSubItem, pfc::string_base& out)
{
	wchar_t buffer[1024];
	m_props.GetItemText(nItem, nSubItem, buffer, 1024);
	out.set_string(string_utf8_from_wide(buffer));
}

void CDialogPref::apply()
{
	OnChanged();
}

void CDialogPref::reset()
{
	LoadProps(true);
}

GUID js_preferences_page_impl::get_guid()
{
	return g_guid_jsp_ui_pref;
}

GUID js_preferences_page_impl::get_parent_guid()
{
	return preferences_page::guid_tools;
}

bool js_preferences_page_impl::get_help_url(pfc::string_base& p_out)
{
	p_out = "https://github.com/marc2k3/foo_jscript_panel/wiki";
	return true;
}

const char* js_preferences_page_impl::get_name()
{
	return JSP_NAME;
}

preferences_page_instance::ptr js_preferences_page_impl::instantiate(HWND parent, preferences_page_callback::ptr callback)
{
	auto p = fb2k::service_new<CDialogPref>(callback);
	p->Create(parent);
	return p;
}

namespace
{
	preferences_page_factory_t<js_preferences_page_impl> g_pref;
}
