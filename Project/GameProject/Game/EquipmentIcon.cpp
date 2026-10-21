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
// Žw’è‚µ‚½ˆÊ’u‚É•`‰æ‚·‚é(Œ³‚ÌˆÊ’u‚Í•Ï‚¦‚È‚¢)
void EquipmentIcon::DrawAt(const CVector2D& pos)
{
	CVector2D old_pos = m_img.m_pos;
	m_img.SetPos(pos);
	m_img.Draw();
	m_img.SetPos(old_pos);
}
