#pragma once
#include "Base/Base.h"

enum class EGamePhase
{
	None = -1,

	Equipment,	//装備画面
	RoundIntro,  //ラウンド開始演出
	RoundPlaying,	//ラウンド進行中
	RoundOutro,	//ラウンド終了演出

	GameClear,	//ゲームクリア


	Start = Equipment,
};

class Equipment;

class Game
	:public Base {
	static Game* ms_instanse;
	EGamePhase m_phase;
	Equipment* mp_equip;
	int m_round;
	float m_elapsed_time;
	float m_resultTimer = 0.0f;
	void StartPhase();

public:
	Game();
	~Game();
	void Update();
	static void SetGamePhase(EGamePhase phase);
	static EGamePhase GetGamePhase();
	static void EndRound();     // publicに移動
	bool m_isTankDefeated = false;
	static int GetEquippedWeapon(int slot);
};