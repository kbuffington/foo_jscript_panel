#include "stdafx.h"

// Script TypeLib
ITypeLibPtr g_typelib;

namespace
{
	DECLARE_COMPONENT_VERSION(COMPONENT_NAME, COMPONENT_VERSION, COMPONENT_INFO);

	VALIDATE_COMPONENT_FILENAME(COMPONENT_DLL_NAME);

	CAppModule _Module;

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

	GdiplusScope scope;
}
