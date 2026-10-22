#include "Flamethrower.h"
#include "../Player.h"
#include "../BulletAll/FlamethrowerBullet.h"
#include "Game/Game.h"

// ---------------------------------------------------------
// レベルごとの性能表(Lv1?Lv10)。値を変えたいときはここだけ直す
// ---------------------------------------------------------
static const int   FLAME_DAMAGE[MAX_WEAPON_LEVEL] = { 5, 10, 15, 20, 25, 30, 35, 40, 45, 50 };
static const float FLAME_DIST[MAX_WEAPON_LEVEL] = { 100, 125, 150, 175, 200, 225, 250, 275, 300, 325 };
static const float FLAME_SPEED[MAX_WEAPON_LEVEL] = { 7, 8, 9, 10, 12, 13, 14, 15, 16, 17 };
static const float FLAME_RANGE[MAX_WEAPON_LEVEL] = { 30, 35, 40, 45, 50, 55, 60, 65, 70, 75 };
static const float FLAME_INTERVAL[MAX_WEAPON_LEVEL] = { 0.50f, 0.45f, 0.40f, 0.35f, 0.30f,
                                                        0.27f, 0.24f, 0.21f, 0.18f, 0.15f };

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

// レベル設定（Lv.1?Lv.10）
void Flamethrower::SetLevel(int level)
{
    m_level = ClampWeaponLevel(level);
}

// 現在のレベルを取得
int Flamethrower::GetLevel() const
{
    return m_level;
}

// レベルアップ
void Flamethrower::LevelUp()
{
    if (m_level < MAX_WEAPON_LEVEL) {
        m_level++;
    }
}

// ---------------------------------------------------------
// 表示用(Equipmentの詳細欄が使う)
// ---------------------------------------------------------
int Flamethrower::GetDamageAtLevel(int level)
{
    return FLAME_DAMAGE[ClampWeaponLevel(level) - 1];
}

float Flamethrower::GetFlyingDistAtLevel(int level)
{
    return FLAME_DIST[ClampWeaponLevel(level) - 1];
}

float Flamethrower::GetBulletSpeedAtLevel(int level)
{
    return FLAME_SPEED[ClampWeaponLevel(level) - 1];
}

// ---------------------------------------------------------
// 撃つときに使う値(表と同じものを返す)
// ---------------------------------------------------------
int Flamethrower::GetDamage() const
{
    return GetDamageAtLevel(m_level);
}

float Flamethrower::GetFlyingDist() const
{
    return GetFlyingDistAtLevel(m_level);
}

float Flamethrower::GetBulletSpeed() const
{
    return GetBulletSpeedAtLevel(m_level);
}

// 攻撃範囲（当たり判定の半径として使用）
float Flamethrower::GetAttackRange() const
{
    return FLAME_RANGE[ClampWeaponLevel(m_level) - 1];
}

// 発射間隔（秒）
float Flamethrower::GetAttackInterval() const
{
    return FLAME_INTERVAL[ClampWeaponLevel(m_level) - 1];
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