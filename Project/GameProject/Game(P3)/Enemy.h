#pragma once
#include "../Base/Base.h"
class Enemy : public Base{
private:
	CModelA3M model;
	CVector3D m_pos;
	CVector3D m_rot = CVector3D(0, 0, 0);
	CVector3D m_vec;
	float move_speed = 0.05f;
public:
	Enemy(CVector3D &pos);
	void Update();
	void Render();
};