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
public:
	Player(CVector3D pos);
	void Update();
	void Render();
};