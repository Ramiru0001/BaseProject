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
public:
	Player(CVector3D pos);
	void Update();
	void Render();
};