#pragma once
#include "Base/Base.h"

enum eEnemyType {
    eZombie,       // ゾンビ
    eRobot,        // ロボ
    eDog,          // 犬
    eSnake,        // 巨大な蛇
    eMutant,       // 変異ゾンビ（中間ボス / 通常）
    eTank          // 戦車（ボス）
};

class Enemy : public Base {
private:
    CImage  m_img;
    int     m_enemy_type; // 敵の種類
    int     m_hp;         // 体力
    float   m_speed;      // 移動速度
    bool    m_flip;       // 左右反転

    int m_anim_frame; // 現在のコマ番号（0 または 1）
    int m_anim_timer; // コマ切り替え用のカウントタイマー

public:
    // ★ 修正：main.cpp からアニメーションデータを使えるように宣言を追加
    static TexAnimData _anim_data[];

    // コンストラクタ（出現位置, 敵の種類）
    Enemy(const CVector2D& pos, int enemy_type);

    void Update() override;
    void Draw() override;
    void Collision(Base* b) override;
    void TakeDamage(int damage); // ダメージ処理
};