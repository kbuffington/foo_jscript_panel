#pragma once

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
	bool dragdrop;
	pfc::string_list_impl imports;
	pfc::string8_fast name;
	pfc::string8_fast version;
	pfc::string8_fast author;

	t_script_info(GUID& guid_ref) : m_guid_ref(guid_ref)
	{
	}

	void clear()
	{
		imports.remove_all();
		dragdrop = false;
		name = "";
		version = "";
		author = "";
	}

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
	bool extract_preprocessor_block(const wchar_t* script, int& block_begin, int& block_end);
	void preprocess(const wchar_t* script);

	pfc::array_t<wchar_t> m_directive_buffer;
	pfc::array_t<wchar_t> m_value_buffer;
	pfc::list_t<t_directive_value> m_directive_value_list;
};
