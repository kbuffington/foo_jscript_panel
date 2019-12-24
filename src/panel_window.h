#pragma once
#include "drop_target_impl.h"
#include "helpers.h"
#include "panel_config.h"
#include "script_host.h"

class panel_window
{
public:
	virtual COLORREF get_colour_ui(size_t type) = 0;
	virtual IGdiFont* get_font_ui(size_t type) = 0;
	virtual void notify_size_limit_changed() = 0;

public:
	struct size
	{
		void reset_min_max()
		{
			min.SetPoint(0, 0);
			max.SetPoint(INT_MAX, INT_MAX);
		}

		CPoint min, max;
		int height = 0;
		int width = 0;
	};

	bool is_dui();
	bool is_transparent();
	panel_tooltip_param_ptr& panel_tooltip();
	void repaint();
	void repaint_rect(int x, int y, int w, int h);
	void script_invoke(callback_id id, VARIANTARG* argv = nullptr, size_t argc = 0, VARIANT* ret = nullptr) const;
	void show_configure_popup(HWND parent);
	void show_property_popup(HWND parent);
	void unload_script();
	void update_script();

	CWindow m_hwnd = nullptr;
	bool m_dragdrop = false;
	bool m_grabfocus = false;
	bool m_supports_transparency = false;
	panel_config m_panel_config{};
	script_host* m_script_host;
	size m_size{};

protected:
	panel_window();
	virtual ~panel_window();

	enum class instance_type
	{
		cui,
		dui,
	};

	bool handle_message(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	void build_context_menu(HMENU menu, int id_base);
	void execute_context_menu_command(int id, int id_base);
	void on_context_menu(LPARAM lp);

	HDC m_hdc = nullptr;
	instance_type m_instance_type = instance_type::cui;

private:
	void create_context();
	void delete_context();
	void load_script();
	void on_paint();
	void on_size();
	void refresh_background();
	void redraw();

	CComPtr<IDropTargetImpl> m_drop_target;
	HBITMAP m_gr_bmp = nullptr;
	HBITMAP m_gr_bmp_bk = nullptr;
	IGdiGraphicsPtr m_gr_wrap;
	bool m_is_mouse_tracked = false;
	bool m_refreshbk = true;
	panel_tooltip_param_ptr m_panel_tooltip_param_ptr;
	ui_selection_holder::ptr m_selection_holder;
};
