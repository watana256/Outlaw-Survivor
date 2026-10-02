#include "Money.h"
#include "Score.h"
static TexAnim _eMoney_Gold[] =
{
	{0,1}
};
static TexAnim _eMoney_Silver[] =
{
	{1,1}
};
TexAnimData Money::_anim_data[] = 
{
		ANIMDATA(_eMoney_Gold),
		ANIMDATA(_eMoney_Silver)
};

Money::Money(const CVector2D& pos,int money_type) :Base(eType_Money)
{
	m_img = COPY_RESOURCE("Money", CImage);
	m_pos = pos;
	m_rad = 8;
	m_img.SetSize(16, 16);
	m_img.SetCenter(8, 8);
	m_money_type = money_type;

	switch (m_money_type) {
	case eGold:
		m_score = 100;
		break;
	case eSilver:
		m_score = 50;
		break;

	
	}
	m_img.ChangeAnimation(m_money_type);
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
	//Utility::DrawCircle(m_pos, m_rad, CVector4D(0, 5, 0, 0.5));
}
void Money::Collision(Base* b) {
	switch (b->m_type) {
	case eType_Player:
		if (Base::CollisionRect(this, b))
		{
			SetKill();
			Score::s_score += m_score;
		}
		break;
	}
	
}