
#pragma once
#include "Base/Base.h"
#include "WeaponData.h"

class Pistol : public Base {
private:
    CImage m_img;
    float m_attak_cnt;
    bool m_is_enable;

    // ピストルのレベル
    int m_level;

    // レベルに応じた性能を取得
    int GetDamage() const;
    float GetFlyingDist() const;
    float GetAttackInterval() const;
    float GetBulletSpeed() const;

public:
    Pistol();
    static const WeaponData& GetData();

    void SetEnable(bool enable);

    // レベルを設定・取得
    void SetLevel(int level);
    int GetLevel() const;

    void Update();
    void Draw();
    void Collision(Base* b);
};
