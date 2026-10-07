#include "Player.h"
#include "Bullet.h"
#include "Weapon/Pistol.h"

#define SPEED 5.0f
#define MUTEKI 60.0f
#define ATTACK_TIME 1.0f

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

Player* Player::ms_instance = nullptr;

Player::Player(const CVector2D& pos)
	:Base(eType_Player)
	, m_speed_cnt(0)
{
	ms_instance = this;
	
	m_vec = CVector2D::right;
	m_img = COPY_RESOURCE("Player", CImage);
	m_pos_old = m_pos = pos;
	m_img.SetSize(150, 150);
	m_img.SetCenter(75, 75);
	m_rect = CRect(-15, -20, 15, 30);
	m_img.ChangeAnimation(eState_Damage_up);
	m_hp = 100;
	m_muteki_cnt = 0.0f;

	m_pistol = new Pistol();

	m_pistol->SetEnable(true);
}
Player::~Player() {
	ms_instance = nullptr;
}
Player* Player::Instance() {
	return ms_instance;
}
void Player::TakeDamage(int damage)
{
	if (m_muteki_cnt > 0) return;
	//HP減少。下限0
	m_hp = max(m_hp - damage, 0);
	m_muteki_cnt = MUTEKI;
	if (m_hp <= 0) {
		SetKill();

	}

}
void Player::Update()
{
	// ★追加：無敵時間が残っていればタイマーを減らす処理
	if (m_muteki_cnt > 0.0f) {
		m_muteki_cnt -= CFPS::GetDeltaTime();
		if (m_muteki_cnt < 0.0f) {
			m_muteki_cnt = 0.0f; // 0以下になったら0に補正
		}
	}

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
		m_vec = CVector2D::up;
		m_pos += m_vec * move_Speed;
		Animu = eAnim_Walk_up;
	}
	else if (HOLD(CInput::eDown))
	{
		m_vec = CVector2D::down;
		m_pos += m_vec * move_Speed;
		Animu = eAnim_Walk_down;
	}
	if (HOLD(CInput::eLeft))
	{
		m_vec = CVector2D::left;
		m_pos += m_vec * move_Speed;
		Animu = eAnim_Walk_left;
	}
	else if (HOLD(CInput::eRight))
	{
		m_vec = CVector2D::right;
		m_pos += m_vec * move_Speed;
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
	DrawRect();
	m_img.SetRect(128, 0, 192, 64);
	Utility::DrawCircle(m_pos, m_rad, CVector4D(0, 0, 1, 0.5));
}
