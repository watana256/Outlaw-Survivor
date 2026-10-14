#pragma once
#include "Base/Base.h"


class FlamethrowerBullet :public Base {
public:
    CImage m_img;
    float m_flying_dist;
    float m_move_dist;
    int m_damage;
    void Collision(Base* b);

public:
    FlamethrowerBullet(const CVector2D& pos, const CVector2D& dir, float dist, int damage);
    void Update();
    void Draw();
};