

#pragma once
#include "Base/Base.h"
#include "WeaponData.h"

class StellaBullet;

class Stella : public Base
{
private:
    CImage m_img;
    float m_attak_cnt;
    bool m_is_enable;

    StellaBullet* m_stella_bullet;

    int m_level;

    int GetDamage() const;
    float GetAttackRange() const;
    float GetAttackInterval() const;
    float GetImageSize() const;

    void UpdateBulletStats();

public:
    Stella();
    static const WeaponData& GetData();

    void SetEnable(bool enable);

    void SetLevel(int level);
    int GetLevel() const;
    void LevelUp();

    void Update();
    void Draw();
    void Collision(Base* b);
};
