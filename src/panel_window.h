#pragma once
#include "panel_base.h"
#include "host_drop_target.h"
#include "script_host.h"

class panel_window : public panel_base
{
public:
	panel_window();
	~panel_window();

	void repaint() override;
	void repaint_rect(int x, int y, int w, int h) override;
	void script_invoke(callback_id id, VARIANTARG* argv = nullptr, size_t argc = 0, VARIANT* ret = nullptr) const;
	void show_configure_popup(HWND parent) override;
	void show_property_popup(HWND parent) override;
	void unload_script() override;
	void update_script() override;

protected:
	bool handle_message(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	void build_context_menu(HMENU menu, int id_base);
	void create_context();
	void delete_context();
	void execute_context_menu_command(int id, int id_base);
	void load_script();
	void on_context_menu(LPARAM lp);
	void on_paint();
	void on_size();
	void refresh_background();
	void redraw();

	CComPtr<IDropTargetImpl> m_drop_target;
	HBITMAP m_gr_bmp = nullptr;
	HBITMAP m_gr_bmp_bk = nullptr;
	HDC m_hdc = nullptr;
	IGdiGraphicsPtr m_gr_wrap;
	bool m_is_mouse_tracked = false;
	bool m_paint_pending = false;
	script_host* m_script_host;
	ui_selection_holder::ptr m_selection_holder;
};
