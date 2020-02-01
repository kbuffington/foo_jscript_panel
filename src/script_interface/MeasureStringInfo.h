#pragma once
#include "script_interface.h"

class MeasureStringInfo : public IDispatchImpl3<IMeasureStringInfo>
{
protected:
	MeasureStringInfo(float x, float y, float w, float h, int l, int c);
	~MeasureStringInfo();

	float m_x, m_y, m_w, m_h;
	int m_l, m_c;

public:
	STDMETHODIMP get_chars(int* p) override;
	STDMETHODIMP get_height(float* p) override;
	STDMETHODIMP get_lines(int* p) override;
	STDMETHODIMP get_width(float* p) override;
	STDMETHODIMP get_x(float* p) override;
	STDMETHODIMP get_y(float* p) override;
};
