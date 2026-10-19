
#pragma once
#include "Base/Base.h"

class StellaBullet;  // ‡@ StellaBulletƒNƒ‰ƒX‚ÌéŒ¾

class Stella : public Base {
private:
    CImage m_img;
    float m_attak_cnt;
    bool m_is_enable;

    StellaBullet* m_stella_bullet;  // ‡A ‚±‚±‚É’Ç‰ÁI

    int GetDamage() const;

public:
    Stella();

    void SetEnable(bool enable);
    void Update();
    void Draw();
    void Collision(Base* b);
};
