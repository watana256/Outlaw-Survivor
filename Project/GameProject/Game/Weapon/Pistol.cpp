#include "Pistol.h"
#include "../Player.h"
#include "../BulletAll/Bullet.h"
#include "../GLLibrary/GLLibrary/CInput.h"
#include <cmath>
#include"Game/Game.h"

// ---------------------------------------------------------
// レベルごとの性能表(Lv1〜Lv10)。値を変えたいときはここだけ直す
// ---------------------------------------------------------
static const int   PISTOL_DAMAGE[MAX_WEAPON_LEVEL] = { 10, 20, 30, 40, 50, 60, 70, 80, 90, 100 };
static const float PISTOL_DIST[MAX_WEAPON_LEVEL] = { 500, 600, 700, 800, 1000, 1100, 1200, 1300, 1400, 1500 };
static const float PISTOL_SPEED[MAX_WEAPON_LEVEL] = { 7, 8, 9, 10, 12, 13, 14, 15, 16, 17 };
static const float PISTOL_INTERVAL[MAX_WEAPON_LEVEL] = { 0.50f, 0.45f, 0.40f, 0.35f, 0.30f,
                                                         0.27f, 0.24f, 0.21f, 0.18f, 0.15f };

Pistol::Pistol()
    : Base(eType_Pistil)
    , m_attak_cnt(0.0f)
    , m_is_enable(false)
    , m_level(1)
{
}

const WeaponData& Pistol::GetData()
{
    static const WeaponData data = {
         "ピストル",     "扱いやすい基本の銃",     "連射速度は普通",       10,  100
    };
    return data;
}

void Pistol::SetEnable(bool enable)
{
    m_is_enable = enable;
}

// レベルを設定する(Lv.1〜Lv.10)
void Pistol::SetLevel(int level)
{
    m_level = ClampWeaponLevel(level);
}

// 現在のレベルを取得する
int Pistol::GetLevel() const
{
    return m_level;
}

// ---------------------------------------------------------
// 表示用(Equipmentの詳細欄が使う)
// ---------------------------------------------------------
int Pistol::GetDamageAtLevel(int level)
{
    return PISTOL_DAMAGE[ClampWeaponLevel(level) - 1];
}

float Pistol::GetFlyingDistAtLevel(int level)
{
    return PISTOL_DIST[ClampWeaponLevel(level) - 1];
}

float Pistol::GetBulletSpeedAtLevel(int level)
{
    return PISTOL_SPEED[ClampWeaponLevel(level) - 1];
}

// ---------------------------------------------------------
// 撃つときに使う値(表と同じものを返す)
// ---------------------------------------------------------
int Pistol::GetDamage() const
{
    return GetDamageAtLevel(m_level);
}

// 弾の飛距離
float Pistol::GetFlyingDist() const
{
    return GetFlyingDistAtLevel(m_level);
}

// 攻撃間隔（秒）
float Pistol::GetAttackInterval() const
{
    return PISTOL_INTERVAL[ClampWeaponLevel(m_level) - 1];
}

// 弾の移動速度
float Pistol::GetBulletSpeed() const
{
    return GetBulletSpeedAtLevel(m_level);
}

void Pistol::Update()
{
    if (Game::GetGamePhase() != EGamePhase::RoundPlaying)return;
    if (!m_is_enable) return;

    m_attak_cnt += CFPS::GetDeltaTime();

    // レベルに応じた攻撃間隔
    if (m_attak_cnt < GetAttackInterval()) return;

    Player* player = Player::Instance();
    if (player == nullptr) return;

    CVector2D pos = player->m_pos;
    CVector2D mouse = CInput::GetMousePoint();

    float dx = mouse.x - pos.x;
    float dy = mouse.y - pos.y;

    float length = sqrtf(dx * dx + dy * dy);

    if (length <= 0.001f) return;

    CVector2D dir(dx / length, dy / length);

    // 弾速・飛距離・ダメージをレベルに応じて設定
    new Bullet(
        pos,
        dir,
        GetFlyingDist(),
        GetDamage(),
        15.0f,      // 当たり判定の半径を大きくする
        GetBulletSpeed()
    );

    m_attak_cnt = 0.0f;
}

void Pistol::Draw()
{
}

void Pistol::Collision(Base* b)
{
}