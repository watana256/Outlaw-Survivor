#pragma once
#include "../Base/Base.h"

class Player :public Base
{
private:
	enum {
		eState_Walk_right,
		eState_Walk_left,
		eState_Walk_up,
		eState_Walk_down,
		eState_Damage_right,
		eState_Damage_left,
		eState_Damage_up,
		eState_Damage_down,
		eState_Death
	};
	CImage m_img;
	int m_state;
	int m_speed_cnt;
	enum {
		eAnim_Walk_right,
		eAnim_Walk_left,
		eAnim_Walk_up,
		eAnim_Walk_down,
		eAnim_Damage_right,
		eAnim_Damage_left,
		eAnim_Damage_up,
		eAnim_Damage_down,
		eAnim_Death
	};
	void State_Walk_right();
	void State_Walk_left();
	void State_Walk_up();
	void State_Walk_down();
	void State_Death();
	void Run();


public:
	Player(const CVector2D& pos);
	static TexAnimData _anim_data[];
	void Update();
	void Draw();
	void TakeDamage(int damage);
};