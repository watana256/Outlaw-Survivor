
#pragma once
#include "Base/Base.h"
#include "WeaponData.h"

class Molotov : public Base {
private:
    CImage m_img;
    float m_attak_cnt;
    bool m_is_enable;

    // 現在のレベル
    int m_level;

    // レベルに応じた性能を取得
    int GetDamage() const;
    float GetFlyingDist() const;
    int GetBulletCount() const;
    float GetFlameDuration() const;

    // 火炎瓶を投げる
    void Shot(const CVector2D& pos, const CVector2D& dir);

public:
    Molotov();
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
