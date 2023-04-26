#pragma once
#include "../Base/Base.h"
class Player :public Base {
public:
	//プレイヤーキャラクター
	CModelA3M m_model;
	//プレイヤーの向いている角度
	CVector3D m_ang;
	//プレイヤーの移動方向ベクトル
	CVector3D m_vec;
	//方向キーのベクトル
	CVector3D key_dir;
	//カメラの回転値
	float cam_ang = 0;
	//入力しているキーの角度
	float key_ang;
	//移動速度
	float move_speed = 0.05f;
public:
	Player(CVector3D pos);
	void Update();
};