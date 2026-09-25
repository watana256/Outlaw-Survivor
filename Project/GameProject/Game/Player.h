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
		eState_damage_right,
		eState_damage_left,
		eState_damage_up,
		eState_damage_down,
		eState_death
	};
	CImage m_img;

	enum {
		eAnim_Walk_right,
		eAnim_Walk_left,
		eAnim_Walk_up,
		eAnim_Walk_down,
		eAnim_damage_right,
		eAnim_damage_left,
		eAnim_damage_up,
		eAnim_damage_down,
		eAnim_death
	};
public:
	Player(const CVector2D& pos);
	static TexAnimData _anim_data[];
	void Update();
	void Draw();
	void TakeDamage(int damage);
};