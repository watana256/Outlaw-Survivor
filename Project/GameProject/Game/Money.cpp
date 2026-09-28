#include "Money.h"
static TexAnim _eMoney_Gold[] =
{
	{1,1}
};
static TexAnim _eMoney_Silver[] =
{
	{2,1}
};
TexAnimData Money::_anim_data[] = 
{
		ANIMDATA(_eMoney_Gold),
		ANIMDATA(_eMoney_Silver)
};

Money::Money(const CVector2D& pos) :Base(eType_Money)
{
	m_img.Load("Image/koinn.png" ,_anim_data, 20, 20);
	m_pos = pos;
	m_rad = 8;
	m_img.SetSize(16, 16);
	m_img.SetCenter(8, 8);
	m_img.ChangeAnimation(eState_Gold);

}
void Money ::Update() 
{
	m_img.UpdateAnimation();
	switch (m_state) {
	case eState_Gold:
		eStateGold();
		break;
	case eState_Silver:
		eStateSilver();
		break;
	}
		
}
void Money::eStateGold() 
{
	int Animu = eAnimGold;
}
void Money::eStateSilver() 
{
	int Animu = eState_Silver;
}
void Money:: Draw() 
{
	m_img.SetPos(m_pos);
	m_img.Draw();
	Utility::DrawCircle(m_pos, m_rad, CVector4D(0, 5, 0, 0.5));
}
void Money::Collision(Base* b) {
	switch (b->m_type) {
	case eType_Player:
		if (Base::CollisionRect(this, b))
		{
			SetKill();
		}
		break;
	}
	
}