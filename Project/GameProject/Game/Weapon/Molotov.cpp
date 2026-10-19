
#include "Molotov.h"
#include "../Player.h"
#include "../BulletAll/MolotovBullet.h"

#define SPREAD_ANGLE 180.0f

Molotov::Molotov()
    : Base(eType_Shotgun)
    , m_attak_cnt(0.0f)
    , m_is_enable(false)
    , m_level(1)
{
}

void Molotov::SetEnable(bool enable)
{
    m_is_enable = enable;
}

// レベルを設定（Lv.1～Lv.5）
void Molotov::SetLevel(int level)
{
    if (level < 1) level = 1;
    if (level > 5) level = 5;

    m_level = level;
}

// 現在のレベルを取得
int Molotov::GetLevel() const
{
    return m_level;
}

// レベルアップ
void Molotov::LevelUp()
{
    if (m_level < 5) {
        m_level++;
    }
}

// ダメージ
int Molotov::GetDamage() const
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
float Molotov::GetFlyingDist() const
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

// 1回に投げる火炎瓶の本数
int Molotov::GetBulletCount() const
{
    switch (m_level) {
    case 1: return 2;
    case 2: return 3;
    case 3: return 4;
    case 4: return 5;
    case 5: return 6;
    }
    return 2;
}

// 着弾後の炎の持続時間（秒）
float Molotov::GetFlameDuration() const
{
    switch (m_level) {
    case 1: return 3.0f;
    case 2: return 3.5f;
    case 3: return 4.0f;
    case 4: return 4.5f;
    case 5: return 5.0f;
    }
    return 3.0f;
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
