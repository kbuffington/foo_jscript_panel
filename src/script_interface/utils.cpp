#include "stdafx.h"
#include "helpers.h"
#include "ui_input_box.h"
#include "utils.h"

Utils::Utils() {}
Utils::~Utils() {}

STDMETHODIMP Utils::CheckComponent(BSTR name, VARIANT_BOOL is_dll, VARIANT_BOOL* p)
{
	if (!p) return E_POINTER;

	service_enum_t<componentversion> e;
	componentversion::ptr ptr;
	string_utf8_from_wide uname(name);
	pfc::string8_fast temp;

	*p = VARIANT_FALSE;

	while (e.next(ptr))
	{
		if (is_dll != VARIANT_FALSE)
		{
			ptr->get_file_name(temp);
		}
		else
		{
			ptr->get_component_name(temp);
		}

		if (_stricmp(temp, uname) == 0)
		{
			*p = VARIANT_TRUE;
			break;
		}
	}

	return S_OK;
}

STDMETHODIMP Utils::CheckFont(BSTR name, VARIANT_BOOL* p)
{
	if (!p) return E_POINTER;

	WCHAR family_name_eng[LF_FACESIZE] = { 0 };
	WCHAR family_name_loc[LF_FACESIZE] = { 0 };
	Gdiplus::InstalledFontCollection font_collection;
	Gdiplus::FontFamily* font_families;
	int count = font_collection.GetFamilyCount();
	int recv;

	*p = VARIANT_FALSE;
	font_families = new Gdiplus::FontFamily[count];
	font_collection.GetFamilies(count, font_families, &recv);

	if (recv == count)
	{
		// Find
		for (int i = 0; i < count; ++i)
		{
			font_families[i].GetFamilyName(family_name_eng, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
			font_families[i].GetFamilyName(family_name_loc);

			if (_wcsicmp(name, family_name_loc) == 0 || _wcsicmp(name, family_name_eng) == 0)
			{
				*p = VARIANT_TRUE;
				break;
			}
		}
	}

	delete[] font_families;
	return S_OK;
}

STDMETHODIMP Utils::ColourPicker(UINT window_id, int default_colour, int* p)
{
	if (!p) return E_POINTER;

	COLORREF COLOR = helpers::convert_argb_to_colorref(default_colour);
	COLORREF COLORS[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	uChooseColor(&COLOR, (HWND)window_id, &COLORS[0]);
	*p = helpers::convert_colorref_to_argb(COLOR);
	return S_OK;
}

STDMETHODIMP Utils::FileTest(BSTR path, BSTR mode, VARIANT* p)
{
	if (!p) return E_POINTER;

	if (wcscmp(mode, L"e") == 0) // exists
	{
		p->vt = VT_BOOL;
		p->boolVal = TO_VARIANT_BOOL(PathFileExists(path));
	}
	else if (wcscmp(mode, L"s") == 0)
	{
		HANDLE fh = CreateFile(path, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
		LARGE_INTEGER size = { 0 };

		if (fh != INVALID_HANDLE_VALUE)
		{
			GetFileSizeEx(fh, &size);
			CloseHandle(fh);
		}

		p->vt = VT_I8;
		p->llVal = size.QuadPart;
	}
	else if (wcscmp(mode, L"d") == 0)
	{
		p->vt = VT_BOOL;
		p->boolVal = TO_VARIANT_BOOL(PathIsDirectory(path));
	}
	else if (wcscmp(mode, L"split") == 0)
	{
		const wchar_t* fn = PathFindFileName(path);
		const wchar_t* ext = PathFindExtension(fn);
		wchar_t dir[MAX_PATH] = { 0 };
		helpers::com_array helper;
		_variant_t vars[3];

		if (!helper.create(_countof(vars))) return E_OUTOFMEMORY;

		vars[0].vt = VT_BSTR;
		vars[0].bstrVal = NULL;
		vars[1].vt = VT_BSTR;
		vars[1].bstrVal = NULL;
		vars[2].vt = VT_BSTR;
		vars[2].bstrVal = NULL;

		if (PathIsFileSpec(fn))
		{
			StringCchCopyN(dir, _countof(dir), path, fn - path);
			PathAddBackslash(dir);

			vars[0].bstrVal = SysAllocString(dir);
			vars[1].bstrVal = SysAllocStringLen(fn, ext - fn);
			vars[2].bstrVal = SysAllocString(ext);
		}
		else
		{
			StringCchCopy(dir, _countof(dir), path);
			PathAddBackslash(dir);

			vars[0].bstrVal = SysAllocString(dir);
		}

		for (LONG i = 0; i < helper.get_count(); ++i)
		{
			if (!helper.put_item(i, vars[i])) return E_OUTOFMEMORY;
		}

		p->vt = VT_VARIANT | VT_ARRAY;
		p->parray = helper.get_ptr();
	}
	else if (wcscmp(mode, L"chardet") == 0)
	{
		p->vt = VT_UI4;
		p->ulVal = helpers::detect_charset(string_utf8_from_wide(path));
	}
	else
	{
		return E_INVALIDARG;
	}
	return S_OK;
}

STDMETHODIMP Utils::FormatDuration(double seconds, BSTR* p)
{
	if (!p) return E_POINTER;

	pfc::string8_fast str;
	str = pfc::format_time_ex(seconds, 0);
	*p = SysAllocString(string_wide_from_utf8_fast(str));
	return S_OK;
}

STDMETHODIMP Utils::FormatFileSize(LONGLONG bytes, BSTR* p)
{
	if (!p) return E_POINTER;

	pfc::string8_fast str;
	str = pfc::format_file_size_short(bytes);
	*p = SysAllocString(string_wide_from_utf8_fast(str));
	return S_OK;
}

STDMETHODIMP Utils::GetAlbumArtAsync(UINT window_id, IMetadbHandle* handle, UINT art_id, VARIANT_BOOL need_stub, VARIANT_BOOL only_embed, VARIANT_BOOL no_load, UINT* p)
{
	if (!p) return E_POINTER;

	t_size cookie = 0;
	metadb_handle* ptr = NULL;
	handle->get__ptr((void**)&ptr);
	if (!ptr) return E_INVALIDARG;

	try
	{
		helpers::album_art_async* task = new helpers::album_art_async((HWND)window_id, ptr, art_id, need_stub != VARIANT_FALSE, only_embed != VARIANT_FALSE, no_load != VARIANT_FALSE);

		if (simple_thread_pool::instance().enqueue(task))
			cookie = reinterpret_cast<t_size>(task);
		else
			delete task;
	}
	catch (...)
	{
		cookie = 0;
	}

	*p = cookie;
	return S_OK;
}

STDMETHODIMP Utils::GetAlbumArtEmbedded(BSTR rawpath, UINT art_id, IGdiBitmap** pp)
{
	if (!pp) return E_POINTER;

	*pp = helpers::get_album_art_embedded(string_utf8_from_wide(rawpath).get_ptr(), art_id);
	return S_OK;
}

STDMETHODIMP Utils::GetAlbumArtV2(IMetadbHandle* handle, UINT art_id, VARIANT_BOOL need_stub, IGdiBitmap** pp)
{
	if (!pp) return E_POINTER;

	metadb_handle* ptr = NULL;
	handle->get__ptr((void**)&ptr);
	if (!ptr) return E_INVALIDARG;

	pfc::string8_fast dummy;
	*pp = helpers::get_album_art(ptr, art_id, need_stub != VARIANT_FALSE, dummy);
	return S_OK;
}

STDMETHODIMP Utils::GetSysColour(UINT index, int* p)
{
	if (!p) return E_POINTER;

	*p = 0;
	if (::GetSysColorBrush(index) != NULL)
	{
		*p = helpers::convert_colorref_to_argb(::GetSysColor(index));
	}
	return S_OK;
}

STDMETHODIMP Utils::GetSystemMetrics(UINT index, int* p)
{
	if (!p) return E_POINTER;

	*p = ::GetSystemMetrics(index);
	return S_OK;
}

STDMETHODIMP Utils::Glob(BSTR pattern, UINT exc_mask, UINT inc_mask, VARIANT* p)
{
	if (!p) return E_POINTER;

	pfc::string8_fast path = string_utf8_from_wide(pattern).get_ptr();
	const char* fn = path.get_ptr() + path.scan_filename();
	pfc::string8_fast dir(path.get_ptr(), fn - path.get_ptr());
	puFindFile ff = uFindFirstFile(path.get_ptr());

	pfc::string_list_impl files;

	if (ff)
	{
		do
		{
			DWORD attr = ff->GetAttributes();

			if ((attr& inc_mask) && !(attr& exc_mask))
			{
				pfc::string8_fast fullpath = dir;
				fullpath.add_string(ff->GetFileName());
				files.add_item(fullpath.get_ptr());
			}
		} while (ff->FindNext());
	}

	delete ff;
	ff = NULL;

	helpers::com_array helper;

	if (!helper.create(files.get_count())) return E_OUTOFMEMORY;

	for (LONG i = 0; i < helper.get_count(); ++i)
	{
		_variant_t var;
		var.vt = VT_BSTR;
		var.bstrVal = SysAllocString(string_wide_from_utf8_fast(files[i]).get_ptr());

		if (!helper.put_item(i, var)) return E_OUTOFMEMORY;
	}

	p->vt = VT_ARRAY | VT_VARIANT;
	p->parray = helper.get_ptr();
	return S_OK;
}

STDMETHODIMP Utils::InputBox(UINT window_id, BSTR prompt, BSTR caption, BSTR def, VARIANT_BOOL error_on_cancel, BSTR* p)
{
	if (!p) return E_POINTER;

	modal_dialog_scope scope;
	if (scope.can_create())
	{
		string_utf8_from_wide uprompt(prompt);
		string_utf8_from_wide ucaption(caption);
		string_utf8_from_wide udef(def);

		scope.initialize(HWND(window_id));
		CInputBox dlg(uprompt, ucaption, udef);
		int status = dlg.DoModal(HWND(window_id));
		if (status == IDOK)
		{
			pfc::string8 val;
			dlg.GetValue(val);
			*p = SysAllocString(string_wide_from_utf8_fast(val));
		}
		else if (status == IDCANCEL)
		{
			if (error_on_cancel != VARIANT_FALSE)
			{
				return E_FAIL;
			}
			*p = SysAllocString(def);
		}
	}
	return S_OK;
}

STDMETHODIMP Utils::IsKeyPressed(UINT vkey, VARIANT_BOOL* p)
{
	if (!p) return E_POINTER;

	*p = TO_VARIANT_BOOL(::IsKeyPressed(vkey));
	return S_OK;
}

STDMETHODIMP Utils::ListFiles(BSTR folder, VARIANT_BOOL recur, VARIANT* p)
{
	if (!p) return E_POINTER;

	pfc::string_list_impl list;
	helpers::list(string_utf8_from_wide(folder).get_ptr(), true, recur != VARIANT_FALSE, list);

	LONG count = list.get_count();
	helpers::com_array helper;
	if (!helper.create(count)) return E_OUTOFMEMORY;
	for (LONG i = 0; i < count; ++i)
	{
		_variant_t var;
		var.vt = VT_BSTR;
		var.bstrVal = SysAllocString(string_wide_from_utf8_fast(file_path_display(list.get_item(i))));
		if (!helper.put_item(i, var)) return E_OUTOFMEMORY;
	}
	p->vt = VT_ARRAY | VT_VARIANT;
	p->parray = helper.get_ptr();
	return S_OK;
}

STDMETHODIMP Utils::ListFolders(BSTR folder, VARIANT* p)
{
	if (!p) return E_POINTER;

	pfc::string_list_impl list;
	helpers::list(string_utf8_from_wide(folder).get_ptr(), false, false, list);

	LONG count = list.get_count();
	helpers::com_array helper;
	if (!helper.create(count)) return E_OUTOFMEMORY;
	for (LONG i = 0; i < count; ++i)
	{
		pfc::string8_fast path = file_path_display(list.get_item(i)).get_ptr();
		path.add_char('\\');

		_variant_t var;
		var.vt = VT_BSTR;
		var.bstrVal = SysAllocString(string_wide_from_utf8_fast(path));
		if (!helper.put_item(i, var)) return E_OUTOFMEMORY;
	}
	p->vt = VT_ARRAY | VT_VARIANT;
	p->parray = helper.get_ptr();
	return S_OK;
}

STDMETHODIMP Utils::MapString(BSTR str, UINT lcid, UINT flags, BSTR* p)
{
	if (!p) return E_POINTER;

	int r = ::LCMapStringW(lcid, flags, str, wcslen(str) + 1, NULL, 0);
	if (!r) return E_FAIL;
	wchar_t* dst = new wchar_t[r];
	r = ::LCMapStringW(lcid, flags, str, wcslen(str) + 1, dst, r);
	if (r) *p = SysAllocString(dst);
	delete[] dst;
	return S_OK;
}

STDMETHODIMP Utils::PathWildcardMatch(BSTR pattern, BSTR str, VARIANT_BOOL* p)
{
	if (!p) return E_POINTER;

	*p = TO_VARIANT_BOOL(PathMatchSpec(str, pattern));
	return S_OK;
}

STDMETHODIMP Utils::ReadINI(BSTR filename, BSTR section, BSTR key, VARIANT defaultval, BSTR* p)
{
	if (!p) return E_POINTER;

	enum
	{
		BUFFER_LEN = 255
	};
	TCHAR buff[BUFFER_LEN] = { 0 };

	GetPrivateProfileString(section, key, NULL, buff, BUFFER_LEN, filename);

	if (!buff[0])
	{
		_variant_t var;

		if (SUCCEEDED(VariantChangeType(&var, &defaultval, 0, VT_BSTR)))
		{
			*p = SysAllocString(var.bstrVal);
			return S_OK;
		}
	}

	*p = SysAllocString(buff);
	return S_OK;
}

STDMETHODIMP Utils::ReadTextFile(BSTR filename, UINT codepage, BSTR* p)
{
	if (!p) return E_POINTER;

	pfc::array_t<wchar_t> content;
	if (helpers::read_file_wide(codepage, filename, content))
	{
		*p = SysAllocString(content.get_ptr());
	}
	else
	{
		*p = SysAllocString(L"");
	}
	return S_OK;
}

STDMETHODIMP Utils::WriteINI(BSTR filename, BSTR section, BSTR key, VARIANT val, VARIANT_BOOL* p)
{
	if (!p) return E_POINTER;

	_variant_t var;
	if (FAILED(VariantChangeType(&var, &val, 0, VT_BSTR))) return E_INVALIDARG;
	*p = TO_VARIANT_BOOL(WritePrivateProfileString(section, key, var.bstrVal, filename));
	return S_OK;
}

STDMETHODIMP Utils::WriteTextFile(BSTR filename, BSTR content, VARIANT_BOOL write_bom, VARIANT_BOOL* p)
{
	if (!p) return E_POINTER;

	if (filename == NULL || content == NULL)
	{
		*p = VARIANT_FALSE;
	}
	else
	{
		pfc::string8_fast filename8 = string_utf8_from_wide(filename).get_ptr();
		pfc::string8_fast content8 = string_utf8_from_wide(content).get_ptr();
		*p = TO_VARIANT_BOOL(helpers::write_file(filename8, content8, write_bom != VARIANT_FALSE));
	}
	return S_OK;
}

STDMETHODIMP Utils::get_Version(UINT* v)
{
	if (!v) return E_POINTER;

	*v = 2200;
	return S_OK;
}
