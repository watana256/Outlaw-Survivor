#pragma once
#include "Base/Base.h"
class Shotgun :public Base {
private:
	CImage m_img;
	float m_attak_cnt;
	bool m_is_enable;

	void Shot(const CVector2D& pos, const CVector2D& dir);
	int GetDamage() const;
public:
	Shotgun();
	void SetEnable(bool enable);
	void Update();
	void Draw();
	void Collision(Base* b);

};