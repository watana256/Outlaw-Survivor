#include "Equipment.h"

Equipment::Equipment()
	:Base(eTpe_Equipment)
	,m_is_show(false)
	,m_select_index(-1)
{
	m_img = COPY_RESOURCE("SHOP", CImage);

	//m_font = GET_RESOURCE("ShopFont", CFont);
	for (int i = 0; i < 9; i++) {
		EquipmentIcon& icon = m_icons[i];
		icon.SetIndex(i);
	}

	m_select_img = COPY_RESOURCE("waku", CImage);
	m_select_img.SetSize(229.0f, 195.0f);
	m_select_img.SetCenter(m_select_img.GetSize() * 0.5f);
}
bool Equipment::IsShow() const
{
	return m_is_show;
}
void Equipment::Show()
{
	m_is_show = true;
	m_select_index = -1;
}
CVector2D Equipment::CalcIconPos(int index) const
{
	CVector2D start = CVector2D(414.5f, 375.0f);
	return  start + CVector2D(229.0f * (index % 3), 195.0f * (index / 3));
}
void Equipment::UpdateSelectIcon()
{
	if (PUSH(CInput::eMouseL)) 
	{
		CVector2D pos = CInput::GetMousePoint();
		for (int i = 0; i < 9; i++) {
			EquipmentIcon& icon = m_icons[i];
			if (CCollision::CollisionRectPoint(icon.GetRect(), pos)) {
				m_select_index = i;
				break;
			}
		}
	}
}
void Equipment::Update()
{
	if (!m_is_show)return;
	if (PUSH(CInput::eButton5))
	{
		m_is_show = false;
	}

	UpdateSelectIcon();
}
void Equipment::Draw()
{
	if (!m_is_show)return;
	m_img.Draw();

	for (int i = 0; i < 9; i++) {
		EquipmentIcon& icon = m_icons[i];
		CVector2D pos = CalcIconPos(i);
		icon.SetPos(pos);
		icon.Draw();
	}
	if (m_select_index >= 0)
	{
		m_select_img.SetPos(CalcIconPos(m_select_index));
		m_select_img.Draw();
	}
	//m_font->Draw(64, 256, 0, 0, 0, "Title");
}
