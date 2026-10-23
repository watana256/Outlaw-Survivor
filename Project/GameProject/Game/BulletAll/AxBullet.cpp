
#include "AxBullet.h"
#include "../Enemy.h"

#define ANIM_FRAME 5

static TexAnim _ax_anim[] =
{
    {0, ANIM_FRAME},
    {1, ANIM_FRAME},
    {2, ANIM_FRAME},
    {3, ANIM_FRAME},
    {4, ANIM_FRAME}
};

TexAnimData AxBullet::_anim_data[] =
{
    ANIMDATA(_ax_anim),
};

AxBullet::AxBullet(
    const CVector2D& pos,
    const CVector2D& dir,
    float dist,
    int damage,
    float speed)
    : Base(eType_Bullet)
    , m_flying_dist(dist)
    , m_move_dist(0.0f)
    , m_move_speed(speed)
    , m_damage(damage)
{
    
    m_img = COPY_RESOURCE("AxBullet", CImage);

    m_pos = pos;
    m_vec = dir;
    m_rad = 25;

    m_img.SetSize(50, 50);
    m_img.SetCenter(25, 25);
    m_img.SetAng(atan2f(-m_vec.y, m_vec.x));

    m_img.ChangeAnimation(0, true);
}

void AxBullet::Update()
{
    // レベルに応じた弾速で移動
    m_pos += m_vec * m_move_speed;

    // 移動した距離を記録
    m_move_dist += m_move_speed;

    // 設定された飛距離に到達したら消滅
    if (m_move_dist >= m_flying_dist)
    {
        SetKill();
    }

    m_img.UpdateAnimation();
}

void AxBullet::Draw()
{
    m_img.SetPos(m_pos);
    m_img.Draw();

    // 当たり判定を確認したいときはコメントを外す
    // Utility::DrawCircle(
    //     m_pos,
    //     m_rad,
    //     CVector4D(0, 5, 0, 0.5)
    // );
}

void AxBullet::Collision(Base* b)
{
    if (b == nullptr || b->m_type != eType_Enemy)
        return;

    if (Base::CollisionCircle(this, b))
    {
        if (Enemy* e = dynamic_cast<Enemy*>(b))
        {
            e->TakeDamage(m_damage);
            SetKill();
        }
    }
}
