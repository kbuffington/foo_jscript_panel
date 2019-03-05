#pragma once

class js_panel_window_dui : public js_panel_window, public ui_element_instance
{
public:
	js_panel_window_dui(ui_element_config::ptr cfg, ui_element_instance_callback::ptr callback);
	~js_panel_window_dui();

	static GUID g_get_guid();
	static GUID g_get_subclass();
	static pfc::string8_fast g_get_description();
	static ui_element_config::ptr g_get_default_configuration();
	static void g_get_name(pfc::string_base& out);
	DWORD GetColourUI(t_size type) override;
	HFONT GetFontUI(t_size type) override;
	HWND get_wnd() override;
	LRESULT on_message(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) override;
	bool edit_mode_context_menu_test(const POINT& p_point, bool p_fromkeyboard) override;
	ui_element_config::ptr get_configuration() override;
	void edit_mode_context_menu_build(const POINT& p_point, bool p_fromkeyboard, HMENU p_menu, t_size p_id_base) override;
	void edit_mode_context_menu_command(const POINT& p_point, bool p_fromkeyboard, t_size p_id, t_size p_id_base) override;
	void notify(const GUID& p_what, t_size p_param1, const void* p_param2, t_size p_param2size) override;
	void notify_size_limit_changed(t_size limit) override;
	void set_configuration(ui_element_config::ptr data) override;
	void initialize_window(HWND parent);

private:
	ui_element_instance_callback::ptr m_callback;
};
