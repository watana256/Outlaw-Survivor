#include "Enemy.h"
#include "Player.h"

// アニメーション定義
static TexAnim Zombie[] =
{ 
    {0,5}, 
    {1,5}
};
static TexAnim Dog[] = { 
    {0,5}, 
    {1,5} 
};
static TexAnim Robot[] = 
{ 
    {0,5}, 
    {1,5} 
};

static TexAnim Mutant[] = 
{ 
    {0,5}, 
    {1,5} 
};
static TexAnim TANK[] = {
    {0,5}, 
    {1,5} 
};

// enum（eZombie, eDog, eRobot, eMutant, eTank）の順番と合わせる
TexAnimData Enemy::_anim_data[] = {
    ANIMDATA(Zombie),
    ANIMDATA(Dog),
    ANIMDATA(Robot),
    ANIMDATA(Mutant),
    ANIMDATA(TANK),
};

Enemy::Enemy(const CVector2D& pos, int enemy_type) : Base(eType_Enemy) {
    m_pos = pos;
    int Anim;
    m_enemy_type = enemy_type;
    m_rect = CRect(-15, -25, 15, 25);
    m_flip = false;

    // 敵の種類ごとにパラメータ設定
    switch (m_enemy_type) {
    case eZombie:
        m_hp = 30;
        m_speed = 2.0f;
        m_rad = 16;
        m_img = COPY_RESOURCE("Enemy", CImage);
        break;

    case eDog:
        m_hp = 15;
        m_speed = 4.5f;
        m_rad = 12;
        m_img = COPY_RESOURCE("Enemy", CImage);
        break;

    case eRobot:
        m_hp = 60;
        m_speed = 1.5f;
        m_rad = 20;
        m_img = COPY_RESOURCE("Enemy", CImage);
        break;

    case eMutant:
        m_hp = 300;
        m_speed = 2.5f;
        m_rad = 32;
        m_img = COPY_RESOURCE("Enemy", CImage);
        break;

    case eTank:
        m_hp = 1000;
        m_speed = 1.0f;
        m_rad = 48;
        m_img = COPY_RESOURCE("Enemy", CImage);
        break;
    }
    m_img.ChangeAnimation(0);
}

void Enemy::Update() {

    m_img.UpdateAnimation();
    Base* player = Base::FindObject(eType_Player);

    //  プレイヤーが存在する場合、追尾
    if (player) {
        CVector2D dir = player->m_pos - m_pos;
        float len = dir.Length();

        if (len > 0.0f) {
            dir.x /= len;
            dir.y /= len;
        }

        m_pos += dir * m_speed;

        // 向きの更新
        m_flip = (player->m_pos.x < m_pos.x);
    }

    // 当たり判定用四角形の領域を更新 (中心 m_pos、幅28、高さ52)
    //m_rect = CRect(m_pos.x - 14, m_pos.y - 26, m_pos.x + 14, m_pos.y + 26);
}

void Enemy::Draw() {

    m_img.SetFlipH(m_flip);

    m_img.SetPos(m_pos);
    m_img.SetSize(28, 52);
    m_img.SetCenter(14, 26);
    m_img.Draw();
    //Utility::DrawCircle(m_pos, m_rad, CVector4D(0, 1, 0, 0.5));
    DrawRect();


}

void Enemy::Collision(Base* b) {
    // 相手がプレイヤーの場合
    if (b->m_type == eType_Player) {
        // 当たり判定（円判定または矩形判定）
        if (Base::CollisionCircle(this, b)) { // または Base::CollisionRect(this, b)
            if (Player* p = dynamic_cast<Player*>(b)) {
                p->TakeDamage(10); // プレイヤー側のTakeDamageを呼ぶだけ！
            }
        }
    }
}


void Enemy::TakeDamage(int damage) {
    m_hp -= damage;
    if (m_hp <= 0) {
        SetKill(); // 撃破
    }
}