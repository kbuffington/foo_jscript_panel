#pragma once
#include "drop_impl.h"

class panel_window;

class host_drop_target : public IDropTargetImpl
{
protected:
	virtual void FinalRelease() {}

public:
	host_drop_target(panel_window* host);
	~host_drop_target();

	COM_QI_ONE(IDropTarget)

	HRESULT OnDragEnter(IDataObject* pDataObj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect) override;
	HRESULT OnDragLeave() override;
	HRESULT OnDragOver(DWORD grfKeyState, POINTL pt, DWORD* pdwEffect) override;
	HRESULT OnDrop(IDataObject* pDataObj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect) override;
	void invoke(callback_id id, DWORD keyState, const POINTL& pt, IDropSourceAction* action);

private:
	DropSourceAction* m_action;
	DWORD m_allowed_effect;
	panel_window* m_host;
};
