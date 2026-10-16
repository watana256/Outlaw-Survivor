#pragma once
#include "Base/Base.h"
class Enemy;

class LandmineBullet :public Base {
public:
    CImage m_img;
    CImage m_explosion_img;
    float m_flying_dist;
    float m_move_dist;
    int m_damage;
    bool m_is_explosion;
    std::vector<Enemy*> m_hit_enemies;

    void Collision(Base* b);
    void Explosion();

public:
    LandmineBullet(const CVector2D& pos, const CVector2D& dir, float dist, int damage);
    void Update();
    void Draw();

    static TexAnimData _anim_data[];

};