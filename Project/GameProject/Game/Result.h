#pragma once
#include "Base/Base.h"

// ゲームオーバー / ゲームクリアの結果画面
class Result : public Base {
	CImage m_img;        // 背景画像
	CImage m_score_img;  // スコアの数字(Scoreと同じ画像)
	int m_cnt;           // 表示してからのフレーム数(誤入力防止)
	int m_score;         // 最終スコア
	bool m_is_clear;     // true: ゲームクリア, false: ゲームオーバー

public:
	Result(bool is_clear = false);
	void Update();
	void Draw();
};