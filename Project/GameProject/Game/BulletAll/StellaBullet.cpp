
#include "StellaBullet.h"
#include "../Player.h"
#include "../Enemy.h"

#define ANIM_FRAME 2
#define STELLA_SIZE 180.0f     // 攻撃サークルの大きさ

static TexAnim _Stella_anim[] = 
{
     {0,ANIM_FRAME},{1,ANIM_FRAME},{2,ANIM_FRAME},{3,ANIM_FRAME},
    {4,ANIM_FRAME},{5,ANIM_FRAME},{6,ANIM_FRAME},{7,ANIM_FRAME},
    {8,ANIM_FRAME},{9,ANIM_FRAME},{10,ANIM_FRAME},{11,ANIM_FRAME},
};

TexAnimData StellaBullet::_anim_data[] =
{
    ANIMDATA(_Stella_anim),
};

StellaBullet::StellaBullet(
    float radius,
    float attackTime,
    int damage)
    : Base(eType_StellaBullet)
    , m_attack_time(attackTime)
    , m_attack_cnt(0.0f)
    , m_attack_radius(radius)
    , m_damage(damage)
{
    m_img.Load("Image/StellaBullet.png", _anim_data, 64, 64);

    m_rad = m_attack_radius;

    m_img.SetSize(STELLA_SIZE, STELLA_SIZE);   // 画像サイズ

    m_img.SetCenter(STELLA_SIZE / 2.0f, STELLA_SIZE / 2.0f);

    // アニメーションの初期化
    m_img.ChangeAnimation(0, true);
}

void StellaBullet::Update()
{
    Player* player = Player::Instance();

    if (player == nullptr)
        return;

    m_pos = player->m_pos;
    m_img.UpdateAnimation();

    float deltaTime = CFPS::GetDeltaTime();

    for (auto& entry : m_enemy_attack_timers)
    {
        entry.second += deltaTime;
    }
}

void StellaBullet::Draw()
{
    // プレイヤーと同じ座標系を使用
    m_img.SetPos(m_pos - m_scroll);
    m_img.Draw();
    Utility::DrawCircle(m_pos, m_rad, CVector4D(0, 5, 0, 0.5));
}


void StellaBullet::Collision(Base* b)
{
    if (b->m_type != eType_Enemy)
        return;

    Enemy* enemy = dynamic_cast<Enemy*>(b);
    if (enemy == nullptr)
        return;

    // Stellaの攻撃範囲内か確認
    float dx = enemy->m_pos.x - m_pos.x;
    float dy = enemy->m_pos.y - m_pos.y;
    float distance = sqrtf(dx * dx + dy * dy);

    if (distance > m_attack_radius + enemy->m_rad)
    {
        // 範囲外に出たら次回はすぐ攻撃できる
        m_enemy_attack_timers.erase(enemy);
        return;
    }

    // このEnemyの攻撃タイマーを取得
    auto it = m_enemy_attack_timers.find(enemy);

    if (it == m_enemy_attack_timers.end())
    {
        // 初めて範囲に入った敵はすぐに攻撃
        enemy->TakeDamage(m_damage);
        m_enemy_attack_timers[enemy] = 0.0f;
        return;
    }

    // 個別のタイマーはUpdate()側で進める
    if (it->second >= m_attack_time)
    {
        enemy->TakeDamage(m_damage);
        it->second = 0.0f;
    }
}

