#pragma once
#include "../Base/Base.h"

enum eMoneyType {
	eGold,
	eSilver
};

class Money : public Base {
private:
	enum
	{
		eState_Gold,
		eState_Silver
	};
	int m_state;
	CImage m_img;
	int     m_money_type;
	int m_score;
	enum
	{
		eAnimGold,
		eAnimSilver
	};
	void eStateGold();
	void eStateSilver();
public:
	Money(const CVector2D& pos, int money_type);
	void Update();
	void Draw();
	void Collision(Base* b);
	static TexAnimData _anim_data[];
};