
#include "Flamethrower.h"
#include "../Player.h"
#include "../BulletAll/FlamethrowerBullet.h"
#include "Game/Game.h"

Flamethrower::Flamethrower()
    : Base(eType_Flamethrower)
    , m_attak_cnt(0.0f)
    , m_is_enable(false)
    , m_level(1)
{
}

const WeaponData& Flamethrower::GetData()
{
    static const WeaponData data = {
        "火炎放射器","前方に炎を放射する","敵を貫通する",  20, 1200 
    };
    return data;
}


void Flamethrower::SetEnable(bool enable)
{
    m_is_enable = enable;
}

// レベル設定（Lv.1～Lv.5）
void Flamethrower::SetLevel(int level)
{
    if (level < 1) level = 1;
    if (level > 5) level = 5;

    m_level = level;
}

// 現在のレベルを取得
int Flamethrower::GetLevel() const
{
    return m_level;
}

// レベルアップ
void Flamethrower::LevelUp()
{
    if (m_level < 5) {
        m_level++;
    }
}

// ダメージ
int Flamethrower::GetDamage() const
{
    switch (m_level) {
    case 1: return 5;
    case 2: return 10;
    case 3: return 15;
    case 4: return 20;
    case 5: return 25;
    }
    return 5;
}

// 飛距離
float Flamethrower::GetFlyingDist() const
{
    switch (m_level) {
    case 1: return 100.0f;
    case 2: return 125.0f;
    case 3: return 150.0f;
    case 4: return 175.0f;
    case 5: return 200.0f;
    }
    return 100.0f;
}

// 弾速
float Flamethrower::GetBulletSpeed() const
{
    switch (m_level) {
    case 1: return 7.0f;
    case 2: return 8.0f;
    case 3: return 9.0f;
    case 4: return 10.0f;
    case 5: return 12.0f;
    }
    return 7.0f;
}

// 攻撃範囲（当たり判定の半径として使用）
float Flamethrower::GetAttackRange() const
{
    switch (m_level) {
    case 1: return 30.0f;
    case 2: return 35.0f;
    case 3: return 40.0f;
    case 4: return 45.0f;
    case 5: return 50.0f;
    }
    return 30.0f;
}

// 発射間隔（秒）
float Flamethrower::GetAttackInterval() const
{
    switch (m_level) {
    case 1: return 0.50f;
    case 2: return 0.45f;
    case 3: return 0.40f;
    case 4: return 0.35f;
    case 5: return 0.30f;
    }
    return 0.50f;
}

void Flamethrower::Update()
{
    if (Game::GetGamePhase() != EGamePhase::RoundPlaying)return;
    if (!m_is_enable) {
        return;
    }

    m_attak_cnt += CFPS::GetDeltaTime();

    if (m_attak_cnt >= GetAttackInterval()) {
        m_attak_cnt = 0.0f;

        Player* player = Player::Instance();

        if (player != nullptr) {
            CVector2D pos = player->m_pos;
            CVector2D dir = player->m_vec;

            new FlamethrowerBullet(
                pos,
                dir,
                GetFlyingDist(),
                GetDamage(),
                GetBulletSpeed(),
                GetAttackRange()
            );
        }
    }
}

void Flamethrower::Draw()
{
}

void Flamethrower::Collision(Base* b)
{
}

int Flamethrower::GetDamageAtLevel(int level)
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
float Flamethrower::GetFlyingDistAtLevel(int level)
{
    switch (level) {
    case 1: return 100.0f;
    case 2: return 125.0f;
    case 3: return 150.0f;
    case 4: return 175.0f;
    case 5: return 200.0f;
    }
    return 100.0f;
}

float Flamethrower::GetBulletSpeedAtLevel(int level)
{
    switch (level) {
    case 1: return 7.0f;
    case 2: return 8.0f;
    case 3: return 9.0f;
    case 4: return 10.0f;
    case 5: return 12.0f;
    }
    return 7.0f;
}