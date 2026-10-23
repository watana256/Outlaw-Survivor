
#pragma once
#include "Base/Base.h"

class AxBullet : public Base {
public:
    CImage m_img;
    CImage m_Ax_img;

    float m_flying_dist;
    float m_move_dist;
    float m_move_speed;

    int m_damage;

    void Collision(Base* b);

public:
    AxBullet(
        const CVector2D& pos,
        const CVector2D& dir,
        float dist,
        int damage,
        float speed = 7.0f
    );

    void Update();
    void Draw();

    static TexAnimData _anim_data[];
};
