#include "Ax.h"
#include "../Player.h"
#include "../BulletAll/AxBullet.h"

#define ATTACK_TIME 0.5f
#define FLYING_DIST 200.0f
#define BULLET_COUNT 2
#define SPREAD_ANGLE 180.0f

Ax::Ax()
	: Base(eType_Shotgun)
	, m_attak_cnt(0)
	, m_is_enable(false)
{

}
void Ax::SetEnable(bool enable)
{
	m_is_enable = enable;
}
void Ax::Shot(const CVector2D& pos, const CVector2D& dir)
{
	for (int i = 0; i < BULLET_COUNT; i++) {
		float per = (float)i / (BULLET_COUNT - 1);
		float ang = Utility::Lerp(-SPREAD_ANGLE, SPREAD_ANGLE, per);
		CVector3D shot_dir = CQuaternion::Euler(0.0f, 0.0f, DtoR(ang)) * dir;
		new AxBullet
		(
			pos,
			CVector2D(shot_dir.x, shot_dir.y),
			FLYING_DIST,
			GetDamage()
		);
	}
}
int Ax::GetDamage() const
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

void Ax::Update()
{
	if (m_is_enable) {
		if (m_attak_cnt > ATTACK_TIME) {
			m_attak_cnt = 0;
			Player* player = Player::Instance();
			if (player != nullptr) {
				CVector2D pos = player->m_pos;
				CVector2D dir = player->m_vec;
				Shot(pos, dir);
			}

		}
		m_attak_cnt += CFPS::GetDeltaTime();
	}
}
void Ax::Draw()
{

}
void Ax::Collision(Base* b)
{

}