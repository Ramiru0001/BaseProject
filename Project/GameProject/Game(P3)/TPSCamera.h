#pragma once
#include "../Base/Base.h"
class Player;
class TPSCamera : public Base {
private:
	CMatrix cam_matrix;
	CVector3D p_pos;//プレイヤー座標
	CVector2D mouse_vec;//マウス移動ベクトル
	float cam_speed = 0.005f;//回転速度
public:
	TPSCamera();
	void Update();
	void Render();
};