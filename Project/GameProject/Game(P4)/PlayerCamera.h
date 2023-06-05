#pragma once
#include "../Base/Base.h"
class PlayerCamera :public Base {
private:
	CVector3D p_pos;
	float cam_ang = 0;//ƒJƒƒ‰‚Ì‰ñ“]’l
	float cam_speed = 0.003f;
public:
	PlayerCamera();
	void Update();
};