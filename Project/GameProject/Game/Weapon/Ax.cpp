#include "Ax.h"
#include "../Player.h"
#include "../BulletAll/AxBullet.h"
#include"Game/Game.h"

// ---------------------------------------------------------
// レベルごとの性能表(Lv1〜Lv10)。値を変えたいときはここだけ直す
// ---------------------------------------------------------
static const int   AX_DAMAGE[MAX_WEAPON_LEVEL] = { 5, 10, 15, 20, 25, 30, 45, 50, 55, 60 };//攻撃力
static const float AX_DIST[MAX_WEAPON_LEVEL] = { 200, 250, 300, 350, 400, 450, 500, 550, 600, 650 };//飛距離
static const float AX_SPEED[MAX_WEAPON_LEVEL] = { 7, 8, 9, 10, 12, 13, 14, 15, 16, 17 };//弾速
static const int   AX_COUNT[MAX_WEAPON_LEVEL] = { 2, 3, 5, 8, 12, 14, 16, 18, 20, 24 };//弾数
static const float AX_INTERVAL[MAX_WEAPON_LEVEL] = { 0.50f, 0.45f, 0.40f, 0.40f, 0.40f,
                                                      0.40f,  0.40f,  0.40f, 0.40f, 0.40f };//発射間隔

Ax::Ax()
    : Base(eType_Ax)
    , m_attak_cnt(0.0f)
    , m_is_enable(false)
    , m_level(1)
{
}

const WeaponData& Ax::GetData()
{
    static const WeaponData data = {
        "斧", "斧は投げる物", "攻撃範囲が広い", 1, 100
    };
    return data;
}

void Ax::SetEnable(bool enable)
{
    m_is_enable = enable;
}

// レベル設定：Lv.1〜Lv.10
void Ax::SetLevel(int level)
{
    m_level = ClampWeaponLevel(level);
}

int Ax::GetLevel() const
{
    return m_level;
}

void Ax::LevelUp()
{
    if (m_level < MAX_WEAPON_LEVEL)
    {
        m_level++;
    }
}

// ---------------------------------------------------------
// 表示用(Equipmentの詳細欄が使う)
// ---------------------------------------------------------
int Ax::GetDamageAtLevel(int level)
{
    return AX_DAMAGE[ClampWeaponLevel(level) - 1];
}

float Ax::GetFlyingDistAtLevel(int level)
{
    return AX_DIST[ClampWeaponLevel(level) - 1];
}

float Ax::GetBulletSpeedAtLevel(int level)
{
    return AX_SPEED[ClampWeaponLevel(level) - 1];
}

// ---------------------------------------------------------
// 撃つときに使う値(表と同じものを返す)
// ---------------------------------------------------------
int Ax::GetDamage() const
{
    return GetDamageAtLevel(m_level);
}

float Ax::GetFlyingDist() const
{
    return GetFlyingDistAtLevel(m_level);
}

float Ax::GetBulletSpeed() const
{
    return GetBulletSpeedAtLevel(m_level);
}

// 弾数
int Ax::GetBulletCount() const
{
    return AX_COUNT[ClampWeaponLevel(m_level) - 1];
}

// 発射間隔（秒）
float Ax::GetAttackInterval() const
{
    return AX_INTERVAL[ClampWeaponLevel(m_level) - 1];
}

// 斧を発射する
void Ax::Shot(const CVector2D& pos, const CVector2D& dir)
{
    int bulletCount = GetBulletCount();

    for (int i = 0; i < bulletCount; i++)
    {
        // 弾が1発の場合は正面へ発射
        float per = (bulletCount > 1)
            ? (float)i / (bulletCount - 1)
            : 0.5f;

        // 弾を左右に広げる
        float ang = Utility::Lerp(-180.0f, 180.0f, per);

        CVector3D shot_dir =
            CQuaternion::Euler(0.0f, 0.0f, DtoR(ang)) * dir;

        // 現在のAxBulletのコンストラクタを使用
        new AxBullet(
            pos,
            CVector2D(shot_dir.x, shot_dir.y),
            GetFlyingDist(),
            GetDamage(),
            GetBulletSpeed()
        );
    }
}

void Ax::Update()
{
    if (Game::GetGamePhase() != EGamePhase::RoundPlaying)return;
    if (!m_is_enable)
        return;

    m_attak_cnt += CFPS::GetDeltaTime();

    if (m_attak_cnt < GetAttackInterval())
        return;

    Player* player = Player::Instance();

    if (player == nullptr)
        return;

    Shot(player->m_pos, player->m_vec);

    m_attak_cnt = 0.0f;
}

void Ax::Draw()
{
}

void Ax::Collision(Base* b)
{
}