#pragma once

#define _WIN32_WINNT _WIN32_WINNT_WINXP
#define WINVER _WIN32_WINNT_WINXP

#include <atomic>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <vector>

#include <WinSock2.h>
#include <Windows.h>
#include <GdiPlus.h>
#include <ActivScp.h>
#include <ComDef.h>

#include <atlstr.h>
#include <atlwin.h>
#include <atltypes.h>

#include <atlapp.h>
#include <atldlgs.h>
#include <atlctrls.h>
#include <atlcrack.h>

#include <Scintilla.h>
#include <SciLexer.h>

#include <resource.h>
#include <foobar2000/SDK/foobar2000.h>
#include <foobar2000/helpers/atl-misc.h>
#include <foobar2000/helpers/filetimetools.h>
#include <libPPUI/CDialogResizeHelper.h>
#include <libPPUI/CListControlOwnerData.h>
#include <libPPUI/CListControlComplete.h>
#include <libPPUI/CListControl-Cells.h>
#include <columns_ui-sdk/ui_extension.h>

#ifdef _INC_WINDOWSX
#undef SubclassWindow
#endif

#include <component_defines.h>

using namespace pfc::stringcvt;
