#include "Medical.h"
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
	m_rad = 8;
	m_img.SetSize(40, 30);
	m_img.SetCenter(8, 8);
	m_Medical_type = Medical_type;

	switch (m_Medical_type) {
	case eKit:
		m_Medical = 100;
		break;
	case eBin:
		m_Medical = 50;
		break;


	}
	m_img.ChangeAnimation(m_Medical_type);
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

}