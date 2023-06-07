#pragma once
#include "../Base/Base.h"
class Player :public Base {
private:
	CVector3D cam_rot=CVector3D(0, 0, 0);
	CModelA3M m_model;
	float move_speed=0.1f;
public:
	Player(const CVector3D &pos);
	void Update();
	void Render();
	void PushToMove();
	void Collision(Base* b);
};
