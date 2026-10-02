#include "UI.h"
#include "Player.h"
#include "Enemy.h"

UI::UI() :Base(eType_UI)
{
	m_img = COPY_RESOURCE("UI",CImage);

	//位置設定
	m_img.SetPos(728, 30);
	//画像のサイズ設定
	m_img.SetSize(250, 100);
	//描画
	UIType = 0;


}
void UI::Draw()
{

		for (int i = 0; i < 10; i++) {


			//画像の切り抜き
			m_img.SetRect(UIType * 360, 0, UIType * 360 + 360, 148);
			m_img.Draw();
		}

}