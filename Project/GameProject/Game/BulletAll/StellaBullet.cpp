
#include "StellaBullet.h"
#include "../Player.h"
#include "../Enemy.h"

StellaBullet::StellaBullet(
    float radius,
    float attackTime,
    int damage)
    : Base(eType_StellaBullet)
    , m_attack_time(attackTime)
    , m_attack_cnt(0.0f)
    , m_attack_radius(radius)
    , m_damage(damage)
{
    m_img.Load("Image/StellaBullet.png");

    m_rad = m_attack_radius;
    m_img.SetSize(
        m_attack_radius * 2.0f,
        m_attack_radius * 2.0f
    );
    m_img.SetCenter(
        m_attack_radius,
        m_attack_radius
    );
}

void StellaBullet::Update()
{
    Player* player = Player::Instance();

    if (player == nullptr)
        return;

    // ƒvƒŒƒCƒ„[‚ÌˆÊ’u‚É’Ç]
    m_pos = player->m_pos;

    // UŒ‚ŠÔŠu‚ðŠÇ—
    m_attack_cnt += CFPS::GetDeltaTime();

    if (m_attack_cnt >= m_attack_time)
    {
        m_attack_cnt -= m_attack_time;
        m_hit_enemies.clear();
    }
}

void StellaBullet::Draw()
{
    m_img.SetPos(m_pos - m_scroll);
    m_img.Draw();
}

void StellaBullet::Collision(Base* b)
{
    if (b->m_type != eType_Enemy)
        return;

    Enemy* enemy = dynamic_cast<Enemy*>(b);

    if (enemy == nullptr)
        return;

    if (m_attack_cnt < m_attack_time)
        return;

    // UŒ‚ŠÔŠu“à‚Ìd•¡UŒ‚‚ð–hŽ~
    for (Enemy* hitEnemy : m_hit_enemies)
    {
        if (hitEnemy == enemy)
            return;
    }

    float dx = enemy->m_pos.x - m_pos.x;
    float dy = enemy->m_pos.y - m_pos.y;
    float distance = sqrtf(dx * dx + dy * dy);

    if (distance <= m_attack_radius + enemy->m_rad)
    {
        enemy->TakeDamage(m_damage);
        m_hit_enemies.push_back(enemy);
    }
}

