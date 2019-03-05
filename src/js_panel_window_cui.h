#pragma once

class js_panel_window_cui : public js_panel_window, public uie::window, public cui::fonts::common_callback, public cui::colours::common_callback
{
protected:
	DWORD GetColourUI(t_size type) override;
	HFONT GetFontUI(t_size type) override;
	HWND create_or_transfer_window(HWND parent, const uie::window_host_ptr& host, const ui_helpers::window_position_t& p_position) override;
	HWND get_wnd() const override;
	LRESULT on_message(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) override;
	bool have_config_popup() const override;
	bool is_available(const uie::window_host_ptr& p) const override;
	bool show_config_popup(HWND parent) override;
	const GUID& get_extension_guid() const override;
	t_size get_type() const override;
	void destroy_window() override;
	void get_category(pfc::string_base& out) const override;
	void get_config(stream_writer* writer, abort_callback& abort) const override;
	void get_name(pfc::string_base& out) const override;
	void notify_size_limit_changed(t_size limit) override;
	void on_bool_changed(t_size mask) const override;
	void on_colour_changed(t_size mask) const override;
	void on_font_changed(t_size mask) const override;
	void set_config(stream_reader* reader, t_size size, abort_callback& abort) override;

private:
	uie::window_host_ptr m_host;
};
