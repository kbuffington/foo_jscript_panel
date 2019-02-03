#pragma once
#include "script_interface.h"

#include <OleIdl.h>
#include <ShlObj.h>

class js_panel_window;

class IDropSourceImpl : public IDropSource
{
public:
	IDropSourceImpl();
	virtual ~IDropSourceImpl();

	STDMETHODIMP GiveFeedback(DWORD dwEffect);
	STDMETHODIMP QueryContinueDrag(BOOL fEscapePressed, DWORD grfKeyState);
	ULONG STDMETHODCALLTYPE AddRef();
	ULONG STDMETHODCALLTYPE Release();

private:
	DWORD m_dwLastEffect;
	LONG m_refCount;

	BEGIN_COM_QI_IMPL()
		COM_QI_ENTRY_MULTI(IUnknown, IDropSource)
		COM_QI_ENTRY(IDropSource)
	END_COM_QI_IMPL()
};

_COM_SMARTPTR_TYPEDEF(IDropTargetHelper, IID_IDropTargetHelper);

class IDropTargetImpl : public IDropTarget
{
public:
	IDropTargetImpl(HWND hWnd = NULL);
	virtual ~IDropTargetImpl();

	HRESULT RegisterDragDrop();
	HRESULT RevokeDragDrop();
	HWND GetHWND();
	STDMETHODIMP DragEnter(IDataObject* pDataObj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect);
	STDMETHODIMP DragLeave();
	STDMETHODIMP DragOver(DWORD grfKeyState, POINTL pt, DWORD* pdwEffect);
	STDMETHODIMP Drop(IDataObject* pDataObj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect);
	virtual HRESULT OnDragEnter(IDataObject* pDataObj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect);
	virtual HRESULT OnDragLeave();
	virtual HRESULT OnDragOver(DWORD grfKeyState, POINTL pt, DWORD* pdwEffect);
	virtual HRESULT OnDrop(IDataObject* pDataObj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect);
	void SetHWND(HWND hWnd);

protected:
	HWND m_hWnd;
	IDropTargetHelperPtr m_dropTargetHelper;
};

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
