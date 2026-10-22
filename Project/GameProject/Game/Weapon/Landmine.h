
#pragma once
#include "Base/Base.h"

class Landmine : public Base {
private:
    CImage m_img;
    float m_attak_cnt;
    bool m_is_enable;

    // 現在のレベル
    int m_level;

    // レベルごとの性能
    int GetDamage() const;
    float GetAttackInterval() const;

public:
    Landmine();

    void SetEnable(bool enable);

    // レベル操作
    void SetLevel(int level);
    int GetLevel() const;
    void LevelUp();

    void Update();
    void Draw();
    void Collision(Base* b);
};
