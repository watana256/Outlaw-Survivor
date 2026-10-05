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

    // プレイヤーが存在する場合、追尾移動
    if (player) {
        CVector2D dir = player->m_pos - m_pos;

        // ★ 横(X)と縦(Y)それぞれの停止距離を設定
        // 横方向：敵半分(15) + プレイヤー半分(15) + 8 = 38.0f
        float stop_x = 12.0f + 12.0f + 2.0f;
        // 縦方向：敵半分(25) + プレイヤー半分(25) + 8 = 58.0f
        float stop_y = 20.0f + 22.0f + 2.0f;

        CVector2D move_dir(0.0f, 0.0f);

        // X方向（横）：枠から8ドットより離れていれば移動
        if (fabsf(dir.x) > stop_x) {
            move_dir.x = (dir.x > 0) ? 1.0f : -1.0f;
        }

        // Y方向（縦）：枠から8ドットより離れていれば移動
        if (fabsf(dir.y) > stop_y) {
            move_dir.y = (dir.y > 0) ? 1.0f : -1.0f;
        }

        // 移動成分がある場合、正規化して移動（斜め移動の加速防止）
        float move_len = move_dir.Length();
        if (move_len > 0.0f) {
            move_dir.x /= move_len;
            move_dir.y /= move_len;
            m_pos += move_dir * m_speed;
        }

        // 向きの更新
        m_flip = (player->m_pos.x < m_pos.x);
    }
}
void Enemy::Draw() {
    m_img.SetFlipH(m_flip);
    m_img.SetPos(m_pos);
    m_img.SetSize(28, 52);
    m_img.SetCenter(14, 26);
    m_img.Draw();

    // ★ 当たり判定の四角を「青色」で描画
    CRect rect(m_pos.x + m_rect.m_left, m_pos.y + m_rect.m_top, m_pos.x + m_rect.m_right, m_pos.y + m_rect.m_bottom);
    Utility::DrawQuad(rect.m_pos, rect.m_size, CVector4D(0, 0, 1, 0.5f)); // CVector4D(R, G, B, Alpha)
}

void Enemy::Collision(Base* b) {
    // 相手がプレイヤーの場合
    if (b->m_type == eType_Player) {
        // ★ 円判定(CollisionCircle)ではなく、四角判定(CollisionRect)を使用！
        if (Base::CollisionRect(this, b)) {
            if (Player* p = dynamic_cast<Player*>(b)) {
                p->TakeDamage(10);
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