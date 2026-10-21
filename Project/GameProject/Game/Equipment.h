#pragma once
#include "../Base/Base.h"
#include "EquipmentIcon.h"

class Equipment : public Base {
	CImage m_img;
	CFont* m_font;
	bool m_is_show;
	int m_select_index;

	EquipmentIcon m_icons[9];
	CImage m_select_img;
	CImage m_end_img;
	CImage m_get_img;

	CVector2D CalcIconPos(int index) const;
	void UpdateSelectIcon();
	void UpdateEndButton();
	void UpdateGetButton();
	void BuyWeapon(int index);

public:
	Equipment();
	bool IsShow() const;
	void Show();
	void Update();
	void Draw();
	
};
