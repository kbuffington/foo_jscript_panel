#include "stdafx.h"
#include "helpers.h"
#include "ui_input_box.h"
#include "utils.h"

Utils::Utils() : m_colours(16, 0) {}
Utils::~Utils() {}

STDMETHODIMP Utils::Chardet(BSTR filename, UINT* p)
{
	if (!p) return E_POINTER;

	*p = helpers::detect_charset(string_utf8_from_wide(filename));
	return S_OK;
}

STDMETHODIMP Utils::CheckComponent(BSTR name, VARIANT_BOOL* p)
{
	if (!p) return E_POINTER;

	auto uname = string_utf8_from_wide(name);

	*p = VARIANT_FALSE;

	for (auto e = service_enum_t<componentversion>(); !e.finished(); ++e)
	{
		pfc::string8_fast str;
		e.get()->get_file_name(str);
		if (_stricmp(str, uname) == 0)
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

	*p = VARIANT_FALSE;

	wchar_t family_name[LF_FACESIZE] = { 0 };
	Gdiplus::InstalledFontCollection fonts;
	const int count = fonts.GetFamilyCount();
	pfc::ptrholder_t<Gdiplus::FontFamily> families = new Gdiplus::FontFamily[count];
	int found;
	if (families.is_valid() && fonts.GetFamilies(count, families.get_ptr(), &found) == Gdiplus::Ok)
	{
		for (int i = 0; i < found; ++i)
		{
			families.get_ptr()[i].GetFamilyName(family_name);
			if (_wcsicmp(name, family_name) == 0)
			{
				*p = VARIANT_TRUE;
				break;
			}
		}
	}
	return S_OK;
}

STDMETHODIMP Utils::ColourPicker(UINT window_id, int default_colour, int* p)
{
	if (!p) return E_POINTER;

	COLORREF COLOR = helpers::convert_argb_to_colorref(default_colour);
	uChooseColor(&COLOR, reinterpret_cast<HWND>(window_id), &m_colours[0]);
	*p = helpers::convert_colorref_to_argb(COLOR);
	return S_OK;
}

STDMETHODIMP Utils::DateStringToTimestamp(BSTR str, UINT* p)
{
	if (!p) return E_POINTER;

	*p = static_cast<UINT>(pfc::fileTimeWtoU(filetimestamp_from_string(string_utf8_from_wide(str).get_ptr())));
	return S_OK;
}

STDMETHODIMP Utils::FormatDuration(double seconds, BSTR* p)
{
	if (!p) return E_POINTER;

	pfc::string8_fast str = pfc::format_time_ex(seconds, 0).get_ptr();
	*p = TO_BSTR(str);
	return S_OK;
}

STDMETHODIMP Utils::FormatFileSize(__int64 bytes, BSTR* p)
{
	if (!p) return E_POINTER;

	pfc::string8_fast str = pfc::format_file_size_short(bytes);
	*p = TO_BSTR(str);
	return S_OK;
}

STDMETHODIMP Utils::GetAlbumArtAsync(UINT window_id, IMetadbHandle* handle, UINT art_id, VARIANT_BOOL need_stub, VARIANT_BOOL only_embed, VARIANT_BOOL no_load)
{
	metadb_handle* ptr = nullptr;
	GET_PTR(handle, ptr)

	auto task = new helpers::album_art_async(reinterpret_cast<HWND>(window_id), ptr, art_id, need_stub != VARIANT_FALSE, only_embed != VARIANT_FALSE, no_load != VARIANT_FALSE);
	simple_thread_pool::instance().enqueue(task);
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

	metadb_handle* ptr = nullptr;
	GET_PTR(handle, ptr)

	pfc::string8_fast dummy;
	*pp = helpers::get_album_art(ptr, art_id, need_stub != VARIANT_FALSE, false, dummy);
	return S_OK;
}

STDMETHODIMP Utils::GetFileSize(BSTR path, __int64* p)
{
	if (!p) return E_POINTER;

	*p = 0;
	WIN32_FILE_ATTRIBUTE_DATA data;
	if (GetFileAttributesEx(path, GetFileExInfoStandard, &data) && !(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	{
		*p = static_cast<t_filesize>(data.nFileSizeHigh) << 32 | data.nFileSizeLow;
	}
	return S_OK;
}

STDMETHODIMP Utils::GetSysColour(UINT index, int* p)
{
	if (!p) return E_POINTER;

	*p = 0;
	if (::GetSysColorBrush(index) != nullptr)
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
			const DWORD attr = ff->GetAttributes();

			if ((attr& inc_mask) && !(attr& exc_mask))
			{
				pfc::string8_fast fullpath = dir;
				fullpath.add_string(ff->GetFileName());
				files.add_item(fullpath.get_ptr());
			}
		} while (ff->FindNext());
	}

	delete ff;
	ff = nullptr;

	helpers::com_array helper;

	if (!helper.create(files.get_count())) return E_OUTOFMEMORY;

	for (LONG i = 0; i < helper.get_count(); ++i)
	{
		_variant_t var;
		var.vt = VT_BSTR;
		var.bstrVal = TO_BSTR(files[i]);

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
		HWND hwnd = reinterpret_cast<HWND>(window_id);
		scope.initialize(hwnd);

		auto uprompt = string_utf8_from_wide(prompt);
		auto ucaption = string_utf8_from_wide(caption);
		auto udef = string_utf8_from_wide(def);

		CInputBox dlg(uprompt, ucaption, udef);
		const int status = dlg.DoModal(hwnd);
		if (status == IDOK)
		{
			*p = TO_BSTR(dlg.m_value);
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

STDMETHODIMP Utils::IsFile(BSTR filename, VARIANT_BOOL* p)
{
	if (!p) return E_POINTER;

	*p = TO_VARIANT_BOOL(!PathIsDirectory(filename) && PathFileExists(filename));
	return S_OK;
}

STDMETHODIMP Utils::IsFolder(BSTR folder, VARIANT_BOOL* p)
{
	if (!p) return E_POINTER;

	*p = TO_VARIANT_BOOL(PathIsDirectory(folder));
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
	helpers::list_files(string_utf8_from_wide(folder).get_ptr(), recur != VARIANT_FALSE, list);

	const LONG count = list.get_count();
	helpers::com_array helper;
	if (!helper.create(count)) return E_OUTOFMEMORY;
	for (LONG i = 0; i < count; ++i)
	{
		_variant_t var;
		var.vt = VT_BSTR;
		var.bstrVal = TO_BSTR(file_path_display(list[i]));
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
	helpers::list_folders(string_utf8_from_wide(folder).get_ptr(), list);

	const LONG count = list.get_count();
	helpers::com_array helper;
	if (!helper.create(count)) return E_OUTOFMEMORY;
	for (LONG i = 0; i < count; ++i)
	{
		pfc::string8_fast path = file_path_display(list[i]).get_ptr();
		path.add_char('\\');

		_variant_t var;
		var.vt = VT_BSTR;
		var.bstrVal = TO_BSTR(path);
		if (!helper.put_item(i, var)) return E_OUTOFMEMORY;
	}
	p->vt = VT_ARRAY | VT_VARIANT;
	p->parray = helper.get_ptr();
	return S_OK;
}

STDMETHODIMP Utils::MapString(BSTR str, UINT lcid, UINT flags, BSTR* p)
{
	if (!p) return E_POINTER;

	int r = ::LCMapStringW(lcid, flags, str, wcslen(str) + 1, nullptr, 0);
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

	GetPrivateProfileString(section, key, nullptr, buff, BUFFER_LEN, filename);

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

	std::vector<wchar_t> content;
	helpers::read_file_wide(codepage, filename, content);
	if (content.empty()) content.emplace_back(0);
	*p = SysAllocString(content.data());
	return S_OK;
}

STDMETHODIMP Utils::ReadUTF8(BSTR filename, BSTR* p)
{
	if (!p) return E_POINTER;

	*p = TO_BSTR(helpers::read_file(string_utf8_from_wide(filename)));
	return S_OK;
}

STDMETHODIMP Utils::TimestampToDateString(UINT ts, BSTR* p)
{
	if (!p) return E_POINTER;

	*p = TO_BSTR(format_filetimestamp(pfc::fileTimeUtoW(ts)));
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

STDMETHODIMP Utils::WriteTextFile(BSTR filename, BSTR content, VARIANT_BOOL* p)
{
	if (!p) return E_POINTER;

	*p = VARIANT_FALSE;
	if (content != nullptr) // empty strings are perfectly valid but weird shit can happen when feeding this method directly with xmlhttp.responseText - string_utf8_from_wide can throw exceptions unless stopped here
	{
		*p = TO_VARIANT_BOOL(helpers::write_file(string_utf8_from_wide(filename).get_ptr(), string_utf8_from_wide(content).get_ptr()));
	}
	return S_OK;
}

STDMETHODIMP Utils::get_Version(UINT* p)
{
	if (!p) return E_POINTER;

	*p = 2331;
	return S_OK;
}
