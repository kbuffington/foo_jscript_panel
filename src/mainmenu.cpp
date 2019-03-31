#include "stdafx.h"
#include "panel_manager.h"

class my_mainmenu_commands : public mainmenu_commands
{
public:
	enum
	{
		cmd_one = 0,
		cmd_two,
		cmd_three,
		cmd_four,
		cmd_five,
		cmd_six,
		cmd_seven,
		cmd_eight,
		cmd_nine,
		cmd_ten,
		cmd_total
	};

	GUID get_command(t_size p_index) override
	{
		switch (p_index)
		{
		case cmd_one: return jsp_guids::menu_one;
		case cmd_two: return jsp_guids::menu_two;
		case cmd_three: return jsp_guids::menu_three;
		case cmd_four: return jsp_guids::menu_four;
		case cmd_five: return jsp_guids::menu_five;
		case cmd_six: return jsp_guids::menu_six;
		case cmd_seven: return jsp_guids::menu_seven;
		case cmd_eight: return jsp_guids::menu_eight;
		case cmd_nine: return jsp_guids::menu_nine;
		case cmd_ten: return jsp_guids::menu_ten;
		default: uBugCheck();
		}
	}

	GUID get_parent() override
	{
		return jsp_guids::mainmenu_group_id;
	}

	bool get_description(t_size p_index, pfc::string_base& p_out) override
	{
		p_out << "Invoke on_main_menu(" << (p_index + 1) << ")";
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
		return cmd_total;
	}

	void execute(t_size p_index, service_ptr_t<service_base> p_callback) override
	{
		panel_manager::instance().post_msg_to_all(callback_id::on_main_menu, p_index + 1);
	}

	void get_name(t_size p_index, pfc::string_base & p_out) override
	{
		p_out << (p_index + 1);
	}
};

static service_factory_single_t<my_mainmenu_commands> g_my_mainmenu_commands;
static mainmenu_group_popup_factory g_my_mainmenu_group(jsp_guids::mainmenu_group_id, mainmenu_groups::file, mainmenu_commands::sort_priority_base, JSP_NAME);
