#include "stdafx.h"

// Script TypeLib
ITypeLibPtr g_typelib;

namespace
{
	DECLARE_COMPONENT_VERSION(
		JSP_NAME,
		JSP_VERSION,
		JSP_NAME_VERSION " by marc2003\n"
		"Based on WSH Panel Mod by T.P. Wang\n"
		"Thanks for the contributions by TheQwertiest and kbuffington\n\n"
		"Build: " __TIME__ ", " __DATE__ "\n"
		"Columns UI SDK Version: " UI_EXTENSION_VERSION
	);

	VALIDATE_COMPONENT_FILENAME(JSP_DLL_NAME);

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
