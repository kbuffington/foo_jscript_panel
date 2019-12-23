#pragma once
#include "panel_window.h"
#include "ui_property_control.h"

class CDialogProperty : public CDialogImpl<CDialogProperty>
{
public:
	CDialogProperty(panel_window* p_parent);

	BEGIN_MSG_MAP_EX(CDialogProperty)
		CHAIN_MSG_MAP_MEMBER(m_resizer)
		MSG_WM_INITDIALOG(OnInitDialog)
		COMMAND_RANGE_HANDLER_EX(IDOK, IDCANCEL, OnCloseCmd)
		COMMAND_ID_HANDLER_EX(IDC_BTN_APPLY, OnCloseCmd)
		COMMAND_HANDLER_EX(IDC_BTN_CLEAR, BN_CLICKED, OnClearBnClicked)
		COMMAND_HANDLER_EX(IDC_BTN_EXPORT, BN_CLICKED, OnExportBnClicked)
		COMMAND_HANDLER_EX(IDC_BTN_IMPORT, BN_CLICKED, OnImportBnClicked)
	END_MSG_MAP()

	enum { IDD = IDD_DIALOG_PROPERTY };

	BOOL OnInitDialog(CWindow, LPARAM);
	void Apply();
	void LoadProperties(bool reload = true);
	void OnClearBnClicked(UINT, int, CWindow);
	void OnCloseCmd(UINT, int nid, CWindow);
	void OnExportBnClicked(UINT, int, CWindow);
	void OnImportBnClicked(UINT, int, CWindow);

private:
	CButton m_clear_btn, m_export_btn;
	CDialogResizeHelper m_resizer;
	MyCList m_properties;
	panel_window* m_parent;
	pfc::string8_fast m_caption;
	panel_properties::property_map m_dup_prop_map;
};
