#include "Score.h"

int Score::s_score = 0;

Score::Score() :Base(eType_Score)
{
	m_img = COPY_RESOURCE("Score", CImage);
	Score::s_score = 0;
}


void Score::Update()
{
	Score::s_score;
}
void Score::Draw()
{
	int score = s_score;
	for (int i = 0; i < 6; i++, score /= 10) {
		int s = score % 10;
		m_img.SetRect(100 * s, 0, 100 * s + 100, 128);
		m_img.SetSize(150, 110);
		m_img.SetPos(1700 - 102 * i, 10);
		m_img.Draw();
	}
}