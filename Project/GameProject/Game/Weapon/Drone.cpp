#include "Drone.h"
#include "../Player.h"
#include "../Enemy.h"
#include "../BulletAll/Bullet.h"
#include "Game/Game.h"

#include <cmath>
#include <vector>

#define DRONE_SIZE 30.0f
#define DRONE_RANGE 300.0f
#define DRONE_BULLET_RADIUS 25.0f

// ---------------------------------------------------------
// レベルごとの性能表(Lv1〜Lv10)。値を変えたいときはここだけ直す
// ---------------------------------------------------------
static const int   DRONE_DAMAGE[MAX_WEAPON_LEVEL] = { 10, 20, 30, 40, 50, 60, 70, 80, 90, 100 };
static const float DRONE_DIST[MAX_WEAPON_LEVEL] = { 500, 600, 700, 800, 900, 1000, 1100, 1200, 1300, 1400 };
static const float DRONE_SPEED[MAX_WEAPON_LEVEL] = { 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
static const float DRONE_INTERVAL[MAX_WEAPON_LEVEL] = { 0.50f, 0.45f, 0.40f, 0.35f, 0.30f,
                                                        0.27f, 0.24f, 0.21f, 0.18f, 0.15f };

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
const WeaponData& Drone::GetData()
{
    static const WeaponData data = {
        "ドローン", "自動で敵を攻撃する", "プレイヤーに追従する", 10, 1200
    };
    return data;
}
void Drone::SetEnable(bool enable)
{
    m_is_enable = enable;
}

void Drone::SetLevel(int level)
{
    // レベルを1〜最大の範囲に制限
    m_level = ClampWeaponLevel(level);
}

int Drone::GetLevel() const
{
    return m_level;
}

void Drone::LevelUp()
{
    // 最大レベルまで
    if (m_level < MAX_WEAPON_LEVEL)
    {
        m_level++;
    }
}

// ---------------------------------------------------------
// 表示用(Equipmentの詳細欄が使う)
// ---------------------------------------------------------
int Drone::GetDamageAtLevel(int level)
{
    return DRONE_DAMAGE[ClampWeaponLevel(level) - 1];
}

float Drone::GetFlyingDistAtLevel(int level)
{
    return DRONE_DIST[ClampWeaponLevel(level) - 1];
}

float Drone::GetBulletSpeedAtLevel(int level)
{
    return DRONE_SPEED[ClampWeaponLevel(level) - 1];
}

// ---------------------------------------------------------
// 撃つときに使う値(表と同じものを返す)
// ---------------------------------------------------------
int Drone::GetDamage() const
{
    return GetDamageAtLevel(m_level);
}

float Drone::GetFlyingDist() const
{
    return GetFlyingDistAtLevel(m_level);
}

float Drone::GetBulletSpeed() const
{
    return GetBulletSpeedAtLevel(m_level);
}

float Drone::GetAttackInterval() const
{
    return DRONE_INTERVAL[ClampWeaponLevel(m_level) - 1];
}

void Drone::Update()
{
    if (Game::GetGamePhase() != EGamePhase::RoundPlaying)return;
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
