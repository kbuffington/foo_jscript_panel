#pragma once
#include "panel_window.h"
#include "panel_properties.h"
#include "ui_property_control.h"

class CDialogProperty : public CDialogImpl<CDialogProperty>
{
public:
	CDialogProperty(panel_window* p_parent);
	~CDialogProperty();

	BEGIN_MSG_MAP(CDialogProperty)
		CHAIN_MSG_MAP_MEMBER(m_resizer)
		MSG_WM_INITDIALOG(OnInitDialog)
		MESSAGE_HANDLER(UWM_PROPERTIES_CLEARED, OnPropertiesCleared)
		COMMAND_RANGE_HANDLER_EX(IDOK, IDCANCEL, OnCloseCmd)
		COMMAND_ID_HANDLER_EX(IDC_BTN_APPLY, OnCloseCmd)
		COMMAND_HANDLER_EX(IDC_BTN_CLEAR, BN_CLICKED, OnClearBnClicked)
		COMMAND_HANDLER_EX(IDC_BTN_EXPORT, BN_CLICKED, OnExportBnClicked)
		COMMAND_HANDLER_EX(IDC_BTN_IMPORT, BN_CLICKED, OnImportBnClicked)
	END_MSG_MAP()

	enum { IDD = IDD_DIALOG_PROPERTY };

	BOOL OnInitDialog(HWND, LPARAM);
	LRESULT OnPropertiesCleared(UINT, WPARAM, LPARAM, BOOL&);
	void Apply();
	void LoadProperties(bool reload = true);
	void OnClearBnClicked(UINT, int, HWND);
	void OnCloseCmd(UINT, int, HWND);
	void OnExportBnClicked(UINT, int, HWND);
	void OnImportBnClicked(UINT, int, HWND);

private:
	CButton m_clear_btn, m_export_btn;
	CDialogResizeHelper m_resizer;
	MyCList m_properties;
	panel_window* m_parent;
	pfc::string8_fast m_caption;
	panel_properties::property_map m_dup_prop_map;
};
