#include "Shotgun.h"
#include "../Player.h"
#include "../BulletAll/Bullet.h"
#include "Game/Game.h"

#define SPREAD_ANGLE 45.0f

// ---------------------------------------------------------
// レベルごとの性能表(Lv1〜Lv10)。値を変えたいときはここだけ直す
// ---------------------------------------------------------
static const int   SHOTGUN_DAMAGE[MAX_WEAPON_LEVEL] = { 10, 15, 20, 25, 30, 35, 40, 45, 50, 55 };
static const float SHOTGUN_DIST[MAX_WEAPON_LEVEL] = { 200, 250, 300, 350, 400, 450, 500, 550, 600, 650 };
static const float SHOTGUN_SPEED[MAX_WEAPON_LEVEL] = { 7, 8, 9, 10, 12, 13, 14, 15, 16, 17 };
static const int   SHOTGUN_COUNT[MAX_WEAPON_LEVEL] = { 5, 7, 9, 11, 13, 15, 17, 19, 21, 23 };
static const float SHOTGUN_INTERVAL[MAX_WEAPON_LEVEL] = { 0.50f, 0.45f, 0.40f, 0.35f, 0.30f,
                                                          0.27f, 0.24f, 0.21f, 0.18f, 0.15f };

Shotgun::Shotgun()
    : Base(eType_Shotgun)
    , m_attak_cnt(0.0f)
    , m_is_enable(false)
    , m_level(1)
{
}

const WeaponData& Shotgun::GetData()
{
    static const WeaponData data = {
        "ショットガン", "近距離で高威力",         "散弾で複数に当たる",   25,  100
    };
    return data;
}

// 使用状態の切り替え
void Shotgun::SetEnable(bool enable)
{
    m_is_enable = enable;
}

// レベルを設定（Lv.1〜Lv.10）
void Shotgun::SetLevel(int level)
{
    m_level = ClampWeaponLevel(level);
}

// 現在のレベルを取得
int Shotgun::GetLevel() const
{
    return m_level;
}

// レベルアップ
void Shotgun::LevelUp()
{
    if (m_level < MAX_WEAPON_LEVEL) {
        m_level++;
    }
}

// ---------------------------------------------------------
// 表示用(Equipmentの詳細欄が使う)
// ---------------------------------------------------------
int Shotgun::GetDamageAtLevel(int level)
{
    return SHOTGUN_DAMAGE[ClampWeaponLevel(level) - 1];
}

float Shotgun::GetFlyingDistAtLevel(int level)
{
    return SHOTGUN_DIST[ClampWeaponLevel(level) - 1];
}

float Shotgun::GetBulletSpeedAtLevel(int level)
{
    return SHOTGUN_SPEED[ClampWeaponLevel(level) - 1];
}

// ---------------------------------------------------------
// 撃つときに使う値(表と同じものを返す)
// ---------------------------------------------------------
int Shotgun::GetDamage() const
{
    return GetDamageAtLevel(m_level);
}

float Shotgun::GetFlyingDist() const
{
    return GetFlyingDistAtLevel(m_level);
}

float Shotgun::GetBulletSpeed() const
{
    return GetBulletSpeedAtLevel(m_level);
}

// 1回に発射する弾数
int Shotgun::GetBulletCount() const
{
    return SHOTGUN_COUNT[ClampWeaponLevel(m_level) - 1];
}

// 発射間隔（秒）
float Shotgun::GetAttackInterval() const
{
    return SHOTGUN_INTERVAL[ClampWeaponLevel(m_level) - 1];
}

// 弾を発射する
void Shotgun::Shot(const CVector2D& pos, const CVector2D& dir)
{
    int bulletCount = GetBulletCount();

    for (int i = 0; i < bulletCount; i++) {

        // 弾を扇状に配置する
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

        // 弾を生成
        new Bullet(
            pos,
            CVector2D(shot_dir.x, shot_dir.y),
            GetFlyingDist(),
            GetDamage(),
            15.0f,
            GetBulletSpeed()
        );
    }
}

// 毎フレーム処理
void Shotgun::Update()
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

            Shot(pos, dir);
        }
    }
}

void Shotgun::Draw()
{
}

void Shotgun::Collision(Base* b)
{
}