
#include "Stella.h"
#include "../Player.h"
#include "../BulletAll/StellaBullet.h"

#define STELLA_RADIUS 150.0f
#define STELLA_ATTACK_TIME 0.5f

Stella::Stella()
    : Base(eType_Stella)
    , m_attak_cnt(0.0f)
    , m_is_enable(false)
    , m_stella_bullet(nullptr)
{
}

void Stella::SetEnable(bool enable)
{
    // すでに同じ状態なら何もしない
    if (m_is_enable == enable)
        return;

    m_is_enable = enable;

    if (m_is_enable)
    {
        // StellaBulletを1個だけ生成
        m_stella_bullet = new StellaBullet(
            STELLA_RADIUS,
            STELLA_ATTACK_TIME,
            GetDamage()
        );
    }
    else
    {
        // 武器が無効になったら攻撃範囲を削除
        if (m_stella_bullet != nullptr)
        {
            m_stella_bullet->SetKill();
            m_stella_bullet = nullptr;
        }
    }
}

int Stella::GetDamage() const
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

void Stella::Update()
{
    // StellaBullet自身が攻撃範囲の追従と
    // ダメージ処理を担当する
}

void Stella::Draw()
{
    // サークルの描画はStellaBullet側で行う
}

void Stella::Collision(Base* b)
{
    // Stella本体では衝突判定を行わない
}
