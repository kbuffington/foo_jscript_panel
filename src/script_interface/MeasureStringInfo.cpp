#include "stdafx.h"
#include "MeasureStringInfo.h"

MeasureStringInfo::MeasureStringInfo(float x, float y, float w, float h, int l, int c) : m_x(x), m_y(y), m_w(w), m_h(h), m_l(l), m_c(c) {}
MeasureStringInfo::~MeasureStringInfo() {}

STDMETHODIMP MeasureStringInfo::get_chars(int* p)
{
	if (!p) return E_POINTER;

	*p = m_c;
	return S_OK;
}

STDMETHODIMP MeasureStringInfo::get_height(float* p)
{
	if (!p) return E_POINTER;

	*p = m_h;
	return S_OK;
}

STDMETHODIMP MeasureStringInfo::get_lines(int* p)
{
	if (!p) return E_POINTER;

	*p = m_l;
	return S_OK;
}

STDMETHODIMP MeasureStringInfo::get_width(float* p)
{
	if (!p) return E_POINTER;

	*p = m_w;
	return S_OK;
}

STDMETHODIMP MeasureStringInfo::get_x(float* p)
{
	if (!p) return E_POINTER;

	*p = m_x;
	return S_OK;
}

STDMETHODIMP MeasureStringInfo::get_y(float* p)
{
	if (!p) return E_POINTER;

	*p = m_y;
	return S_OK;
}
