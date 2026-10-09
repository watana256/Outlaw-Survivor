#pragma once
#include "Base/Base.h"
class Flamethrower :public Base {
private:
	CImage m_img;
	float m_attak_cnt;
	bool m_is_enable;
public:
	Flamethrower();
	void SetEnable(bool enable);
	void Update();
	void Draw();
	void Collision(Base* b);

};