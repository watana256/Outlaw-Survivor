
#include "Pistol.h"
#include "../Player.h"
#include "../BulletAll/Bullet.h"
#include "../GLLibrary/GLLibrary/CInput.h"
#include <cmath>
#include"Game/Game.h"

Pistol::Pistol()
    : Base(eType_Pistil)
    , m_attak_cnt(0.0f)
    , m_is_enable(false)
    , m_level(1)
{
}

const WeaponData& Pistol::GetData()
{
    static const WeaponData data = {
         "ピストル",     "扱いやすい基本の銃",     "連射速度は普通",       10,  100
    };
    return data;
}

void Pistol::SetEnable(bool enable)
{
    m_is_enable = enable;
}

// レベルを設定する
void Pistol::SetLevel(int level)
{
    if (level < 1) level = 1;
    if (level > 5) level = 5;

    m_level = level;
}

// 現在のレベルを取得する
int Pistol::GetLevel() const
{
    return m_level;
}

// ダメージ
int Pistol::GetDamage() const
{
    switch (m_level) {
    case 1: return 10;
    case 2: return 2;
    case 3: return 3;
    case 4: return 4;
    case 5: return 5;
    default: return 1;
    }
}

// 弾の飛距離
float Pistol::GetFlyingDist() const
{
    switch (m_level) {
    case 1: return 500.0f;
    case 2: return 600.0f;
    case 3: return 700.0f;
    case 4: return 800.0f;
    case 5: return 1000.0f;
    default: return 500.0f;
    }
}

// 攻撃間隔（秒）
float Pistol::GetAttackInterval() const
{
    switch (m_level) {
    case 1: return 0.50f;
    case 2: return 0.45f;
    case 3: return 0.40f;
    case 4: return 0.35f;
    case 5: return 0.30f;
    default: return 0.50f;
    }
}

// 弾の移動速度
float Pistol::GetBulletSpeed() const
{
    switch (m_level) {
    case 1: return 7.0f;
    case 2: return 8.0f;
    case 3: return 9.0f;
    case 4: return 10.0f;
    case 5: return 12.0f;
    default: return 7.0f;
    }
}

void Pistol::Update()
{
    if (Game::GetGamePhase() != EGamePhase::RoundPlaying)return;
    if (!m_is_enable) return;

    m_attak_cnt += CFPS::GetDeltaTime();

    // レベルに応じた攻撃間隔
    if (m_attak_cnt < GetAttackInterval()) return;

    Player* player = Player::Instance();
    if (player == nullptr) return;

    CVector2D pos = player->m_pos;
    CVector2D mouse = CInput::GetMousePoint();

    float dx = mouse.x - pos.x;
    float dy = mouse.y - pos.y;

    float length = sqrtf(dx * dx + dy * dy);

    if (length <= 0.001f) return;

    CVector2D dir(dx / length, dy / length);

    // 弾速・飛距離・ダメージをレベルに応じて設定
    new Bullet(
        pos,
        dir,
        GetFlyingDist(),
        GetDamage(),
        15.0f,      // 当たり判定の半径を大きくする
        GetBulletSpeed()
    );

    m_attak_cnt = 0.0f;
}

void Pistol::Draw()
{
}

void Pistol::Collision(Base* b)
{
}

int Pistol::GetDamageAtLevel(int level)
{
    switch (level)
    {
    case 1: return 10;
    case 2: return 20;
    case 3: return 30;
    case 4: return 40;
    case 5: return 50;
    }
    return 10;
}