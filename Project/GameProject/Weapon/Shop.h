#pragma once
#include "../Base/Base.h"

class Shop : public Base {
public:
    CImage m_img;
public:
    Shop();
    void Update();
    void Draw();
};