#pragma once
#include "../Base/Base.h"

class Score : public Base {
public:
    CImage m_img;
    CFont* m_font;
    static int GetScore() { return s_score; }
    static void AddScore(int value) { s_score += value; }
public:
    Score();
    static int s_score;
    void Update();
    void Draw();
};
