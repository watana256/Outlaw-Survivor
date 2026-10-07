#pragma once
#include"../Base/Base.h"

class Title :public Base {
private:
	CImage m_img;
	CImage m_Logo;
	int m_cnt;
public:
	Title();
	void Update();
	void Draw();
	float GetGroundY() {
	}
};