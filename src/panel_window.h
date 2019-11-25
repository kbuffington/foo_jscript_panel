#pragma once
#include "host_comm.h"
#include "host_drop_target.h"
#include "panel_manager.h"
#include "script_host.h"

class panel_window : public host_comm, public ui_helpers::container_window
{
public:
	panel_window();
	~panel_window();

	LRESULT on_message(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) override;
	class_data& get_class_data() const override;
	bool show_configure_popup(HWND parent) override;
	void repaint() override;
	void repaint_rect(int x, int y, int w, int h) override;
	void script_invoke(callback_id id, VARIANTARG* argv = nullptr, size_t argc = 0, VARIANT* ret = nullptr);
	void show_property_popup(HWND parent) override;
	void unload_script() override;
	void update_script() override;

protected:
	void build_context_menu(HMENU menu, int id_base);
	void execute_context_menu_command(int id, int id_base);

	HDC m_hdc;

private:
	void create_context();
	void delete_context();
	void load_script();
	void on_paint();
	void on_paint_error(HDC memdc);
	void on_paint_user(HDC memdc, LPRECT lpUpdateRect);
	void on_size();
	void redraw();
	void refresh_background(LPRECT lprcUpdate);

	CComPtr<IDropTargetImpl> m_drop_target;
	HBITMAP m_gr_bmp;
	HBITMAP m_gr_bmp_bk;
	IGdiGraphicsPtr m_gr_wrap;
	bool m_is_droptarget_registered;
	bool m_is_mouse_tracked;
	bool m_paint_pending;
	bool m_suppress_drawing;
	script_host* m_script_host;
};
