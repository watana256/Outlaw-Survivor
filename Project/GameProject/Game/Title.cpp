#include"Title.h"
#include"Game.h"
#include"../Base/Base.h"
Title::Title() :Base(eType_Scene)
{
	m_img = COPY_RESOURCE("1117", CImage);
	m_Logo = COPY_RESOURCE("1118", CImage);
	//位置設定
	m_img.SetPos(0, 0);
	//画像のサイズ設定
	m_img.SetSize(1920, 1080);
	//位置設定
	m_Logo.SetPos(110, 700);
	//画像のサイズ設定
	m_Logo.SetSize(1567, 320);
}
void Title::Update()
{
	//ボタン１でタイトル破棄
	if (m_cnt++ > 2 && PUSH(CInput::eButton1)) {
		//全てのオブジェクトを破棄
		Base::KillAll();
		//ゲームシーンへ
		//new Game();//後で追加
	}
}
void Title::Draw()
{
	m_img.Draw();
	m_Logo.Draw();

}