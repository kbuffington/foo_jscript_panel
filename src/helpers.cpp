#include "stdafx.h"
#include "helpers.h"

#include <MLang.h>

namespace helpers
{
	COLORREF convert_argb_to_colorref(DWORD argb)
	{
		return RGB(argb >> RED_SHIFT, argb >> GREEN_SHIFT, argb >> BLUE_SHIFT);
	}

	DWORD convert_colorref_to_argb(COLORREF color)
	{
		return GetRValue(color) << RED_SHIFT | GetGValue(color) << GREEN_SHIFT | GetBValue(color) << BLUE_SHIFT | 0xff000000;
	}

	IGdiBitmap* get_album_art(const metadb_handle_ptr& handle, t_size art_id, bool need_stub, bool no_load, pfc::string_base& image_path)
	{
		IGdiBitmap* ret = nullptr;
		const GUID what = convert_artid_to_guid(art_id);

		auto api = album_art_manager_v2::get();
		album_art_data_ptr data;
		album_art_extractor_instance_v2::ptr ptr;

		try
		{
			ptr = api->open(pfc::list_single_ref_t<metadb_handle_ptr>(handle), pfc::list_single_ref_t<GUID>(what), fb2k::noAbort);
			data = ptr->query(what, fb2k::noAbort);
		}
		catch (...)
		{
			if (need_stub)
			{
				try
				{
					ptr = api->open_stub(fb2k::noAbort);
					data = ptr->query(what, fb2k::noAbort);
				}
				catch (...) {}
			}
		}

		if (data.is_valid())
		{
			if (!no_load)
			{
				ret = read_album_art_into_bitmap(data);
			}
			album_art_path_list::ptr pathlist = ptr->query_paths(what, fb2k::noAbort);
			if (pathlist->get_count() > 0)
			{
				image_path.set_string(file_path_display(pathlist->get_path(0)));
			}
		}
		return ret;
	}

	IGdiBitmap* get_album_art_embedded(const pfc::string8_fast& path, t_size art_id)
	{
		IGdiBitmap* ret = nullptr;
		const GUID what = convert_artid_to_guid(art_id);

		album_art_data_ptr data;
		album_art_extractor::ptr ptr;

		if (album_art_extractor::g_get_interface(ptr, path))
		{
			try
			{
				album_art_extractor_instance_ptr aaep = ptr->open(nullptr, path, fb2k::noAbort);
				data = aaep->query(what, fb2k::noAbort);
				ret = read_album_art_into_bitmap(data);
			}
			catch (...) {}
		}
		return ret;
	}

	IGdiBitmap* load_image(BSTR path)
	{
		IGdiBitmap* ret = nullptr;
		CComPtr<IStream> stream;
		if (SUCCEEDED(SHCreateStreamOnFileEx(path, STGM_READ | STGM_SHARE_DENY_WRITE, GENERIC_READ, FALSE, nullptr, &stream)))
		{
			auto img = new Gdiplus::Bitmap(stream, TRUE);
			if (ensure_gdiplus_object(img))
			{
				ret = new com_object_impl_t<GdiBitmap>(img);
			}
			else
			{
				if (img) delete img;
				img = nullptr;
			}
		}
		return ret;
	}

	IGdiBitmap* read_album_art_into_bitmap(const album_art_data_ptr& data)
	{
		IGdiBitmap* ret = nullptr;

		if (!data.is_valid())
			return ret;

		CComPtr<IStream> stream;
		if (SUCCEEDED(CreateStreamOnHGlobal(nullptr, TRUE, &stream)))
		{
			ULONG bytes_written = 0;
			if (SUCCEEDED(stream->Write(data->get_ptr(), data->get_size(), &bytes_written)) && bytes_written == data->get_size())
			{
				auto img = new Gdiplus::Bitmap(stream, TRUE);
				if (ensure_gdiplus_object(img))
				{
					ret = new com_object_impl_t<GdiBitmap>(img);
				}
				else
				{
					if (img) delete img;
					img = nullptr;
				}
			}
		}
		return ret;
	}

	bool execute_context_command_by_name(const char* p_command, metadb_handle_list_cref p_handles)
	{
		contextmenu_manager::ptr cm;
		contextmenu_manager::g_create(cm);
		cm->init_context(p_handles, contextmenu_manager::flag_view_full);
		pfc::string8_fast path;
		return execute_context_command_recur(p_command, path, cm->get_root());
	}

	bool execute_context_command_recur(const char* p_command, const pfc::string_base& p_path, contextmenu_node* p_parent)
	{
		for (t_size i = 0; i < p_parent->get_num_children(); ++i)
		{
			contextmenu_node* child = p_parent->get_child(i);
			pfc::string8_fast path = p_path;
			path << child->get_name();

			switch (child->get_type())
			{
			case contextmenu_item_node::type_group:
				path.add_char('/');
				if (execute_context_command_recur(p_command, path, child))
				{
					return true;
				}
				break;
			case contextmenu_item_node::type_command:
				if (_stricmp(p_command, path) == 0)
				{
					child->execute();
					return true;
				}
				break;
			}
		}
		return false;
	}

	bool execute_mainmenu_command_by_name(const char* p_command)
	{
		auto hash = [](const GUID& g)
		{
			return hasher_md5::get()->process_single_string(pfc::print_guid(g).get_ptr()).xorHalve();
		};

		std::unordered_map<t_uint64, mainmenu_group::ptr> group_guid_map;

		for (auto e = service_enum_t<mainmenu_group>(); !e.finished(); ++e)
		{
			auto ptr = *e;
			group_guid_map.emplace(hash(ptr->get_guid()), ptr);
		}

		for (auto e = service_enum_t<mainmenu_commands>(); !e.finished(); ++e)
		{
			auto ptr = *e;
			for (t_size i = 0; i < ptr->get_command_count(); ++i)
			{
				pfc::string8_fast path;

				GUID parent = ptr->get_parent();
				while (parent != pfc::guid_null)
				{
					mainmenu_group::ptr group_ptr = group_guid_map.at(hash(parent));
					mainmenu_group_popup::ptr group_popup_ptr;

					if (group_ptr->cast(group_popup_ptr))
					{
						pfc::string8_fast str;
						group_popup_ptr->get_display_string(str);
						if (str.get_length())
						{
							str.add_char('/');
							str.add_string(path);
							path = str;
						}
					}
					parent = group_ptr->get_parent();
				}

				mainmenu_commands_v2::ptr v2_ptr;
				if (ptr->cast(v2_ptr) && v2_ptr->is_command_dynamic(i))
				{
					mainmenu_node::ptr node = v2_ptr->dynamic_instantiate(i);
					if (execute_mainmenu_command_recur(p_command, path, node))
					{
						return true;
					}
				}
				else
				{
					pfc::string8_fast command;
					ptr->get_name(i, command);
					path.add_string(command);
					if (_stricmp(p_command, path) == 0)
					{
						ptr->execute(i, nullptr);
						return true;
					}
				}
			}
		}
		return false;
	}

	bool execute_mainmenu_command_recur(const char* p_command, pfc::string8_fast path, mainmenu_node::ptr node)
	{
		pfc::string8_fast text;
		t_size flags;
		node->get_display(text, flags);
		path += text;

		switch (node->get_type())
		{
		case mainmenu_node::type_group:
			if (text.get_length())
			{
				path.add_char('/');
			}
			for (t_size i = 0; i < node->get_children_count(); ++i)
			{
				mainmenu_node::ptr child = node->get_child(i);
				if (execute_mainmenu_command_recur(p_command, path, child))
				{
					return true;
				}
			}
			break;
		case mainmenu_node::type_command:
			if (_stricmp(p_command, path) == 0)
			{
				node->execute(nullptr);
				return true;
			}
			break;
		}
		return false;
	}

	bool get_encoder_clsid(const wchar_t* format, CLSID* pClsid)
	{
		using namespace Gdiplus;

		bool ret = false;
		t_size num = 0;
		t_size size = 0;

		GetImageEncodersSize(&num, &size);
		if (size == 0) return ret;

		pfc::ptrholder_t<ImageCodecInfo> pImageCodecInfo = new ImageCodecInfo[size];
		if (pImageCodecInfo.is_empty()) return ret;

		GetImageEncoders(num, size, pImageCodecInfo.get_ptr());

		for (t_size i = 0; i < num; ++i)
		{
			if (wcscmp(pImageCodecInfo.get_ptr()[i].MimeType, format) == 0)
			{
				*pClsid = pImageCodecInfo.get_ptr()[i].Clsid;
				ret = true;
				break;
			}
		}
		return ret;
	}

	bool is_wrap_char(wchar_t current, wchar_t next)
	{
		if (iswpunct(current))
			return false;

		if (next == '\0')
			return true;

		if (iswspace(current))
			return true;

		const int currentAlphaNum = iswalnum(current);

		if (currentAlphaNum)
		{
			if (iswpunct(next))
				return false;
		}

		return currentAlphaNum == 0 || iswalnum(next) == 0;
	}

	bool supports_chakra()
	{
		HKEY hKey;
		return RegOpenKeyExW(HKEY_CLASSES_ROOT, L"CLSID\\{16d51579-a30b-4c8b-a276-0ff4dc41e755}", 0, KEY_READ, &hKey) == ERROR_SUCCESS;
	}

	bool write_file(const char* path, const pfc::string8_fast& content)
	{
		std::ofstream f(fs::u8path(path), std::ios::binary);
		if (f.is_open())
		{
			f << content.get_ptr();
			f.close();
			return true;
		}
		return false;
	}

	const GUID convert_artid_to_guid(t_size art_id)
	{
		static constexpr std::array<const GUID*, 5> guids =
		{
			&album_art_ids::cover_front,
			&album_art_ids::cover_back,
			&album_art_ids::disc,
			&album_art_ids::icon,
			&album_art_ids::artist,
		};
		return art_id < guids.size() ? *guids[art_id] : *guids[0];
	}

	int get_text_height(HDC hdc, const wchar_t* text, int len)
	{
		SIZE size;
		GetTextExtentPoint32(hdc, text, len, &size);
		return size.cy;
	}

	int get_text_width(HDC hdc, const wchar_t* text, int len)
	{
		SIZE size;
		GetTextExtentPoint32(hdc, text, len, &size);
		return size.cx;
	}

	pfc::string8_fast get_fb2k_component_path()
	{
		pfc::string8_fast path;
		uGetModuleFileName(core_api::get_my_instance(), path);
		path = pfc::string_directory(path);
		path.add_char('\\');
		return path;
	}

	pfc::string8_fast get_fb2k_path()
	{
		pfc::string8_fast path;
		uGetModuleFileName(nullptr, path);
		path = pfc::string_directory(path);
		path.add_char('\\');
		return path;
	}

	pfc::string8_fast get_profile_path()
	{
		pfc::string8_fast path = file_path_display(core_api::get_profile_path()).get_ptr();
		path.add_char('\\');
		return path;
	}

	pfc::string8_fast get_resource_text(int id)
	{
		puResource pures = uLoadResource(core_api::get_my_instance(), uMAKEINTRESOURCE(id), "TEXT");
		pfc::string8_fast content(static_cast<const char*>(pures->GetPointer()), pures->GetSize());
		return content;
	}

	pfc::string8_fast read_file(const char* path)
	{
		pfc::string8_fast content;
		std::ifstream f(fs::u8path(path));
		if (f.is_open())
		{
			std::string line;
			while (std::getline(f, line))
			{
				content << line.c_str() << "\r\n";
			}
			f.close();
		}
		return content;
	}

	str_list split_string(const std::string& str, const std::string& delims)
	{
		str_list output;
		t_size first = 0;
		while (first < str.size())
		{
			const auto second = str.find_first_of(delims, first);
			if (first != second)
				output.emplace_back(str.substr(first, second - first));
			if (second == std::string::npos)
				break;
			first = second + 1;
		}
		return output;
	}

	t_size detect_charset(const char* fileName)
	{
		pfc::string8_fast text;
		int textSize = 0;

		try
		{
			file_ptr io;
			filesystem::g_open_read(io, fileName, fb2k::noAbort);
			io->read_string_raw(text, fb2k::noAbort);
			textSize = text.get_length();
		}
		catch (...)
		{
			return 0;
		}

		constexpr int maxEncodings = 2;
		int encodingCount = maxEncodings;
		DetectEncodingInfo encodings[maxEncodings];

		_COM_SMARTPTR_TYPEDEF(IMultiLanguage2, IID_IMultiLanguage2);
		IMultiLanguage2Ptr lang;
		if (FAILED(lang.CreateInstance(CLSID_CMultiLanguage, nullptr, CLSCTX_INPROC_SERVER))) return 0;
		if (FAILED(lang->DetectInputCodepage(MLDETECTCP_NONE, 0, const_cast<char*>(text.get_ptr()), &textSize, encodings, &encodingCount))) return 0;

		t_size codepage = encodings[0].nCodePage;

		if (encodingCount == 2 && encodings[0].nCodePage == 1252)
		{
			switch (encodings[1].nCodePage)
			{
			case 850:
			case 65001:
				codepage = 65001;
				break;
			case 932: // shift-jis
			case 936: // gbk
			case 949: // korean
			case 950: // big5
				codepage = encodings[1].nCodePage;
				break;
			}
		}

		// ASCII?
		if (codepage == 20127)
		{
			codepage = 0;
		}

		return codepage;
	}

	void estimate_line_wrap(HDC hdc, const wchar_t* text, int max_width, wrapped_item_list& out)
	{
		for (;;)
		{
			const wchar_t* next = wcschr(text, '\n');
			if (next == nullptr)
			{
				estimate_line_wrap_recur(hdc, text, wcslen(text), max_width, out);
				break;
			}

			const wchar_t* walk = next;

			while (walk > text && walk[-1] == '\r')
			{
				--walk;
			}

			estimate_line_wrap_recur(hdc, text, walk - text, max_width, out);
			text = next + 1;
		}
	}

	void estimate_line_wrap_recur(HDC hdc, const wchar_t* text, int len, int max_width, wrapped_item_list& out)
	{
		int textLength = len;
		const int textWidth = get_text_width(hdc, text, len);

		if (textWidth <= max_width || len <= 1)
		{
			wrapped_item item = { SysAllocStringLen(text, len), textWidth };
			out.emplace_back(item);
		}
		else
		{
			textLength = (len * max_width) / textWidth;

			if (get_text_width(hdc, text, textLength) < max_width)
			{
				while (get_text_width(hdc, text, std::min(len, textLength + 1)) <= max_width)
				{
					++textLength;
				}
			}
			else
			{
				while (get_text_width(hdc, text, textLength) > max_width && textLength > 1)
				{
					--textLength;
				}
			}

			{
				const int fallbackTextLength = std::max(textLength, 1);

				while (textLength > 0 && !is_wrap_char(text[textLength - 1], text[textLength]))
				{
					--textLength;
				}

				if (textLength == 0)
				{
					textLength = fallbackTextLength;
				}

				wrapped_item item = { SysAllocStringLen(text, textLength), get_text_width(hdc, text, textLength) };
				out.emplace_back(item);
			}

			if (textLength < len)
			{
				estimate_line_wrap_recur(hdc, text + textLength, len - textLength, max_width, out);
			}
		}
	}

	void list_files(const char* path, bool recur, pfc::string_list_impl& out)
	{
		pfc::string8_fast cpath;
		filesystem::g_get_canonical_path(path, cpath);

		try
		{
			if (recur)
			{
				foobar2000_io::listFilesRecur(cpath, out, fb2k::noAbort);
			}
			else
			{
				foobar2000_io::listFiles(cpath, out, fb2k::noAbort);
			}
		}
		catch (...) {}
	}

	void list_folders(const char* path, pfc::string_list_impl& out)
	{
		pfc::string8_fast cpath;
		filesystem::g_get_canonical_path(path, cpath);

		try
		{
			foobar2000_io::listDirectories(cpath, out, fb2k::noAbort);
		}
		catch (...) {}
	}

	void read_file_wide(t_size codepage, const wchar_t* path, std::vector<wchar_t>& content)
	{
		HANDLE hFile = CreateFile(path, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (hFile == INVALID_HANDLE_VALUE)
		{
			return;
		}

		HANDLE hFileMapping = CreateFileMapping(hFile, nullptr, PAGE_READONLY, 0, 0, nullptr);

		if (hFileMapping == nullptr)
		{
			CloseHandle(hFile);
			return;
		}

		LPCBYTE pAddr = static_cast<LPCBYTE>(MapViewOfFile(hFileMapping, FILE_MAP_READ, 0, 0, 0));

		if (pAddr == nullptr)
		{
			CloseHandle(hFileMapping);
			CloseHandle(hFile);
			return;
		}

		const DWORD dwFileSize = GetFileSize(hFile, nullptr);

		if (dwFileSize == INVALID_FILE_SIZE)
		{
			UnmapViewOfFile(pAddr);
			CloseHandle(hFileMapping);
			CloseHandle(hFile);
			return;
		}

		if (dwFileSize >= 2 && pAddr[0] == 0xFF && pAddr[1] == 0xFE) // UTF16 LE?
		{
			const wchar_t* pSource = reinterpret_cast<const wchar_t*>(pAddr + 2);
			const t_size len = (dwFileSize - 2) >> 1;

			content.resize(len + 1);
			pfc::__unsafe__memcpy_t(content.data(), pSource, len);
			content[len] = 0;
		}
		else if (dwFileSize >= 3 && pAddr[0] == 0xEF && pAddr[1] == 0xBB && pAddr[2] == 0xBF) // UTF8-BOM?
		{
			const char* pSource = reinterpret_cast<const char*>(pAddr + 3);
			const t_size pSourceSize = dwFileSize - 3;

			const t_size size = estimate_utf8_to_wide_quick(pSource, pSourceSize);
			content.resize(size);
			convert_utf8_to_wide(content.data(), size, pSource, pSourceSize);
		}
		else
		{
			const char* pSource = reinterpret_cast<const char*>(pAddr);

			if (detect_charset(string_utf8_from_wide(path)) == CP_UTF8)
			{
				const t_size size = estimate_utf8_to_wide_quick(pSource, dwFileSize);
				content.resize(size);
				convert_utf8_to_wide(content.data(), size, pSource, dwFileSize);
			}
			else
			{
				const t_size size = estimate_codepage_to_wide(codepage, pSource, dwFileSize);
				content.resize(size);
				convert_codepage_to_wide(codepage, content.data(), size, pSource, dwFileSize);
			}
		}

		UnmapViewOfFile(pAddr);
		CloseHandle(hFileMapping);
		CloseHandle(hFile);
		return;
	}

	wchar_t* make_sort_string(const char* in)
	{
		auto out = new wchar_t[estimate_utf8_to_wide(in) + 1];
		out[0] = ' '; // StrCmpLogicalW bug workaround.
		convert_utf8_to_wide_unchecked(out + 1, in);
		return out;
	}
}
