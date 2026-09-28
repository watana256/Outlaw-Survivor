#include "Enemy.h"
#include "Player.h"

Enemy::Enemy(const CVector2D& pos, int enemy_type) : Base(eType_Enemy) {
    m_pos = pos;
    m_enemy_type = enemy_type;
    m_flip = false;

    // 敵の種類ごとに HP・速度・当たり判定・画像を切り替え
    switch (m_enemy_type) {
    case eZombie:
        m_hp = 30;
        m_speed = 2.0f;
        m_rad = 16;
        m_img.Load("Image/zonnbi.png");
        break;

    case eDog:
        m_hp = 15;
        m_speed = 4.5f; // 足が速い
        m_rad = 12;
        m_img.Load("Image/Dog.png");
        break;

    case eRobot:
        m_hp = 60;
        m_speed = 1.5f; // 硬くて遅い
        m_rad = 20;
        m_img.Load("Image/Robot.png");
        break;

    case eMutant: // 中間ボス（変異ゾンビ）
        m_hp = 300;
        m_speed = 2.5f;
        m_rad = 32;
        m_img.Load("Image/hennizonnbi.png");
        break;

    case eTank: // ボス（戦車）
        m_hp = 1000;
        m_speed = 1.0f;
        m_rad = 48;
        m_img.Load("Image/TANK.png");
        break;


    }
}

void Enemy::Update() {
    // 1. プレイヤー（主人公）の探索
    Base* player = Base::FindObject(eType_Player);

    // 2. プレイヤーが存在する場合、その方向へ360度追尾
    if (player) {
        // 自分からプレイヤーへのベクトルを計算
        CVector2D dir = player->m_pos - m_pos;
        float len = dir.Length();

        if (len > 0) {
            dir.x /= len; // 正規化（単位ベクトル化）
            dir.y /= len;
        }

        // プレイヤーの方向へ移動
        m_pos += dir * m_speed;

        // 向きの更新（左にいるなら反転）
        if (player->m_pos.x < m_pos.x) {
            m_flip = true;
        }
        else {
            m_flip = false;
        }
    }
}

void Enemy::Draw() {

}

void Enemy::Collision(Base* b){

}

void Enemy::TakeDamage(int damage) {
    m_hp -= damage;
    if (m_hp <= 0) {
        SetKill(); // 撃破
    }
}