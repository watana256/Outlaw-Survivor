#include "Enemy.h"
#include "Player.h"
#include "EnemySpawner.h"
#include "Medical.h"

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
static TexAnim Snake[] = {
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

// ★ Enemy.h の enum(eZombie, eRobot, eDog, eSnake, eMutant, eTank) の順番と完全に一致させる
TexAnimData Enemy::_anim_data[] = {
    ANIMDATA(Zombie),
    ANIMDATA(Robot),
    ANIMDATA(Dog),
    ANIMDATA(Snake),
    ANIMDATA(Mutant),
    ANIMDATA(TANK),
};
struct EnemyParam {
    int hp;             // HP
    float speed;        // 移動速度
    CVector2D size;     //　当たり判定のサイズ
    CVector2D render_size; // 画面上のサイズ(?)
    const char* res_name; // 画像リソース名
};
static const EnemyParam g_enemy_params[] = {
    // HP,   speed,当たり判定  サイズ     画像名
    {  30,   2.0f, {14 ,24}, {14,24},"Enemy"}, // eZombie
    {  60,   1.5f, {32 ,16}, {12,12},"Robot"}, // eRobot
    {  15,   4.5f, {32 ,16}, {12,12},"Dog"}, // eDog
    {  40,   3.0f, {14 ,24}, {12,12},"Snake"}, // eSnake
    { 300,   2.5f, {61 ,49}, {12,12},"hennizonnbi"}, // eMutant
    {1000,   1.0f, {14 ,24}, {140, 240},"Tank"}, // eTank
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
}


Enemy::~Enemy()
{
    if (m_enemy_type == eMutant||m_enemy_type == eTank) {
        EnemySpawner::boss_kill = true;
    }
    int u = rand() % 31;//回復ランダム出現
        if (u == 2 || u == 3 || u == 4 || u == 5 || u == 6) {
            new Medical(m_pos, eKit);
            int u = 0;
        }
        else if (u == 1) {
            new Medical(m_pos, eBin);
            int u = 0;
        }
        else {
            int u = 0;
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
    m_img.SetSize(28, 52);
    m_img.SetCenter(14, 26);
    m_img.Draw();

    // ★ 当たり判定の確認用（必要に応じてコメントアウト解除）
    /* CRect rect(m_pos.x + m_rect.m_left, m_pos.y + m_rect.m_top, m_pos.x + m_rect.m_right, m_pos.y + m_rect.m_bottom);
    Utility::DrawQuad(rect.m_pos, rect.m_size, CVector4D(0, 0, 1, 0.5f)); */
}

void Enemy::Collision(Base* b) {
    if (b->m_type == eType_Player) {
        // 当たり判定（四角形判定）
        if (Base::CollisionRect(this, b)) {

            // ★ 敵自身のHPを10減らす！
            TakeDamage(10);

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