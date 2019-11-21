#include "stdafx.h"
#include "panel_manager.h"

namespace
{
	class my_mainmenu_commands : public mainmenu_commands
	{
	public:
		GUID get_command(t_size p_index) override
		{
			return jsp_guids::mainmenu_items[p_index];
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
			return jsp_guids::mainmenu_items.size();
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

	static mainmenu_group_popup_factory g_my_mainmenu_group(jsp_guids::mainmenu_group_id, mainmenu_groups::file, mainmenu_commands::sort_priority_base, JSP_NAME);
	FB2K_SERVICE_FACTORY(my_mainmenu_commands);
}
