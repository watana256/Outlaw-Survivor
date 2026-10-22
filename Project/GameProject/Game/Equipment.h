#pragma once
#include "../Base/Base.h"
#include "EquipmentIcon.h"

class Equipment : public Base {
	CImage m_img;
	CFont* m_font;
	bool m_is_show;
	int m_select_index;
	int m_equipped[3];      // 装備枠(武器番号。-1なら空)
	int m_level[9];			//武器ごとのレベル(1?5)

	int GetUpgradeCost(int index) const;//次のレベルへの費用
	int FindEquipSlot(int index) const;
	void UpdateEquip();
	CVector2D CalcSlotPos(int slot) const;

	EquipmentIcon m_icons[9];
	CImage m_select_img;
	CImage m_end_img;
	CImage m_get_img;
	CImage m_upg_img;
	CImage m_ki_img;
	int m_show_count;          //  Showが呼ばれた回数
	bool m_free_available;     //  無料枠が残っているか
	bool IsFreePick(int index) const;

	CVector2D CalcIconPos(int index) const;
	void UpdateSelectIcon();
	void UpdateEndButton();
	void UpdateGetButton();
	void BuyWeapon(int index);
	void UpgradeWeapon(int index);
	bool CanAct(int index) const;
	bool m_owned[9];

public:
	Equipment();
	bool IsShow() const;
	void Show();
	void Update();
	void Draw();
	bool IsOwned(int index) const;
	int GetEquipped(int slot) const;
	int GetLevel(int index) const;	//Playerが参照する用
};