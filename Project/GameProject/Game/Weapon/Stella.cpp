#include "Stella.h"
#include "../Player.h"
#include "../BulletAll/StellaBullet.h"
#include "Game/Game.h"

#define STELLA_RADIUS 100.0f
#define STELLA_ATTACK_TIME 0.5f

// ---------------------------------------------------------
// レベルごとの性能表(Lv1?Lv10)。値を変えたいときはここだけ直す
// 画像サイズは攻撃範囲の1.8倍(Lv1?5と同じ比率)
// ---------------------------------------------------------
static const int   STELLA_DAMAGE[MAX_WEAPON_LEVEL] = { 10, 20, 30, 40, 50, 60, 70, 80, 90, 100 };
static const float STELLA_RANGE[MAX_WEAPON_LEVEL] = { 100, 125, 150, 175, 200, 225, 250, 275, 300, 325 };
static const float STELLA_IMAGE[MAX_WEAPON_LEVEL] = { 180, 225, 270, 315, 360, 405, 450, 495, 540, 585 };

Stella::Stella()
    : Base(eType_Stella)
    , m_attak_cnt(0.0f)
    , m_is_enable(false)
    , m_stella_bullet(nullptr)
    , m_level(1)
{
}

const WeaponData& Stella::GetData()
{
    static const WeaponData data = {
        "ステラ",       "特殊な弾を発射する",     "高い性能を持つ",       40, 1500
    };
    return data;
}

void Stella::SetEnable(bool enable)
{
    if (m_is_enable == enable)
        return;

    m_is_enable = enable;

    if (m_is_enable)
    {
        m_stella_bullet = new StellaBullet(
            GetAttackRange(),
            GetAttackInterval(),
            GetDamage()
        );

        // 生成時のレベルに合わせて画像の大きさも設定する
        m_stella_bullet->SetImageSize(GetImageSize());
    }
    else
    {
        if (m_stella_bullet != nullptr)
        {
            m_stella_bullet->SetKill();
            m_stella_bullet = nullptr;
        }
    }
}

void Stella::SetLevel(int level)
{
    m_level = ClampWeaponLevel(level);

    // 生成済みの攻撃範囲にも性能を反映
    UpdateBulletStats();
}

int Stella::GetLevel() const
{
    return m_level;
}

void Stella::LevelUp()
{
    if (m_level < MAX_WEAPON_LEVEL)
    {
        m_level++;
        UpdateBulletStats();
    }
}

// 表示用(Equipmentの詳細欄が使う)
int Stella::GetDamageAtLevel(int level)
{
    return STELLA_DAMAGE[ClampWeaponLevel(level) - 1];
}

// 撃つときに使う値(表と同じものを返す)
int Stella::GetDamage() const
{
    return GetDamageAtLevel(m_level);
}

float Stella::GetAttackRange() const
{
    return STELLA_RANGE[ClampWeaponLevel(m_level) - 1];
}

//レベルごとに画像サイズ
float Stella::GetImageSize() const
{
    return STELLA_IMAGE[ClampWeaponLevel(m_level) - 1];
}

float Stella::GetAttackInterval() const
{
    return STELLA_ATTACK_TIME;
}

void Stella::UpdateBulletStats()
{
    if (m_stella_bullet != nullptr)
    {
        m_stella_bullet->SetStats(
            GetAttackRange(),
            GetAttackInterval(),
            GetDamage()
        );

        m_stella_bullet->SetImageSize(GetImageSize());
    }
}

void Stella::Update()
{
    if (Game::GetGamePhase() != EGamePhase::RoundPlaying)return;
    // 攻撃範囲とダメージ処理は StellaBullet 側で行う
}

void Stella::Draw()
{
    // 攻撃範囲の描画は StellaBullet 側で行う
}

void Stella::Collision(Base* b)
{
    // Stella本体では衝突判定を行わない
}