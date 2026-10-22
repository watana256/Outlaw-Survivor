#pragma once
class EquipmentIcon {
	CImage m_img;
	CFont* m_font;
	bool m_is_show;


public:
	EquipmentIcon();
	bool IsShow() const;
	void Show();
	void Update();
	void Draw();
	void SetPos(const CVector2D& pos);
	const CVector2D& GetPos() const;
	void SetIndex(int index);
	CRect GetRect() const;
	void DrawAt(const CVector2D& pos);
	void SetColor(float r, float g, float b, float a);   
	void DrawFit(const CVector2D& pos, float max_w, float max_h);
};