#pragma once
#include "../Base/Base.h"

class Player :public Base
{
private:


public:
	Player(const CVector2D& pos);
	static TexAnimData _anim_data[];
	void Update();
	void Draw();
};