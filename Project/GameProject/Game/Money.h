#pragma once
#include "../Base/Base.h"
class Money : public Base {
private:
	enum
	{
		eState_Gold,
		eState_Silver
	};
	int m_state;
	CImage m_img;
	enum
	{
		eAnimGold,
		eAnimSilver
	};
	void eStateGold();
	void eStateSilver();
public:
	Money(const CVector2D& pos);
	void Update();
	void Draw();
	void Collision(Base* b);
	static TexAnimData _anim_data[];
};