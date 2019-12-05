#pragma once

#define JSP_NAME "JScript Panel"
#define JSP_DLL_NAME "foo_jscript_panel.dll"

// Remember to bump utils.version too
#define JSP_VERSION_NUMBER "2.3.4"
#define JSP_VERSION_TEST "-dev"

#ifdef _DEBUG
#define JSP_VERSION_DEBUG_SUFFIX " (Debug)"
#else
#define JSP_VERSION_DEBUG_SUFFIX ""
#endif

#define JSP_VERSION JSP_VERSION_NUMBER JSP_VERSION_TEST JSP_VERSION_DEBUG_SUFFIX
#define JSP_NAME_VERSION JSP_NAME " v" JSP_VERSION

#define TO_VARIANT_BOOL(v) (v ? VARIANT_TRUE : VARIANT_FALSE)
#define TO_BSTR(str) SysAllocString(string_wide_from_utf8_fast(str))
#define TO_INT(num) (num == SIZE_MAX ? -1 : static_cast<int>(num))

#define GET_PTR(X, Y) \
	X->get__ptr(reinterpret_cast<void**>(&Y)); \
	if (!Y) return E_INVALIDARG;

#define UWM_TIMER WM_USER + 1
#define UWM_UNLOAD WM_USER + 2
