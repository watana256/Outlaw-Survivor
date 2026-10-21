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
	float m_elapsed_time;

	void StartPhase();
public:
	Game();
	~Game();
	void Update();
	static void SetGamePhase(EGamePhase phase);
	static EGamePhase GetGamePhase();

};