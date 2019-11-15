#include "stdafx.h"
#include "console.h"

Console::Console() {}
Console::~Console() {}

STDMETHODIMP Console::Log(SAFEARRAY* p)
{
	pfc::string8_fast str;
	LONG nLBound = 0, nUBound = -1;

	HRESULT hr = SafeArrayGetLBound(p, 1, &nLBound);
	if (SUCCEEDED(hr)) hr = SafeArrayGetUBound(p, 1, &nUBound);
	if (FAILED(hr)) return hr;

	for (LONG i = nLBound; i <= nUBound; ++i)
	{
		_variant_t var;
		LONG n = i;

		if (FAILED(SafeArrayGetElement(p, &n, &var)))
			continue;

		if (FAILED(VariantChangeType(&var, &var, VARIANT_ALPHABOOL, VT_BSTR)))
			continue;

		str.add_string(string_utf8_from_wide(var.bstrVal));

		if (i < nUBound)
		{
			str.add_byte(' ');
		}
	}
	FB2K_console_formatter() << str;
	return S_OK;
}
