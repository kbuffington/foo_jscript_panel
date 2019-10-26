#pragma once

struct script_info
{
	script_info() : id(0) {}

	const std::vector<std::pair<std::string, std::string>> expand_table =
	{
		{ "%fb2k_profile_path%", helpers::get_profile_path().get_ptr() },
		{ "%fb2k_component_path%", helpers::get_fb2k_component_path().get_ptr() },
		{ "%fb2k_path%", helpers::get_fb2k_path().get_ptr() },
	};

	pfc::string8_fast build_info_string() const
	{
		pfc::string8_fast ret;
		ret << JSP_NAME_VERSION " (";
		if (name.length())
		{
			ret << name.c_str();
		}
		else
		{
			ret << "id:" << id;
		}
		if (version.length())
		{
			ret << " v" << version.c_str();
		}
		if (author.length())
		{
			ret << " by " << author.c_str();
		}
		ret << ")";
		return ret;
	}

	std::string expand_import(t_size idx)
	{
		std::string tmp = imports[idx];
		for (const auto& [key, value] : expand_table)
		{
			if (tmp.find(key) == 0)
			{
				tmp = value + tmp.substr(key.length());
				break;
			}
		}
		return tmp;
	}

	void clear()
	{
		imports.clear();
		name = "";
		version = "";
		author = "";
		id = 0;
	}

	std::string name;
	std::string version;
	std::string author;
	str_list imports;
	t_size id;
};
