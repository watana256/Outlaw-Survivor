#include "Landmine.h"
#include "../Player.h"
#include "../BulletAll/LandmineBullet.h"

#define ATTACK_TIME 3.0f
#define FLYING_DIST 100.0f
Landmine::Landmine()
	: Base(eType_Landmine)
	, m_attak_cnt(0)
	, m_is_enable(false)
{

}
void Landmine::SetEnable(bool enable)
{
	m_is_enable = enable;
}
int Landmine::GetDamage() const
{
	int lv = 1;
	switch (lv)
	{
	case 1: return 10;
	case 2: return 20;
	case 3: return 30;
	case 4: return 40;
	}
	return 1;
}
void Landmine::Update()
{
	if (m_is_enable) {
		if (m_attak_cnt > ATTACK_TIME) {
			m_attak_cnt = 0;
			Player* player = Player::Instance();
			if (player != nullptr) {
				CVector2D pos = player->m_pos;
				CVector2D dir = player->m_vec;
				new LandmineBullet(pos, dir, FLYING_DIST, GetDamage());
			}

		}
		m_attak_cnt += CFPS::GetDeltaTime();
	}
}
void Landmine::Draw()
{

}
void Landmine::Collision(Base* b)
{

}