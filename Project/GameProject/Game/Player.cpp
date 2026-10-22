#include "Player.h"
#include "Weapon/Pistol.h"
#include "Weapon/Ax.h"
#include "Weapon/Drone.h"
#include "Weapon/Flamethrower.h"
#include "Weapon/Landmine.h"
#include "Weapon/Molotov.h"
#include "Weapon/Rocket.h"
#include "Weapon/Shotgun.h"
#include "Weapon/Stella.h"
<<<<<<< HEAD
#include "Game.h"
=======
#include "Game/Game.h"

static Base* CreateWeapon(int index)
{
	switch (index)
	{
	case 0: return new Ax();
	case 1: return new Pistol();
	case 2: return new Shotgun();
	case 3: return new Molotov();
	case 4: return new Flamethrower();
	case 5: return new Landmine();
	case 6: return new Drone();
	case 7: return new Stella();
	case 8: return new Rocket();
	}
	return nullptr;
}
>>>>>>> af7ba514e6715645ab87f5d6d8ab1c9ec7c5e8f4

static void SetWeaponEnable(Base* w, int index, bool enable)
{
	switch (index)
	{
	case 0: ((Ax*)w)->SetEnable(enable); break;
	case 1: ((Pistol*)w)->SetEnable(enable); break;
	case 2: ((Shotgun*)w)->SetEnable(enable); break;
	case 3: ((Molotov*)w)->SetEnable(enable); break;
	case 4: ((Flamethrower*)w)->SetEnable(enable); break;
	case 5: ((Landmine*)w)->SetEnable(enable); break;
	case 6: ((Drone*)w)->SetEnable(enable); break;
	case 7: ((Stella*)w)->SetEnable(enable); break;
	case 8: ((Rocket*)w)->SetEnable(enable); break;
	}
}



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
	{33,30},
	{34,30},
	{35,30},
	{36,30},
	{37,30},
	{38,30},
	{39,30},
	{40,30},
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

	//Pistol* pistol = new Pistol();
	//pistol->SetEnable(true);

	/*Rocket* rocket = new Rocket();
	rocket->SetEnable(true);

	Shotgun* shotgun = new Shotgun();
	shotgun->SetEnable(true);

	Flamethrower* flamethrower = new Flamethrower();
	flamethrower->SetEnable(true);

	Ax* ax = new Ax();
	ax->SetEnable(true);

	Molotov* molotov = new Molotov();
	molotov->SetEnable(true);
	Landmine* landmine = new Landmine();
	landmine->SetEnable(true);

	Stella* stella = new Stella();
	stella->SetEnable(true);

	Drone* drone = new Drone();
	drone->SetEnable(true);

	*/
	for (int i = 0; i < 9; i++) m_weapons[i] = nullptr;
}

	void Player::ClampPosition() {
		const float margin_x = 30.0f;
		const float margin_y = 140.0f;

		if (m_pos.x < margin_x) m_pos.x = margin_x;
		if (m_pos.x > SCREEN_WIDTH - margin_x) m_pos.x = SCREEN_WIDTH - margin_x;

		if (m_pos.y < margin_y) m_pos.y = margin_y;
		if (m_pos.y > SCREEN_HEIGHT - margin_y) m_pos.y = SCREEN_HEIGHT - margin_y;
	}









Player::~Player() {
	ms_instance = nullptr;
}
Player* Player::Instance() {
	return ms_instance;
}

void Player::Update()
{
	static bool s_playing_prev = false;
	bool playing = (Game::GetGamePhase() == EGamePhase::RoundPlaying);
	if (playing && !s_playing_prev) ApplyEquipment();   // 入った瞬間だけ
	s_playing_prev = playing;
	if (!playing) return;

	if (Game::GetGamePhase() != EGamePhase::RoundPlaying)return;
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
	ClampPosition();
}
void Player::TakeDamage(int damage)
{
	if (m_muteki_cnt > 0) return;
	//HP減少。下限0
	m_hp = max(m_hp - damage, 0);
	
	m_muteki_cnt = MUTEKI;
	if (m_hp <= 0) {
		SetKill();
		m_img.ChangeAnimation(eState_Death);
	}

}
void Player::TakeHell(int Hell) {
	m_hp = min(m_hp - Hell, Getmaxhp());
	m_muteki_cnt = MUTEKI;
}
void Player::ApplyEquipment()
{
	// いったん全部無効化
	for (int i = 0; i < 9; i++)
		if (m_weapons[i]) SetWeaponEnable(m_weapons[i], i, false);

	// 装備中の3つだけ有効化(未生成なら作る)
	for (int s = 0; s < 3; s++)
	{
		int w = Game::GetEquippedWeapon(s);
		if (w < 0) continue;
		if (!m_weapons[w]) m_weapons[w] = CreateWeapon(w);
		SetWeaponEnable(m_weapons[w], w, true);
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
	//Utility::DrawCircle(m_pos, m_rad, CVector4D(0, 0, 1, 0.5));
}
