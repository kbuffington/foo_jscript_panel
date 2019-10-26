#include "stdafx.h"
#include "panel_manager.h"

class my_mainmenu_commands : public mainmenu_commands
{
public:
	GUID get_command(t_size p_index) override
	{
		switch (p_index)
		{
		case 0: return jsp_guids::menu_one;
		case 1: return jsp_guids::menu_two;
		case 2: return jsp_guids::menu_three;
		case 3: return jsp_guids::menu_four;
		case 4: return jsp_guids::menu_five;
		case 5: return jsp_guids::menu_six;
		case 6: return jsp_guids::menu_seven;
		case 7: return jsp_guids::menu_eight;
		case 8: return jsp_guids::menu_nine;
		case 9: return jsp_guids::menu_ten;
		default: uBugCheck();
		}
	}

	GUID get_parent() override
	{
		return jsp_guids::mainmenu_group_id;
	}

	bool get_description(t_size p_index, pfc::string_base& p_out) override
	{
		p_out = PFC_string_formatter() << "Invoke on_main_menu(" << (p_index + 1) << ")";
		return true;
	}

	bool get_display(t_size p_index, pfc::string_base& p_out, t_size& p_flags) override
	{
		get_name(p_index, p_out);
		p_flags = mainmenu_commands::flag_defaulthidden;
		return true;
	}

	t_size get_command_count() override
	{
		return 10;
	}

	void execute(t_size p_index, service_ptr_t<service_base> p_callback) override
	{
		panel_manager::instance().post_msg_to_all(callback_id::on_main_menu, p_index + 1);
	}

	void get_name(t_size p_index, pfc::string_base& p_out) override
	{
		p_out = std::to_string(p_index + 1).c_str();
	}
};

static service_factory_single_t<my_mainmenu_commands> g_my_mainmenu_commands;
static mainmenu_group_popup_factory g_my_mainmenu_group(jsp_guids::mainmenu_group_id, mainmenu_groups::file, mainmenu_commands::sort_priority_base, JSP_NAME);
