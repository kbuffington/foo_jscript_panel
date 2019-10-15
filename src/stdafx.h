#pragma once

#define _WIN32_WINNT _WIN32_WINNT_WINXP
#define WINVER _WIN32_WINNT_WINXP

#include <algorithm>
#include <atomic>
#include <filesystem>
#include <fstream>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <vector>

#define NOMINMAX
namespace Gdiplus
{
	using std::min;
	using std::max;
};

#include <WinSock2.h>
#include <Windows.h>
#include <windowsx.h>
#include <GdiPlus.h>
#include <ActivScp.h>
#include <ComDef.h>
#include <ShlObj.h>

#include <Scintilla.h>
#include <SciLexer.h>

#include <resource.h>
#include <foobar2000/helpers/foobar2000+atl.h>
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
using str_list = std::vector<std::string>;
