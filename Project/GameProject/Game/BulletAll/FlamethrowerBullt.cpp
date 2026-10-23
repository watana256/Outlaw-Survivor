
#include "FlamethrowerBullet.h"
#include "../Enemy.h"

#define ANIM_FRAME 5

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

FlamethrowerBullet::FlamethrowerBullet(
    const CVector2D& pos,
    const CVector2D& dir,
    float dist,
    int damage,
    float speed,
    float attackRange)
    : Base(eType_RoketBullet)
    , m_flying_dist(dist)
    , m_move_dist(0.0f)
    , m_move_speed(speed)
    , m_damage(damage)
    , m_is_flameeffects(false)
    , m_attack_range(attackRange)
{
    m_img = COPY_RESOURCE("Flameeffects", CImage);

    m_pos = pos;
    m_vec = dir;
    m_rad = m_attack_range;
    m_rect = CRect(-20, -20, 20, 20);

    m_img.SetSize(100, 100);
    m_img.SetCenter(50, 50);
    m_img.SetAng(atan2f(-m_vec.y, m_vec.x));

    m_img.ChangeAnimation(0, true);
}

void FlamethrowerBullet::Update()
{
    // レベルに応じた弾速で移動
    m_pos += m_vec * m_move_speed;

    // 移動距離を記録
    m_move_dist += m_move_speed;

    // 飛距離に達したら消す
    if (m_move_dist >= m_flying_dist) {
        SetKill();
    }

    m_img.UpdateAnimation();
}

void FlamethrowerBullet::Draw()
{
    m_img.SetPos(m_pos);
    m_img.SetAng(atan2(-m_vec.x, -m_vec.y));
    m_img.Draw();

    // 攻撃範囲の確認用
    //Utility::DrawCircle(
       // m_pos,
       // m_attack_range,
       // CVector4D(0, 5, 0, 0.5)
   // );
}

void FlamethrowerBullet::Collision(Base* b)
{
    if (b == nullptr || b->m_type != eType_Enemy) {
        return;
    }

    if (Base::CollisionCircle(this, b)) {
        if (Enemy* e = dynamic_cast<Enemy*>(b)) {
            e->TakeDamage(m_damage);
            SetKill();
        }
    }
}

void FlamethrowerBullet::Flameeffects()
{
    // 現在のコードでは未使用
}
