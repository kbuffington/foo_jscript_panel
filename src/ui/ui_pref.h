#pragma once

class CDialogPref : public CDialogImpl<CDialogPref>, public preferences_page_instance, private IListControlOwnerDataSource
{
public:
	CDialogPref(preferences_page_callback::ptr callback);

	BEGIN_MSG_MAP(CDialogPref)
		MSG_WM_INITDIALOG(OnInitDialog)
		COMMAND_HANDLER_EX(IDC_IMPORT, BN_CLICKED, OnImportBnClicked)
		COMMAND_HANDLER_EX(IDC_EXPORT, BN_CLICKED, OnExportBnClicked)
		COMMAND_HANDLER_EX(IDC_PRESETS, BN_CLICKED, OnPresetsBnClicked)
	END_MSG_MAP()

	enum
	{
		IDD = IDD_DIALOG_PREF
	};

	BOOL OnInitDialog(HWND, LPARAM);
	HWND get_wnd() override;
	t_size get_state() override;
	void LoadProps(bool reset = false);
	void OnExportBnClicked(UINT, int, HWND);
	void OnImportBnClicked(UINT, int, HWND);
	void OnPresetsBnClicked(UINT, int, HWND);
	void apply() override;
	void reset() override;

private:
	bool listCanSelectItem(ctx_t, t_size) override;
	bool listIsColumnEditable(ctx_t, t_size sub_item) override;
	pfc::string8 listGetSubItemText(ctx_t, t_size item, t_size sub_item) override;
	t_size listGetItemCount(ctx_t) override;
	void listSetEditField(ctx_t, t_size item, t_size sub_item, const char* value) override;
	void listSubItemClicked(ctx_t, t_size item, t_size sub_item) override;

	CListControlOwnerData m_props;
	preferences_page_callback::ptr m_callback;
};

class my_preferences_page_impl : public preferences_page_impl<CDialogPref>
{
public:
	GUID get_guid() override
	{
		return jsp_guids::ui_pref;
	}

	GUID get_parent_guid() override
	{
		return preferences_page::guid_tools;
	}

	bool get_help_url(pfc::string_base& p_out) override
	{
		p_out = "https://github.com/marc2k3/foo_jscript_panel/wiki";
		return true;
	}

	const char* get_name() override
	{
		return JSP_NAME;
	}
};

static service_factory_single_t<my_preferences_page_impl> g_my_preferences_page_impl;
