#pragma once
#include "helpers.h"
#include "host_comm.h"
#include "host_drop_target.h"
#include "panel_manager.h"
#include "script_host.h"

class panel_window : public host_comm, public ui_helpers::container_window
{
public:
	panel_window();
	~panel_window();

	HRESULT script_invoke(t_size callbackId, VARIANTARG* argv = nullptr, t_size argc = 0, VARIANT* ret = nullptr);
	LRESULT on_message(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) override;
	class_data& get_class_data() const override;
	bool show_configure_popup(HWND parent) override;
	void show_property_popup(HWND parent) override;
	void unload_script() override;
	void update_script() override;

protected:
	void build_context_menu(HMENU menu, int id_base);
	void execute_context_menu_command(int id, int id_base);

private:
	void load_script();
	void on_paint(HDC dc, LPRECT lpUpdateRect);
	void on_paint_error(HDC memdc);
	void on_paint_user(HDC memdc, LPRECT lpUpdateRect);
	void on_size(int w, int h);

	CComPtr<IDropTargetImpl> m_drop_target;
	IGdiGraphicsPtr m_gr_wrap;
	bool m_is_droptarget_registered;
	bool m_is_mouse_tracked;
	script_host* m_script_host;
};
