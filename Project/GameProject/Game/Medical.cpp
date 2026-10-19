#include "Medical.h"
#include "Enemy.h"
#include "Player.h"
static TexAnim _eMedical_Kit[] =
{
	{1,1}
};
static TexAnim _eMedical_Bin[] =
{
	{0,1}
};
TexAnimData Medical::_anim_data[] =
{
		ANIMDATA(_eMedical_Kit),
		ANIMDATA(_eMedical_Bin)
};

Medical::Medical(const CVector2D& pos, int Medical_type) :Base(eType_Medical)
{
	m_img = COPY_RESOURCE("Medical", CImage);
	m_pos = pos;
	m_rad = 10;
	m_img.SetSize(40, 30);
	m_img.SetCenter(0, 5);
	m_Medical_type = Medical_type;

	switch (m_Medical_type) {
	case eKit:
		m_Medical = 10;
		m_Medicalk = 10;
		if (m_Medicalk >= 11) {
			m_Medicalk = 10;
		}
		break;
	case eBin:
		m_Medical = 50;
		m_Medicalb = 50;
		if (m_Medicalk >= 51) {
			m_Medicalk = 50;
		}
		break;

	}
	m_img.ChangeAnimation(m_Medical_type);
}
void Medical::ClampPosition() {
	const float margin_x = 30.0f;
	const float margin_y = 140.0f;

	if (m_pos.x < margin_x) m_pos.x = margin_x;
	if (m_pos.x > SCREEN_WIDTH - margin_x) m_pos.x = SCREEN_WIDTH - margin_x;

	if (m_pos.y < margin_y) m_pos.y = margin_y;
	if (m_pos.y > SCREEN_HEIGHT - margin_y) m_pos.y = SCREEN_HEIGHT - margin_y;
}

void Medical::Update()
{
	m_img.UpdateAnimation();
	switch (m_Medical) {
	case eState_Kit:
		eStateKit();
		break;
	case eState_Bin:
		eStateBin();
		break;
	}

}
void Medical::eStateKit()
{
	int Animu = eAnimKit;
}
void Medical::eStateBin()
{
	int Animu = eState_Bin;
}
void Medical::Draw()
{
	m_img.SetPos(m_pos);
	m_img.Draw();
	//Utility::DrawCircle(m_pos, m_rad, CVector4D(0, 5, 0, 0.5));
}
void Medical::Collision(Base* b) {
	switch (b->m_type) {
	case eType_Player:
		if (Base::CollisionRect(this, b))
		{
			SetKill();
			Medical::s_Medical += m_Medical;
		}
		break;
	}
	if (b->m_type == eType_Player) {
		// 当たり判定（四角形判定）
		if (Base::CollisionRect(this, b)) {

			if (Player* p = dynamic_cast<Player*>(b)) {
				
				
					p->TakeDamage(-m_Medical);
					m_Medical=NULL;
			}
		}
	}


}