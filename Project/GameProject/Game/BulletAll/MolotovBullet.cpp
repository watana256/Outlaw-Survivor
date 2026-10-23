

#include "MolotovBullet.h"
#include "../Enemy.h"

#define BULLET_SPEED 7.0f

// 炎の設定
#define FLAME_DAMAGE_INTERVAL 0.5f
#define EXP_RAD 64.0f
#define EXP_IMG_SIZE 256.0f

// アニメーション設定
#define ANIM_FRAME 7

static TexAnim _molotov_anim[] =
{
    {0, ANIM_FRAME},
    {1, ANIM_FRAME},
    {2, ANIM_FRAME},
    {3, ANIM_FRAME},
    {4, ANIM_FRAME}
};

static TexAnim _flameeteffect_anim[] =
{
    {0, ANIM_FRAME},
    {1, ANIM_FRAME},
    {2, ANIM_FRAME},
    {3, ANIM_FRAME},
    {4, ANIM_FRAME},
    {5, ANIM_FRAME},
    {6, ANIM_FRAME},
    {7, ANIM_FRAME},
    {8, ANIM_FRAME},
    {9, ANIM_FRAME},
    {10, ANIM_FRAME},
    {11, ANIM_FRAME}
};

TexAnimData MolotovBullet::_anim_data[] =
{
    ANIMDATA(_molotov_anim),
    ANIMDATA(_flameeteffect_anim),
};

MolotovBullet::MolotovBullet(
    const CVector2D& pos,
    const CVector2D& dir,
    float dist,
    int damage,
    float flameDuration)
    : Base(eType_RoketBullet)
    , m_flying_dist(dist)
    , m_move_dist(0.0f)
    , m_damage(damage)
    , m_is_flameeteffect(false)
    , m_flame_duration(flameDuration)
    , m_flame_timer(0.0f)
    , m_damage_timer(0.0f)
{
    m_img = COPY_RESOURCE("MolotovBullet", CImage);
    m_flameeteffect_img = COPY_RESOURCE("FlameEffect", CImage);

    m_pos = pos;
    m_vec = dir;
    m_rad = 10.0f;
    m_rect = CRect(-20, -20, 20, 20);

    m_img.SetSize(40, 40);
    m_img.SetCenter(20, 20);
    m_img.SetAng(atan2f(-m_vec.y, m_vec.x));
    m_img.ChangeAnimation(0, true);

    m_flameeteffect_img.SetSize(EXP_IMG_SIZE, EXP_IMG_SIZE);
    m_flameeteffect_img.SetCenter(
        EXP_IMG_SIZE * 0.5f,
        EXP_IMG_SIZE * 0.5f
    );
}

void MolotovBullet::Update()
{
    const float dt = CFPS::GetDeltaTime();

    // まだ飛んでいる場合
    if (!m_is_flameeteffect)
    {
        m_pos += m_vec * BULLET_SPEED;
        m_move_dist += BULLET_SPEED;

        m_img.UpdateAnimation();

        // 飛距離に達したら炎を発生
        if (m_move_dist >= m_flying_dist)
        {
            FlameEffect();
        }

        return;
    }

    // 炎になった後の処理
    m_flame_timer += dt;
    m_damage_timer += dt;

    // 炎のアニメーション
    m_flameeteffect_img.UpdateAnimation();

    // 一定間隔で範囲内の敵にダメージを与える
    if (m_damage_timer >= FLAME_DAMAGE_INTERVAL)
    {
        m_damage_timer -= FLAME_DAMAGE_INTERVAL;

        auto enemies = Base::FindObjects(eType_Enemy);

        for (Base* b : enemies)
        {
            if (b == nullptr || b->m_kill)
                continue;

            if (!Base::CollisionCircle(this, b))
                continue;

            Enemy* e = dynamic_cast<Enemy*>(b);

            if (e != nullptr)
            {
                // レベルに応じたダメージ
                e->TakeDamage(m_damage);
            }
        }
    }

    // レベルに応じた持続時間が終わったら炎を消す
    if (m_flame_timer >= m_flame_duration)
    {
        SetKill();
    }
}

void MolotovBullet::Draw()
{
    if (!m_is_flameeteffect)
    {
        m_img.SetPos(m_pos);
        m_img.Draw();

        //Utility::DrawCircle(
           // m_pos,
           // m_rad,
           // CVector4D(0, 5, 0, 0.5)
       // );
    }
    else
    {
        m_flameeteffect_img.SetPos(m_pos);
        m_flameeteffect_img.Draw();
    }
}

void MolotovBullet::Collision(Base* b)
{
    if (b == nullptr || b->m_type != eType_Enemy)
        return;

    if (m_is_flameeteffect)
        return;

    // 飛んでいる火炎瓶が敵に当たったら炎に変化
    if (Base::CollisionCircle(this, b))
    {
        FlameEffect();
    }
}

void MolotovBullet::FlameEffect()
{
    if (m_is_flameeteffect)
        return;

    m_is_flameeteffect = true;

    // 炎のタイマーを初期化
    m_flame_timer = 0.0f;
    m_damage_timer = 0.0f;

    // 炎のアニメーションを開始
    m_flameeteffect_img.ChangeAnimation(0, true);

    // 炎の範囲
    m_rad = EXP_RAD;
}
