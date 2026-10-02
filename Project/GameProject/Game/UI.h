#pragma once
#include "../Base/Base.h"

class UI : public Base {
private:

    int UIType;

public:
    CImage m_img;
    UI();
    void Draw();
};