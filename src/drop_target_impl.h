#pragma once

_COM_SMARTPTR_TYPEDEF(IDropTargetHelper, IID_IDropTargetHelper);

class IDropTargetImpl : public IDropTarget
{
public:
	IDropTargetImpl(CWindow hwnd = nullptr) : m_hwnd(hwnd)
	{
		m_drop_target_helper.CreateInstance(CLSID_DragDropHelper, nullptr, CLSCTX_INPROC_SERVER);
	}

	virtual ~IDropTargetImpl()
	{
		RevokeDragDrop();
	}

	virtual HRESULT OnDragEnter(IDataObject* pDataObj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect) = 0;
	virtual HRESULT OnDragLeave() = 0;
	virtual HRESULT OnDragOver(DWORD grfKeyState, POINTL pt, DWORD* pdwEffect) = 0;
	virtual HRESULT OnDrop(IDataObject* pDataObj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect) = 0;

	HRESULT RegisterDragDrop()
	{
		return ::RegisterDragDrop(m_hwnd, this);
	}

	HRESULT RevokeDragDrop()
	{
		return ::RevokeDragDrop(m_hwnd);
	}

	STDMETHODIMP DragEnter(IDataObject* pDataObj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect) override
	{
		if (pDataObj == nullptr) return E_FAIL;
		if (pdwEffect == nullptr) return E_POINTER;

		CPoint point(pt.x, pt.y);
		m_drop_target_helper->DragEnter(m_hwnd, pDataObj, &point, *pdwEffect);
		return OnDragEnter(pDataObj, grfKeyState, pt, pdwEffect);
	}

	STDMETHODIMP DragLeave() override
	{
		m_drop_target_helper->DragLeave();
		return OnDragLeave();
	}

	STDMETHODIMP DragOver(DWORD grfKeyState, POINTL pt, DWORD* pdwEffect) override
	{
		if (pdwEffect == nullptr) return E_POINTER;

		CPoint point(pt.x, pt.y);
		m_drop_target_helper->DragOver(&point, *pdwEffect);
		return OnDragOver(grfKeyState, pt, pdwEffect);
	}

	STDMETHODIMP Drop(IDataObject* pDataObj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect) override
	{
		if (pDataObj == nullptr) return E_FAIL;
		if (pdwEffect == nullptr) return E_POINTER;

		CPoint point(pt.x, pt.y);
		m_drop_target_helper->Drop(pDataObj, &point, *pdwEffect);
		return OnDrop(pDataObj, grfKeyState, pt, pdwEffect);
	}

protected:
	CWindow m_hwnd;
	IDropTargetHelperPtr m_drop_target_helper;
};
