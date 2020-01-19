#include "stdafx.h"

#include <Scintilla.h>
#include <libPPUI/gdiplus_helpers.h>

// Script TypeLib
ITypeLibPtr g_typelib;

namespace jsp
{
	DECLARE_COMPONENT_VERSION(component_name, component_version, component_info);
	VALIDATE_COMPONENT_FILENAME(component_dll_name);

	CAppModule _Module;
	GdiplusScope scope;

	extern "C" BOOL WINAPI DllMain(HINSTANCE ins, DWORD reason, LPVOID lp)
	{
		switch (reason)
		{
		case DLL_PROCESS_ATTACH:
			{
				pfc::string8_fast path;
				uGetModuleFileName(ins, path);
				if (FAILED(LoadTypeLibEx(string_wide_from_utf8_fast(path), REGKIND_NONE, &g_typelib))) return FALSE;

				Scintilla_RegisterClasses(ins);
				_Module.Init(nullptr, ins);
			}
			break;
		case DLL_PROCESS_DETACH:
			_Module.Term();
			Scintilla_ReleaseResources();
			break;
		}
		return TRUE;
	}
}
