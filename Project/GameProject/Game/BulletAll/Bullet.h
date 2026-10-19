
#pragma once
#include "Base/Base.h"

class Bullet : public Base {
public:
    CImage m_img;
    float m_flying_dist;
    float m_move_dist;
    int m_damage;

    void Collision(Base* b);

public:
    Bullet(
        const CVector2D& pos,
        const CVector2D& dir,
        float dist,
        int damage,
        float radius = 10.0f
    );

    void Update();
    void Draw();
};
