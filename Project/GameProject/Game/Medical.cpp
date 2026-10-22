#include "Medical.h"
#include "Enemy.h"
#include "Player.h"
#include "Game.h"
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
	ClampPosition();
	m_rad = 10;
	m_rect = CRect(-30.0f, -30.0f, 55.0f, 45.0f);
	m_img.SetSize(45, 35);
	m_img.SetCenter(10, 10);
	m_Medical_type = Medical_type;

	switch (m_Medical_type) {
	case eKit:
		m_Medical = 20;
		m_Medicalk = 20;
		if (m_Medicalk >= 11) {
			m_Medicalk = 20;
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
	// 1. プレイヤーと接触した時の処理（回復して消える）
	if (b->m_type == eType_Player) {
		if (Base::CollisionRect(this, b)) {
			if (Player* p = dynamic_cast<Player*>(b)) {
				p->TakeHell(-m_Medical); // HP回復
				SetKill();               // 拾ったので消去
			}
		}
	}

	// ★ 2. 回復アイテム同士、または敵と重なった時に押し出す処理！
	if (b->m_type == eType_Medical || b->m_type == eType_Enemy) {
		CVector2D v = m_pos - b->m_pos;
		float len = v.Length();
		float dist = m_rad + b->m_rad; // お互いの判定半径の合計

		// めり込んでいる場合、外側に押し出す
		if (len < dist && len > 0.0001f) {
			CVector2D dir = v.GetNormalize();
			float push = (dist - len) * 0.5f;
			m_pos += dir * push;

			ClampPosition(); // 押し出された拍子に画面外へ出ないよう画面内に収める
		}
	}
}