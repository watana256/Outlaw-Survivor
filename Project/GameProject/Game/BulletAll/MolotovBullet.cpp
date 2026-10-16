#include "MolotovBullet.h"
#include "../Enemy.h"

#define  BULLET_SPEED 7.0f
#define ANIM_FRAME 7
#define EXP_RAD 64.0f
#define EXP_IMG_SIZE 256.0f

static TexAnim _molotov_anim[] =
{
    {0,ANIM_FRAME},{1,ANIM_FRAME},{2,ANIM_FRAME},{3,ANIM_FRAME}, {4,ANIM_FRAME}
};

static TexAnim _flameeteffect_anim[] =
{
    {0,ANIM_FRAME},{1,ANIM_FRAME},{2,ANIM_FRAME},{3,ANIM_FRAME},
    {4,ANIM_FRAME},{5,ANIM_FRAME},{6,ANIM_FRAME},{7,ANIM_FRAME},
    {8,ANIM_FRAME},{9,ANIM_FRAME},{10,ANIM_FRAME},{11,ANIM_FRAME},

};

TexAnimData MolotovBullet::_anim_data[] =
{
    ANIMDATA(_molotov_anim),
    ANIMDATA(_flameeteffect_anim),
};

MolotovBullet::MolotovBullet(const CVector2D& pos, const CVector2D& dir, float dist, int damage)
    :Base(eType_RoketBullet)
    , m_flying_dist(dist)
    , m_move_dist(0.0f)
    , m_damage(damage)
    , m_is_flameeteffect(false)
{
    m_img.Load("Image/MolotovBullet.png", _anim_data, 145, 145);
    m_flameeteffect_img = COPY_RESOURCE("FlameEffect", CImage);

    m_pos = pos;
    m_vec = dir;
    m_rad = 10;
    m_rect = CRect(-20, -20, 20, 20);

    m_img.SetSize(40, 40);
    m_img.SetCenter(20, 20);
    m_img.SetAng(atan2f(-m_vec.y, m_vec.x));

    m_img.ChangeAnimation(0, true);

    m_flameeteffect_img.SetSize(EXP_IMG_SIZE, EXP_IMG_SIZE);
    m_flameeteffect_img.SetCenter(EXP_IMG_SIZE * 0.5f, EXP_IMG_SIZE * 0.5f);
}

void MolotovBullet::Update()
{
    if (!m_is_flameeteffect)
    {
        const float move_speed = BULLET_SPEED;
        m_pos += m_vec * move_speed;

        m_move_dist += move_speed;
        if (m_move_dist >= m_flying_dist) {
            
            FlameEffect();
        }
        m_img.UpdateAnimation();
    }
    else {
        m_flameeteffect_img.UpdateAnimation();

        if (m_flameeteffect_img.CheckAnimationEnd()) {
            SetKill();
        }
    }

}

void MolotovBullet::Draw()
{
    if (!m_is_flameeteffect) {
        m_img.SetPos(m_pos);
        m_img.Draw();
        //Utility::DrawCircle(m_pos, m_rad, CVector4D(0, 5, 0, 0.5));
    }
    else
    {
        m_flameeteffect_img.SetPos(m_pos);
        m_flameeteffect_img.Draw();
        //Utility::DrawCircle(m_pos, m_rad, CVector4D(1, 0, 0, 0.5));
    }
}

void MolotovBullet::Collision(Base* b)
{
    if (b->m_type == eType_Enemy) {
        Enemy* e = dynamic_cast<Enemy*>(b);
        if (e == nullptr) return;

        if (!m_is_flameeteffect) {
            // 当たり判定（四角形判定）
            if (Base::CollisionCircle(this, b)) {
                FlameEffect();
            }
        }
        else {
            auto it = std::find(m_hit_enemies.begin(), m_hit_enemies.end(), e);
            if (it != m_hit_enemies.end()) return;

            if (Base::CollisionCircle(this, b)) {
                e->TakeDamage(m_damage);
                m_hit_enemies.push_back(e);
            }
        }
    }
}

void MolotovBullet::FlameEffect()
{
    if (m_is_flameeteffect) return;
    m_is_flameeteffect = true;
    m_flameeteffect_img.ChangeAnimation(0, false);
    m_rad = EXP_RAD;
}
