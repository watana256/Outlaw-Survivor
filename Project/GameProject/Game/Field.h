#pragma once
#include"../Base/Base.h"

class Field :public Base {
private:
	CImage m_img;
public:
	Field();
	void Draw();
	float GetGroundY() {
	}
};