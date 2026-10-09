#define FLYING_DIST 100.0f
Pistol::Pistol()
	: Base(eType_Pistil)
	, m_attak_cnt(0)
	, m_is_enable(false)
{

}
void Pistol::SetEnable(bool enable)
{
	m_is_enable = enable;
}
void Pistol::Update()
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
void Pistol::Draw()
{

}
void Pistol::Collision(Base* b)
{

}