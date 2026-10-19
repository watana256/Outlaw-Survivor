#include "Rocket.h"
#include "../Player.h"
#include "../Enemy.h"
#include "../BulletAll/RocketBullet.h"

#include <cmath>
#include <vector>

#define ATTACK_TIME 1.0f
#define FLYING_DIST 500.0f

Rocket::Rocket()
    : Base(eType_Rocket)
    , m_attak_cnt(0.0f)
    , m_is_enable(false)
{
}

void Rocket::SetEnable(bool enable)
{
    m_is_enable = enable;
}

int Rocket::GetDamage() const
{
    int lv = 1;

    switch (lv)
    {
    case 1: return 10;
    case 2: return 20;
    case 3: return 30;
    case 4: return 40;
    }

    return 1;
}

void Rocket::Update()
{
    if (!m_is_enable)
        return;

    m_attak_cnt += CFPS::GetDeltaTime();

    if (m_attak_cnt < ATTACK_TIME)
        return;

    Player* player = Player::Instance();

    if (player == nullptr)
        return;

    // プレイヤーの位置
    CVector2D pos = player->m_pos;

    // Enemy一覧を取得
    std::vector<Base*> enemies =
        Base::FindObjects(eType_Enemy);

    Enemy* target = nullptr;

    // 最も近いEnemyを探す
    float nearest_dist_sq =
        FLYING_DIST * FLYING_DIST;

    for (Base* b : enemies)
    {
        Enemy* enemy = dynamic_cast<Enemy*>(b);

        if (enemy == nullptr || enemy->m_kill)
            continue;

        float dx = enemy->m_pos.x - pos.x;
        float dy = enemy->m_pos.y - pos.y;

        float dist_sq = dx * dx + dy * dy;

        if (dist_sq < nearest_dist_sq)
        {
            nearest_dist_sq = dist_sq;
            target = enemy;
        }
    }

    // 射程内にEnemyがいなければ発射しない
    if (target == nullptr)
        return;

    // Enemyに向かう方向を計算
    float dx = target->m_pos.x - pos.x;
    float dy = target->m_pos.y - pos.y;

    float length = sqrtf(dx * dx + dy * dy);

    if (length <= 0.001f)
        return;

    CVector2D dir(
        dx / length,
        dy / length
    );

    // RocketBulletを発射
    new RocketBullet(
        pos,
        dir,
        FLYING_DIST,
        GetDamage()
    );

    // 発射間隔をリセット
    m_attak_cnt = 0.0f;
}

void Rocket::Draw()
{
}

void Rocket::Collision(Base* b)
{
}