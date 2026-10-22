#include "Equipment.h"
#include "Game.h"
Equipment::Equipment() 
	:Base(eTpe_Equipment)
	,m_is_show(false)
{
	m_img = COPY_RESOURCE("SHOP", CImage);
	//m_font = GET_RESOURCE("ShopFont", CFont);
	
}
bool Equipment::IsShow() const
{
	return m_is_show;
}
void Equipment::Show()
{
	m_is_show = true;
}
void Equipment::Update()
{
	if (!m_is_show)return;
	if (PUSH(CInput::eButton5))
	{
		m_is_show = false;
	}
}
void Equipment::Draw()
{
	if (!m_is_show)return;
	m_img.Draw();
	//m_font->Draw(64, 256, 0, 0, 0, "Title");
}
