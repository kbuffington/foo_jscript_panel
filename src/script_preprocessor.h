#pragma once
#include "helpers.h"

class host_comm;

struct t_directive_value
{
	typedef pfc::array_t<wchar_t> t_array;

	t_array directive;
	t_array value;

	t_directive_value()	{}
	t_directive_value(const t_array& p_directive, const t_array& p_value) : directive(p_directive), value(p_value) {}
};

struct t_script_info
{
	t_script_info(GUID& guid_ref) : m_guid_ref(guid_ref) {}

	struct
	{
		std::string which;
		std::string path;
	} expand_table[3] = {
		{ "%fb2k_path%", helpers::get_fb2k_path().get_ptr() },
		{ "%fb2k_component_path%", helpers::get_fb2k_component_path().get_ptr() },
		{ "%fb2k_profile_path%", helpers::get_profile_path().get_ptr() },
	};

	pfc::string8_fast build_info_string() const
	{
		pfc::string8_fast ret;
		ret << JSP_NAME_VERSION " (";
		if (name.get_length())
		{
			ret << name;
		}
		else
		{
			ret << "{" << pfc::print_guid(m_guid_ref) << "}";
		}

		if (version.get_length())
		{
			ret << " v" << version;
		}
		if (author.get_length())
		{
			ret << " by " << author;
		}
		ret << ")";
		return ret;
	}

	pfc::string8_fast expand_import(t_size idx)
	{
		std::string tmp = imports[idx];
		for (t_size i = 0; i < _countof(expand_table); ++i)
		{
			if (tmp.find(expand_table[i].which) == 0)
			{
				tmp = expand_table[i].path + tmp.substr(expand_table[i].which.length());
				break;
			}
		}
		return tmp.c_str();
	}

	void clear()
	{
		imports.remove_all();
		dragdrop = false;
		name = "";
		version = "";
		author = "";
	}

	bool dragdrop;
	pfc::string_list_impl imports;
	pfc::string8_fast name;
	pfc::string8_fast version;
	pfc::string8_fast author;

private:
	GUID& m_guid_ref;
};

class script_preprocessor
{
public:
	script_preprocessor(const wchar_t* script)
	{
		PFC_ASSERT(script != NULL);
		preprocess(script);
	}

	void process_script_info(t_script_info& info);

private:
	bool scan_directive_and_value(const wchar_t*& p, const wchar_t* pend);
	bool scan_value(const wchar_t*& p, const wchar_t* pend);
	bool extract_preprocessor_block(const wchar_t* script, t_size& block_begin, t_size& block_end);
	void preprocess(const wchar_t* script);

	pfc::array_t<wchar_t> m_directive_buffer;
	pfc::array_t<wchar_t> m_value_buffer;
	pfc::list_t<t_directive_value> m_directive_value_list;
};
