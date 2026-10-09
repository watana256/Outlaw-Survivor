#pragma once
#include "Base/Base.h"
class Landmine :public Base {
private:
	CImage m_img;
	float m_attak_cnt;
	bool m_is_enable;
public:
	Landmine();
	void SetEnable(bool enable);
	void Update();
	void Draw();
	void Collision(Base* b);

};