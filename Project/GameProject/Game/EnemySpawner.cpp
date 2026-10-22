#include "EnemySpawner.h"
#include "Game/Enemy.h"    
#include "Game/Player.h"  
#include "Game/Game.h"  
#include "UI.h"
#include <cmath>
#include <cstdlib>

bool EnemySpawner::boss_kill;
int EnemySpawner::m_phase = 1;

EnemySpawner::EnemySpawner() : Base(eType_System) {
    m_phase = 1;               // フェイズ1からスタート
    m_spawn_timer = 0.0f;
    m_spawned_count = 0;
    m_max_spawn_count = 5;    // フェイズ1の敵の数
    m_boss_spawned = false;
    boss_kill = false;
}

void EnemySpawner::Update() {
    if (Game::GetGamePhase() != EGamePhase::RoundPlaying)return;
    Base* player = Base::FindObject(eType_Player);
    if (!player) return; // プレイヤー死亡時は何もしない

    // =============================================================
    //  ボスフェイズ（フェイズ6 または フェイズ11）
    // =============================================================
    if (m_phase == 6 || m_phase == 11) {

        // 1. ボスの生成（まだ出していない場合、1体だけ湧かせる）
        if (!m_boss_spawned) {
            CVector2D boss_pos = GetRandomSpawnPos(player->m_pos);

            if (m_phase == 6) {
                //  フェイズ6の中ボス（例: eMutant）
                new Enemy(boss_pos, eMutant);
            }
            else if (m_phase == 11) {
                //  フェイズ11のラストボス（例: eTank）
                new Enemy(boss_pos, eTank);
                m_isTankSpawned = true;
            }

            m_boss_spawned = true;
            m_spawn_timer = 0.0f;
        }

        // 2. ボス戦中の取り巻き雑魚の出現（3.0秒に1体の遅いペースで無限湧き）
        m_spawn_timer += CFPS::GetDeltaTime();
        if (m_spawn_timer >= 3.0f) {
            m_spawn_timer = 0.0f;
            CVector2D spawn_pos = GetRandomSpawnPos(player->m_pos);
            int enemy_type = rand() % 3; // 雑魚敵（Zombie, Dog, Robot）
            new Enemy(spawn_pos, enemy_type);
        }

        // 3. ボス撃破（全滅）検知 で 次のフェイズへ！
       /*if (m_boss_spawned && boss_kill) {//念のためのコメントアウト
            Base::Kill(1<<eType_Enemy);
            NextPhase();           
            m_boss_spawned = false;
            boss_kill = false;
        }*/ 
        // ボス撃破検知
        if (m_boss_spawned && boss_kill)
        {
            // ラスボスのタンクを撃破した場合
            if (m_phase == 11)
            {
                // ゲームクリア処理はGame側に任せる
                return;
            }

            // フェイズ6の中ボスを撃破した場合
            Base::Kill(1 << eType_Enemy);

            NextPhase();
            Game::EndRound();

            m_boss_spawned = false;
            boss_kill = false;
        }
    }
    // =============================================================
    //  通常雑魚フェイズ（フェイズ 1-5、7-10）
    // =============================================================
    else if (m_phase <= 10) {

        // 1. 規定数まで1.0秒間隔で雑魚敵を出現させる
        if (m_spawned_count < m_max_spawn_count) {
            m_spawn_timer += CFPS::GetDeltaTime();

            if (m_spawn_timer >= 1.0f) {
                m_spawn_timer = 0.0f;
                CVector2D spawn_pos = GetRandomSpawnPos(player->m_pos);
                // =========================================================
                // ここを変更！フェーズ7以降は「3:変位ゾンビ」も出現対象にする
                // =========================================================
                int enemy_type = 0;
                if (m_phase >= 7) {
                    enemy_type = rand() % 4; // 0,1,2 (雑魚) + 3 (変位ゾンビ)
                }
                else {
                    enemy_type = rand() % 3; // 0,1,2 (雑魚)
                }

                // ★ ここを分岐させて、3番(変位ゾンビ)の時だけポインタで雑魚化！
                if (enemy_type == 3) {
                    Enemy* enemy = new Enemy(spawn_pos, 3);
                    enemy->SetAsMutantMinion(); // ★ エラーが出ていた処理をこの1行に変更！
                }
                else {
                    new Enemy(spawn_pos, enemy_type);
                }


                m_spawned_count++;
            }
        }

        // 2. 規定数出し切り ＆ 画面の敵が全滅 で 次のフェイズへ！
        if (m_spawned_count >= m_max_spawn_count && Base::FindObject(eType_Enemy) == nullptr) {
            NextPhase(); // 関数を呼び出して次のフェイズへ
            Game::EndRound();
        }
    }
}

void EnemySpawner::NextPhase()
{
    m_phase++;            // フェーズを進める（1 -> 2 -> 3 ...）
    m_spawned_count = 0;  // 出現数をリセット
    m_spawn_timer = 0.0f; // タイマーリセット

    // ★ UIの切り抜き（フェーズ表示）を1進める
    UI* ui = dynamic_cast<UI*>(Base::FindObject(eType_UI));
    if (ui != nullptr) {
        ui->AddUIType(); // または ui->UIType++;
    }

    // ★ フェーズごとに出す数を設定 (2から←1は上にある)
    switch (m_phase) {
    case 2:  m_max_spawn_count = 8; break;
    case 3:  m_max_spawn_count = 12; break;
    case 4:  m_max_spawn_count = 17; break;
    case 5:  m_max_spawn_count = 24; break;

        // ★ 6はボスフェイズなので飛ばしてOK！

    case 7:  m_max_spawn_count = 28; break; // ボス撃破後のフェイズ7
    case 8:  m_max_spawn_count = 32; break;
    case 9:  m_max_spawn_count = 37; break;
    case 10: m_max_spawn_count = 45; break;

        // ★ 11もラスボスフェイズなので飛ばしてOK！

    default: m_max_spawn_count = 1; break;

    case 12:
        // ★ ゲームクリア時の処理（クリア画面を呼ぶなど）

        break;
    }
}

// プレイヤーの周囲750px（画面外）のランダム位置計算
CVector2D EnemySpawner::GetRandomSpawnPos(const CVector2D& player_pos) {
    float angle = ((float)rand() / RAND_MAX) * 3.14159f * 2.0f;
    float distance = 750.0f;

    float x = player_pos.x + cosf(angle) * distance;
    float y = player_pos.y + sinf(angle) * distance;

    return CVector2D(x, y);
}