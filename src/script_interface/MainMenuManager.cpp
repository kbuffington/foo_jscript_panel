#include "stdafx.h"
#include "MainMenuManager.h"

MainMenuManager::MainMenuManager() {}
MainMenuManager::~MainMenuManager() {}

void MainMenuManager::FinalRelease()
{
	m_mm.release();
}

STDMETHODIMP MainMenuManager::BuildMenu(IMenuObj* p, UINT base_id)
{
	if (m_mm.is_empty()) return E_POINTER;

	HMENU menuid;
	p->get__ID(&menuid);

	// HACK: workaround for foo_menu_addons
	try
	{
		m_mm->generate_menu_win32(menuid, base_id, SIZE_MAX, mainmenu_manager::flag_show_shortcuts);
	}
	catch (...) {}

	return S_OK;
}

STDMETHODIMP MainMenuManager::ExecuteByID(UINT id, VARIANT_BOOL* p)
{
	if (m_mm.is_empty() || !p) return E_POINTER;

	*p = TO_VARIANT_BOOL(m_mm->execute_command(id));
	return S_OK;
}

STDMETHODIMP MainMenuManager::Init(BSTR root_name)
{
	using t_valid_root_name = std::pair<const wchar_t*, const GUID*>;

	static const std::vector<t_valid_root_name> valid_root_names =
	{
		{ L"file", &mainmenu_groups::file },
		{ L"edit", &mainmenu_groups::edit },
		{ L"view", &mainmenu_groups::view },
		{ L"playback", &mainmenu_groups::playback },
		{ L"library", &mainmenu_groups::library },
		{ L"help", &mainmenu_groups::help }
	};

	const auto it = FIND_IF(valid_root_names, [&](const t_valid_root_name& item) { return _wcsicmp(root_name, item.first) == 0; });
	if (it != valid_root_names.end())
	{
		m_mm = standard_api_create_t<mainmenu_manager>();
		m_mm->instantiate(*it->second);
		return S_OK;
	}
	return E_INVALIDARG;
}
