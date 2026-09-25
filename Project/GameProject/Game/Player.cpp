#include "Player.h"

static TexAnim _Walk_right[] =
{
	{2,2},
};
static TexAnim _Walk_left[] =
{
	{1,1},
};
static TexAnim _Walk_up[] =
{
	{1,1},
};
static TexAnim _Walk_down[] =
{
	{1,1},
};
static TexAnim _damage_right[] =
{
	{1,1},
};
static TexAnim _damage_left[] =
{
	{1,1},
};
static TexAnim _damage_up[] =
{
	{1,1},
};
static TexAnim _damage_down[] =
{
	{1,1},
};
static TexAnim _death[] =
{
	{1,1},
};

TexAnimData Player::_anim_data[] =
{
	ANIMDATA(_Walk_right),
	ANIMDATA(_Walk_left),
	ANIMDATA(_Walk_up),
	ANIMDATA(_Walk_down),
	ANIMDATA(_damage_right),
	ANIMDATA(_damage_left),
	ANIMDATA(_damage_up),
	ANIMDATA(_damage_down),
	ANIMDATA(_death),
};
Player::Player(const CVector2D& pos) 
	:Base(eType_Player)
{
	m_img = COPY_RESOURCE("Player", CImage);
	m_pos_old = m_pos = pos;
	m_img.SetSize(150, 150);
	m_img.SetCenter(40, 40);
	m_img.ChangeAnimation(eState_damage_up);
}
void Player::Update()
{
	m_img.UpdateAnimation();
	m_pos_old = m_pos;
}
void Player::Draw()
{
	m_img.SetPos(m_pos);
	m_img.Draw();
	m_img.SetRect(128, 0, 192, 64);
}
