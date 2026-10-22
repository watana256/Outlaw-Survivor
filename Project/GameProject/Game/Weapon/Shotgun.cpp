
#include "Shotgun.h"
#include "../Player.h"
#include "../BulletAll/Bullet.h"
#include "Game/Game.h"

#define SPREAD_ANGLE 45.0f

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

// レベルを設定（Lv.1～Lv.5）
void Shotgun::SetLevel(int level)
{
    if (level < 1) level = 1;
    if (level > 5) level = 5;

    m_level = level;
}

// 現在のレベルを取得
int Shotgun::GetLevel() const
{
    return m_level;
}

// レベルアップ
void Shotgun::LevelUp()
{
    if (m_level < 5) {
        m_level++;
    }
}

// ダメージ
int Shotgun::GetDamage() const
{
    switch (m_level) {
    case 1: return 10;
    case 2: return 15;
    case 3: return 20;
    case 4: return 25;
    case 5: return 30;
    }
    return 1;
}

// 飛距離
float Shotgun::GetFlyingDist() const
{
    switch (m_level) {
    case 1: return 200.0f;
    case 2: return 250.0f;
    case 3: return 300.0f;
    case 4: return 350.0f;
    case 5: return 400.0f;
    }
    return 200.0f;
}

// 弾速
float Shotgun::GetBulletSpeed() const
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

// 1回に発射する弾数
int Shotgun::GetBulletCount() const
{
    switch (m_level) {
    case 1: return 5;
    case 2: return 7;
    case 3: return 9;
    case 4: return 11;
    case 5: return 13;
    }
    return 5;
}

// 発射間隔（秒）
float Shotgun::GetAttackInterval() const
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
