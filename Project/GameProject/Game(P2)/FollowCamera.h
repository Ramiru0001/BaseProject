#pragma once
#include "../Base/Base.h"
class FollowCamera :public Base {
public:
	CVector2D mouse_vec;//マウスの移動量
	CVector3D cam_pos;//座標
	CVector3D p_pos;//プレイヤーの座標
	CVector3D cam_rot;//回転値
	CVector3D cam_at;//注視点
	float fov = DtoR(60.0f);//視野角
	float z_near = 1.0f;//表示される最短距離
	float z_far = 15.0f;//表示される最長距離
	float cam_dist = 10.0f;//注視点までの距離。初期値
	float cam_speed = 0.005f;//回転速度係数
public:
	FollowCamera();
	void Update();
};
