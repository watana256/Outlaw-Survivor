#include "Player.h"

#define SPEED 5.0f

static TexAnim _Walk_right[] =
{
	{6,5},
	{7,5}
};
static TexAnim _Walk_left[] =
{
	{1,5},
	{3,5}
};
static TexAnim _Walk_up[] =
{
	{13,5},
	{14,5}
};
static TexAnim _Walk_down[] =
{
	{11,10},
	{10,10}
};
static TexAnim _Damage_right[] =
{
	{20,5},
	{21,5},
	{22,5},
	{23,5}
};
static TexAnim _Damage_left[] =
{
	{19,5},
	{18,5},
	{17,5},
	{16,5}
};
static TexAnim _Damage_up[] =
{
	{31,5},
	{30,5},
	{29,5},
	{28,5},
};
static TexAnim _Damage_down[] =
{
	{27,5},
	{26,5},
	{25,5},
	{24,5}
};
static TexAnim _Death[] =
{
	{33,15},
	{34,15},
	{35,15},
	{36,15},
	{37,15},
	{38,15},
	{39,15},
	{40,15},
};

TexAnimData Player::_anim_data[] =
{
	ANIMDATA(_Walk_right),
	ANIMDATA(_Walk_left),
	ANIMDATA(_Walk_up),
	ANIMDATA(_Walk_down),
	ANIMDATA(_Damage_right),
	ANIMDATA(_Damage_left),
	ANIMDATA(_Damage_up),
	ANIMDATA(_Damage_down),
	ANIMDATA(_Death),
};
Player::Player(const CVector2D& pos)
	:Base(eType_Player)
	, m_speed_cnt(0)
{
	m_img = COPY_RESOURCE("Player", CImage);
	m_pos_old = m_pos = pos;
	m_img.SetSize(150, 150);
	m_img.SetCenter(40, 40);
	m_img.ChangeAnimation(eState_Damage_up);
}
void Player::Update()
{
	m_img.UpdateAnimation();
	m_pos_old = m_pos;
	Run();
	switch (m_state) {
	case eState_Walk_up:
		State_Walk_left();
		break;
	case eState_Walk_down:
		State_Walk_down();
		break;
	case eState_Walk_left:
		State_Walk_left();
		break;
	case eState_Walk_right:
		State_Walk_right();
		break;
	case eState_Death:
		State_Death();
		break;
	}


}

void Player::Run()
{
	int Animu = eAnim_Walk_down;
	const int move_Speed = SPEED;

	if (HOLD(CInput::eUp))
	{
		m_pos.y -= move_Speed;
		Animu = eAnim_Walk_up;
	}
	else if (HOLD(CInput::eDown))
	{
		m_pos.y += move_Speed;
		Animu = eAnim_Walk_down;
	}
	if (HOLD(CInput::eLeft))
	{
		m_pos.x -= move_Speed;
		Animu = eAnim_Walk_left;
	}
	else if (HOLD(CInput::eRight))
	{
		m_pos.x += move_Speed;
		Animu = eAnim_Walk_right;
	}
	m_img.ChangeAnimation(Animu);
}
void Player::State_Walk_up()
{
	/*m_img.ChangeAnimation(eAnim_Walk_up, false);
	if (m_img.CheckAnimationEnd()) {
		m_img.ChangeAnimation(eAnim_Walk_up, false);
		m_state = eState_Walk_down;
	}*/
}
void Player::State_Walk_down()
{
	/*m_img.ChangeAnimation(eAnim_Walk_down, false);
	if (m_img.CheckAnimationEnd()) {
		m_img.ChangeAnimation(eAnim_Walk_down, false);
		m_state = eState_Walk_down;
	}*/
}
void Player::State_Walk_left()
{
	/*m_img.ChangeAnimation(eAnim_Walk_left, false);
	if (m_img.CheckAnimationEnd()) {
		m_img.ChangeAnimation(eAnim_Walk_left, false);
		m_state = eState_Walk_down;
	}*/
}
void Player::State_Walk_right()
{
	/*m_img.ChangeAnimation(eAnim_Walk_right, false);
	if (m_img.CheckAnimationEnd()) {
		m_img.ChangeAnimation(eAnim_Walk_right, false);
		m_state = eState_Walk_down;
	}*/
}
void Player::State_Death()
{
	/*m_img.ChangeAnimation(eAnim_Death, false);
	if (m_img.CheckAnimationEnd()) {
		m_img.ChangeAnimation(eAnim_Death, false);
		m_state = eState_Walk_down;
	}*/
}
void Player::Draw()
{
	m_img.SetPos(m_pos);
	m_img.Draw();
	m_img.SetRect(128, 0, 192, 64);
}
