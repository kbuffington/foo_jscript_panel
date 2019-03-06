#pragma once

class script_callback_invoker
{
public:
	script_callback_invoker();
	~script_callback_invoker();

	HRESULT invoke(t_size callbackId, VARIANTARG* argv = nullptr, t_size argc = 0, VARIANT* ret = nullptr);
	void init(IDispatch* pActiveScriptRoot);
	void reset();

private:
	IDispatchPtr m_active_script_root;
	pfc::map_t<t_size, DISPID> m_callback_invoker_map;
};
