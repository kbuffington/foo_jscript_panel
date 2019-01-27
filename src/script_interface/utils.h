#pragma once
#include "script_interface.h"

class Utils : public IDispatchImpl3<IUtils>
{
protected:
	Utils();
	virtual ~Utils();

public:
	STDMETHODIMP CheckComponent(BSTR name, VARIANT_BOOL is_dll, VARIANT_BOOL* p);
	STDMETHODIMP CheckFont(BSTR name, VARIANT_BOOL* p);
	STDMETHODIMP ColourPicker(UINT window_id, int default_colour, int* p);
	STDMETHODIMP FileTest(BSTR path, BSTR mode, VARIANT* p);
	STDMETHODIMP FormatDuration(double seconds, BSTR* p);
	STDMETHODIMP FormatFileSize(LONGLONG bytes, BSTR* p);
	STDMETHODIMP GetAlbumArtAsync(UINT window_id, IMetadbHandle* handle, UINT art_id, VARIANT_BOOL need_stub, VARIANT_BOOL only_embed, VARIANT_BOOL no_load, UINT* p);
	STDMETHODIMP GetAlbumArtEmbedded(BSTR rawpath, UINT art_id, IGdiBitmap** pp);
	STDMETHODIMP GetAlbumArtV2(IMetadbHandle* handle, UINT art_id, VARIANT_BOOL need_stub, IGdiBitmap** pp);
	STDMETHODIMP GetSysColour(UINT index, int* p);
	STDMETHODIMP GetSystemMetrics(UINT index, int* p);
	STDMETHODIMP Glob(BSTR pattern, UINT exc_mask, UINT inc_mask, VARIANT* p);
	STDMETHODIMP InputBox(UINT window_id, BSTR prompt, BSTR caption, BSTR def, VARIANT_BOOL error_on_cancel, BSTR* p);
	STDMETHODIMP IsKeyPressed(UINT vkey, VARIANT_BOOL* p);
	STDMETHODIMP ListFiles(BSTR folder, VARIANT_BOOL recur, VARIANT* p);
	STDMETHODIMP ListFolders(BSTR folder, VARIANT* p);
	STDMETHODIMP MapString(BSTR str, UINT lcid, UINT flags, BSTR* p);
	STDMETHODIMP PathWildcardMatch(BSTR pattern, BSTR str, VARIANT_BOOL* p);
	STDMETHODIMP ReadINI(BSTR filename, BSTR section, BSTR key, VARIANT defaultval, BSTR* p);
	STDMETHODIMP ReadTextFile(BSTR filename, UINT codepage, BSTR* p);
	STDMETHODIMP WriteINI(BSTR filename, BSTR section, BSTR key, VARIANT val, VARIANT_BOOL* p);
	STDMETHODIMP WriteTextFile(BSTR filename, BSTR content, VARIANT_BOOL write_bom, VARIANT_BOOL* p);
	STDMETHODIMP get_Version(UINT* v);
};
