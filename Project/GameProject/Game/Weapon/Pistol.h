#pragma once
#include "Base/Base.h"
class Pistil :public Base {
private:
	CImage m_img;
	
public:
	Pistil(const CVector2D& pos);
	void Update();
	void Draw();
	void Collision(Base* b);

};