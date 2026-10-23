#include "LandmineBullet.h"
#include "../Enemy.h"

#define  BULLET_SPEED 7.0f
#define ANIM_FRAME 2
#define EXP_RAD 64.0f
#define EXP_IMG_SIZE 128.0f
#define LANDMINE_TRIGGER_RAD 40.0f  // 地雷の起爆判定
#define EXP_RAD 64.0f               // 爆発の攻撃範囲

static TexAnim _explosion_anim[] =
{
    {0,ANIM_FRAME},{1,ANIM_FRAME},{2,ANIM_FRAME},{3,ANIM_FRAME},
    {4,ANIM_FRAME},{5,ANIM_FRAME},{6,ANIM_FRAME},{7,ANIM_FRAME},
    {8,ANIM_FRAME},{9,ANIM_FRAME},{10,ANIM_FRAME},{11,ANIM_FRAME},

};

TexAnimData LandmineBullet::_anim_data[] =
{
    ANIMDATA(_explosion_anim),
};

LandmineBullet::LandmineBullet(const CVector2D& pos, const CVector2D& dir, float dist, int damage)
    :Base(eType_RoketBullet)
    , m_flying_dist(dist)
    , m_move_dist(0.0f)
    , m_damage(damage)
    , m_is_explosion(false)
{
    m_img = COPY_RESOURCE("LandmineBullet", CImage);
    m_explosion_img = COPY_RESOURCE("Explosion", CImage);

    m_pos = pos;
    m_vec = dir;
    m_rad = 10;
    m_rect = CRect(-20, -20, 20, 20);

    m_img.SetSize(30, 30);
    m_img.SetCenter(15, 15);
    m_img.SetAng(atan2f(-m_vec.y, m_vec.x));

    m_explosion_img.SetSize(EXP_IMG_SIZE, EXP_IMG_SIZE);
    m_explosion_img.SetCenter(EXP_IMG_SIZE * 0.5f, EXP_IMG_SIZE * 0.5f);
}


void LandmineBullet::Update()
{
    if (!m_is_explosion)
    {
        // 設置中の地雷は移動しない。
        // Enemyとの衝突はCollision()で判定する。
    }
    else
    {
        // 爆発アニメーションを再生
        m_explosion_img.UpdateAnimation();

        // アニメーション終了後に削除
        if (m_explosion_img.CheckAnimationEnd())
        {
            SetKill();
        }
    }
}

void LandmineBullet::Draw()
{
    if (!m_is_explosion) {
        m_img.SetPos(m_pos);
        m_img.Draw();
        //Utility::DrawCircle(m_pos, m_rad, CVector4D(0, 5, 0, 0.5));
    }
    else
    {
        m_explosion_img.SetPos(m_pos);
        m_explosion_img.Draw();
        //Utility::DrawCircle(m_pos, m_rad, CVector4D(1, 0, 0, 0.5));
    }
}


void LandmineBullet::Collision(Base* b)
{
    if (b->m_type != eType_Enemy)
        return;

    Enemy* e = dynamic_cast<Enemy*>(b);
    if (e == nullptr)
        return;

    // Enemyが地雷に触れたら爆発
    if (!m_is_explosion)
    {
        // Enemyが地雷に近づいたら爆発
        float dx = b->m_pos.x - m_pos.x;
        float dy = b->m_pos.y - m_pos.y;

        float distance = sqrtf(dx * dx + dy * dy);

        if (distance <= LANDMINE_TRIGGER_RAD + b->m_rad)
        {
            Explosion();
        }
        return;
    }

    // 爆発範囲内の敵にダメージを与える
    // 同じ敵に何度もダメージを与えない
    auto it = std::find(
        m_hit_enemies.begin(),
        m_hit_enemies.end(),
        e
    );

    if (it != m_hit_enemies.end())
        return;

    if (Base::CollisionCircle(this, b))
    {
        e->TakeDamage(m_damage);
        m_hit_enemies.push_back(e);
    }
}


void LandmineBullet::Explosion()
{
    if (m_is_explosion) return;
    m_is_explosion = true;
    m_explosion_img.ChangeAnimation(0, false);
    m_rad = EXP_RAD;
}