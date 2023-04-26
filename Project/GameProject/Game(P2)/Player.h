#pragma once
#include "../Base/Base.h"
class Player :public Base {
public:
	//�v���C���[�L�����N�^�[
	CModelA3M m_model;
	//�v���C���[�̌����Ă���p�x
	CVector3D m_ang;
	//�v���C���[�̈ړ������x�N�g��
	CVector3D m_vec;
	//�����L�[�̃x�N�g��
	CVector3D key_dir;
	//�J�����̉�]�l
	float cam_ang = 0;
	//���͂��Ă���L�[�̊p�x
	float key_ang;
	//�ړ����x
	float move_speed = 0.05f;
public:
	Player(CVector3D pos);
	void Update();
};