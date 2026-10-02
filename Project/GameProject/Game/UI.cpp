#include "UI.h"
#include "Player.h"
#include "Enemy.h"

UI::UI() :Base(eType_UI)
{
	//位置設定
	m_img.SetPos(728, 30);
	//画像のサイズ設定
	m_img.SetSize(250, 100);
	//描画
	
}
void UI::Draw()
{

		for (int i = 0; i < 10; i++) {


			//画像の切り抜き
			m_img.SetRect(11 * 360, 0, 11 * 360 + 360, 148);
			m_img.Draw();
		}

}