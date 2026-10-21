#include "Enemy.h"
#include "Player.h"
#include "EnemySpawner.h"
#include "Medical.h"
#include "Money.h"
#include"Game.h"
// アニメーション定義
static TexAnim Zombie[] = {
    {0,9},
    {1,9}
};
static TexAnim Robot[] = {
    {0,5},
    {1,5}
};
static TexAnim Dog[] = {
    {0,5},
    {1,5}
};
static TexAnim Mutant[] = {
    {0,5},
    {1,5}
};
static TexAnim TANK[] = {
    {0,5},
    {1,5}
};

// ★ Enemy.h の enum(eZombie, eRobot, eDog, eMutant, eTank) の順番と完全に一致させる
TexAnimData Enemy::_anim_data[] = {
    ANIMDATA(Zombie),
    ANIMDATA(Robot),
    ANIMDATA(Dog),
    ANIMDATA(Mutant),
    ANIMDATA(TANK),
};
struct EnemyParam {
    int hp;               // HP
    float speed;          // 移動速度
    CVector2D size;  //　見た目変更 ←(いります！！！！)
    const char* res_name; // 画像リソース名
};

static const EnemyParam g_enemy_params[] = {
    // HP,   speed,  見た目変更,  画像名
    {  30,   2.0f,    {28, 48}, "Enemy"},       // 0: ゾンビ 
    {  60,   1.5f,    {28, 48},  "Robot"},       // 1: ロボット
    {  15,   5.5f,    {48, 24},  "Dog"},         // 2: ゾンビ犬 
    { 500,   2.5f,    {124, 98},  "hennizonnbi"}, // 3: 変位ゾンビ(ボス) 
    {1000,   1.0f,    {186, 147},  "Tank"},        // 4: 戦車 
};
Enemy::Enemy(const CVector2D& pos, int enemy_type) : Base(eType_Enemy) {
    m_pos = pos;
    m_enemy_type = enemy_type;
    m_flip = false;

    // ★ 1. 敵の種類ごとにパラメータ設定
    const EnemyParam& param = g_enemy_params[m_enemy_type];

    // ★ 2. 画像をリソースからコピーしてセットする
    m_img = COPY_RESOURCE(param.res_name, CImage);

    m_img.ChangeAnimation(0);

    m_hp = param.hp;       // HPもテーブルからセット
    m_speed = param.speed; // 速度もテーブルからセット

    // 画像サイズと中心位置の設定
    m_img.SetSize(param.size);
    m_img.SetCenter(param.size / 2.0f);

    // =========================================================
    // ★ 表示サイズ（param.size）に合わせて当たり判定を自動変更！
    // =========================================================
    float half_w = param.size.x / 2.0f;
    float half_h = param.size.y / 2.0f;

    // 中心から上下左右に広がる判定を作成
    m_rect = CRect(-half_w, -half_h, half_w, half_h);

    if (m_enemy_type == 2) {
        m_rect = CRect(-24.0f, -24.0f, 24.0f, 24.0f); // 縦幅 24px を 48px に拡大！
    }
}

Enemy::~Enemy()
{
    // ---------------------------------------------------------
    // 1. ボス撃破判定
    // ---------------------------------------------------------
    if (m_enemy_type == eMutant || m_enemy_type == eTank) {
        EnemySpawner::boss_kill = true;
        if (m_enemy_type = eTank) {
            m_isTankSpawned = true;
        }
    }

    // ---------------------------------------------------------
    // 2. お金（コイン）のドロップ処理
    // ---------------------------------------------------------

    //  ① まず最初に「ボス（eMutant / eTank）」かを判定！ 金貨大量ドロップ！
    if (m_enemy_type == eMutant || m_enemy_type == eTank) {
        int coin_count = (m_enemy_type == eTank) ? 20 : 15; // 戦車は20枚、ボス変位ゾンビは15枚

        for (int i = 0; i < coin_count; i++) {
            // ボスの位置を中心に散らばせる
            float rx = (float)(rand() % 121 - 60); // -60px  から +60px
            float ry = (float)(rand() % 121 - 60);
            new Money(m_pos + CVector2D(rx, ry), eGold);
        }
    }
    //  ② 中堅雑魚（1:ロボット、 -1:雑魚化された変位ゾンビ）で 金貨 1枚
    else if (m_enemy_type == 1 || m_enemy_type == -1) {
        new Money(m_pos, eGold);
    }
    //  ③ 普通の雑魚（0:ゾンビ、 2:犬） で　銀貨 1枚
    else {
        new Money(m_pos, eSilver);
    }

    // ---------------------------------------------------------
    // 3. 既存の回復アイテム出現処理（そのまま）
    // ---------------------------------------------------------
    int u = rand() % 31;
    if (u == 2 || u == 3 || u == 4 || u == 5 || u == 6) {
        new Medical(m_pos, eKit);
    }
    else if (u == 1) {
        new Medical(m_pos, eBin);
    }
}


void Enemy::Update() {
    m_img.UpdateAnimation();
    Base* player = Base::FindObject(eType_Player);

    // プレイヤーが存在する場合、追尾移動
    if (player) {
        CVector2D dir = player->m_pos - m_pos;

        float stop_x = 12.0f + 12.0f + 2.0f;
        float stop_y = 20.0f + 22.0f + 2.0f;

        CVector2D move_dir(0.0f, 0.0f);

        if (fabsf(dir.x) > stop_x) {
            move_dir.x = (dir.x > 0) ? 1.0f : -1.0f;
        }

        if (fabsf(dir.y) > stop_y) {
            move_dir.y = (dir.y > 0) ? 1.0f : -1.0f;
        }

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
    m_img.Draw();

    // ★ 当たり判定の確認用（必要に応じてコメントアウト解除）
    //  CRect rect(m_pos.x + m_rect.m_left, m_pos.y + m_rect.m_top, m_pos.x + m_rect.m_right, m_pos.y + m_rect.m_bottom);
    //  Utility::DrawQuad(rect.m_pos, rect.m_size, CVector4D(0, 0, 1, 0.5f)); 
}

void Enemy::Collision(Base* b) {
    if (b->m_type == eType_Player) {
        // 当たり判定（四角形判定）
        if (Base::CollisionRect(this, b)) {

            // ★ 敵自身のHPを10減らす！
            // TakeDamage(10);

            // ※もしプレイヤー側のHPも減らしたい場合は、下の行も入れておきます
             if (Player* p = dynamic_cast<Player*>(b)) {
                 p->TakeDamage(10); // プレイヤーにも10ダメージ
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

void Enemy::SetAsMutantMinion() {
    m_hp = 80;                                    // 雑魚用HP
    m_img.SetSize(CVector2D(62, 49));             // 等倍サイズ
    m_img.SetCenter(CVector2D(31.0f, 24.5f));     // 中心位置
    m_rect = CRect(-31.0f, -24.5f, 31.0f, 24.5f); // 当たり判定
    m_enemy_type = -1;                            // ボス撃破フラグ(boss_kill)回避
}