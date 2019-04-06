#pragma once
#include "panel_window.h"
#include "properties.h"

#include <PropertyList.h>

class CDialogProperty : public CDialogImpl<CDialogProperty>, public CDialogResize<CDialogProperty>
{
public:
	CDialogProperty(panel_window* p_parent);
	~CDialogProperty();

	BEGIN_DLGRESIZE_MAP(CDialogProperty)
		DLGRESIZE_CONTROL(IDC_LIST_PROPERTIES, DLSZ_SIZE_X | DLSZ_SIZE_Y)
		DLGRESIZE_CONTROL(IDC_DEL, DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_CLEAR, DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_IMPORT, DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_EXPORT, DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDOK, DLSZ_MOVE_X | DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_APPLY, DLSZ_MOVE_X | DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDCANCEL, DLSZ_MOVE_X | DLSZ_MOVE_Y)
	END_DLGRESIZE_MAP()

	BEGIN_MSG_MAP(CDialogProperty)
		MSG_WM_INITDIALOG(OnInitDialog)
		COMMAND_RANGE_HANDLER_EX(IDOK, IDCANCEL, OnCloseCmd)
		COMMAND_ID_HANDLER_EX(IDC_APPLY, OnCloseCmd)
		COMMAND_HANDLER_EX(IDC_DEL, BN_CLICKED, OnDelBnClicked)
		COMMAND_HANDLER_EX(IDC_CLEAR, BN_CLICKED, OnClearBnClicked)
		COMMAND_HANDLER_EX(IDC_IMPORT, BN_CLICKED, OnImportBnClicked)
		COMMAND_HANDLER_EX(IDC_EXPORT, BN_CLICKED, OnExportBnClicked)
		NOTIFY_CODE_HANDLER_EX(PIN_ITEMCHANGED, OnPinItemChanged)
		CHAIN_MSG_MAP(CDialogResize<CDialogProperty>)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	enum
	{
		IDD = IDD_DIALOG_PROPERTY
	};

	BOOL OnInitDialog(HWND hwndFocus, LPARAM lParam);
	LRESULT OnPinItemChanged(LPNMHDR pnmh);
	void Apply();
	void LoadProperties(bool reload = true);
	void OnClearBnClicked(UINT uNotifyCode, int nID, HWND wndCtl);
	void OnCloseCmd(UINT uNotifyCode, int nID, HWND wndCtl);
	void OnDelBnClicked(UINT uNotifyCode, int nID, HWND wndCtl);
	void OnExportBnClicked(UINT uNotifyCode, int nID, HWND wndCtl);
	void OnImportBnClicked(UINT uNotifyCode, int nID, HWND wndCtl);

private:
	CPropertyListCtrl m_properties;
	panel_window* m_parent;
	pfc::string8_fast m_caption;
	properties::t_map m_dup_prop_map;
};
