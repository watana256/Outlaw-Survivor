#include "FlamethrowerBullet.h"
#include "../Enemy.h"

#define  BULLET_SPEED 7.0f
#define ANIM_FRAME 5
#define EXP_RAD 64.0f
#define EXP_IMG_SIZE 128.0f

static TexAnim _flameeffects_anim[] =
{
    {0,ANIM_FRAME},{1,ANIM_FRAME},{2,ANIM_FRAME},{3,ANIM_FRAME},
    {4,ANIM_FRAME},{5,ANIM_FRAME},{6,ANIM_FRAME},{7,ANIM_FRAME},
    {8,ANIM_FRAME},{9,ANIM_FRAME},{10,ANIM_FRAME},{11,ANIM_FRAME},
    {12,ANIM_FRAME},{13,ANIM_FRAME}
};

TexAnimData FlamethrowerBullet::_anim_data[] =
{
    ANIMDATA(_flameeffects_anim),
};

FlamethrowerBullet::FlamethrowerBullet(const CVector2D& pos, const CVector2D& dir, float dist, int damage)
    :Base(eType_RoketBullet)
    , m_flying_dist(dist)
    , m_move_dist(0.0f)
    , m_damage(damage)
    ,m_is_flameeffects(false)
{
    m_img.Load("Image/FlamethrowerBullet.png",_anim_data, 64, 64);
    m_pos = pos;
    m_vec = dir;
    m_rad = 10;
    m_rect = CRect(-20, -20, 20, 20);

    m_img.SetSize(200, 200);
    m_img.SetCenter(100, 100);
    m_img.SetAng(atan2f(-m_vec.y, m_vec.x));

    m_img.ChangeAnimation(0, true);
}
void FlamethrowerBullet::Update()
{
    const float move_speed = BULLET_SPEED;
    m_pos += m_vec * move_speed;

    m_move_dist += move_speed;
    if (m_move_dist >= m_flying_dist) {
        SetKill();
    }
    m_img.UpdateAnimation();
}
void FlamethrowerBullet::Draw()
{
    m_img.SetPos(m_pos);
    m_img.Draw();
    Utility::DrawCircle(m_pos, m_rad, CVector4D(0, 5, 0, 0.5));
}
void FlamethrowerBullet::Collision(Base* b)
{
    if (b->m_type == eType_Enemy) {
        // 当たり判定（四角形判定）
        if (Base::CollisionCircle(this, b)) {
            if (Enemy* e = dynamic_cast<Enemy*>(b)) {
                e->TakeDamage(m_damage); // エネミーにダメージ
                SetKill();

            }
        }
    }
}
//火炎を縦から横に