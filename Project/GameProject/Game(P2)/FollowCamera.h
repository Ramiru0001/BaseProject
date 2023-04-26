#pragma once
#include "../Base/Base.h"
class FollowCamera :public Base {
public:
	CVector2D mouse_vec;//�}�E�X�̈ړ���
	CVector3D p_pos;//�v���C���[�̍��W
	CVector3D cam_at;//�����_
	float fov = DtoR(60.0f);//����p
	float z_near = 1.0f;//�\�������ŒZ����
	float z_far = 1000.0f;//�\�������Œ�����
	float cam_dist = 10.0f;//�����_�܂ł̋����B�����l
	float cam_speed = 0.005f;//��]���x�W��
public:
	FollowCamera();
	void Update();
};
