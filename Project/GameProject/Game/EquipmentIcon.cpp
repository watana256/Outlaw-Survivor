#include"Game/EquipmentIcon.h"

EquipmentIcon::EquipmentIcon()
	: m_is_show(false)
{
	
}
bool EquipmentIcon::IsShow() const
{
	return m_is_show;
}
void EquipmentIcon::Show()
{
	m_is_show = true;
}
void EquipmentIcon::Update()
{
}
void EquipmentIcon::Draw()
{
	m_img.Draw();
}
void EquipmentIcon::SetPos(const CVector2D& pos)
{
	m_img.SetPos(pos);
}
const CVector2D& EquipmentIcon::GetPos() const
{
	return m_img.m_pos;
}
void EquipmentIcon::SetIndex(int index) 
{
	std::string name;
	switch (index)
	{
	case 0: name = "Ax"; break;
	case 1: name = "Pistol"; break;
	case 2: name = "Shotgun"; break;
	case 3: name = "Molotov"; break;
	case 4: name = "Flamethrower"; break;
	case 5: name = "Landmine"; break;
	case 6: name = "Drone"; break;
	case 7: name = "Stella"; break;
	case 8: name = "Rocket"; break;
	}
	m_img = COPY_RESOURCE(name, CImage);
	m_img.SetCenter(m_img.GetSize() * 0.5f);
}

CRect EquipmentIcon::GetRect() const
{
	return CRect(m_img.m_pos-m_img.m_center, m_img.m_size);
}
// éwíËÇµÇΩà íuÇ…ï`âÊÇ∑ÇÈ(å≥ÇÃà íuÇÕïœÇ¶Ç»Ç¢)
void EquipmentIcon::DrawAt(const CVector2D& pos)
{
	CVector2D old_pos = m_img.m_pos;
	m_img.SetPos(pos);
	m_img.Draw();
	m_img.SetPos(old_pos);
}

// ècâ°î‰Çï€Ç¡ÇΩÇ‹Ç‹ max_w Å~ max_h Ç…é˚ÇﬂÇƒÅAíÜêSÇ pos Ç…ÇµÇƒï`Ç≠
void EquipmentIcon::DrawFit(const CVector2D& pos, float max_w, float max_h)
{
	CVector2D old_pos = m_img.m_pos;
	CVector2D old_size = m_img.m_size;
	CVector2D old_center = m_img.m_center;

	float s = min(max_w / old_size.x, max_h / old_size.y);
	CVector2D new_size = old_size * s;

	m_img.SetSize(new_size.x, new_size.y);
	m_img.SetCenter(new_size * 0.5f);
	m_img.SetPos(pos);
	m_img.Draw();

	m_img.SetSize(old_size.x, old_size.y);
	m_img.SetCenter(old_center);
	m_img.SetPos(old_pos);
}
