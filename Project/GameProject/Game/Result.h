#pragma once
#include"../Base/Base.h"

class Result :public Base {
private:
	CImage m_img;
	CFont* m_font = nullptr;
	int m_cnt;
	int m_score = 0;
public:
	Result();
	void Update();
	void Draw();
	//float GetGroundY() {}
};