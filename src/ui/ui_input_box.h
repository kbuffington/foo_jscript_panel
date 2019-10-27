#pragma once

class CInputBox : public CDialogImpl<CInputBox>
{
public:
	CInputBox(const char* p_prompt, const char* p_caption, const char* p_value);

	BEGIN_MSG_MAP(CInputBox)
		MSG_WM_INITDIALOG(OnInitDialog)
		COMMAND_RANGE_HANDLER_EX(IDOK, IDCANCEL, OnCloseCmd)
	END_MSG_MAP()

	enum { IDD = IDD_DIALOG_INPUT };

	BOOL OnInitDialog(HWND, LPARAM);
	void OnCloseCmd(UINT, int, HWND);

	pfc::string8_fast m_value;

private:
	CEdit m_edit;
	CWindow m_label;
	pfc::string8_fast m_prompt;
	pfc::string8_fast m_caption;
};
