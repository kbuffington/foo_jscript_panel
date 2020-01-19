#pragma once

#define _WIN32_WINNT _WIN32_WINNT_WIN7
#define WINVER _WIN32_WINNT_WIN7

#include <algorithm>
#include <array>
#include <atomic>
#include <filesystem>
#include <fstream>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <numeric>
#include <set>
#include <vector>

#define NOMINMAX
namespace Gdiplus
{
	using std::min;
	using std::max;
};

#include "resource.h"
#include <columns_ui-sdk/ui_extension.h>
#include <foobar2000/helpers/foobar2000+atl.h>
#include <foobar2000/helpers/atl-misc.h>

#include <ActivScp.h>
#include <ComDef.h>
#include <GdiPlus.h>
#include <ShlObj.h>

#include "foo_jscript_panel.h"
#include "callback_id.h"
#include "guids.h"

using namespace pfc::stringcvt;
using str_vec = std::vector<std::string>;
