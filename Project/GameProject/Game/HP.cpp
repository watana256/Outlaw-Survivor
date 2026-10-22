#include "HP.h"
#include "Player.h"
#include "Medical.h"
HP::HP() :Base(eType_Hp)
{
	char* nem[10] = {
		"hp10",
		"hp20",
		"hp30",
		"hp40",
		"hp50",
		"hp60",
		"hp70",
		"hp80",
		"hp90",
		"hp100"
	};
	for (int i = 0; i < 10; i++) {
		
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
			int i = hp / (float)maxhp * 9;
			m_img[i].SetPos(-90, 10);
			m_img[i].SetSize(500, 150);
			m_img[i].Draw();
		}
	}
}