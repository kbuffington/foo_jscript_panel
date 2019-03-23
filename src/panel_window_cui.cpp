#include "stdafx.h"
#include "panel_window.h"

class panel_window_cui : public panel_window, public uie::window, public cui::fonts::common_callback, public cui::colours::common_callback
{
protected:
	DWORD get_colour_ui(t_size type) override
	{
		if (type <= cui::colours::colour_active_item_frame)
		{
			cui::colours::helper helper(pfc::guid_null);
			return helpers::convert_colorref_to_argb(helper.get_colour((cui::colours::colour_identifier_t)type));
		}
		return 0;
	}

	HFONT get_font_ui(t_size type) override
	{
		if (type <= cui::fonts::font_type_labels)
		{
			return static_api_ptr_t<cui::fonts::manager>()->get_font((cui::fonts::font_type_t)type);
		}
		return nullptr;
	}

	HWND create_or_transfer_window(HWND parent, const uie::window_host_ptr& host, const ui_helpers::window_position_t& p_position) override
	{
		if (m_hwnd)
		{
			ShowWindow(m_hwnd, SW_HIDE);
			SetParent(m_hwnd, parent);
			m_host->relinquish_ownership(m_hwnd);
			m_host = host;

			SetWindowPos(m_hwnd, nullptr, p_position.x, p_position.y, p_position.cx, p_position.cy, SWP_NOZORDER);
		}
		else
		{
			m_host = host;
			create(parent, this, p_position);
		}
		return m_hwnd;
	}

	HWND get_wnd() const override
	{
		return m_hwnd;
	}

	LRESULT on_message(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) override
	{
		switch (msg)
		{
		case WM_CREATE:
			try
			{
				static_api_ptr_t<cui::fonts::manager>()->register_common_callback(this);
				static_api_ptr_t<cui::colours::manager>()->register_common_callback(this);
			}
			catch (...) {}
			break;
		case WM_DESTROY:
			try
			{
				static_api_ptr_t<cui::fonts::manager>()->deregister_common_callback(this);
				static_api_ptr_t<cui::colours::manager>()->deregister_common_callback(this);
			}
			catch (...) {}
			break;
		case WM_SYSKEYDOWN:
		case WM_KEYDOWN:
			if (uie::window::g_process_keydown_keyboard_shortcuts(wp))
				return 0;
			break;
		}
		return panel_window::on_message(hwnd, msg, wp, lp);
	}

	bool have_config_popup() const override
	{
		return true;
	}

	bool is_available(const uie::window_host_ptr& p) const override
	{
		return true;
	}

	bool show_config_popup(HWND parent) override
	{
		return show_configure_popup();
	}

	const GUID& get_extension_guid() const override
	{
		return g_guid_jsp_window_cui;
	}

	t_size get_type() const override
	{
		return uie::type_toolbar | uie::type_panel;
	}

	void destroy_window() override
	{
		destroy();
		m_host.release();
	}

	void get_category(pfc::string_base& out) const override
	{
		out = "Panels";
	}

	void get_config(stream_writer* writer, abort_callback& abort) const override
	{
		save_config(writer, abort);
	}

	void get_name(pfc::string_base& out) const override
	{
		out = JSP_NAME;
	}

	void on_bool_changed(t_size mask) const override
	{
		// TODO: may be implemented one day
	}

	void on_colour_changed(t_size mask) const override
	{
		PostMessage(m_hwnd, callback_id::on_colours_changed, 0, 0);
	}

	void on_font_changed(t_size mask) const override
	{
		PostMessage(m_hwnd, callback_id::on_font_changed, 0, 0);
	}

	void set_config(stream_reader* reader, t_size size, abort_callback& abort) override
	{
		load_config(reader, size, abort);
	}

	void notify_size_limit_changed() override
	{
		if (m_host.is_valid())
		{
			m_host->on_size_limit_change(m_hwnd, uie::size_limit_all);
		}
	}

private:
	uie::window_host_ptr m_host;
};

// CUI panel instance
static uie::window_factory<panel_window_cui> g_panel_window_cui;
