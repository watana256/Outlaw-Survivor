
#pragma once
#include "Base/Base.h"
#include "WeaponData.h"

class Flamethrower : public Base {
private:
    CImage m_img;
    float m_attak_cnt;
    bool m_is_enable;

    // 現在のレベル
    int m_level;

    // レベルごとの性能を取得
    int GetDamage() const;
    float GetFlyingDist() const;
    float GetBulletSpeed() const;
    float GetAttackRange() const;
    float GetAttackInterval() const;

public:
    Flamethrower();
    static const WeaponData& GetData();

    void SetEnable(bool enable);

    // レベル操作
    void SetLevel(int level);
    int GetLevel() const;
    void LevelUp();

    void Update();
    void Draw();
    void Collision(Base* b);
};
