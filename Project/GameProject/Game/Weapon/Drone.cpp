
#include "Drone.h"
#include "../Player.h"
#include "../Enemy.h"
#include "../BulletAll/Bullet.h"

#include <cmath>

#define ATTACK_TIME 0.5f
#define FLYING_DIST 500.0f

#define DRONE_SIZE 30.0f        //サイズ
#define DRONE_RANGE 300.0f

#define DRONE_BULLET_RADIUS 25.0f

Drone::Drone()
    : Base(eType_Drone)
    , m_attak_cnt(0.0f)
    , m_is_enable(false)
    , m_attack_range(DRONE_RANGE)
    , m_offset_x(30.0f)
    , m_offset_y(-20.0f)
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

int Drone::GetDamage() const
{
    int lv = 1;

    switch (lv)
    {
    case 1: return 10;
    case 2: return 2;
    case 3: return 3;
    case 4: return 4;
    }

    return 1;
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

    if (m_attak_cnt < ATTACK_TIME)
        return;

    // Enemy一覧を取得
    std::vector<Base*> enemies =
        Base::FindObjects(eType_Enemy);

    Enemy* target = nullptr;

    float nearest_dist_sq =
        m_attack_range * m_attack_range;

    // 最も近いEnemyを探す
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


    // 既存のBulletを使用して発射
    // ドローンの弾だけ当たり判定を大きくする
    new Bullet(
        m_pos,
        dir,
        FLYING_DIST,
        GetDamage(),
        DRONE_BULLET_RADIUS
    );


    // 発射したときだけタイマーをリセット
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
