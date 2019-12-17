#pragma once

namespace jsp
{
	static constexpr const char* component_name = "JScript Panel";
	static constexpr const char* component_version = "2.3.4";
	static constexpr const char* component_dll_name = "foo_jscript_panel.dll";
	static constexpr const char* component_info = "Copyright (C) 2015-2019 marc2003\nBased on WSH Panel Mod by T.P. Wang\nThanks for the contributions by TheQwertiest and kbuffington\n\nBuild: " __TIME__ ", " __DATE__ "\nColumns UI SDK Version: " UI_EXTENSION_VERSION;
	static constexpr size_t uwm_refreshbk = WM_USER + 1;
	static constexpr size_t uwm_timer = WM_USER + 2;
	static constexpr size_t uwm_unload = WM_USER + 3;
	static constexpr size_t version = 2340;
	static const wchar_t* class_name = L"jsp_class";
}

#define TO_VARIANT_BOOL(v) (v ? VARIANT_TRUE : VARIANT_FALSE)
#define TO_BSTR(str) SysAllocString(string_wide_from_utf8_fast(str))
#define TO_INT(num) (num == SIZE_MAX ? -1 : static_cast<int>(num))
#define TO_UINT(num) static_cast<unsigned int>(num)

#define GET_PTR(X, Y) \
	X->get__ptr(reinterpret_cast<void**>(&Y)); \
	if (!Y) return E_INVALIDARG;
