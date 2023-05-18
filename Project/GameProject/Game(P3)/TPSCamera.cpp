#include "TPSCamera.h"
#include "Player.h"
#include "../Base/Base.h"
#include <iostream>
TPSCamera::TPSCamera():Base(BaseType::eCamera) {
	m_rot = CVector3D(DtoR(30), DtoR(125), 0);
}
void TPSCamera::Update() {
	if (Base* p = Base::FindObject(BaseType::ePlayer)) {
		//�v���C���[�̍��W�����
		p_pos = p->m_pos;
	}
	//�L�����N�^�[�̌������z���ɃJ������ݒu
	cam_matrix = CMatrix::MTranselate(p_pos) * CMatrix::MTranselate(CVector3D(0, 1.5, 0)) * CMatrix::MRotation(m_rot) * CMatrix::MTranselate(CVector3D(-0.5, 0.2, -2));
	mouse_vec = CInput::GetMouseVec();
	m_rot += CVector3D(mouse_vec.y, -mouse_vec.x, 0) * cam_speed;
	m_rot.x = min(DtoR(45), max(DtoR(-45), m_rot.x));
}
void TPSCamera::Render() {
	//�J�����ʒu���s��Ŏw��
	//std::cout << "Camera Render" << std::endl;
	CCamera::GetCurrent()->SetTranseRot(cam_matrix);
}