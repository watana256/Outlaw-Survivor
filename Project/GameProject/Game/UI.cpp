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
	UIType = 0; //ここを変更でフェイズが進む


}
void UI::Draw()
{
    // 横6列・縦2段の計算
    int col = UIType % 6; // 列（横の位置：1から6）
    int row = UIType / 6; // 行（縦の位置：1または2）

    // 1コマの幅と高さ
    int width = 360;
    int height = 148;

    // 切り抜き範囲（left, top, right, bottom）の計算
    int left = col * width;
    int top = row * height;
    int right = left + width;
    int bottom = top + height;

    // 切り抜いて描画
    m_img.SetRect(left, top, right, bottom);
    m_img.Draw();
}