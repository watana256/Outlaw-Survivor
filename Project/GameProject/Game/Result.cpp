#include"Result.h"
#include"Game.h"
#include"../Base/Base.h"
Result::Result() :Base(eType_Scene)
{
	m_img = COPY_RESOURCE("1119", CImage);
	//位置設定
	m_img.SetPos(0, 0);
	//画像のサイズ設定
	m_img.SetSize(1920, 1080);
}
void Result::Update()
{
	//ボタン１でタイトル破棄
	if (m_cnt++ > 2 && PUSH(CInput::eButton1)) {
		//全てのオブジェクトを破棄
		Base::KillAll();
		//ゲームシーンへ
		//new Game();//後で追加
	}
}
void Result::Draw()
{
	m_img.Draw();

}