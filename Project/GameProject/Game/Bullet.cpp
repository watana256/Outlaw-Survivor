#include "Bullet.h"

#define  BULLET_SPEED 5.0f

Bullet::Bullet(const CVector2D& pos, const CVector2D& dir) :Base(eType_Bullet)
{
    m_img.Load("Image/Bullet.png");
    m_pos = pos;
    m_vec = dir;
    m_rect = CRect(-10, -10, 10, 10);
    m_img.SetSize(10, 10);
    m_img.SetCenter(10, 10);
    m_img.SetAng(atan2f(-m_vec.y, m_vec.x));

}
void Bullet::Update()
{
    const float move_speed = BULLET_SPEED;
    m_pos += m_vec * move_speed;
}
void Bullet::Draw()
{
    m_img.SetPos(m_pos);
    m_img.Draw();
    Utility::DrawCircle(m_pos, m_rad, CVector4D(0, 5, 0, 0.5));
}
void Bullet::Collision(Base* b)
{

}