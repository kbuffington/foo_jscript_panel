#include "stdafx.h"
#include "mainmenu.h"
#include "panel_manager.h"

static service_factory_single_t<my_mainmenu_commands> g_my_mainmenu_commands;
static mainmenu_group_popup_factory g_my_mainmenu_group(g_guid_jsp_mainmenu_group_id, mainmenu_groups::file, mainmenu_commands::sort_priority_dontcare, JSP_NAME);

GUID my_mainmenu_commands::get_command(t_size p_index)
{
	switch (p_index)
	{
	case cmd_one: return g_guid_jsp_menu_one;
	case cmd_two: return g_guid_jsp_menu_two;
	case cmd_three: return g_guid_jsp_menu_three;
	case cmd_four: return g_guid_jsp_menu_four;
	case cmd_five: return g_guid_jsp_menu_five;
	case cmd_six: return g_guid_jsp_menu_six;
	case cmd_seven: return g_guid_jsp_menu_seven;
	case cmd_eight: return g_guid_jsp_menu_eight;
	case cmd_nine: return g_guid_jsp_menu_nine;
	case cmd_ten: return g_guid_jsp_menu_ten;
	default: uBugCheck();
	}
}

GUID my_mainmenu_commands::get_parent()
{
	return g_guid_jsp_mainmenu_group_id;
}

bool my_mainmenu_commands::get_description(t_size p_index, pfc::string_base& p_out)
{
	p_out = "Invoke on_main_menu()";
	return true;
}

bool my_mainmenu_commands::get_display(t_size p_index, pfc::string_base& p_out, t_size& p_flags)
{
	get_name(p_index, p_out);
	p_flags = mainmenu_commands::flag_defaulthidden;
	return true;
}

t_size my_mainmenu_commands::get_command_count()
{
	return cmd_total;
}

void my_mainmenu_commands::execute(t_size p_index, service_ptr_t<service_base> p_callback)
{
	panel_manager::instance().post_msg_to_all(callback_id::on_main_menu, p_index + 1);
}

void my_mainmenu_commands::get_name(t_size p_index, pfc::string_base& p_out)
{
	p_out = pfc::format_uint(p_index + 1);
}
