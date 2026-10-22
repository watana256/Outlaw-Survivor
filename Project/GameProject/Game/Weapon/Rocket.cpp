
#include "Rocket.h"
#include "../Player.h"
#include "../BulletAll/RocketBullet.h"
#include "Game/Game.h"

#define ROCKET_ATTACK_TIME 0.5f

Rocket::Rocket()
    : Base(eType_Rocket)
    , m_attak_cnt(0.0f)
    , m_is_enable(false)
    , m_level(1)
{
}

const WeaponData& Rocket::GetData()
{
    static const WeaponData data = {
        "ロケット",     "爆発する弾を発射", "範囲ダメージが大きい", 60, 2000
    };
    return data;
}

void Rocket::SetEnable(bool enable)
{
    m_is_enable = enable;
}

void Rocket::SetLevel(int level)
{
    if (level < 1) level = 1;
    if (level > 5) level = 5;

    m_level = level;
}

int Rocket::GetLevel() const
{
    return m_level;
}

void Rocket::LevelUp()
{
    if (m_level < 5)
    {
        m_level++;
    }
}

int Rocket::GetDamage() const
{
    switch (m_level)
    {
    case 1: return 10;
    case 2: return 20;
    case 3: return 30;
    case 4: return 40;
    case 5: return 50;
    }

    return 10;
}

float Rocket::GetFlyingDist() const
{
    switch (m_level)
    {
    case 1: return 200.0f;
    case 2: return 250.0f;
    case 3: return 300.0f;
    case 4: return 350.0f;
    case 5: return 400.0f;
    }

    return 200.0f;
}

void Rocket::Update()
{
    if (Game::GetGamePhase() != EGamePhase::RoundPlaying)return;
    if (!m_is_enable)
        return;

    Player* player = Player::Instance();

    if (player == nullptr)
        return;

    m_attak_cnt += CFPS::GetDeltaTime();

    if (m_attak_cnt >= ROCKET_ATTACK_TIME)
    {
        m_attak_cnt = 0.0f;

        CVector2D pos = player->m_pos;
        CVector2D dir = player->m_vec;

        // プレイヤーが停止している場合は右向きにする
        float length = sqrtf(dir.x * dir.x + dir.y * dir.y);

        if (length > 0.001f)
        {
            dir.x /= length;
            dir.y /= length;
        }
        else
        {
            dir.x = 1.0f;
            dir.y = 0.0f;
        }

        // レベルに応じた飛距離とダメージで発射
        new RocketBullet(
            pos,
            dir,
            GetFlyingDist(),
            GetDamage()
        );
    }
}

void Rocket::Draw()
{
}

void Rocket::Collision(Base* b)
{
}

