#pragma once
#include "Base/Base.h"

class EnemySpawner : public Base {
private:
    int   m_phase;             // 現在のフェイズ（1-11）
    float m_spawn_timer;       // スポーン用タイマー
    int   m_spawned_count;     // 現在のフェイズで出現させた雑魚敵の数
    int   m_max_spawn_count;   // 1フェイズあたりの雑魚敵の出現上限数（例: 15体）

    bool  m_boss_spawned;      // 現在のフェイズでボスを出したか

    // 画面外の安全な位置（プレイヤーの周囲750px）を計算する関数
    CVector2D GetRandomSpawnPos(const CVector2D& player_pos);

public:
    EnemySpawner();
    void Update() override;

    void NextPhase();

    // 現在のフェイズを取得（UI表示用など）
    int GetPhase() const { return m_phase; }
};