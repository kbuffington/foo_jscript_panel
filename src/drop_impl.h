#pragma once
#include "script_interface_impl.h"

#include <OleIdl.h>
#include <ShlObj.h>

class IDropSourceImpl : public IDropSource
{
public:
	IDropSourceImpl() : m_refCount(0), m_dwLastEffect(DROPEFFECT_NONE) {};
	virtual ~IDropSourceImpl() {};

	STDMETHODIMP GiveFeedback(DWORD dwEffect)
	{
		m_dwLastEffect = dwEffect;
		return DRAGDROP_S_USEDEFAULTCURSORS;
	}

	STDMETHODIMP QueryContinueDrag(BOOL fEscapePressed, DWORD grfKeyState)
	{
		if (fEscapePressed || (grfKeyState & MK_RBUTTON) || (grfKeyState & MK_MBUTTON))
		{
			return DRAGDROP_S_CANCEL;
		}

		if (!(grfKeyState & MK_LBUTTON))
		{
			return m_dwLastEffect == DROPEFFECT_NONE ? DRAGDROP_S_CANCEL : DRAGDROP_S_DROP;
		}

		return S_OK;
	}

	ULONG STDMETHODCALLTYPE AddRef()
	{
		return InterlockedIncrement(&m_refCount);
	}

	ULONG STDMETHODCALLTYPE Release()
	{
		LONG rv = InterlockedDecrement(&m_refCount);
		if (!rv)
		{
			delete this;
		}
		return rv;
	}

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
protected:
	HWND m_hWnd;
	IDropTargetHelperPtr m_dropTargetHelper;

public:
	IDropTargetImpl(HWND hWnd = NULL) : m_hWnd(hWnd)
	{
		CoCreateInstance(CLSID_DragDropHelper, NULL, CLSCTX_INPROC_SERVER, IID_IDropTargetHelper, (LPVOID*)&m_dropTargetHelper);
	}

	virtual ~IDropTargetImpl()
	{
		RevokeDragDrop();
	}

	HRESULT RegisterDragDrop()
	{
		return ::RegisterDragDrop(m_hWnd, this);
	}

	HRESULT RevokeDragDrop()
	{
		return ::RevokeDragDrop(m_hWnd);
	}

	HWND GetHWND()
	{
		return m_hWnd;
	}

	STDMETHODIMP DragEnter(IDataObject* pDataObj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect)
	{
		if (pDataObj == NULL) return E_FAIL;
		if (pdwEffect == NULL) return E_POINTER;

		HRESULT hr = S_OK;
		try
		{
			if (m_dropTargetHelper)
			{
				POINT point = { pt.x, pt.y };
				m_dropTargetHelper->DragEnter(m_hWnd, pDataObj, &point, *pdwEffect);
			}

			hr = OnDragEnter(pDataObj, grfKeyState, pt, pdwEffect);
			if (FAILED(hr)) return hr;
		}
		catch (...)
		{
			return E_FAIL;
		}
		return hr;
	}

	STDMETHODIMP DragLeave()
	{
		HRESULT hr = S_OK;

		try
		{
			if (m_dropTargetHelper)
			{
				m_dropTargetHelper->DragLeave();
			}

			hr = OnDragLeave();
			if (FAILED(hr)) return hr;
		}
		catch (...)
		{
			return E_FAIL;
		}
		return hr;
	}

	STDMETHODIMP DragOver(DWORD grfKeyState, POINTL pt, DWORD* pdwEffect)
	{
		if (pdwEffect == NULL) return E_POINTER;

		HRESULT hr = S_OK;
		try
		{
			if (m_dropTargetHelper)
			{
				POINT point = { pt.x, pt.y };
				m_dropTargetHelper->DragOver(&point, *pdwEffect);
			}

			hr = OnDragOver(grfKeyState, pt, pdwEffect);
			if (FAILED(hr)) return hr;
		}
		catch (...)
		{
			return E_FAIL;
		}
		return hr;
	}

	STDMETHODIMP Drop(IDataObject* pDataObj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect)
	{
		HRESULT hr = S_OK;

		if (pDataObj == NULL) return E_FAIL;
		if (pdwEffect == NULL) return E_POINTER;

		try
		{
			if (m_dropTargetHelper)
			{
				POINT point = { pt.x, pt.y };
				m_dropTargetHelper->Drop(pDataObj, &point, *pdwEffect);
			}

			hr = OnDrop(pDataObj, grfKeyState, pt, pdwEffect);
			if (FAILED(hr)) return hr;
		}
		catch (...)
		{
			return E_FAIL;
		}
		return hr;
	}

	virtual HRESULT OnDragEnter(IDataObject* pDataObj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect)
	{
		return S_OK;
	}

	virtual HRESULT OnDragLeave()
	{
		return S_OK;
	}

	virtual HRESULT OnDragOver(DWORD grfKeyState, POINTL pt, DWORD* pdwEffect)
	{
		return S_OK;
	}

	virtual HRESULT OnDrop(IDataObject* pDataObj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect)
	{
		return S_OK;
	}

	void SetHWND(HWND hWnd)
	{
		m_hWnd = hWnd;
	}
};
