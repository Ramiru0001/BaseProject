#pragma once
#include "../Base/Base.h"
class Player : public Base {
private:
	CModelA3M model;
	//CVector3D m_pos;
	//CVector3D m_rot = CVector3D(0, 0, 0);
	//CVector3D m_vec;
	CVector3D cam_rot;
	CVector3D key_dir;//入力ベクトル
	float move_speed = 0.05f;;
public:
	Player(CVector3D &pos);
	void Update();
	void Render();
	//キーが押されたら動く処理
	void Move();
};