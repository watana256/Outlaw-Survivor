#include "Money.h"
#include "Score.h"
#include "Game.h"
static TexAnim _eMoney_Gold[] =
{
	{1,1}
};
static TexAnim _eMoney_Silver[] =
{
	{0,1}
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
	ClampPosition();
	m_rad = 20;
	m_rect = CRect(-20.0f, -20.0f, 20.0f, 20.0f);   //拾える範囲(コインの中心から±40px)
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
// 画面の端から外に出ないよう、内側へ押し戻す(Playerと同じ範囲)
void Money::ClampPosition()
{
	const float margin_x = 30.0f;
	const float margin_y = 140.0f;

	if (m_pos.x < margin_x) m_pos.x = margin_x;
	if (m_pos.x > SCREEN_WIDTH - margin_x) m_pos.x = SCREEN_WIDTH - margin_x;

	if (m_pos.y < margin_y) m_pos.y = margin_y;
	if (m_pos.y > SCREEN_HEIGHT - margin_y) m_pos.y = SCREEN_HEIGHT - margin_y;
}
