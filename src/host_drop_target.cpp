#include "stdafx.h"
#include "host_drop_target.h"

host_drop_target::host_drop_target(panel_window* host) : IDropTargetImpl(host->m_hwnd), m_host(host), m_action(new com_object_impl_t<DropSourceAction, true>()) {}

host_drop_target::~host_drop_target()
{
	m_action->Release();
}

HRESULT host_drop_target::OnDragEnter(IDataObject* pDataObj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect)
{
	if (!pdwEffect) return E_POINTER;

	m_action->Reset();

	bool native;
	if (SUCCEEDED(ole_interaction::get()->check_dataobject(pDataObj, m_allowed_effect, native)))
	{
		if (native && (*pdwEffect & DROPEFFECT_MOVE))
		{
			m_allowed_effect |= DROPEFFECT_MOVE; // Remove check_dataobject move suppression for intra fb2k interactions
		}
	}
	else
	{
		m_allowed_effect = DROPEFFECT_NONE;
	}

	m_action->m_effect = *pdwEffect & m_allowed_effect;

	m_hwnd.ScreenToClient(reinterpret_cast<LPPOINT>(&pt));
	invoke(callback_id::on_drag_enter, grfKeyState, pt, m_action);

	*pdwEffect = m_action->m_effect;
	return S_OK;
}

HRESULT host_drop_target::OnDragLeave()
{
	m_host->script_invoke(callback_id::on_drag_leave);
	return S_OK;
}

HRESULT host_drop_target::OnDragOver(DWORD grfKeyState, POINTL pt, DWORD* pdwEffect)
{
	if (!pdwEffect) return E_POINTER;

	m_action->m_effect = *pdwEffect & m_allowed_effect;

	m_hwnd.ScreenToClient(reinterpret_cast<LPPOINT>(&pt));
	invoke(callback_id::on_drag_over, grfKeyState, pt, m_action);

	*pdwEffect = m_action->m_effect;
	return S_OK;
}

HRESULT host_drop_target::OnDrop(IDataObject* pDataObj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect)
{
	if (!pdwEffect) return E_POINTER;

	m_action->m_effect = *pdwEffect & m_allowed_effect;

	m_hwnd.ScreenToClient(reinterpret_cast<LPPOINT>(&pt));
	invoke(callback_id::on_drag_drop, grfKeyState, pt, m_action);

	if (*pdwEffect == DROPEFFECT_NONE || m_action->m_effect == DROPEFFECT_NONE)
	{
		*pdwEffect = DROPEFFECT_NONE;
		return S_OK;
	}

	dropped_files_data_impl droppedData;
	HRESULT hr = ole_interaction::get()->parse_dataobject(pDataObj, droppedData);
	if (SUCCEEDED(hr))
	{
		droppedData.to_handles_async_ex(
			playlist_incoming_item_filter_v2::op_flag_delay_ui,
			core_api::get_main_window(),
			fb2k::service_new<helpers::js_process_locations>(m_action->m_to_select, m_action->m_base, m_action->m_playlist_idx)
		);
	}

	*pdwEffect = m_action->m_effect;
	return S_OK;
}

void host_drop_target::invoke(callback_id id, DWORD keyState, const POINTL& pt, IDropSourceAction* action)
{
	VARIANTARG args[4];
	args[0].vt = VT_I4;
	args[0].lVal = keyState;
	args[1].vt = VT_I4;
	args[1].lVal = pt.y;
	args[2].vt = VT_I4;
	args[2].lVal = pt.x;
	args[3].vt = VT_DISPATCH;
	args[3].pdispVal = action;
	m_host->script_invoke(id, args, _countof(args));
}
