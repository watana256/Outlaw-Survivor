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
#include "EnemySpawner.h"

Game* Game::ms_instanse = nullptr;



Game::Game() 
	:Base(eType_Scene)
	,m_phase(EGamePhase::None)
	, m_round(1)
	, m_elapsed_time(0.0f)
{
	ms_instanse = this;
	mp_equip = new Equipment();
	SetGamePhase(EGamePhase::Start);
	new HP();
	new Money(CVector2D(900, 400), eGold);
	new Score();
	new Player(CVector2D(950, 500));//125,165
	new UI;
	new Field;
	new Medical(CVector2D(500, 300), eBin);
	new EnemySpawner();

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
		m_elapsed_time = 0.0f;
		break;
	case EGamePhase::RoundPlaying:
		break;
	case EGamePhase::RoundOutro:
		m_elapsed_time = 0.0f;      
		break;
	case EGamePhase::GameClear:
		break;

	}
}
// ラウンド終了を通知する(EnemySpawnerなどから呼ぶ)
void Game::EndRound()
{
	if (ms_instanse == nullptr) return;
	// プレイ中のときだけ受け付ける(二重呼び出し防止)
	if (ms_instanse->m_phase != EGamePhase::RoundPlaying) return;
	SetGamePhase(EGamePhase::RoundOutro);
}
int Game::GetEquippedWeapon(int slot)
{
	if (ms_instanse == nullptr || ms_instanse->mp_equip == nullptr) return -1;
	return ms_instanse->mp_equip->GetEquipped(slot);
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
		// 2秒待ってから装備画面へ
		m_elapsed_time += CFPS::GetDeltaTime();
		if (m_elapsed_time >= 2.0f)
		{
			m_round++;
			SetGamePhase(EGamePhase::Equipment);
		}
		break;
	case EGamePhase::GameClear:
		break;
		
	}
		// プレイヤーが死亡したらGameOver
		if (!Base::FindObject(eType_Player))
		{
			new Result();
			return;
		}

	// ラスボスのタンクが撃破されたらゲームクリア
	if (m_isTankDefeated)
	{
		m_resultTimer += CFPS::GetDeltaTime();

		// 1秒後にリザルト画面を表示
		if (m_resultTimer >= 1.0f)
		{
			// 二重生成防止
			if (m_phase != EGamePhase::GameClear)
			{
				SetGamePhase(EGamePhase::GameClear);
				new Result();
			}

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
