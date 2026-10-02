#pragma once
#include"../Base/Base.h"

class Bullet :public Base {
public:
    CImage m_img;
    void Collision(Base* b);

public:
    Bullet(const CVector2D& pos, const CVector2D& dir);
    void Update();
    void Draw();
};