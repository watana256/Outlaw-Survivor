
#pragma once

#include "Base/Base.h"
#include <unordered_map>

class Enemy;

class StellaBullet : public Base
{
public:
    CImage m_img;

    float m_attack_time;
    float m_attack_cnt;
    float m_attack_radius;
    int m_damage;

    std::unordered_map<Enemy*, float> 
        m_enemy_attack_timers;

public:
    StellaBullet(float radius, float attackTime, int damage);

    void Update() override;
    void Draw() override;
    void Collision(Base* b) override;

    static TexAnimData _anim_data[];
};

