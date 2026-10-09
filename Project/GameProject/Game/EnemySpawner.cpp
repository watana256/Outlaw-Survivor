#include "EnemySpawner.h"
#include "Game/Enemy.h"    
#include "Game/Player.h"  
#include <cmath>
#include <cstdlib>

EnemySpawner::EnemySpawner() : Base(eType_System) {
    m_phase = 1;               // フェイズ1からスタート
    m_spawn_timer = 0.0f;
    m_spawned_count = 0;
    m_max_spawn_count = 15;    // 通常フェイズでの雑魚敵の数（調整OK）
    m_boss_spawned = false;
}

void EnemySpawner::Update() {
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
                Base::Add(new Enemy(boss_pos, eMutant));
            }
            else if (m_phase == 11) {
                //  フェイズ11のラストボス（例: eTank）
                Base::Add(new Enemy(boss_pos, eTank));
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
            Base::Add(new Enemy(spawn_pos, enemy_type));
        }

        // 3. ボス撃破（全滅）検知 で 次のフェイズへ！
        if (m_boss_spawned && Base::FindObject(eType_Enemy) == nullptr) {
            m_phase++;              // フェイズを進める（6から7、11から12[クリア]）
            m_spawned_count = 0;
            m_spawn_timer = 0.0f;
            m_boss_spawned = false;
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
                int enemy_type = rand() % 3; // 雑魚敵
                Base::Add(new Enemy(spawn_pos, enemy_type));
                m_spawned_count++;
            }
        }

        // 2. 規定数出し切り ＆ 画面の敵が全滅 で 次のフェイズへ！
        if (m_spawned_count >= m_max_spawn_count && Base::FindObject(eType_Enemy) == nullptr) {
            NextPhase(); // 関数を呼び出して次のフェイズへ
        }
    }
}

void EnemySpawner::NextPhase()
{
}

// プレイヤーの周囲750px（画面外）のランダム位置計算
CVector2D EnemySpawner::GetRandomSpawnPos(const CVector2D& player_pos) {
    float angle = ((float)rand() / RAND_MAX) * 3.14159f * 2.0f;
    float distance = 750.0f;

    float x = player_pos.x + cosf(angle) * distance;
    float y = player_pos.y + sinf(angle) * distance;

    return CVector2D(x, y);
}