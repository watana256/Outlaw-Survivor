

#pragma once
#include "Base/Base.h"
#include"WeaponData.h"

class Drone : public Base
{
private:
    CImage m_img;

    float m_attak_cnt;
    bool m_is_enable;

    float m_attack_range;
    float m_offset_x;
    float m_offset_y;

    // レベル
    int m_level;

    // レベルに応じた性能を取得
    int GetDamage() const;
    float GetFlyingDist() const;
    float GetBulletSpeed() const;
    float GetAttackInterval() const;

public:
    Drone();
    static const WeaponData& GetData();

    void SetEnable(bool enable);

    // レベル操作
    void SetLevel(int level);
    int GetLevel() const;
    void LevelUp();

    void Update() override;
    void Draw() override;
    void Collision(Base* b) override;
    static int GetDamageAtLevel(int level);
    static float GetFlyingDistAtLevel(int level);
    static float GetBulletSpeedAtLevel(int level);
};

