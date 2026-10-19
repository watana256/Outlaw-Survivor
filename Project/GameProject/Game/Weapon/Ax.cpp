
#include "Ax.h"
#include "../Player.h"
#include "../BulletAll/AxBullet.h"

Ax::Ax()
    : Base(eType_Shotgun)
    , m_attak_cnt(0.0f)
    , m_is_enable(false)
    , m_level(1)
{
}

void Ax::SetEnable(bool enable)
{
    m_is_enable = enable;
}

// レベル設定：Lv.1～Lv.5
void Ax::SetLevel(int level)
{
    if (level < 1) level = 1;
    if (level > 5) level = 5;

    m_level = level;
}

int Ax::GetLevel() const
{
    return m_level;
}

void Ax::LevelUp()
{
    if (m_level < 5)
    {
        m_level++;
    }
}

// ダメージ
int Ax::GetDamage() const
{
    switch (m_level)
    {
    case 1: return 1;
    case 2: return 2;
    case 3: return 3;
    case 4: return 4;
    case 5: return 5;
    default: return 1;
    }
}

// 飛距離
float Ax::GetFlyingDist() const
{
    switch (m_level)
    {
    case 1: return 200.0f;
    case 2: return 250.0f;
    case 3: return 300.0f;
    case 4: return 350.0f;
    case 5: return 400.0f;
    default: return 200.0f;
    }
}

// 弾速
float Ax::GetBulletSpeed() const
{
    switch (m_level)
    {
    case 1: return 7.0f;
    case 2: return 8.0f;
    case 3: return 9.0f;
    case 4: return 10.0f;
    case 5: return 12.0f;
    default: return 7.0f;
    }
}

// 弾数
int Ax::GetBulletCount() const
{
    switch (m_level)
    {
    case 1: return 4;
    case 2: return 5;
    case 3: return 6;
    case 4: return 7;
    case 5: return 8;
    default: return 4;
    }
}

// 発射間隔（秒）
float Ax::GetAttackInterval() const
{
    switch (m_level)
    {
    case 1: return 0.50f;
    case 2: return 0.45f;
    case 3: return 0.40f;
    case 4: return 0.35f;
    case 5: return 0.30f;
    default: return 0.50f;
    }
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
