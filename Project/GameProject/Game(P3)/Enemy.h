#pragma once
#include "../Base/Base.h"
class Enemy : public Base{
private:
	CModelA3M model;
	CVector3D p_pos;
	float move_speed = 0.02f;
public:
	Enemy(CVector3D &pos);
	void Update();
	void Render();
};