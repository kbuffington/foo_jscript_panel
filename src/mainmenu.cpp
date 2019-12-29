#include "stdafx.h"
#include "panel_manager.h"

namespace
{
	class my_mainmenu_commands : public mainmenu_commands
	{
	public:
		GUID get_command(size_t index) override
		{
			return jsp::guids::mainmenu_items[index];
		}

		GUID get_parent() override
		{
			return jsp::guids::mainmenu_group_id;
		}

		bool get_description(size_t index, pfc::string_base& out) override
		{
			out = PFC_string_formatter() << "Invoke on_main_menu(" << (index + 1) << ")";
			return true;
		}

		bool get_display(size_t index, pfc::string_base& out, size_t& flags) override
		{
			get_name(index, out);
			flags = mainmenu_commands::flag_defaulthidden;
			return true;
		}

		size_t get_command_count() override
		{
			return jsp::guids::mainmenu_items.size();
		}

		void execute(size_t index, service_ptr_t<service_base> callback) override
		{
			panel_manager::instance().post_msg_to_all(callback_id::on_main_menu, index + 1);
		}

		void get_name(size_t index, pfc::string_base& out) override
		{
			out = std::to_string(index + 1).c_str();
		}
	};

	static mainmenu_group_popup_factory g_my_mainmenu_group(jsp::guids::mainmenu_group_id, mainmenu_groups::file, mainmenu_commands::sort_priority_base, jsp::component_name);
	FB2K_SERVICE_FACTORY(my_mainmenu_commands);
}
