#pragma once
#include "drop_impl.h"
#include "script_interface_impl.h"

class js_panel_window;

class host_drop_target : public IDropTargetImpl
{
protected:
	virtual void FinalRelease() {}

public:
	host_drop_target(js_panel_window* host);
	virtual ~host_drop_target();

	HRESULT OnDragEnter(IDataObject* pDataObj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect);
	HRESULT OnDragLeave();
	HRESULT OnDragOver(DWORD grfKeyState, POINTL pt, DWORD* pdwEffect);
	HRESULT OnDrop(IDataObject* pDataObj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect);
	void on_drag_enter(DWORD keyState, POINTL& pt, IDropSourceAction* action);
	void on_drag_leave();
	void on_drag_over(DWORD keyState, POINTL& pt, IDropSourceAction* action);
	void on_drag_drop(DWORD keyState, POINTL& pt, IDropSourceAction* action);

private:
	DropSourceAction* m_action;
	DWORD m_fb2kAllowedEffect;
	js_panel_window* m_host;

	BEGIN_COM_QI_IMPL()
		COM_QI_ENTRY_MULTI(IUnknown, IDropTarget)
		COM_QI_ENTRY(IDropTarget)
	END_COM_QI_IMPL()
};
