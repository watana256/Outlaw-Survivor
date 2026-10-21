#include "Game.h"
#include "Base/Base.h"
#include "Player.h"
#include "Enemy.h"
#include "HP.h"
#include "Money.h"
#include"UI.h"
#include "BulletAll/Bullet.h"
#include "Score.h"
#include"Field.h"
#include"Title.h"
#include "Weapon/Pistol.h"
#include"Weapon/Ax.h"
#include"Weapon/Drone.h"
#include"Weapon/Flamethrower.h"
#include"Weapon/Landmine.h"
#include"Weapon/Molotov.h"
#include"Weapon/Rocket.h"
#include"Weapon/Shotgun.h"
#include"Weapon/Stella.h"
#include"Medical.h"
#include"BulletAll/RocketBullet.h"
#include"BulletAll/FlamethrowerBullet.h"
#include"BulletAll/AxBullet.h"
#include"BulletAll/MolotovBullet.h"
#include"Equipment.h"
#include"Result.h"

Game* Game::ms_instanse = nullptr;



Game::Game() 
	:Base(eType_Scene)
	,m_phase(EGamePhase::None)
{
	ms_instanse = this;
	mp_equip = new Equipment();
	SetGamePhase(EGamePhase::Start);
}
Game::~Game() 
{
	ms_instanse = nullptr;
}
void Game::StartPhase()
{
	switch (m_phase)
	{
	case EGamePhase::Equipment:
		mp_equip->Show();
		break;
	case EGamePhase::RoundIntro:
		break;
	case EGamePhase::RoundPlaying:
		break;
	case EGamePhase::RoundOutro:
		break;
	case EGamePhase::GameClear:
		break;

	}
}
void Game::Update()
{
	switch (m_phase) 
	{
	case EGamePhase::Equipment:
		if (!mp_equip->IsShow()) 
		{
			SetGamePhase(EGamePhase::RoundIntro);
		}

		break;
	case EGamePhase::RoundIntro:
		if (m_elapsed_time >= 3.0f) 
		{
			SetGamePhase(EGamePhase::RoundPlaying);
		}
		m_elapsed_time += CFPS::GetDeltaTime();
		break;
	case EGamePhase::RoundPlaying:
		break;
	case EGamePhase::RoundOutro:
		break;
	case EGamePhase::GameClear:
		break;
		
	}
	// プレイヤーが死亡したらGameOver
	if (!Base::FindObject(eType_Player))
	{
		//Base::KillAll();
		new Result();
		return;
	}
	
	// ゴールしたらGameCrear
	if (m_isTankDefeated)
	{
		m_resultTimer += 1.0f / 60.0f; // 60FPSを想定

		if (m_resultTimer >= 1.0f)
		{
		//	Base::KillAll();
			new Result();
			return;
		}
	}
}

void Game::SetGamePhase(EGamePhase phase)
{
	if (ms_instanse == nullptr) return;
	if (ms_instanse->m_phase == phase)return;
	ms_instanse->m_phase = phase;
	ms_instanse->StartPhase();
}

EGamePhase Game::GetGamePhase()
{
	if (ms_instanse == nullptr) return EGamePhase::None;
	return ms_instanse->m_phase;
}
