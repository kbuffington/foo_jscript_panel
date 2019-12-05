#pragma once

#define COMPONENT_NAME "JScript Panel"
#define COMPONENT_DLL_NAME "foo_jscript_panel.dll"
#define COMPONENT_INFO \
	COMPONENT_NAME " by marc2003\n" \
	"Based on WSH Panel Mod by T.P. Wang\n" \
	"Thanks for the contributions by TheQwertiest and kbuffington\n\n" \
	"Build: " __TIME__ ", " __DATE__ "\n" \
	"Columns UI SDK Version: " UI_EXTENSION_VERSION

// Remember to bump utils.version too
#define COMPONENT_VERSION_NUMBER "2.3.4"
#define COMPONENT_VERSION_TEST "-dev"

#ifdef _DEBUG
#define COMPONENT_VERSION COMPONENT_VERSION_NUMBER COMPONENT_VERSION_TEST " (Debug)"
#else
#define COMPONENT_VERSION COMPONENT_VERSION_NUMBER COMPONENT_VERSION_TEST
#endif

#define COMPONENT_NAME_VERSION COMPONENT_NAME " v" COMPONENT_VERSION

#define TO_VARIANT_BOOL(v) (v ? VARIANT_TRUE : VARIANT_FALSE)
#define TO_BSTR(str) SysAllocString(string_wide_from_utf8_fast(str))
#define TO_INT(num) (num == SIZE_MAX ? -1 : static_cast<int>(num))
#define TO_UINT(num) static_cast<unsigned int>(num)

#define GET_PTR(X, Y) \
	X->get__ptr(reinterpret_cast<void**>(&Y)); \
	if (!Y) return E_INVALIDARG;

#define UWM_TIMER WM_USER + 1
#define UWM_UNLOAD WM_USER + 2
