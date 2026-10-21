#pragma once
#include "../Base/Base.h"

class Equipment : public Base {
	CImage m_img;
	CFont* m_font;
	bool m_is_show;


public:
	Equipment();
	bool IsShow() const;
	void Show();
	void Update();
	void Draw();
};