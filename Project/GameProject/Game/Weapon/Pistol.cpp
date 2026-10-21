
#include "Pistol.h"
#include "../Player.h"
#include "../BulletAll/Bullet.h"
#include "../GLLibrary/GLLibrary/CInput.h"
#include <cmath>
#include"Game/Game.h"

Pistol::Pistol()
    : Base(eType_Pistil)
    , m_attak_cnt(0.0f)
    , m_is_enable(false)
    , m_level(1)
{
}

void Pistol::SetEnable(bool enable)
{
    m_is_enable = enable;
}

// ƒŒƒxƒ‹‚ğİ’è‚·‚é
void Pistol::SetLevel(int level)
{
    if (level < 1) level = 1;
    if (level > 5) level = 5;

    m_level = level;
}

// Œ»İ‚ÌƒŒƒxƒ‹‚ğæ“¾‚·‚é
int Pistol::GetLevel() const
{
    return m_level;
}

// ƒ_ƒ[ƒW
int Pistol::GetDamage() const
{
    switch (m_level) {
    case 1: return 1;
    case 2: return 2;
    case 3: return 3;
    case 4: return 4;
    case 5: return 5;
    default: return 1;
    }
}

// ’e‚Ì”ò‹——£
float Pistol::GetFlyingDist() const
{
    switch (m_level) {
    case 1: return 500.0f;
    case 2: return 600.0f;
    case 3: return 700.0f;
    case 4: return 800.0f;
    case 5: return 1000.0f;
    default: return 500.0f;
    }
}

// UŒ‚ŠÔŠui•bj
float Pistol::GetAttackInterval() const
{
    switch (m_level) {
    case 1: return 0.50f;
    case 2: return 0.45f;
    case 3: return 0.40f;
    case 4: return 0.35f;
    case 5: return 0.30f;
    default: return 0.50f;
    }
}

// ’e‚ÌˆÚ“®‘¬“x
float Pistol::GetBulletSpeed() const
{
    switch (m_level) {
    case 1: return 7.0f;
    case 2: return 8.0f;
    case 3: return 9.0f;
    case 4: return 10.0f;
    case 5: return 12.0f;
    default: return 7.0f;
    }
}

void Pistol::Update()
{
    if (Game::GetGamePhase() != EGamePhase::RoundPlaying)return;
    if (!m_is_enable) return;

    m_attak_cnt += CFPS::GetDeltaTime();

    // ƒŒƒxƒ‹‚É‰‚¶‚½UŒ‚ŠÔŠu
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

    // ’e‘¬E”ò‹——£Eƒ_ƒ[ƒW‚ğƒŒƒxƒ‹‚É‰‚¶‚Äİ’è
    new Bullet(
        pos,
        dir,
        GetFlyingDist(),
        GetDamage(),
        15.0f,      // “–‚½‚è”»’è‚Ì”¼Œa‚ğ‘å‚«‚­‚·‚é
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
