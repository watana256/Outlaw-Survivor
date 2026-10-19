
#pragma once
#include "Base/Base.h"

class Drone : public Base
{
private:
    CImage m_img;

    float m_attak_cnt;
    bool m_is_enable;

    float m_attack_range;
    float m_offset_x;
    float m_offset_y;

    int GetDamage() const;

public:
    Drone();

    void SetEnable(bool enable);
    void Update() override;
    void Draw() override;
    void Collision(Base* b) override;
};
