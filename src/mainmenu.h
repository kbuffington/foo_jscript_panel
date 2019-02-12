#pragma once

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

	GUID get_command(t_size p_index) override;
	GUID get_parent() override;
	bool get_description(t_size p_index, pfc::string_base& p_out) override;
	bool get_display(t_size p_index, pfc::string_base& p_out, t_size& p_flags) override;
	t_size get_command_count() override;
	void execute(t_size p_index, service_ptr_t<service_base> p_callback) override;
	void get_name(t_size p_index, pfc::string_base& p_out) override;
};
