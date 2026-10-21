
#include "Landmine.h"
#include "../Player.h"
#include "../BulletAll/LandmineBullet.h"
#include "Game/Game.h"

#define FLYING_DIST 100.0f

Landmine::Landmine()
    : Base(eType_Landmine)
    , m_attak_cnt(0.0f)
    , m_is_enable(false)
    , m_level(1)
{
}

void Landmine::SetEnable(bool enable)
{
    m_is_enable = enable;
}

// レベルを設定（Lv.1～Lv.5）
void Landmine::SetLevel(int level)
{
    if (level < 1) level = 1;
    if (level > 5) level = 5;

    m_level = level;
}

// 現在のレベルを取得
int Landmine::GetLevel() const
{
    return m_level;
}

// レベルアップ
void Landmine::LevelUp()
{
    if (m_level < 5) {
        m_level++;
    }
}

// ダメージ
int Landmine::GetDamage() const
{
    switch (m_level) {
    case 1: return 20;
    case 2: return 30;
    case 3: return 40;
    case 4: return 50;
    case 5: return 60;
    }

    return 10;
}

// 設置間隔（秒）
float Landmine::GetAttackInterval() const
{
    switch (m_level) {
    case 1: return 3.0f;
    case 2: return 2.5f;
    case 3: return 2.0f;
    case 4: return 1.5f;
    case 5: return 1.0f;
    }

    return 3.0f;
}

void Landmine::Update()
{
    if (Game::GetGamePhase() != EGamePhase::RoundPlaying)return;
    if (!m_is_enable) {
        return;
    }

    // 経過時間を加算
    m_attak_cnt += CFPS::GetDeltaTime();

    // レベルに応じた時間が経過したら地雷を設置
    if (m_attak_cnt >= GetAttackInterval()) {
        m_attak_cnt = 0.0f;

        Player* player = Player::Instance();

        if (player != nullptr) {
            CVector2D pos = player->m_pos;
            CVector2D dir = player->m_vec;

            new LandmineBullet(
                pos,
                dir,
                FLYING_DIST,
                GetDamage()
            );
        }
    }
}

void Landmine::Draw()
{
}

void Landmine::Collision(Base* b)
{
}
