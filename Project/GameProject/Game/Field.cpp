#include"Field.h"
#include"../Base/Base.h"
Field::Field() :Base(eType_Field)
{
	m_img = COPY_RESOURCE("Field", CImage);

	//位置設定
	m_img.SetPos(0,0);
	//画像のサイズ設定
	m_img.SetSize(1920,1080);
	//描画

}
void Field::Draw()
{
	m_img.Draw();
}