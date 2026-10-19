

#include "Drone.h"
#include "../Player.h"
#include "../Enemy.h"
#include "../BulletAll/Bullet.h"

#include <cmath>
#include <vector>

#define DRONE_SIZE 30.0f
#define DRONE_RANGE 300.0f
#define DRONE_BULLET_RADIUS 25.0f

Drone::Drone()
    : Base(eType_Drone)
    , m_attak_cnt(0.0f)
    , m_is_enable(false)
    , m_attack_range(DRONE_RANGE)
    , m_offset_x(30.0f)
    , m_offset_y(-20.0f)
    , m_level(1)
{
    // ドローン画像を読み込む
    m_img.Load("Image/DroneOBJ.png");

    // ドローン画像の表示サイズ
    m_img.SetSize(DRONE_SIZE, DRONE_SIZE);

    // 画像の中心を設定
    m_img.SetCenter(
        DRONE_SIZE / 2.0f,
        DRONE_SIZE / 2.0f
    );

    m_rad = 10.0f;
}

void Drone::SetEnable(bool enable)
{
    m_is_enable = enable;
}

void Drone::SetLevel(int level)
{
    // レベルを1～5の範囲に制限
    if (level < 1)
        level = 1;

    if (level > 5)
        level = 5;

    m_level = level;
}

int Drone::GetLevel() const
{
    return m_level;
}

void Drone::LevelUp()
{
    // 最大レベルは5
    if (m_level < 5)
    {
        m_level++;
    }
}

int Drone::GetDamage() const
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

float Drone::GetFlyingDist() const
{
    switch (m_level)
    {
    case 1: return 500.0f;
    case 2: return 600.0f;
    case 3: return 700.0f;
    case 4: return 800.0f;
    case 5: return 900.0f;
    }

    return 500.0f;
}

float Drone::GetBulletSpeed() const
{
    switch (m_level)
    {
    case 1: return 7.0f;
    case 2: return 8.0f;
    case 3: return 9.0f;
    case 4: return 10.0f;
    case 5: return 11.0f;
    }

    return 7.0f;
}

float Drone::GetAttackInterval() const
{
    switch (m_level)
    {
    case 1: return 0.50f;
    case 2: return 0.45f;
    case 3: return 0.40f;
    case 4: return 0.35f;
    case 5: return 0.30f;
    }

    return 0.50f;
}

void Drone::Update()
{
    if (!m_is_enable)
        return;

    Player* player = Player::Instance();

    if (player == nullptr)
        return;

    // Playerの横にドローンを配置
    m_pos.x = player->m_pos.x + m_offset_x;
    m_pos.y = player->m_pos.y + m_offset_y;

    // 発射間隔を計測
    m_attak_cnt += CFPS::GetDeltaTime();

    if (m_attak_cnt < GetAttackInterval())
        return;

    // Enemy一覧を取得
    std::vector<Base*> enemies =
        Base::FindObjects(eType_Enemy);

    Enemy* target = nullptr;

    float nearest_dist_sq =
        m_attack_range * m_attack_range;

    // 射程内で最も近いEnemyを探す
    for (Base* b : enemies)
    {
        Enemy* enemy = dynamic_cast<Enemy*>(b);

        if (enemy == nullptr || enemy->m_kill)
            continue;

        float dx = enemy->m_pos.x - m_pos.x;
        float dy = enemy->m_pos.y - m_pos.y;

        float dist_sq = dx * dx + dy * dy;

        if (dist_sq < nearest_dist_sq)
        {
            nearest_dist_sq = dist_sq;
            target = enemy;
        }
    }

    // 射程内にEnemyがいなければ撃たない
    if (target == nullptr)
        return;

    // ドローンからEnemyへ向かう方向を計算
    float dx = target->m_pos.x - m_pos.x;
    float dy = target->m_pos.y - m_pos.y;

    float length = sqrtf(dx * dx + dy * dy);

    if (length <= 0.001f)
        return;

    CVector2D dir(
        dx / length,
        dy / length
    );

    // 現在のレベルに応じた性能で弾を発射
    new Bullet(
        m_pos,
        dir,
        GetFlyingDist(),
        GetDamage(),
        DRONE_BULLET_RADIUS,
        GetBulletSpeed()
    );

    // 発射したのでタイマーをリセット
    m_attak_cnt = 0.0f;
}

void Drone::Draw()
{
    if (!m_is_enable)
        return;

    // スクロールを反映して描画
    m_img.SetPos(m_pos - m_scroll);
    m_img.Draw();
}

void Drone::Collision(Base* b)
{
    // ドローン本体には当たり判定を設けない
}
