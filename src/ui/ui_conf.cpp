#include "stdafx.h"
#include "panel_window.h"
#include "ui_conf.h"
#include "ui_find.h"
#include "ui_goto.h"
#include "ui_replace.h"

static const CDialogResizeHelper::Param resize_data[] =
{
	{ IDC_EDIT, 0, 0, 1, 1 },
	{ IDC_RESET, 0, 1, 0, 1 },
	{ IDC_ENGINE_LABEL, 0, 1, 0, 1 },
	{ IDC_ENGINE_COMBO, 0, 1, 0, 1 },
	{ IDC_EDGE_LABEL, 0, 1, 0, 1 },
	{ IDC_EDGE_COMBO, 0, 1, 0, 1 },
	{ IDC_PSEUDO_TRANSPARENT, 0, 1, 0, 1 },
	{ IDOK, 1, 1, 1, 1 },
	{ IDCANCEL, 1, 1, 1, 1 },
	{ IDC_APPLY, 1, 1, 1, 1 },
};

static const CRect resize_min_max(620, 381, 0, 0);

CDialogConf::CDialogConf(panel_window* p_parent) : m_parent(p_parent), m_dlgfind(nullptr), m_dlgreplace(nullptr), m_lastFlags(0), m_resizer(resize_data, resize_min_max)
{
	m_caption << JSP_NAME " Configuration (id:" << m_parent->m_script_info.id << ")";
}

CDialogConf::~CDialogConf() {}

BOOL CDialogConf::OnInitDialog(HWND hwndFocus, LPARAM lParam)
{
	m_edge = GetDlgItem(IDC_EDGE_COMBO);
	m_edit = GetDlgItem(IDC_EDIT);
	m_engine = GetDlgItem(IDC_ENGINE_COMBO);
	m_menu = GetMenu();
	m_pseudo = GetDlgItem(IDC_PSEUDO_TRANSPARENT);

	pfc::string8_fast base = helpers::get_fb2k_component_path();

	// Generate samples menu
	HMENU samples = CreateMenu();

	pfc::string_list_impl folders;
	helpers::list_folders(base + "samples\\", folders);

	t_size i, j, count;

	count = folders.get_count();

	for (i = 0; i < count; ++i)
	{
		HMENU sub = CreatePopupMenu();
		pfc::string8_fast folder = folders[i];

		pfc::string_list_impl sub_files;
		helpers::list_files(folder, false, sub_files);

		for (j = 0; j < sub_files.get_count(); ++j)
		{
			m_samples.add_item(sub_files[j]);
			pfc::string8_fast display = pfc::string_filename(sub_files[j]);
			uAppendMenu(sub, MF_STRING, ID_SAMPLES_BEGIN + m_samples.get_count() - 1, display);
		}

		slist path_split = helpers::split_string(folder.get_ptr(), "\\");
		uAppendMenu(samples, MF_STRING | MF_POPUP, (UINT_PTR)sub, path_split[path_split.size() - 1].c_str());
	}

	uAppendMenu(m_menu, MF_STRING | MF_POPUP, (UINT_PTR)samples, "Samples");

	// Generate docs menu
	HMENU docs = CreateMenu();

	helpers::list_files(base + "docs\\", false, m_docs);
	count = m_docs.get_count();

	for (i = 0; i < count; ++i)
	{
		pfc::string8_fast display;
		uFixAmpersandChars_v2(pfc::string_filename(m_docs[i]), display);
		uAppendMenu(docs, MF_STRING, ID_DOCS_BEGIN + i, display);
	}

	uAppendMenu(m_menu, MF_STRING | MF_POPUP, (UINT_PTR)docs, "Docs");

	// Generate links menu
	HMENU links = CreateMenu();
	uAppendMenu(links, MF_STRING, ID_LINKS_BEGIN, "Wiki");
	uAppendMenu(links, MF_STRING, ID_LINKS_BEGIN + 1, "Releases");
	uAppendMenu(links, MF_STRING, ID_LINKS_BEGIN + 2, "Report an issue");
	uAppendMenu(m_menu, MF_STRING | MF_POPUP, (UINT_PTR)links , "Links");

	// Set caption text
	uSetWindowText(m_hWnd, m_caption);

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

	// Script Engine
	m_engine.AddString(L"Chakra");
	m_engine.AddString(L"JScript");

	if (helpers::supports_chakra())
	{
		uComboBox_SelectString(m_engine, m_parent->m_script_engine_str);
	}
	else
	{
		uComboBox_SelectString(m_engine, "JScript");
		m_engine.EnableWindow(false);
	}

	// Edge Style & Pseudo Transparent
	m_edge.AddString(L"None");
	m_edge.AddString(L"Sunken");
	m_edge.AddString(L"Grey");

	if (m_parent->get_instance_type() == host_comm::KInstanceTypeDUI)
	{
		// disable both options in DUI
		m_edge.SetCurSel(0);
		m_edge.EnableWindow(false);

		m_pseudo.SetCheck(false);
		m_pseudo.EnableWindow(false);
	}
	else
	{
		m_edge.SetCurSel(m_parent->m_edge_style);

		m_pseudo.SetCheck(m_parent->m_pseudo_transparent);
	}

	// Edit Control
	m_editorctrl.SubclassWindow(m_edit);
	m_editorctrl.SetJScript();
	m_editorctrl.SetContent(m_parent->m_script_code);
	m_editorctrl.EmptyUndoBuffer();
	m_editorctrl.SetSavePoint();

	return FALSE;
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
				m_dlgreplace = new CDialogReplace(m_edit);

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
				CDialogGoto dlg(m_edit);
				dlg.DoModal(m_hWnd);
			}
			break;

		case 'S':
			Apply();
			break;
		}
	}
	else if (wParam == VK_F3 && (modifiers == 0 || modifiers == SCMOD_SHIFT))
	{
		if (m_lastSearchText.get_length())
		{
			if (modifiers == 0) // Next
			{
				FindNext(m_hWnd, m_editorctrl.m_hWnd, m_lastFlags, m_lastSearchText);
			}
			else if (modifiers == SCMOD_SHIFT) // Previous
			{
				FindPrevious(m_hWnd, m_editorctrl.m_hWnd, m_lastFlags, m_lastSearchText);
			}
		}
		else
		{
			OpenFindDialog();
		}
	}
	return 0;
}

bool CDialogConf::FindNext(HWND hWnd, HWND hWndEdit, t_size flags, const char* which)
{
	SendMessage(::GetAncestor(hWndEdit, GA_PARENT), UWM_FIND_TEXT_CHANGED, flags, reinterpret_cast<LPARAM>(which));
	SendMessage(hWndEdit, SCI_CHARRIGHT, 0, 0);
	SendMessage(hWndEdit, SCI_SEARCHANCHOR, 0, 0);
	int pos = SendMessage(hWndEdit, SCI_SEARCHNEXT, flags, reinterpret_cast<LPARAM>(which));
	return FindResult(hWnd, hWndEdit, pos, which);
}

bool CDialogConf::FindPrevious(HWND hWnd, HWND hWndEdit, t_size flags, const char* which)
{
	SendMessage(::GetAncestor(hWndEdit, GA_PARENT), UWM_FIND_TEXT_CHANGED, flags, reinterpret_cast<LPARAM>(which));
	SendMessage(hWndEdit, SCI_SEARCHANCHOR, 0, 0);
	int pos = SendMessage(hWndEdit, SCI_SEARCHPREV, flags, reinterpret_cast<LPARAM>(which));
	return FindResult(hWnd, hWndEdit, pos, which);
}

bool CDialogConf::FindResult(HWND hWnd, HWND hWndEdit, int pos, const char* which)
{
	if (pos != -1)
	{
		// Scroll to view
		SendMessage(hWndEdit, SCI_SCROLLCARET, 0, 0);
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
	uGetWindowText(m_engine, m_parent->m_script_engine_str);
	m_parent->m_edge_style = static_cast<host_comm::t_edge_style>(m_edge.GetCurSel());
	m_parent->m_pseudo_transparent = m_pseudo.GetCheck() == BST_CHECKED;

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

void CDialogConf::OnCloseCmd(UINT uNotifyCode, int nID, HWND wndCtl)
{
	switch (nID)
	{
	case IDOK:
		Apply();
		break;
	case IDC_APPLY:
		Apply();
		return;
	case IDCANCEL:
		if (m_editorctrl.GetModify() && uMessageBox(m_hWnd, "Unsaved changes will be lost. Are you sure?", JSP_NAME, MB_ICONWARNING | MB_SETFOREGROUND | MB_YESNO) != IDYES)
		{
			return;
		}
	}
	EndDialog(nID);
}

void CDialogConf::OnDocs(UINT uNotifyCode, int nID, HWND wndCtl)
{
	pfc::string8_fast tmp = file_path_display(m_docs[nID - ID_DOCS_BEGIN]).get_ptr();
	string_wide_from_utf8_fast path(tmp);
	ShellExecute(nullptr, L"open", path, nullptr, nullptr, SW_SHOW);
}

void CDialogConf::OnFileImport(UINT uNotifyCode, int nID, HWND wndCtl)
{
	pfc::string8_fast filename;
	if (uGetOpenFileName(m_hWnd, "Text files|*.txt|JScript files|*.js|All files|*.*", 0, "txt", "Import from", nullptr, filename, FALSE))
	{
		m_editorctrl.SetContent(helpers::read_file(filename));
	}
}

void CDialogConf::OnFileExport(UINT uNotifyCode, int nID, HWND wndCtl)
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
}

void CDialogConf::OnLinks(UINT uNotifyCode, int nID, HWND wndCtl)
{
	static const std::array<const wchar_t*, 3> links =
	{
		L"https://github.com/marc2k3/foo_jscript_panel/wiki",
		L"https://github.com/marc2k3/foo_jscript_panel/releases",
		L"https://github.com/marc2k3/foo_jscript_panel/issues"
	};
	ShellExecute(nullptr, L"open", links[nID - ID_LINKS_BEGIN], nullptr, nullptr, SW_SHOW);
}

void CDialogConf::OnReset(UINT uNotifyCode, int nID, HWND wndCtl)
{
	uComboBox_SelectString(m_engine, host_comm::get_default_script_engine_str());
	m_edge.SetCurSel(0);
	m_pseudo.SetCheck(false);
	m_editorctrl.SetContent(host_comm::get_default_script_code());
}

void CDialogConf::OnSamples(UINT uNotifyCode, int nID, HWND wndCtl)
{
	m_editorctrl.SetContent(helpers::read_file(file_path_display(m_samples[nID - ID_SAMPLES_BEGIN])));
}

void CDialogConf::OpenFindDialog()
{
	if (!m_dlgfind)
	{
		m_dlgfind = new CDialogFind(m_edit);
		m_dlgfind->Create(m_hWnd);
	}

	m_dlgfind->ShowWindow(SW_SHOW);
	m_dlgfind->SetFocus();
}
