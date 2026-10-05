#include "Shop.h"
Shop::Shop() :Base(eTpe_Shop)
{
	m_img = COPY_RESOURCE("SHOP", CImage);
	//m_font = GET_RESOURCE("ShopFont", CFont);
	m_cnt = 0;
}
void Shop::Update()
{
	 
	if (m_cnt++ > 60 && PUSH(CInput::eButton5)) {
		Base::KillAll();
		//ƒQ[ƒ€ƒV[ƒ“‚Ö
	}
}
void Shop::Draw()
{
	m_img.Draw();
	//m_font->Draw(64, 256, 0, 0, 0, "Title");
}
