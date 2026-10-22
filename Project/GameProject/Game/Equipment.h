#pragma once
#include "../Base/Base.h"
#include "EquipmentIcon.h"

class Equipment : public Base {
	CImage m_img;
	CFont* m_font;
	bool m_is_show;
	int m_select_index;
	int m_equipped[3];      // ‘•”õ˜g(•Ší”Ô†B-1‚È‚ç‹ó)

	int FindEquipSlot(int index) const;
	void UpdateEquip();          
	CVector2D CalcSlotPos(int slot) const;

	EquipmentIcon m_icons[9];
	CImage m_select_img;
	CImage m_end_img;
	CImage m_get_img;
	CImage m_upg_img;
	int m_show_count;          //  Show‚ªŒÄ‚Î‚ê‚½‰ñ”
	bool m_free_available;     //  –³—¿˜g‚ªc‚Á‚Ä‚¢‚é‚©
	bool IsFreePick(int index) const;

	CVector2D CalcIconPos(int index) const;
	void UpdateSelectIcon();
	void UpdateEndButton();
	void UpdateGetButton();
	void BuyWeapon(int index);
	void UpgradeWeapon(int index);
	bool m_owned[9];

public:
	Equipment();
	bool IsShow() const;
	void Show();
	void Update();
	void Draw();
	bool IsOwned(int index) const;
	int GetEquipped(int slot) const;
};
