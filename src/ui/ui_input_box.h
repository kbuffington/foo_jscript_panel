#pragma once

class CInputBox : public CDialogImpl<CInputBox>
{
public:
	CInputBox(pfc::stringp prompt, pfc::stringp caption, pfc::stringp value);

	BEGIN_MSG_MAP_EX(CInputBox)
		MSG_WM_INITDIALOG(OnInitDialog)
		COMMAND_RANGE_HANDLER_EX(IDOK, IDCANCEL, OnCloseCmd)
	END_MSG_MAP()

	enum { IDD = IDD_DIALOG_INPUT };

	BOOL OnInitDialog(CWindow, LPARAM);
	void OnCloseCmd(UINT, int nID, CWindow);

	pfc::string8_fast m_value;

private:
	CEdit m_edit;
	CWindow m_label;
	pfc::string8_fast m_prompt;
	pfc::string8_fast m_caption;
};
