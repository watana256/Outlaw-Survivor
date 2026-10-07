#pragma once
#include"../Base/Base.h"

class Bullet :public Base {
public:
    CImage m_img;
    float m_flying_dist;
    float m_move_dist;
    void Collision(Base* b);

public:
    Bullet(const CVector2D& pos, const CVector2D& dir, float dist);
    void Update();
    void Draw();
};