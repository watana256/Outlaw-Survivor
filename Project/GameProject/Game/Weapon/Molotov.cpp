#include "Molotov.h"
#include "../Player.h"
#include "../BulletAll/Bullet.h"

#define ATTACK_TIME 0.5f
#define FLYING_DIST 100.0f
Molotov::Molotov()
	: Base(eType_Landmine)
	, m_attak_cnt(0)
	, m_is_enable(false)
{

}
void Molotov::SetEnable(bool enable)
{
	m_is_enable = enable;
}
int Molotov::GetDamage() const
{
	int lv = 1;
	switch (lv)
	{
	case 1: return 1;
	case 2: return 2;
	case 3: return 3;
	case 4: return 4;
	}
	return 1;
}
void Molotov::Update()
{
	if (m_is_enable) {
		if (m_attak_cnt > ATTACK_TIME) {
			m_attak_cnt = 0;
			Player* player = Player::Instance();
			if (player != nullptr) {
				CVector2D pos = player->m_pos;
				CVector2D dir = player->m_vec;
				new Bullet(pos, dir, FLYING_DIST, GetDamage());
			}

		}
		m_attak_cnt += CFPS::GetDeltaTime();
	}
}
void Molotov::Draw()
{

}
void Molotov::Collision(Base* b)
{

}