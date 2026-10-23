#include "Rocket.h"
#include "../Player.h"
#include "../BulletAll/RocketBullet.h"
#include "Game/Game.h"

#define ROCKET_ATTACK_TIME 0.5f

// ---------------------------------------------------------
// レベルごとの性能表(Lv1〜Lv10)。値を変えたいときはここだけ直す
// ---------------------------------------------------------
static const int   ROCKET_DAMAGE[MAX_WEAPON_LEVEL] = { 100, 120, 130, 140, 150, 160, 170, 180, 190, 200 };//攻撃力
static const float ROCKET_DIST[MAX_WEAPON_LEVEL] = { 200, 250, 300, 350, 400, 450, 500, 550, 600, 650 };//飛距離

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
        "ロケット",     "爆発する弾を発射", "範囲ダメージが大きい", 100, 2000
    };
    return data;
}

void Rocket::SetEnable(bool enable)
{
    m_is_enable = enable;
}

void Rocket::SetLevel(int level)
{
    m_level = ClampWeaponLevel(level);
}

int Rocket::GetLevel() const
{
    return m_level;
}

void Rocket::LevelUp()
{
    if (m_level < MAX_WEAPON_LEVEL)
    {
        m_level++;
    }
}

// ---------------------------------------------------------
// 表示用(Equipmentの詳細欄が使う)
// ---------------------------------------------------------
int Rocket::GetDamageAtLevel(int level)
{
    return ROCKET_DAMAGE[ClampWeaponLevel(level) - 1];
}

float Rocket::GetFlyingDistAtLevel(int level)
{
    return ROCKET_DIST[ClampWeaponLevel(level) - 1];
}

// ---------------------------------------------------------
// 撃つときに使う値(表と同じものを返す)
// ---------------------------------------------------------
int Rocket::GetDamage() const
{
    return GetDamageAtLevel(m_level);
}

float Rocket::GetFlyingDist() const
{
    return GetFlyingDistAtLevel(m_level);
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