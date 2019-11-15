#include "stdafx.h"
#include "ContextMenuManager.h"

ContextMenuManager::ContextMenuManager() {}
ContextMenuManager::~ContextMenuManager() {}

void ContextMenuManager::FinalRelease()
{
	m_cm.release();
}

STDMETHODIMP ContextMenuManager::BuildMenu(IMenuObj* p, int base_id)
{
	if (m_cm.is_empty()) return E_POINTER;

	HMENU menuid;
	p->get__ID(&menuid);
	contextmenu_node* parent = m_cm->get_root();
	m_cm->win32_build_menu(menuid, parent, base_id, -1);
	return S_OK;
}

STDMETHODIMP ContextMenuManager::ExecuteByID(UINT id, VARIANT_BOOL* p)
{
	if (m_cm.is_empty() || !p) return E_POINTER;

	*p = TO_VARIANT_BOOL(m_cm->execute_by_id(id));
	return S_OK;
}

STDMETHODIMP ContextMenuManager::InitContext(IMetadbHandleList* handles)
{
	metadb_handle_list* handles_ptr = nullptr;
	GET_PTR(handles, handles_ptr)

	contextmenu_manager::g_create(m_cm);
	m_cm->init_context(*handles_ptr, contextmenu_manager::flag_show_shortcuts);
	return S_OK;
}

STDMETHODIMP ContextMenuManager::InitContextPlaylist()
{
	contextmenu_manager::g_create(m_cm);
	m_cm->init_context_playlist(contextmenu_manager::flag_show_shortcuts);
	return S_OK;
}

STDMETHODIMP ContextMenuManager::InitNowPlaying()
{
	contextmenu_manager::g_create(m_cm);
	m_cm->init_context_now_playing(contextmenu_manager::flag_show_shortcuts);
	return S_OK;
}
