#include "Bullet.h"

Bullet::Bullet(const CVector2D& pos) :Base(eType_Bullet)
{
    m_img.Load("Image/Bullet.png");
    m_pos = pos;
    m_rad = 16;
    m_img.SetSize(32, 32);
    m_img.SetCenter(16, 16);

}
void Bullet::Update()
{

}
void Bullet::Draw()
{

}
void Bullet::Collision(Base* b)
{

}