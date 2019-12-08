#pragma once

class IDropSourceImpl : public ImplementCOMRefCounter<IDropSource>
{
public:
	IDropSourceImpl() : m_effect(DROPEFFECT_NONE) {}
	virtual ~IDropSourceImpl() {}

	COM_QI_ONE(IDropSource)

	STDMETHODIMP GiveFeedback(DWORD dwEffect) override
	{
		m_effect = dwEffect;
		return DRAGDROP_S_USEDEFAULTCURSORS;
	}

	STDMETHODIMP QueryContinueDrag(BOOL fEscapePressed, DWORD grfKeyState) override
	{
		if (fEscapePressed || (grfKeyState & MK_RBUTTON) || (grfKeyState & MK_MBUTTON))
		{
			return DRAGDROP_S_CANCEL;
		}

		if (!(grfKeyState & MK_LBUTTON))
		{
			return m_effect == DROPEFFECT_NONE ? DRAGDROP_S_CANCEL : DRAGDROP_S_DROP;
		}

		return S_OK;
	}

private:
	DWORD m_effect;
};

_COM_SMARTPTR_TYPEDEF(IDropTargetHelper, IID_IDropTargetHelper);

class IDropTargetImpl : public IDropTarget
{
public:
	IDropTargetImpl(HWND hwnd = nullptr) : m_hwnd(hwnd)
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

		HRESULT hr = S_OK;
		try
		{
			if (m_drop_target_helper)
			{
				POINT point = { pt.x, pt.y };
				m_drop_target_helper->DragEnter(m_hwnd, pDataObj, &point, *pdwEffect);
			}

			hr = OnDragEnter(pDataObj, grfKeyState, pt, pdwEffect);
		}
		catch (...)
		{
			return E_FAIL;
		}
		return hr;
	}

	STDMETHODIMP DragLeave() override
	{
		HRESULT hr = S_OK;

		try
		{
			if (m_drop_target_helper)
			{
				m_drop_target_helper->DragLeave();
			}

			hr = OnDragLeave();
		}
		catch (...)
		{
			return E_FAIL;
		}
		return hr;
	}


	STDMETHODIMP DragOver(DWORD grfKeyState, POINTL pt, DWORD* pdwEffect) override
	{
		if (pdwEffect == nullptr) return E_POINTER;

		HRESULT hr = S_OK;

		try
		{
			if (m_drop_target_helper)
			{
				POINT point = { pt.x, pt.y };
				m_drop_target_helper->DragOver(&point, *pdwEffect);
			}

			hr = OnDragOver(grfKeyState, pt, pdwEffect);
		}
		catch (...)
		{
			return E_FAIL;
		}
		return hr;
	}

	STDMETHODIMP Drop(IDataObject* pDataObj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect) override
	{
		if (pDataObj == nullptr) return E_FAIL;
		if (pdwEffect == nullptr) return E_POINTER;

		HRESULT hr = S_OK;

		try
		{
			if (m_drop_target_helper)
			{
				POINT point = { pt.x, pt.y };
				m_drop_target_helper->Drop(pDataObj, &point, *pdwEffect);
			}

			hr = OnDrop(pDataObj, grfKeyState, pt, pdwEffect);
		}
		catch (...)
		{
			return E_FAIL;
		}
		return hr;
	}

protected:
	HWND m_hwnd;
	IDropTargetHelperPtr m_drop_target_helper;
};
