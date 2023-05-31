#pragma once
#include "../Base/Base.h"
class Player : public Base {
private:
	CModelA3M model;
	//CVector3D m_pos;
	//CVector3D m_rot = CVector3D(0, 0, 0);
	//CVector3D m_vec;
	CVector3D cam_rot;
	CVector3D key_dir;//���̓x�N�g��
	CModelObj gun_model;
	CMatrix gun_matrix;
	float move_speed = 0.05f;;
	float bullet_speed = 1.0f;
	CVector3D bullet_pos;
public:
	Player(CVector3D &pos);
	void Update();
	void Render();
	//�L�[�������ꂽ�瓮������
	void Move();
	void static GetFrameMatrixHand(CMatrix* p);
};
class Gun {
private:
	CVector3D _pos;
	CVector3D m_rot;
public:
	Gun();
	void Update();
	void Render();
};