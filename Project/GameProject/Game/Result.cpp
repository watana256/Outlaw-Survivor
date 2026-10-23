#include"Result.h"
#include"Game.h"
#include "Score.h"
#include"../Base/Base.h"

Result::Result(bool is_clear) :Base(eType_Scene)
, m_cnt(0)
, m_is_clear(is_clear)
{
	m_img = COPY_RESOURCE("1119", CImage);
	//位置設定
	m_img.SetPos(0, 0);
	//画像のサイズ設定
	m_img.SetSize(1920, 1080);

	// スコアの数字画像(Score.cppと同じものを使う)
	m_score_img = COPY_RESOURCE("Score", CImage);

	// 最終スコアを保存(先に保存してから表示用のScoreを消す)
	m_score = Score::GetScore();

	// 結果画面の上にHPバーやスコアが重ならないように消す
	Base::Kill((1 << eType_Hp) | (1 << eType_Score));
}
void Result::Update()
{
	//ボタン１(Zキー)でもう一度遊ぶ。表示して約1秒間は受け付けない
	if (m_cnt++ > 60 && PUSH(CInput::eButton1)) {
		//全てのオブジェクトを破棄
		Base::KillAll();
		//ゲームシーンへ
		new Game();
	}
}
void Result::Draw()
{
	m_img.Draw();

	CFont* font = FONT_T();
	float cx = SCREEN_WIDTH * 0.5f;
	float cy = SCREEN_HEIGHT * 0.5f;

	// ゲームクリア / ゲームオーバーの文字
	if (m_is_clear)
		font->Draw(cx - 100.0f, cy - 140.0f, 1, 1, 0, "ゲームクリア!");
	else
		font->Draw(cx - 100.0f, cy - 140.0f, 1, 0, 0, "ゲームオーバー");

	// スコアの数字(Score::Draw と同じ6桁。画面の中央に並べる)
	const float digit_w = 102.0f;       // 数字1つ分の間隔
	const float digit_size_w = 150.0f;  // 数字1つの幅
	const int digits = 6;
	float total_w = digit_w * (digits - 1) + digit_size_w;
	float left = cx - total_w * 0.5f;
	float top = cy - 70.0f;

	int score = m_score;
	for (int i = 0; i < digits; i++, score /= 10) {
		int s = score % 10;
		m_score_img.SetRect(100 * s, 0, 100 * s + 100, 128);
		m_score_img.SetSize(150, 110);
		m_score_img.SetPos(left + digit_w * (digits - 1 - i), top);
		m_score_img.Draw();
	}

	if (m_cnt > 60)
		font->Draw(cx - 100.0f, cy + 100.0f, 1, 1, 1, "Zキーでもう一度");
}
