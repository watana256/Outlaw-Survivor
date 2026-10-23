
#include "Bullet.h"
#include "../Enemy.h"
#include <cmath>

Bullet::Bullet(
    const CVector2D& pos,
    const CVector2D& dir,
    float dist,
    int damage,
    float radius,
    float speed)
    : Base(eType_Bullet)
    , m_flying_dist(dist)
    , m_move_dist(0.0f)
    , m_move_speed(speed)
    , m_damage(damage)
{
    m_img = COPY_RESOURCE("Bullet", CImage);

    m_pos = pos;
    m_vec = dir;
    m_rad = radius;

    m_rect = CRect(-10, -10, 10, 10);
    m_img.SetSize(10, 10);
    m_img.SetCenter(10, 10);
    m_img.SetAng(atan2f(-m_vec.y, m_vec.x));
}

void Bullet::Update()
{
    // ‚±‚Ì’eŽ©g‚ÉÝ’è‚³‚ê‚½‘¬“x‚ÅˆÚ“®
    m_pos += m_vec * m_move_speed;

    m_move_dist += m_move_speed;

    // Ý’è‚³‚ê‚½”ò‹——£‚É’B‚µ‚½‚çÁ–Å
    if (m_move_dist >= m_flying_dist) {
        SetKill();
    }
}

void Bullet::Draw()
{
    m_img.SetPos(m_pos);
    m_img.Draw();
}

void Bullet::Collision(Base* b)
{
    if (b->m_type == eType_Enemy) {
        if (Base::CollisionCircle(this, b)) {
            if (Enemy* e = dynamic_cast<Enemy*>(b)) {
                e->TakeDamage(m_damage);
                SetKill();
            }
        }
    }
}
