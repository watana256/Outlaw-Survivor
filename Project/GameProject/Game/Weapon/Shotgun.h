
#pragma once
#include "Base/Base.h"
#include "WeaponData.h"

class Shotgun : public Base {
private:
    CImage m_img;
    float m_attak_cnt;
    bool m_is_enable;

    // ショットガンのレベル
    int m_level;

    // レベルに応じた性能を取得
    int GetDamage() const;
    float GetFlyingDist() const;
    float GetBulletSpeed() const;
    int GetBulletCount() const;
    float GetAttackInterval() const;

    // 弾を発射
    void Shot(const CVector2D& pos, const CVector2D& dir);

public:
    Shotgun();
    static const WeaponData& GetData();

    void SetEnable(bool enable);

    // レベル操作
    void SetLevel(int level);
    int GetLevel() const;
    void LevelUp();

    void Update();
    void Draw();
    void Collision(Base* b);
    static int GetDamageAtLevel(int level);
};
