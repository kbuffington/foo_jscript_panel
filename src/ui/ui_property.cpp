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

	LoadProperties();

	return FALSE;
}

void CDialogProperty::Apply()
{
	m_parent->m_config_prop.m_map = m_dup_prop_map;
	m_parent->update_script();
	LoadProperties();
}

void CDialogProperty::LoadProperties(bool reload)
{


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
