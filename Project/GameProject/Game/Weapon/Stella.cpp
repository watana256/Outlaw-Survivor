

#include "Stella.h"
#include "../Player.h"
#include "../BulletAll/StellaBullet.h"
#include "Game/Game.h"

#define STELLA_RADIUS 100.0f
#define STELLA_ATTACK_TIME 0.5f

Stella::Stella()
    : Base(eType_Stella)
    , m_attak_cnt(0.0f)
    , m_is_enable(false)
    , m_stella_bullet(nullptr)
    , m_level(1)
{
}

const WeaponData& Stella::GetData()
{
    static const WeaponData data = {
        "ステラ",       "特殊な弾を発射する",     "高い性能を持つ",       40, 1500
    };
    return data;
}

void Stella::SetEnable(bool enable)
{
    if (m_is_enable == enable)
        return;

    m_is_enable = enable;

    if (m_is_enable)
    {
        m_stella_bullet = new StellaBullet(
            GetAttackRange(),
            GetAttackInterval(),
            GetDamage()
        );
    }
    else
    {
        if (m_stella_bullet != nullptr)
        {
            m_stella_bullet->SetKill();
            m_stella_bullet = nullptr;
        }
    }
}

void Stella::SetLevel(int level)
{
    if (level < 1)
        level = 1;

    if (level > 5)
        level = 5;

    m_level = level;

    // 生成済みの攻撃範囲にも性能を反映
    UpdateBulletStats();
}

int Stella::GetLevel() const
{
    return m_level;
}

void Stella::LevelUp()
{
    if (m_level < 5)
    {
        m_level++;
        UpdateBulletStats();
    }
}

int Stella::GetDamage() const
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

float Stella::GetAttackRange() const
{
    switch (m_level)
    {
    case 1: return 100.0f;
    case 2: return 125.0f;
    case 3: return 150.0f;
    case 4: return 175.0f;
    case 5: return 200.0f;
    }

    return STELLA_RADIUS;
}
//レベルごとに画像サイズ
float Stella::GetImageSize() const
{
    switch (m_level)
    {
    case 1: return 180.0f;
    case 2: return 225.0f;
    case 3: return 270.0f;
    case 4: return 315.0f;
    case 5: return 360.0f;
    }

    return 180.0f;
}
float Stella::GetAttackInterval() const
{
    return STELLA_ATTACK_TIME;
}

void Stella::UpdateBulletStats()
{
    if (m_stella_bullet != nullptr)
    {
        m_stella_bullet->SetStats(
            GetAttackRange(),
            GetAttackInterval(),
            GetDamage()
        );

        m_stella_bullet->SetImageSize(GetImageSize());
    }
}

void Stella::Update()
{
    if (Game::GetGamePhase() != EGamePhase::RoundPlaying)return;
    // 攻撃範囲とダメージ処理は StellaBullet 側で行う
}

void Stella::Draw()
{
    // 攻撃範囲の描画は StellaBullet 側で行う
}

void Stella::Collision(Base* b)
{
    // Stella本体では衝突判定を行わない
}
