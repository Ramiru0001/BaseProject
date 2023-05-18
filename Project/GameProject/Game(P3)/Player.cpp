#include "Player.h"
#include "Bullet.h"
#include <iostream>
Player::Player(CVector3D& pos):Base(BaseType::ePlayer) {
	model = COPY_RESOURCE("Antman", CModelA3M);
	m_rot = CVector3D(0, 0, 0);
	m_pos = pos;
}
void Player::Render() {
	model.UpdateAnimation();
	model.SetScale(0.01, 0.01, 0.01);
	model.SetPos(m_pos);
	model.SetRot(m_rot);
	model.Render();
}
void Player::Update() {
	//std::cout << "Player _pos x:" << m_pos.x << " z:" << m_pos.z << std::endl;
	Move();
	if(Base* p = Base::FindObject(BaseType::eCamera)){
		cam_rot = p->m_rot;
		//std::cout << "cam_rot y:" << cam_rot.y << std::endl;
	};
	//�L�����N�^�[�̉�]�l���J�����̉�]�l�ɍ��킹��
	m_rot.y = cam_rot.y;
}
void Player::Move() {
	//WASD�ŕ��s�ړ�
	//W���������ꍇ
	//���͕����x�N�g���̐ݒ�
	key_dir = CVector3D(0, 0, 0);
	if (HOLD(CInput::eUp)) {
		key_dir.z = 1;
	}
	if (HOLD(CInput::eDown)) {
		key_dir.z = -1;
	}
	if (HOLD(CInput::eLeft)) {
		key_dir.x = 1;
	}
	if (HOLD(CInput::eRight)) {
		key_dir.x = -1;
	}
	//�L�[���͂������
	if (key_dir.LengthSq() > 0) {
		//�ړ��������L�[���̓x�N�g��
		CVector3D dir = CMatrix::MRotationY(m_rot.y) * key_dir;
		m_pos += dir * move_speed;
	}
}
