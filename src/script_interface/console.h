#pragma once
#include "script_interface.h"

class Console : public IDispatchImpl3<IConsole>
{
protected:
	Console();
	virtual ~Console();

public:
	STDMETHODIMP Log(SAFEARRAY* p);
};
