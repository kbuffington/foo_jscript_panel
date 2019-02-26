#pragma once

#define _WIN32_WINNT _WIN32_WINNT_WINXP
#define WINVER _WIN32_WINNT_WINXP
#define _WIN32_IE _WIN32_IE_IE60

#define TO_VARIANT_BOOL(v) ((v) ? (VARIANT_TRUE) : (VARIANT_FALSE))

#include <WinSock2.h>
#include <Windows.h>
#include <GdiPlus.h>
#include <ActivScp.h>
#include <activdbg.h>
#include <ComDef.h>
#include <StrSafe.h>

#include <atlstr.h>
#include <atlwin.h>

#include <atlapp.h>
#include <atlframe.h>
#include <atldlgs.h>
#include <atlctrls.h>
#include <atlddx.h>
#include <atlcrack.h>
#include <atltheme.h>

#include <Scintilla.h>
#include <SciLexer.h>

#include "../foobar2000/SDK/foobar2000.h"
#include "../columns_ui-sdk/ui_extension.h"

#ifdef _INC_WINDOWSX
#undef SubclassWindow
#endif

#include "component_defines.h"
#include "resource.h"

using namespace pfc::stringcvt;
