
#pragma once
#include "Base/Base.h"
#include "WeaponData.h"

class Rocket : public Base
{
private:
    CImage m_img;
    float m_attak_cnt;
    bool m_is_enable;

    // •ŠíƒŒƒxƒ‹
    int m_level;

    int GetDamage() const;
    float GetFlyingDist() const;

public:
    Rocket();
    static const WeaponData& GetData();

    void SetEnable(bool enable);

    void SetLevel(int level);
    int GetLevel() const;
    void LevelUp();

    void Update();
    void Draw();
    void Collision(Base* b);
    static int GetDamageAtLevel(int level);
    static float GetFlyingDistAtLevel(int level);
};