
#pragma once

#include "Base/Base.h"

class Enemy;

class StellaBullet : public Base
{
public:
    CImage m_img;

    float m_attack_time;
    float m_attack_cnt;
    float m_attack_radius;
    int m_damage;

    std::vector<Enemy*> m_hit_enemies;

public:
    StellaBullet(float radius, float attackTime, int damage);

    void Update() override;
    void Draw() override;
    void Collision(Base* b) override;
};

