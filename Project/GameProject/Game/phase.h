#pragma once
#include "Base/Base.h"
class phase : public Base {
private:

    //ó‘Ô•Ï”
    int m_state;
    int phaseType;
    CImage m_img;

    bool m_flip;
    //‘Ì—Í


public:
    //Enemy(const CVector2D& pos, bool flip);
    phase();
    void eStatephase();
    void Update();
    void Draw();
    void Collision(Base* b);
    static void Downphase();
    static int m_phase;

};
extern TexAnimData enemy_anim_data[];

