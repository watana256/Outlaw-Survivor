#pragma once
#include "../Base/Base.h"

class Score : public Base {
public:
    CImage m_img;
    CFont* m_font;
public:
    Score();
    static int s_score;
    void Update();
    void Draw();
};
