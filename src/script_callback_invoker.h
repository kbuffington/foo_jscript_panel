#pragma once

class script_callback_invoker
{
public:
	script_callback_invoker();
	~script_callback_invoker();

	HRESULT Invoke(t_size callbackId, VARIANTARG* argv = nullptr, t_size argc = 0, VARIANT* ret = nullptr);
	void Init(IDispatch* pActiveScriptRoot);
	void Reset();

private:
	IDispatchPtr m_activeScriptRoot;
	pfc::map_t<t_size, DISPID> m_callbackInvokerMap;
};
