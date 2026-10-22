#include "Molotov.h"
#include "../Player.h"
#include "../BulletAll/MolotovBullet.h"
#include"Game/Game.h"

#define SPREAD_ANGLE 180.0f

// ---------------------------------------------------------
// レベルごとの性能表(Lv1〜Lv10)。値を変えたいときはここだけ直す
// ---------------------------------------------------------
static const int   MOLOTOV_DAMAGE[MAX_WEAPON_LEVEL] = { 5, 10, 15, 20, 25, 30, 35, 40, 45, 50 };
static const float MOLOTOV_DIST[MAX_WEAPON_LEVEL] = { 200, 250, 300, 350, 400, 450, 500, 550, 600, 650 };
static const int   MOLOTOV_COUNT[MAX_WEAPON_LEVEL] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
static const float MOLOTOV_FLAME[MAX_WEAPON_LEVEL] = { 3.0f, 3.5f, 4.0f, 4.5f, 5.0f,
                                                          5.5f, 6.0f, 6.5f, 7.0f, 7.5f };

Molotov::Molotov()
    : Base(eType_Molotov)
    , m_attak_cnt(0.0f)
    , m_is_enable(false)
    , m_level(1)
{
}

const WeaponData& Molotov::GetData()
{
    static const WeaponData data = {
       "火炎瓶","着弾地点を燃やす","継続ダメージを与える", 15,  600
    };
    return data;
}

void Molotov::SetEnable(bool enable)
{
    m_is_enable = enable;
}

// レベルを設定（Lv.1〜Lv.10）
void Molotov::SetLevel(int level)
{
    m_level = ClampWeaponLevel(level);
}

// 現在のレベルを取得
int Molotov::GetLevel() const
{
    return m_level;
}

// レベルアップ
void Molotov::LevelUp()
{
    if (m_level < MAX_WEAPON_LEVEL) {
        m_level++;
    }
}

// ---------------------------------------------------------
// 表示用(Equipmentの詳細欄が使う)
// ---------------------------------------------------------
int Molotov::GetDamageAtLevel(int level)
{
    return MOLOTOV_DAMAGE[ClampWeaponLevel(level) - 1];
}

float Molotov::GetFlyingDistAtLevel(int level)
{
    return MOLOTOV_DIST[ClampWeaponLevel(level) - 1];
}

// ---------------------------------------------------------
// 撃つときに使う値(表と同じものを返す)
// ---------------------------------------------------------
int Molotov::GetDamage() const
{
    return GetDamageAtLevel(m_level);
}

float Molotov::GetFlyingDist() const
{
    return GetFlyingDistAtLevel(m_level);
}

// 1回に投げる火炎瓶の本数
int Molotov::GetBulletCount() const
{
    return MOLOTOV_COUNT[ClampWeaponLevel(m_level) - 1];
}

// 着弾後の炎の持続時間（秒）
float Molotov::GetFlameDuration() const
{
    return MOLOTOV_FLAME[ClampWeaponLevel(m_level) - 1];
}

// 火炎瓶を投げる
void Molotov::Shot(const CVector2D& pos, const CVector2D& dir)
{
    int bulletCount = GetBulletCount();

    for (int i = 0; i < bulletCount; i++) {

        float per = (bulletCount > 1)
            ? (float)i / (bulletCount - 1)
            : 0.5f;

        float ang = Utility::Lerp(
            -SPREAD_ANGLE,
            SPREAD_ANGLE,
            per
        );

        CVector3D shot_dir =
            CQuaternion::Euler(0.0f, 0.0f, DtoR(ang)) * dir;

        // 飛距離・ダメージ・炎の持続時間を渡す
        new MolotovBullet(
            pos,
            CVector2D(shot_dir.x, shot_dir.y),
            GetFlyingDist(),
            GetDamage(),
            GetFlameDuration()
        );
    }
}

void Molotov::Update()
{
    if (Game::GetGamePhase() != EGamePhase::RoundPlaying)return;
    if (!m_is_enable) {
        return;
    }

    m_attak_cnt += CFPS::GetDeltaTime();

    // 発射間隔は現状0.5秒
    if (m_attak_cnt >= 0.5f) {
        m_attak_cnt = 0.0f;

        Player* player = Player::Instance();

        if (player != nullptr) {
            CVector2D pos = player->m_pos;
            CVector2D dir = player->m_vec;

            Shot(pos, dir);
        }
    }
}

void Molotov::Draw()
{
}

void Molotov::Collision(Base* b)
{
}