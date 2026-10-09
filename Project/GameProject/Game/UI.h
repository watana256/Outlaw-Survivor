#pragma once
#include "../Base/Base.h"

class UI : public Base {
private:

public:

    int UIType;

    CImage m_img;
    UI();
    void Draw();

    void AddUIType() {
        UIType++;
    }
};