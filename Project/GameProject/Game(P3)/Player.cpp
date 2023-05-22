#include "Player.h"
#include "Bullet.h"
#include <iostream>
Player::Player(CVector3D& pos):Base(BaseType::ePlayer) {
	model = COPY_RESOURCE("Antman", CModelA3M);
	gun_model= COPY_RESOURCE("Scar", CModelObj);
	m_rot = CVector3D(0, 0, 0);
	m_pos = pos;
}
void Player::Render() {
	model.UpdateAnimation();
	model.SetScale(0.01, 0.01, 0.01);
	model.SetPos(m_pos);
	model.SetRot(m_rot);
	model.Render();
	//�s����w�肵�ĕ`��
	gun_model.Render(gun_matrix);
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
	//���̃{�[���𕔕��I�ɓ�����
	model.BindFrameMatrix(5, CMatrix::MRotation(cam_rot));
	//����̍s��@��̃{�[���s����擾����
	gun_matrix = model.GetFrameMatrix(31)*
	CMatrix::MTranselate(-18.00f,-5.00f,8.600f)*
	CMatrix::MRotationX(DtoR(0))*
	CMatrix::MRotationY(DtoR(-90))*
	CMatrix::MRotationZ(DtoR(-90))*
	CMatrix::MScale(1.0f,1.0f,1.0f);
	//���N���b�N������e�őł�
	bullet_pos = gun_matrix * CVector4D(0, 0, 40.0f, 1);
	CVector3D dir = CMatrix::MRotation(cam_rot).GetFront();
	if (PUSH(CInput::eMouseL)) {
		
		Base::Add(new Bullet(bullet_pos, dir * bullet_speed));
	}
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
		model.ChangeAnimation(1);
	}
	else {
		model.ChangeAnimation(0);
	}
}
