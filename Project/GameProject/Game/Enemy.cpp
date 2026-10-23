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
    {  30,   3.0f,    {28, 48}, "Enemy"},       // 0: ゾンビ 
    {  60,   2.0f,    {28, 48},  "Robot"},       // 1: ロボット
    {  15,   6.0f,    {48, 24},  "Dog"},         // 2: ゾンビ犬 
    { 800,   1.7f,    {124, 98},  "hennizonnbi"}, // 3: 変位ゾンビ(ボス) 
    {2000,   1.0f,    {186, 147},  "Tank"},        // 4: 戦車 
};
Enemy::Enemy(const CVector2D& pos, int enemy_type) : Base(eType_Enemy) {
    m_pos = pos;
    m_enemy_type = enemy_type;
    m_flip = false;

    // 安全対策（-1の場合はボス変位ゾンビの要素3を参照）
    int param_idx = (m_enemy_type < 0) ? 3 : m_enemy_type;
    const EnemyParam& param = g_enemy_params[param_idx];

    m_img = COPY_RESOURCE(param.res_name, CImage);
    m_img.ChangeAnimation(0);

    // =========================================================
    // ★ HPを (基本HP * ラウンド数) に計算！
    // =========================================================
    int current_round = EnemySpawner::m_phase; // ※プロジェクトで使っているラウンド（フェーズ）変数名に合わせてください
    m_hp = param.hp * current_round;           // ★ 基本HP × ラウンド数

    m_speed = param.speed; // 速度はそのまま

    // 画像サイズと中心位置の設定
    m_img.SetSize(param.size);
    m_img.SetCenter(param.size / 2.0f);

    // 当たり判定の自動設定
    float half_w = param.size.x / 2.0f;
    float half_h = param.size.y / 2.0f;
    m_rect = CRect(-half_w, -half_h, half_w, half_h);

    if (m_enemy_type == 2) {
        m_rect = CRect(-24.0f, -24.0f, 24.0f, 24.0f); // 犬の判定補正
    }
    m_rad = half_w; // 押し戻し計算用の半径をセット
}

Enemy::~Enemy()
{
    // ---------------------------------------------------------
    // 1. ボス撃破判定
    // ---------------------------------------------------------
    if (m_enemy_type == eMutant || m_enemy_type == eTank) {
        EnemySpawner::boss_kill = true;
        if (m_enemy_type == eTank) {
        }
    }

    // ---------------------------------------------------------
// 2. お金（コイン）のドロップ処理
// ---------------------------------------------------------

//  ① ボス（eMutant / eTank）: 金貨を大量ドロップ
    if (m_enemy_type == eMutant || m_enemy_type == eTank) {
        int coin_count = (m_enemy_type == eTank) ? 30 : 20; // 戦車は30枚、変位ゾンビは20枚

        for (int i = 0; i < coin_count; i++) {
            // ボスの位置を中心に散らばせる
            float rx = (float)(rand() % 121 - 60); // -60px から +60px
            float ry = (float)(rand() % 121 - 60);
            new Money(m_pos + CVector2D(rx, ry), eGold);
        }
    }
    //  ② 中堅雑魚（1:ロボット、-1:雑魚化された変位ゾンビ）
//     金貨2枚 + 銀貨0〜2枚 → 合計 200 / 250 / 300
    else if (m_enemy_type == 1 || m_enemy_type == -1) {
        for (int i = 0; i < 2; i++) {
            float rx = (float)(rand() % 41 - 20); // -20px から +20px
            float ry = (float)(rand() % 41 - 20);
            new Money(m_pos + CVector2D(rx, ry), eGold);
        }

        int silver_count = rand() % 3; // 0, 1, 2 のどれか
        for (int i = 0; i < silver_count; i++) {
            float rx = (float)(rand() % 41 - 20);
            float ry = (float)(rand() % 41 - 20);
            new Money(m_pos + CVector2D(rx, ry), eSilver);
        }
    }
    //  ③ 普通の雑魚（0:ゾンビ、2:犬）: 銀貨2〜4枚
    else {
        int coin_count = 2 + rand() % 3; // 2, 3, 4 のどれか

        for (int i = 0; i < coin_count; i++) {
            float rx = (float)(rand() % 41 - 20);
            float ry = (float)(rand() % 41 - 20);
            new Money(m_pos + CVector2D(rx, ry), eSilver);
        }
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
    // 1. プレイヤーとの判定（今書いているやつ）
    if (b->m_type == eType_Player) {
        if (Base::CollisionRect(this, b)) {
            if (Player* p = dynamic_cast<Player*>(b)) {
                p->TakeDamage(10);
            }
        }
    }

    // 2. 敵同士の重なり防止（Base.h を使わずに直接押し戻す！）
    if (b->m_type == eType_Enemy) {
        CVector2D v = m_pos - b->m_pos;
        float len = v.Length();
        float dist = m_rad + b->m_rad; // お互いの半径の合計

        // めり込んでいる場合、半分ずつ押し戻す
        if (len < dist && len > 0.0001f) {
            CVector2D dir = v.GetNormalize();
            float push = (dist - len) * 0.5f;
            m_pos += dir * push;     // 自分を離す
            b->m_pos -= dir * push; // 相手を離す
        }
    }
}

void Enemy::TakeDamage(int damage)
{
    // すでに撃破されている敵には処理しない
    if (m_hp <= 0) return;

    m_hp -= damage;

    if (m_hp <= 0)
    {
        m_hp = 0;

        // ラスボスのタンクが撃破された
        if (m_enemy_type == eTank)
        {
            Game* game = dynamic_cast<Game*>(
                Base::FindObject(eType_Scene)
                );

            if (game != nullptr)
            {
                game->m_isTankDefeated = true;
            }
        }

        SetKill();
    }
}

void Enemy::SetAsMutantMinion() {
    m_hp = 80;                                    // 雑魚用HP
    m_speed = 2.5f;
    m_img.SetSize(CVector2D(62, 49));             // 等倍サイズ
    m_img.SetCenter(CVector2D(31.0f, 24.5f));     // 中心位置
    m_rect = CRect(-31.0f, -24.5f, 31.0f, 24.5f); // 当たり判定
    m_enemy_type = -1;                            // ボス撃破フラグ(boss_kill)回避
}