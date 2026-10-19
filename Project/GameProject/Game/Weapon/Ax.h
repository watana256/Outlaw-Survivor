
#pragma once
#include "Base/Base.h"

class Ax : public Base {
private:
    CImage m_img;
    float m_attak_cnt;
    bool m_is_enable;

    // 武器レベル
    int m_level;

    // レベルに応じた性能
    int GetDamage() const;
    float GetFlyingDist() const;
    float GetBulletSpeed() const;
    int GetBulletCount() const;
    float GetAttackInterval() const;

    void Shot(const CVector2D& pos, const CVector2D& dir);

public:
    Ax();

    void SetEnable(bool enable);

    // レベルを設定・取得
    void SetLevel(int level);
    int GetLevel() const;

    // 1レベル上げる
    void LevelUp();

    void Update();
    void Draw();
    void Collision(Base* b);
};
