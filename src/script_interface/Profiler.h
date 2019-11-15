#pragma once
#include "script_interface.h"

class Profiler : public IDispatchImpl3<IProfiler>
{
protected:
	Profiler(const char* p_name);
	~Profiler();

	pfc::hires_timer m_timer;
	pfc::string_simple m_name;

public:
	STDMETHODIMP Print() override;
	STDMETHODIMP Reset() override;
	STDMETHODIMP get_Time(int* p) override;
};
