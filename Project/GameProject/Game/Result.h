#pragma once
#include"../Base/Base.h"

class Result :public Base {
private:
	CImage m_img;
	int m_cnt;
public:
	Result();
	void Update();
	void Draw();
	float GetGroundY() {
	}
};