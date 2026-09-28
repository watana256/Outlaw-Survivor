#pragma once
#include "../Base/Base.h"

class HP : public Base {
public:
    CImage m_img[10];
public:
    HP();
    void Draw();
};