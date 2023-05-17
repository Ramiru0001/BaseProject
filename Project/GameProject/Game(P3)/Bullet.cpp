#include "Bullet.h"

Bullet::Bullet(const CVector3D& pos, const CVector3D& vec)
:Base(BaseType::eBullet)
{
	m_pos = pos;
	m_vec = vec;
	m_life = 60;
}

void Bullet::Update()
{

	
	m_pos += m_vec;

	if (m_life-- < 0) SetKill();
}

void Bullet::Render()
{
	
	Utility::DrawSphere(m_pos, 0.02f, CVector4D(1, 0, 0, 1));
}
