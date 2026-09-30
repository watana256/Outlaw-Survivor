#include"phase.h"
int phase::m_phase(11);



//Enemy::Enemy(const CVector2D& pos, bool flip) :Base(eType_Enemy)
phase::phase() : Base(eType_UI)
{
	m_img = COPY_RESOURCE("phase", CImage);
	m_pos = CVector2D(500, 500);
	phaseType = 0;

}
void phase::Collision(Base* b)
{


}

void phase::Update()
{


}
void phase::Downphase() {
	m_phase -= 1;
}
void phase::Draw()
{
	/*#if _DEBUG
	//切り替え
	if (PUSH(CInput::eButton3)) {

		HPType++;
	}
#endif*/
	for (int i = 0; i < m_phase; i++) {
		//画像の切り抜き
		m_img.SetRect(phaseType * 360, 0, phaseType * 360 + 360, 148);
		m_img.Draw();

	}

	//位置設定
	m_img.SetPos(728, 30);
	//画像のサイズ設定
	m_img.SetSize(250, 100);
	//描画





}
