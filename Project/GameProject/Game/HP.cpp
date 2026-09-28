#include "HP.h"
#include "Player.h"

HP::HP() :Base(eType_UI)
{
	char* nem[9] = {
		"hp10",
		"hp20",
		"hp30",
		"hp50",
		"hp60",
		"hp70",
		"hp80",
		"hp90",
		"hp100"
	};
	for (int i = 0; i < 9; i++) {
		m_img[i] = COPY_RESOURCE(nem[i], CImage);
	}
}
void HP::Draw()
{
	Base* player = Base::FindObject(eType_Player);
	if (player)
	{
		if (Player* f = dynamic_cast<Player*>(player)) {
			int hp = f->Gethp(); int maxhp = f->Getmaxhp();
			int i = hp / (float)maxhp * 8;
			m_img[i].SetPos(-90, 10);
			m_img[i].SetSize(500, 150);
			m_img[i].Draw();
		}
	}
}