#include "Landmine.h"
#include "../Player.h"
#include "../Bullet.h"

#define ATTACK_TIME 0.5f
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
void Landmine::Update()
{
	if (m_is_enable) {
		if (m_attak_cnt > ATTACK_TIME) {
			m_attak_cnt = 0;
			Player* player = Player::Instance();
			if (player != nullptr) {
				CVector2D pos = player->m_pos;
				CVector2D dir = player->m_vec;
				new Bullet(pos, dir, FLYING_DIST);
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