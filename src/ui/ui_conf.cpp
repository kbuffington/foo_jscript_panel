#include "stdafx.h"
#include "panel_window.h"
#include "ui_conf.h"
#include "ui_find.h"
#include "ui_goto.h"
#include "ui_replace.h"

CDialogConf::CDialogConf(panel_window* p_parent) : m_parent(p_parent), m_dlgfind(nullptr), m_dlgreplace(nullptr), m_lastSearchText(""), m_lastFlags(0)
{
	m_caption << JSP_NAME " Configuration (id:" << m_parent->m_script_info.id << ")";
}

CDialogConf::~CDialogConf()
{
	m_hWnd = nullptr;
}

BOOL CDialogConf::OnInitDialog(HWND hwndFocus, LPARAM lParam)
{
	m_menu = GetMenu();

	pfc::string8_fast base = helpers::get_fb2k_component_path();

	// Generate samples menu
	HMENU samples = CreateMenu();

	pfc::string_list_impl folders;
	helpers::list(base + "samples\\", false, false, folders);

	t_size i, j, count;

	count = folders.get_count();

	for (i = 0; i < count; ++i)
	{
		HMENU sub = CreatePopupMenu();
		pfc::string8_fast folder = folders[i];

		pfc::string_list_impl sub_files;
		helpers::list(folder, true, false, sub_files);

		for (j = 0; j < sub_files.get_count(); ++j)
		{
			m_samples.add_item(sub_files[j]);
			pfc::string8_fast display = pfc::string_filename(sub_files[j]);
			uAppendMenu(sub, MF_STRING, ID_SAMPLES_BEGIN + m_samples.get_count() - 1, display);
		}

		pfc::string_list_impl path_split;
		pfc::splitStringSimple_toList(path_split, "\\", folder);
		uAppendMenu(samples, MF_STRING | MF_POPUP, (UINT_PTR)sub, path_split[path_split.get_count() - 1]);
	}

	m_menu.AppendMenu(MF_STRING | MF_POPUP, (UINT_PTR)samples, L"Samples");

	// Generate docs menu
	HMENU docs = CreateMenu();

	helpers::list(base + "docs\\", true, false, m_docs);
	count = m_docs.get_count();

	for (i = 0; i < count; ++i)
	{
		pfc::string8_fast display;
		uFixAmpersandChars_v2(pfc::string_filename(m_docs[i]), display);
		uAppendMenu(docs, MF_STRING, ID_DOCS_BEGIN + i, display);
	}

	m_menu.AppendMenu(MF_STRING | MF_POPUP, (UINT_PTR)docs, L"Docs");

	// Generate links menu
	HMENU links = CreateMenu();
	uAppendMenu(links, MF_STRING, ID_LINKS_BEGIN, "Wiki");
	uAppendMenu(links, MF_STRING, ID_LINKS_BEGIN + 1, "Releases");
	uAppendMenu(links, MF_STRING, ID_LINKS_BEGIN + 2, "Report an issue");
	m_menu.AppendMenu(MF_STRING | MF_POPUP, (UINT_PTR)links , L"Links");

	// Set caption text
	uSetWindowText(m_hWnd, m_caption);

	// Init resize
	DlgResize_Init();

	// Apply window placement
	if (m_parent->m_wndpl.length == 0)
	{
		m_parent->m_wndpl.length = sizeof(WINDOWPLACEMENT);
		memset(&m_parent->m_wndpl, 0, sizeof(WINDOWPLACEMENT));
	}
	else
	{
		SetWindowPlacement(&m_parent->m_wndpl);
	}

	// Edit Control
	m_editorctrl.SubclassWindow(GetDlgItem(IDC_EDIT));
	m_editorctrl.SetJScript();
	m_editorctrl.ReadAPI();
	m_editorctrl.SetContent(m_parent->m_script_code, true);
	m_editorctrl.SetSavePoint();

	// Script Engine
	HWND combo_engine = GetDlgItem(IDC_COMBO_ENGINE);
	ComboBox_AddString(combo_engine, L"Chakra");
	ComboBox_AddString(combo_engine, L"JScript");

	if (helpers::supports_chakra())
	{
		uComboBox_SelectString(combo_engine, m_parent->m_script_engine_str);
	}
	else
	{
		uComboBox_SelectString(combo_engine, "JScript");
		GetDlgItem(IDC_COMBO_ENGINE).EnableWindow(false);
	}

	// Edge Style
	HWND combo_edge = GetDlgItem(IDC_COMBO_EDGE);
	ComboBox_AddString(combo_edge, L"None");
	ComboBox_AddString(combo_edge, L"Sunken");
	ComboBox_AddString(combo_edge, L"Grey");

	if (core_version_info_v2::get()->test_version(1, 4, 0, 0) && m_parent->get_instance_type() == host_comm::KInstanceTypeDUI)
	{
		// Disable in default UI fb2k v1.4 and above
		ComboBox_SetCurSel(combo_edge, 0);
		GetDlgItem(IDC_COMBO_EDGE).EnableWindow(false);
	}
	else
	{
		ComboBox_SetCurSel(combo_edge, m_parent->m_edge_style);
	}

	// Pseudo Transparent
	if (m_parent->get_instance_type() == host_comm::KInstanceTypeCUI)
	{
		uButton_SetCheck(m_hWnd, IDC_CHECK_PSEUDO_TRANSPARENT, m_parent->m_pseudo_transparent);
	}
	else
	{
		uButton_SetCheck(m_hWnd, IDC_CHECK_PSEUDO_TRANSPARENT, false);
		GetDlgItem(IDC_CHECK_PSEUDO_TRANSPARENT).EnableWindow(false);
	}

	// Grab Focus
	uButton_SetCheck(m_hWnd, IDC_CHECK_GRABFOCUS, m_parent->m_grab_focus);

	return FALSE;
}

LRESULT CDialogConf::OnCloseCmd(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{
	switch (wID)
	{
	case IDOK:
		Apply();
		EndDialog(IDOK);
		break;

	case IDC_APPLY:
		Apply();
		break;

	case IDCANCEL:
		if (m_editorctrl.GetModify())
		{
			int ret = uMessageBox(m_hWnd, "Unsaved changes will be lost. Are you sure?", JSP_NAME, MB_ICONWARNING | MB_SETFOREGROUND | MB_YESNO);

			switch (ret)
			{
			case IDYES:
				break;

			default:
				return 0;
			}
		}

		EndDialog(IDCANCEL);
	}

	return 0;
}

LRESULT CDialogConf::OnDocs(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{
	pfc::string8_fast tmp = file_path_display(m_docs[wID - ID_DOCS_BEGIN]).get_ptr();
	string_wide_from_utf8_fast path(tmp);
	ShellExecute(nullptr, L"open", path, nullptr, nullptr, SW_SHOW);
	return 0;
}

LRESULT CDialogConf::OnFileSave(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{
	Apply();
	return 0;
}

LRESULT CDialogConf::OnFileImport(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{
	pfc::string8_fast filename;
	if (uGetOpenFileName(m_hWnd, "Text files|*.txt|JScript files|*.js|All files|*.*", 0, "txt", "Import from", nullptr, filename, FALSE))
	{
		m_editorctrl.SetContent(helpers::read_file(filename));
	}
	return 0;
}

LRESULT CDialogConf::OnFileExport(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{
	pfc::string8_fast filename;
	if (uGetOpenFileName(m_hWnd, "Text files|*.txt|All files|*.*", 0, "txt", "Save as", nullptr, filename, TRUE))
	{
		int len = m_editorctrl.GetTextLength();
		pfc::string8_fast text;

		m_editorctrl.GetText(text.lock_buffer(len), len + 1);
		text.unlock_buffer();

		helpers::write_file(filename, text);
	}
	return 0;
}

LRESULT CDialogConf::OnLinks(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{
	const wchar_t* links[] = {
		L"https://github.com/marc2k3/foo_jscript_panel/wiki",
		L"https://github.com/marc2k3/foo_jscript_panel/releases",
		L"https://github.com/marc2k3/foo_jscript_panel/issues"
	};

	ShellExecute(nullptr, L"open", links[wID - ID_LINKS_BEGIN], nullptr, nullptr, SW_SHOW);
	return 0;
}

LRESULT CDialogConf::OnNotify(int idCtrl, LPNMHDR pnmh)
{
	pfc::string8_fast caption = m_caption;

	switch (pnmh->code)
	{
	case SCN_SAVEPOINTLEFT: // dirty
		caption += " *";
		uSetWindowText(m_hWnd, caption);
		break;
	case SCN_SAVEPOINTREACHED: // not dirty
		uSetWindowText(m_hWnd, caption);
		break;
	}

	SetMsgHandled(FALSE);
	return 0;
}

LRESULT CDialogConf::OnReset(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{
	if (helpers::supports_chakra())
	{
		uComboBox_SelectString(GetDlgItem(IDC_COMBO_ENGINE), "Chakra");
	}
	ComboBox_SetCurSel(GetDlgItem(IDC_COMBO_EDGE), 0);
	uButton_SetCheck(m_hWnd, IDC_CHECK_PSEUDO_TRANSPARENT, false);
	uButton_SetCheck(m_hWnd, IDC_CHECK_GRABFOCUS, true);

	m_editorctrl.SetContent(panel_vars::get_default_script_code());
	return 0;
}

LRESULT CDialogConf::OnSamples(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{
	m_editorctrl.SetContent(helpers::read_file(file_path_display(m_samples[wID - ID_SAMPLES_BEGIN])));
	return 0;
}

LRESULT CDialogConf::OnUwmFindTextChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_lastFlags = wParam;
	m_lastSearchText = reinterpret_cast<const char*>(lParam);
	return 0;
}

LRESULT CDialogConf::OnUwmKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int modifiers = (IsKeyPressed(VK_SHIFT) ? SCMOD_SHIFT : 0) | (IsKeyPressed(VK_CONTROL) ? SCMOD_CTRL : 0) | (IsKeyPressed(VK_MENU) ? SCMOD_ALT : 0);

	if (modifiers == SCMOD_CTRL)
	{
		switch (wParam)
		{
		case 'F':
			OpenFindDialog();
			break;

		case 'H':
			if (!m_dlgreplace)
			{
				m_dlgreplace = new CDialogReplace(GetDlgItem(IDC_EDIT));

				if (!m_dlgreplace || !m_dlgreplace->Create(m_hWnd))
				{
					break;
				}
			}

			m_dlgreplace->ShowWindow(SW_SHOW);
			m_dlgreplace->SetFocus();
			break;

		case 'G':
			{
				modal_dialog_scope scope(m_hWnd);
				CDialogGoto dlg(GetDlgItem(IDC_EDIT));
				dlg.DoModal(m_hWnd);
			}
			break;

		case 'S':
			Apply();
			break;
		}
	}
	else if (modifiers == 0)
	{
		if (wParam == VK_F3)
		{
			// Find next one
			if (m_lastSearchText.get_length())
			{
				FindNext(m_hWnd, m_editorctrl.m_hWnd, m_lastFlags, m_lastSearchText);
			}
			else
			{
				OpenFindDialog();
			}
		}
	}
	else if (modifiers == SCMOD_SHIFT)
	{
		if (wParam == VK_F3)
		{
			// Find previous one
			if (m_lastSearchText.get_length())
			{
				FindPrevious(m_hWnd, m_editorctrl.m_hWnd, m_lastFlags, m_lastSearchText);
			}
			else
			{
				OpenFindDialog();
			}
		}
	}
	return 0;
}

bool CDialogConf::FindNext(HWND hWnd, HWND hWndEdit, t_size flags, const char* which)
{
	::SendMessage(::GetAncestor(hWndEdit, GA_PARENT), UWM_FIND_TEXT_CHANGED, flags, reinterpret_cast<LPARAM>(which));

	SendMessage(hWndEdit, SCI_CHARRIGHT, 0, 0);
	SendMessage(hWndEdit, SCI_SEARCHANCHOR, 0, 0);
	int pos = ::SendMessage(hWndEdit, SCI_SEARCHNEXT, flags, reinterpret_cast<LPARAM>(which));
	return FindResult(hWnd, hWndEdit, pos, which);
}

bool CDialogConf::FindPrevious(HWND hWnd, HWND hWndEdit, t_size flags, const char* which)
{
	::SendMessage(::GetAncestor(hWndEdit, GA_PARENT), UWM_FIND_TEXT_CHANGED, flags, reinterpret_cast<LPARAM>(which));

	SendMessage(hWndEdit, SCI_SEARCHANCHOR, 0, 0);
	int pos = ::SendMessage(hWndEdit, SCI_SEARCHPREV, flags, reinterpret_cast<LPARAM>(which));
	return FindResult(hWnd, hWndEdit, pos, which);
}

bool CDialogConf::FindResult(HWND hWnd, HWND hWndEdit, int pos, const char* which)
{
	if (pos != -1)
	{
		// Scroll to view
		::SendMessage(hWndEdit, SCI_SCROLLCARET, 0, 0);
		return true;
	}

	pfc::string8_fast msg;
	msg << "Cannot find \"" << which << "\"";
	uMessageBox(hWnd, msg, JSP_NAME, MB_ICONINFORMATION | MB_SETFOREGROUND);
	return false;
}

void CDialogConf::Apply()
{
	// Save panel settings
	uGetWindowText(GetDlgItem(IDC_COMBO_ENGINE), m_parent->m_script_engine_str);
	m_parent->m_edge_style = static_cast<t_edge_style>(ComboBox_GetCurSel(GetDlgItem(IDC_COMBO_EDGE)));
	m_parent->m_grab_focus = uButton_GetCheck(m_hWnd, IDC_CHECK_GRABFOCUS);
	m_parent->m_pseudo_transparent = uButton_GetCheck(m_hWnd, IDC_CHECK_PSEUDO_TRANSPARENT);

	// Get script text
	pfc::array_t<char> code;
	int len = m_editorctrl.GetTextLength() + 1;
	code.set_size(len);
	m_editorctrl.GetText(code.get_ptr(), len);
	m_parent->m_script_code = code.get_ptr();
	m_parent->update_script();

	// Window position
	GetWindowPlacement(&m_parent->m_wndpl);

	// Save point
	m_editorctrl.SetSavePoint();
}

void CDialogConf::OpenFindDialog()
{
	if (!m_dlgfind)
	{
		m_dlgfind = new CDialogFind(GetDlgItem(IDC_EDIT));
		m_dlgfind->Create(m_hWnd);
	}

	m_dlgfind->ShowWindow(SW_SHOW);
	m_dlgfind->SetFocus();
}
