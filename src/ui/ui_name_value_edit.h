#pragma once

class CNameValueEdit : public CDialogImpl<CNameValueEdit>
{
public:
	CNameValueEdit(const char* p_name, const char* p_value);

	BEGIN_MSG_MAP(CNameValueEdit)
		MSG_WM_INITDIALOG(OnInitDialog)
		COMMAND_RANGE_HANDLER_EX(IDOK, IDCANCEL, OnCloseCmd)
	END_MSG_MAP()

	enum
	{
		IDD = IDD_DIALOG_NAME_VALUE_EDIT
	};

	BOOL OnInitDialog(HWND hwndFocus, LPARAM lParam);
	void GetValue(pfc::string_base& p_value);
	void OnCloseCmd(UINT uNotifyCode, int nID, HWND wndCtl);

private:
	pfc::string8_fast m_name;
	pfc::string8_fast m_value;
};
