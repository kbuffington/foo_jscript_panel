#pragma once

class panel_window;

class IDropSourceImpl : public IDropSource
{
public:
	IDropSourceImpl();
	virtual ~IDropSourceImpl();

	COM_QI_ONE(IDropSource)

	STDMETHODIMP GiveFeedback(DWORD dwEffect) override;
	STDMETHODIMP QueryContinueDrag(BOOL fEscapePressed, DWORD grfKeyState) override;
	ULONG STDMETHODCALLTYPE AddRef() override;
	ULONG STDMETHODCALLTYPE Release() override;

private:
	DWORD m_last_effect;
	LONG m_ref_count;
};

_COM_SMARTPTR_TYPEDEF(IDropTargetHelper, IID_IDropTargetHelper);

class IDropTargetImpl : public IDropTarget
{
public:
	IDropTargetImpl(HWND hwnd = nullptr);
	virtual ~IDropTargetImpl();

	HRESULT RegisterDragDrop();
	HRESULT RevokeDragDrop();
	STDMETHODIMP DragEnter(IDataObject* pDataObj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect) override;
	STDMETHODIMP DragLeave() override;
	STDMETHODIMP DragOver(DWORD grfKeyState, POINTL pt, DWORD* pdwEffect) override;
	STDMETHODIMP Drop(IDataObject* pDataObj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect) override;
	virtual HRESULT OnDragEnter(IDataObject* pDataObj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect) = 0;
	virtual HRESULT OnDragLeave() = 0;
	virtual HRESULT OnDragOver(DWORD grfKeyState, POINTL pt, DWORD* pdwEffect) = 0;
	virtual HRESULT OnDrop(IDataObject* pDataObj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect) = 0;

protected:
	HWND m_hwnd;
	IDropTargetHelperPtr m_drop_target_helper;
};

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
	void on_drag_enter(DWORD keyState, POINTL& pt, IDropSourceAction* action);
	void on_drag_leave();
	void on_drag_over(DWORD keyState, POINTL& pt, IDropSourceAction* action);
	void on_drag_drop(DWORD keyState, POINTL& pt, IDropSourceAction* action);

private:
	DropSourceAction* m_action;
	DWORD m_allowed_effect;
	panel_window* m_host;
};
