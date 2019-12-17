#pragma once

struct panel_config
{
	enum class edge_style : char
	{
		none,
		sunken,
		grey,
	};

	DWORD get_edge_style()
	{
		switch (style)
		{
		case edge_style::sunken: return WS_EX_CLIENTEDGE;
		case edge_style::grey: return WS_EX_STATICEDGE;
		default: return 0;
		}
	}

	void reset()
	{
		code = helpers::get_resource_text(IDR_SCRIPT);
		engine = helpers::supports_chakra() ? "Chakra" : "JScript";
		style = edge_style::none;
		transparent = false;
	}

	bool transparent = false;
	edge_style style = edge_style::none;
	pfc::string8_fast code;
	pfc::string8_fast engine;
};
