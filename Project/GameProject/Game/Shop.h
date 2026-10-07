#pragma once
#include "../Base/Base.h"

class Shop : public Base {
	CImage m_img;
	CFont* m_font;
	int m_cnt;

public:
	Shop();
	void Update();
	void Draw();
};