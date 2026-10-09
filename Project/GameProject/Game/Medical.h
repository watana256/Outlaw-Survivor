#pragma once
#include "../Base/Base.h"

enum eMedicalType {
	eKit,
	eBin
};

class Medical : public Base {
private:
	enum
	{
		eState_Kit,
		eState_Bin
	};
	int m_Medical;
	CImage m_img;
	int     m_Medical_type;
	int s_Medical;
	enum
	{
		eAnimKit,
		eAnimBin
	};
	void eStateKit();
	void eStateBin();
public:
	Medical(const CVector2D& pos, int Medical_type);
	void Update();
	void Draw();
	void Collision(Base* b);
	static TexAnimData _anim_data[];
};