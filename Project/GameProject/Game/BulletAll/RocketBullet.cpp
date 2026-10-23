#include "RocketBullet.h"
#include "../Enemy.h"

#include <cmath>
#include <cfloat>
#include <vector>

#define  BULLET_SPEED 7.0f
#define ANIM_FRAME 2
#define EXP_RAD 64.0f
#define EXP_IMG_SIZE 128.0f
#define ROCKET_IMG_SIZE 50.0f

static TexAnim _explosion_anim[] =
{
    {0,ANIM_FRAME},{1,ANIM_FRAME},{2,ANIM_FRAME},{3,ANIM_FRAME},
    {4,ANIM_FRAME},{5,ANIM_FRAME},{6,ANIM_FRAME},{7,ANIM_FRAME},
    {8,ANIM_FRAME},{9,ANIM_FRAME},{10,ANIM_FRAME},{11,ANIM_FRAME},

};

TexAnimData RocketBullet::_anim_data[] =
{
    ANIMDATA(_explosion_anim),
};

RocketBullet::RocketBullet(const CVector2D& pos, const CVector2D& dir, float dist, int damage)
    :Base(eType_RoketBullet)
    , m_flying_dist(dist)
    , m_move_dist(0.0f)
    , m_damage(damage)
    , m_is_explosion(false)
{
    m_img = COPY_RESOURCE("RocketBullt", CImage);
    m_explosion_img = COPY_RESOURCE("Explosion", CImage);

    m_pos = pos;
    m_vec = dir;
    m_rad = 10;
    m_rect = CRect(-20, -20, 20, 20);

    m_img.SetSize(ROCKET_IMG_SIZE, ROCKET_IMG_SIZE);
    m_img.SetCenter(ROCKET_IMG_SIZE * 0.5f, ROCKET_IMG_SIZE * 0.5f);
    m_img.SetAng(atan2f(-m_vec.y, m_vec.x));

    m_explosion_img.SetSize(EXP_IMG_SIZE, EXP_IMG_SIZE);
    m_explosion_img.SetCenter(EXP_IMG_SIZE * 0.5f, EXP_IMG_SIZE * 0.5f);
}

void RocketBullet::Update()
{
    if (!m_is_explosion)
    {
        const float move_speed = BULLET_SPEED;

        // ìGàÍóóÇéÊìæ
        std::vector<Base*> enemies =
            Base::FindObjects(eType_Enemy);

        Enemy* target = nullptr;

        // ç≈Ç‡ãﬂÇ¢ìGÇíTÇ∑
        float nearest_dist_sq = FLT_MAX;

        for (Base* b : enemies)
        {
            Enemy* enemy = dynamic_cast<Enemy*>(b);

            if (enemy == nullptr || enemy->m_kill)
                continue;

            float dx = enemy->m_pos.x - m_pos.x;
            float dy = enemy->m_pos.y - m_pos.y;

            float dist_sq = dx * dx + dy * dy;

            if (dist_sq < nearest_dist_sq)
            {
                nearest_dist_sq = dist_sq;
                target = enemy;
            }
        }

        // ìGÇ™å©Ç¬Ç©Ç¡ÇΩÇÁÅAÇªÇÃï˚å¸Ç÷å¸Ç´ÇïœÇ¶ÇÈ
        if (target != nullptr)
        {
            float dx = target->m_pos.x - m_pos.x;
            float dy = target->m_pos.y - m_pos.y;

            float length = sqrtf(dx * dx + dy * dy);

            if (length > 0.001f)
            {
                m_vec.x = dx / length;
                m_vec.y = dy / length;
            }
        }

        // ÉçÉPÉbÉgÇÃå¸Ç´ÇâÊëúÇ…Ç‡îΩâf
        m_img.SetAng(atan2f(-m_vec.y, m_vec.x));

        // åªç›ÇÃï˚å¸Ç÷à⁄ìÆ
        m_pos += m_vec * move_speed;

        m_move_dist += move_speed;

        if (m_move_dist >= m_flying_dist)
        {
            Explosion();
        }
    }
    else
    {
        m_explosion_img.UpdateAnimation();

        if (m_explosion_img.CheckAnimationEnd())
        {
            SetKill();
        }
    }
}

void RocketBullet::Draw()
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
       // Utility::DrawCircle(m_pos, m_rad, CVector4D(1, 0, 0, 0.5));
    }
}

void RocketBullet::Collision(Base* b)
{
    if (b->m_type == eType_Enemy) {
        Enemy* e = dynamic_cast<Enemy*>(b);
        if (e == nullptr) return;

        if (!m_is_explosion) {
            // ìñÇΩÇËîªíËÅiéläpå`îªíËÅj
            if (Base::CollisionCircle(this, b)) {
                Explosion();
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

void RocketBullet::Explosion()
{
    if (m_is_explosion) return;
    m_is_explosion = true;
    m_explosion_img.ChangeAnimation(0, false);
    m_rad = EXP_RAD;
}
