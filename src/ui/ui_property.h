#pragma once
#include "panel_window.h"
#include "properties.h"
#include "ui_property_control.h"

class CDialogProperty : public CDialogImpl<CDialogProperty>
{
public:
	CDialogProperty(panel_window* p_parent);
	~CDialogProperty();

	BEGIN_MSG_MAP(CDialogProperty)
		CHAIN_MSG_MAP_MEMBER(m_resizer)
		MSG_WM_INITDIALOG(OnInitDialog)
		COMMAND_RANGE_HANDLER_EX(IDOK, IDCANCEL, OnCloseCmd)
		COMMAND_ID_HANDLER_EX(IDC_APPLY, OnCloseCmd)
		COMMAND_HANDLER_EX(IDC_IMPORT, BN_CLICKED, OnImportBnClicked)
		COMMAND_HANDLER_EX(IDC_EXPORT, BN_CLICKED, OnExportBnClicked)
	END_MSG_MAP()

	enum
	{
		IDD = IDD_DIALOG_PROPERTY
	};

	BOOL OnInitDialog(HWND hwndFocus, LPARAM lParam);
	void Apply();
	void LoadProperties(bool reload = true);
	void OnCloseCmd(UINT uNotifyCode, int nID, HWND wndCtl);
	void OnExportBnClicked(UINT uNotifyCode, int nID, HWND wndCtl);
	void OnImportBnClicked(UINT uNotifyCode, int nID, HWND wndCtl);

private:
	CDialogResizeHelper m_resizer;
	MyCList m_properties;
	panel_window* m_parent;
	pfc::string8_fast m_caption;
	properties::t_map m_dup_prop_map;
};
