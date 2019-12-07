#include "stdafx.h"
#include "config.h"
#include "panel_window.h"
#include "ui_conf.h"

static const CDialogResizeHelper::Param resize_data[] =
{
	{ IDC_EDIT, 0, 0, 1, 1 },
	{ IDC_BTN_RESET, 0, 1, 0, 1 },
	{ IDC_LABEL_ENGINE, 0, 1, 0, 1 },
	{ IDC_COMBO_ENGINE, 0, 1, 0, 1 },
	{ IDC_LABEL_EDGE, 0, 1, 0, 1 },
	{ IDC_COMBO_EDGE, 0, 1, 0, 1 },
	{ IDC_CHECK_PSEUDO_TRANSPARENT, 0, 1, 0, 1 },
	{ IDOK, 1, 1, 1, 1 },
	{ IDCANCEL, 1, 1, 1, 1 },
	{ IDC_BTN_APPLY, 1, 1, 1, 1 },
};

static const CRect resize_min_max(620, 381, 0, 0);

CDialogConf::CDialogConf(panel_window* p_parent) : m_parent(p_parent), m_resizer(resize_data, resize_min_max)
{
	m_caption << jsp::component_name << " Configuration (id:" << m_parent->m_script_info.id << ")";
}

BOOL CDialogConf::OnInitDialog(HWND, LPARAM)
{
	// Init
	m_edge_combo = GetDlgItem(IDC_COMBO_EDGE);
	m_engine_combo = GetDlgItem(IDC_COMBO_ENGINE);
	m_pseudo_check = GetDlgItem(IDC_CHECK_PSEUDO_TRANSPARENT);

	BuildMenu();

	// Set caption text
	uSetWindowText(m_hWnd, m_caption);

	// Apply window placement
	if (g_config.m_conf_wndpl.length == 0)
	{
		g_config.m_conf_wndpl.length = sizeof(WINDOWPLACEMENT);
		memset(&g_config.m_conf_wndpl, 0, sizeof(WINDOWPLACEMENT));
	}
	else
	{
		SetWindowPlacement(&g_config.m_conf_wndpl);
	}

	// Script Engine
	m_engine_combo.AddString(L"Chakra");
	m_engine_combo.AddString(L"JScript");

	if (helpers::supports_chakra())
	{
		uComboBox_SelectString(m_engine_combo, m_parent->m_script_engine_str);
	}
	else
	{
		uComboBox_SelectString(m_engine_combo, "JScript");
		m_engine_combo.EnableWindow(false);
	}

	// Edge Style & Pseudo Transparent
	m_edge_combo.AddString(L"None");
	m_edge_combo.AddString(L"Sunken");
	m_edge_combo.AddString(L"Grey");

	if (m_parent->is_dui())
	{
		// disable both options in DUI
		m_edge_combo.SetCurSel(0);
		m_edge_combo.EnableWindow(false);

		m_pseudo_check.SetCheck(false);
		m_pseudo_check.EnableWindow(false);
	}
	else
	{
		m_edge_combo.SetCurSel(static_cast<int>(m_parent->m_edge_style));

		m_pseudo_check.SetCheck(m_parent->m_pseudo_transparent);
	}

	// Edit Control
	m_editorctrl.SubclassWindow(GetDlgItem(IDC_EDIT));
	m_editorctrl.SetJScript();
	m_editorctrl.SetContent(m_parent->m_script_code);
	m_editorctrl.EmptyUndoBuffer();
	m_editorctrl.SetSavePoint();

	return FALSE;
}

LRESULT CDialogConf::OnNotify(int, LPNMHDR pnmh)
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

void CDialogConf::Apply()
{
	// Save panel settings
	uGetWindowText(m_engine_combo, m_parent->m_script_engine_str);
	m_parent->m_edge_style = static_cast<panel_base::edge_style>(m_edge_combo.GetCurSel());
	m_parent->m_pseudo_transparent = m_pseudo_check.IsChecked();

	// Get script text
	const int len = m_editorctrl.GetTextLength();
	std::vector<char> buff(len + 1);
	m_editorctrl.GetText(buff.size(), buff.data());
	m_parent->m_script_code = buff.data();
	m_parent->update_script();

	// Save point
	m_editorctrl.SetSavePoint();
}

void CDialogConf::BuildMenu()
{
	m_menu = GetMenu();

	pfc::string8_fast base = helpers::get_fb2k_component_path();

	HMENU samples = CreateMenu();

	pfc::string_list_impl folders;
	helpers::list_folders(base + "samples\\", folders);

	size_t i, j, count;

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

		str_vec path_split = helpers::split_string(folder.get_ptr(), "\\");
		uAppendMenu(samples, MF_STRING | MF_POPUP, reinterpret_cast<UINT_PTR>(sub), path_split[path_split.size() - 1].c_str());
	}

	uAppendMenu(m_menu, MF_STRING | MF_POPUP, reinterpret_cast<UINT_PTR>(samples), "Samples");

	HMENU docs = CreateMenu();

	helpers::list_files(base + "docs\\", false, m_docs);
	count = m_docs.get_count();

	for (i = 0; i < count; ++i)
	{
		pfc::string8_fast display;
		uFixAmpersandChars_v2(pfc::string_filename(m_docs[i]), display);
		uAppendMenu(docs, MF_STRING, ID_DOCS_BEGIN + i, display);
	}

	uAppendMenu(m_menu, MF_STRING | MF_POPUP, reinterpret_cast<UINT_PTR>(docs), "Docs");

	HMENU links = CreateMenu();
	uAppendMenu(links, MF_STRING, ID_LINKS_BEGIN, "Home page");
	uAppendMenu(links, MF_STRING, ID_LINKS_BEGIN + 1, "Changelog");
	uAppendMenu(links, MF_STRING, ID_LINKS_BEGIN + 2, "Releases");
	uAppendMenu(links, MF_STRING, ID_LINKS_BEGIN + 3, "Report an issue");
	uAppendMenu(m_menu, MF_STRING | MF_POPUP, reinterpret_cast<UINT_PTR>(links), "Links");
}

void CDialogConf::OnCloseCmd(UINT, int nID, HWND)
{
	GetWindowPlacement(&g_config.m_conf_wndpl);

	switch (nID)
	{
	case IDOK:
		Apply();
		break;
	case IDC_BTN_APPLY:
		Apply();
		return;
	case IDCANCEL:
		if (m_editorctrl.GetModify() && uMessageBox(m_hWnd, "Unsaved changes will be lost. Are you sure?", jsp::component_name, MB_ICONWARNING | MB_SETFOREGROUND | MB_YESNO) != IDYES)
		{
			return;
		}
	}
	EndDialog(nID);
}

void CDialogConf::OnDocs(UINT, int nID, HWND)
{
	pfc::string8_fast tmp = file_path_display(m_docs[nID - ID_DOCS_BEGIN]).get_ptr();
	string_wide_from_utf8_fast path(tmp);
	ShellExecute(nullptr, L"open", path, nullptr, nullptr, SW_SHOW);
}

void CDialogConf::OnFileImport(UINT, int, HWND)
{
	pfc::string8_fast filename;
	if (uGetOpenFileName(m_hWnd, "Text files|*.txt|JScript files|*.js|All files|*.*", 0, "txt", "Import from", nullptr, filename, FALSE))
	{
		m_editorctrl.SetContent(helpers::read_file(filename));
	}
}

void CDialogConf::OnFileExport(UINT, int, HWND)
{
	pfc::string8_fast filename;
	if (uGetOpenFileName(m_hWnd, "Text files|*.txt|All files|*.*", 0, "txt", "Save as", nullptr, filename, TRUE))
	{
		const int len = m_editorctrl.GetTextLength();
		std::vector<char> buff(len + 1);
		m_editorctrl.GetText(buff.size(), buff.data());
		helpers::write_file(filename, buff.data());
	}
}

void CDialogConf::OnLinks(UINT, int nID, HWND)
{
	static constexpr std::array<const wchar_t*, 4> links =
	{
		L"https://marc2k3.github.io/foo_jscript_panel.html",
		L"https://github.com/marc2k3/foo_jscript_panel/blob/master/CHANGELOG.md",
		L"https://github.com/marc2k3/foo_jscript_panel/releases",
		L"https://github.com/marc2k3/foo_jscript_panel/issues"
	};
	ShellExecute(nullptr, L"open", links[nID - ID_LINKS_BEGIN], nullptr, nullptr, SW_SHOW);
}

void CDialogConf::OnReset(UINT, int, HWND)
{
	uComboBox_SelectString(m_engine_combo, panel_base::get_default_script_engine_str());
	m_edge_combo.SetCurSel(0);
	m_pseudo_check.SetCheck(false);
	m_editorctrl.SetContent(panel_base::get_default_script_code());
}

void CDialogConf::OnSamples(UINT, int nID, HWND)
{
	m_editorctrl.SetContent(helpers::read_file(file_path_display(m_samples[nID - ID_SAMPLES_BEGIN])));
}
