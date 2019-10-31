#pragma once

class MyCList : public CListControlComplete
{
public:
	BEGIN_MSG_MAP_EX(MyCList)
		CHAIN_MSG_MAP(CListControlComplete);
		MSG_WM_CREATE(OnCreate)
		MSG_WM_CONTEXTMENU(OnContextMenu)
	END_MSG_MAP()

	int OnCreate(LPCREATESTRUCT)
	{
		AddColumn("Name", MulDiv(360, m_dpi.cx, 96));
		AddColumnAutoWidth("Value");
		m_data.resize(100); // scrollbar hack
		return 0;
	}

	void OnContextMenu(CWindow, CPoint point)
	{
		if (m_data.empty()) return;

		point = this->GetContextMenuPoint(point);
		CMenu menu;
		menu.CreatePopupMenu();
		uAppendMenu(menu, MF_STRING, ID_SELECTALL, "Select all\tCtrl+A");
		uAppendMenu(menu, MF_STRING, ID_SELECTNONE, "Select none");
		uAppendMenu(menu, MF_STRING, ID_INVERTSEL, "Invert selection");
		menu.AppendMenu(MF_SEPARATOR);
		uAppendMenu(menu, GetSelectedCount() ? MF_STRING : MF_GRAYED, ID_REMOVE, "Remove\tDel");

		const int idx = menu.TrackPopupMenu(TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD, point.x, point.y, m_hWnd, nullptr);
		switch (idx)
		{
		case ID_SELECTALL:
			this->SelectAll();
			break;
		case ID_SELECTNONE:
			this->SelectNone();
			break;
		case ID_INVERTSEL:
			this->SetSelection(pfc::bit_array_true(), pfc::bit_array_not(this->GetSelectionMask()));
			break;
		case ID_REMOVE:
			RequestRemoveSelection();
			break;
		}
	}

	bool AllowScrollbar(bool vertical) const override
	{
		return true;
	}

	bool CanSelectItem(t_size row) const override
	{
		return true;
	}

	bool GetCellCheckState(t_size item, t_size sub_item) const override
	{
		if (sub_item == 1 && m_data[item].is_bool) return m_data[item].bool_value;
		return false;
	}

	bool GetCellTypeSupported() const override
	{
		return true;
	}

	bool GetSubItemText(t_size item, t_size sub_item, pfc::string_base& out) const override
	{
		switch(sub_item)
		{
		case 0:
			out = m_data[item].key;
			return true;
		case 1:
			if (m_data[item].is_bool)
			{
				return false;
			}
			out = m_data[item].value;
			return true;
		default:
			out = "";
			return true;
		}
	}

	bool TableEdit_IsColumnEditable(t_size sub_item) const override
	{
		return sub_item == 1;
	}

	cellType_t GetCellType(t_size item, t_size sub_item) const override
	{
		if (sub_item == 1 && m_data[item].is_bool)
		{
			return &PFC_SINGLETON(CListCell_Checkbox);
		}
		else
		{
			return &PFC_SINGLETON(CListCell_Text);
		}
	}

	t_size GetItemCount() const override
	{
		return m_data.size();
	}

	void ExecuteDefaultAction(t_size index) override {}

	void OnItemsRemoved(const pfc::bit_array& mask, t_size oldCount) override
	{
		__super::OnItemsRemoved(mask, oldCount);
		if (m_data.empty())
		{
			::PostMessage(GetParent(), static_cast<unsigned int>(user_message::UWM_PROPERTIES_CLEARED), 0, 0);
		}
	}

	void OnSubItemClicked(t_size item, t_size sub_item, CPoint pt) override
	{
		if (sub_item == 1 && !m_data[item].is_bool)
		{
			TableEdit_Start(item, sub_item);
			return;
		}
		__super::OnSubItemClicked(item, sub_item, pt);
	}

	void RemoveMask(const pfc::bit_array& mask)
	{
		const auto old_count = GetItemCount();
		pfc::remove_mask_t(m_data, mask);
		this->OnItemsRemoved(mask, old_count);
	}

	void RequestRemoveSelection() override
	{
		RemoveMask(GetSelectionMask());
	}

	void RequestReorder(t_size const* order, t_size count) override {}

	void SetCellCheckState(t_size item, t_size sub_item, bool value) override
	{
		if (sub_item == 1 && m_data[item].is_bool)
		{
			m_data[item].bool_value = value;
			ReloadItem(item);
		}
	}

	void TableEdit_SetField(t_size item, t_size sub_item, const char* value) override
	{
		if (sub_item == 1 && !m_data[item].is_bool)
		{
			m_data[item].value = value;
			ReloadItem(item);
		}
	}

	struct data_t
	{
		pfc::string8 key, value;
		bool is_string, is_bool, bool_value;
	};

	std::vector<data_t> m_data;
};
