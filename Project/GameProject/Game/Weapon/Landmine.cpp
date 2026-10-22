#include "Landmine.h"
#include "../Player.h"
#include "../BulletAll/LandmineBullet.h"
#include "Game/Game.h"

#define FLYING_DIST 100.0f

// ---------------------------------------------------------
// レベルごとの性能表(Lv1〜Lv10)。値を変えたいときはここだけ直す
// ---------------------------------------------------------
static const int   MINE_DAMAGE[MAX_WEAPON_LEVEL] = { 20, 30, 40, 50, 60, 70, 80, 90, 100, 110 };
static const float MINE_INTERVAL[MAX_WEAPON_LEVEL] = { 3.0f, 2.5f, 2.0f, 1.5f, 1.0f,
                                                       0.9f, 0.8f, 0.7f, 0.6f, 0.5f };

Landmine::Landmine()
    : Base(eType_Landmine)
    , m_attak_cnt(0.0f)
    , m_is_enable(false)
    , m_level(1)
{
}

const WeaponData& Landmine::GetData()
{
    static const WeaponData data = {
         "地雷",         "設置して敵を待ち伏せ",   "踏むと爆発する",       50,  700
    };
    return data;
}

void Landmine::SetEnable(bool enable)
{
    m_is_enable = enable;
}

// レベルを設定（Lv.1〜Lv.10）
void Landmine::SetLevel(int level)
{
    m_level = ClampWeaponLevel(level);
}

// 現在のレベルを取得
int Landmine::GetLevel() const
{
    return m_level;
}

// レベルアップ
void Landmine::LevelUp()
{
    if (m_level < MAX_WEAPON_LEVEL) {
        m_level++;
    }
}

// 表示用(Equipmentの詳細欄が使う)
int Landmine::GetDamageAtLevel(int level)
{
    return MINE_DAMAGE[ClampWeaponLevel(level) - 1];
}

// ダメージ(表と同じ値)
int Landmine::GetDamage() const
{
    return GetDamageAtLevel(m_level);
}

// 設置間隔（秒）
float Landmine::GetAttackInterval() const
{
    return MINE_INTERVAL[ClampWeaponLevel(m_level) - 1];
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