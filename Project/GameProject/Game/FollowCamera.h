#pragma once
#include "../Base/Base.h"
class FollowCamera :public Base {
private:
	//マウスの移動量
	CVector2D mouse_vec;
	//プレイヤーの座標
	CVector3D p_pos;
	CVector3D cam_at;
	//カメラの位置
	CVector3D cam_pos = CVector3D(-10, 10, 10);;
	//カメラ回転値
	CVector3D cam_rot = CVector3D(DtoR(30), DtoR(125), 0);
	//画角
	float fov=DtoR(60.0f);
	//画角や描画距離を変更
	float z_near = 1.0f;
	float z_far = 1000.0f;
	//注視点までの距離。変更したら初期の視点場所が変わる
	float cam_dist = 6.0f;
	//回転速度係数
	float cam_speed = 0.002f;
	void Update();
public:
	FollowCamera();
};