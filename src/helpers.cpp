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
		const GUID what = convert_artid_to_guid(art_id);
		auto api = album_art_manager_v2::get();
		album_art_extractor_instance_v2::ptr ptr;
		album_art_data_ptr data;
		IGdiBitmap* ret = nullptr;

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

	IGdiBitmap* get_album_art_embedded(const pfc::string8_fast& rawpath, t_size art_id)
	{
		album_art_extractor_instance_ptr aaep;
		const GUID what = convert_artid_to_guid(art_id);
		album_art_extractor::ptr ptr;
		album_art_data_ptr data;
		IGdiBitmap* ret = nullptr;

		if (album_art_extractor::g_get_interface(ptr, rawpath))
		{
			try
			{
				aaep = ptr->open(nullptr, rawpath, fb2k::noAbort);
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
		pfc::string8_fast path;
		constexpr t_size flags = contextmenu_manager::flag_view_full;

		if (p_handles.get_count())
		{
			cm->init_context(p_handles, flags);
		}
		else
		{
			if (!cm->init_context_now_playing(flags))
			{
				return false;
			}
		}

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
		pfc::map_t<GUID, mainmenu_group::ptr> group_guid_map;

		for (auto e = service_enum_t<mainmenu_group>(); !e.finished(); ++e)
		{
			auto ptr = *e;
			group_guid_map.find_or_add(ptr->get_guid()) = ptr;
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
					mainmenu_group::ptr group_ptr = group_guid_map[parent];
					mainmenu_group_popup::ptr group_popup_ptr;

					if (group_ptr->cast(group_popup_ptr))
					{
						pfc::string8_fast temp;
						group_popup_ptr->get_display_string(temp);
						if (temp.get_length())
						{
							temp.add_char('/');
							temp.add_string(path);
							path = temp;
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

	bool read_file_wide(t_size codepage, const wchar_t* path, pfc::array_t<wchar_t>& content)
	{
		HANDLE hFile = CreateFile(path, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (hFile == INVALID_HANDLE_VALUE)
		{
			return false;
		}

		HANDLE hFileMapping = CreateFileMapping(hFile, nullptr, PAGE_READONLY, 0, 0, nullptr);

		if (hFileMapping == nullptr)
		{
			CloseHandle(hFile);
			return false;
		}

		const DWORD dwFileSize = GetFileSize(hFile, nullptr);
		LPCBYTE pAddr = (LPCBYTE)MapViewOfFile(hFileMapping, FILE_MAP_READ, 0, 0, 0);

		if (pAddr == nullptr)
		{
			CloseHandle(hFileMapping);
			CloseHandle(hFile);
			return false;
		}

		if (dwFileSize == INVALID_FILE_SIZE)
		{
			UnmapViewOfFile(pAddr);
			CloseHandle(hFileMapping);
			CloseHandle(hFile);
			return false;
		}

		if (dwFileSize >=2 && pAddr[0] == 0xFF && pAddr[1] == 0xFE) // UTF16 LE?
		{
			const wchar_t* pSource = (const wchar_t*)(pAddr + 2);
			const t_size len = (dwFileSize - 2) >> 1;

			content.set_size(len + 1);
			pfc::__unsafe__memcpy_t(content.get_ptr(), pSource, len);
			content[len] = 0;
		}
		else if (dwFileSize >= 3 && pAddr[0] == 0xEF && pAddr[1] == 0xBB && pAddr[2] == 0xBF) // UTF8-BOM?
		{
			const char* pSource = (const char*)(pAddr + 3);
			const t_size pSourceSize = dwFileSize - 3;

			const t_size size = estimate_utf8_to_wide_quick(pSource, pSourceSize);
			content.set_size(size);
			convert_utf8_to_wide(content.get_ptr(), size, pSource, pSourceSize);
		}
		else
		{
			const char* pSource = (const char*)(pAddr);

			if (pfc::is_valid_utf8(pSource))
			{
				const t_size size = estimate_utf8_to_wide_quick(pSource, dwFileSize);
				content.set_size(size);
				convert_utf8_to_wide(content.get_ptr(), size, pSource, dwFileSize);
			}
			else
			{
				const t_size size = estimate_codepage_to_wide(codepage, pSource, dwFileSize);
				content.set_size(size);
				convert_codepage_to_wide(codepage, content.get_ptr(), size, pSource, dwFileSize);
			}
		}

		UnmapViewOfFile(pAddr);
		CloseHandle(hFileMapping);
		CloseHandle(hFile);
		return true;
	}

	bool supports_chakra()
	{
		HKEY hKey;
		return RegOpenKeyExW(HKEY_CLASSES_ROOT, L"CLSID\\{16d51579-a30b-4c8b-a276-0ff4dc41e755}", 0, KEY_READ, &hKey) == ERROR_SUCCESS;
	}

	bool write_file(const char* path, const pfc::string8_fast& content)
	{
		std::ofstream f(std::filesystem::u8path(path), std::ios::binary);
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
		static const std::array<const GUID*, 5> guids =
		{
			&album_art_ids::cover_front,
			&album_art_ids::cover_back,
			&album_art_ids::disc,
			&album_art_ids::icon,
			&album_art_ids::artist,
		};
		return art_id < guids.size() ? *guids[art_id] : *guids[0];
	}

	int get_encoder_clsid(const wchar_t* format, CLSID* pClsid)
	{
		int ret = -1;
		t_size num = 0;
		t_size size = 0;

		Gdiplus::GetImageEncodersSize(&num, &size);
		if (size == 0) return ret;

		auto pImageCodecInfo = new Gdiplus::ImageCodecInfo[size];
		if (pImageCodecInfo == nullptr) return ret;

		Gdiplus::GetImageEncoders(num, size, pImageCodecInfo);

		for (t_size i = 0; i < num; ++i)
		{
			if (wcscmp(pImageCodecInfo[i].MimeType, format) == 0)
			{
				*pClsid = pImageCodecInfo[i].Clsid;
				ret = i;
				break;
			}
		}

		delete[] pImageCodecInfo;
		return ret;
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

	pfc::string8_fast iterator_to_string(json::iterator j)
	{
		std::string value = j.value().type() == json::value_t::string ? j.value().get<std::string>() : j.value().dump();
		return value.c_str();
	}

	pfc::string8_fast read_file(const char* path)
	{
		pfc::string8_fast content;
		std::ifstream f(std::filesystem::u8path(path));
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
			if (pfc::is_valid_utf8(text)) return 65001;
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

	wchar_t* make_sort_string(const char* in)
	{
		wchar_t* out = new wchar_t[estimate_utf8_to_wide(in) + 1];
		out[0] = ' '; // StrCmpLogicalW bug workaround.
		convert_utf8_to_wide_unchecked(out + 1, in);
		return out;
	}
}
