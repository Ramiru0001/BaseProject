#include "Player.h"
Player::Player(const CVector3D pos) :Base(BaseType::ePlayer) {
	m_pos = pos;
	m_model = COPY_RESOURCE("Antman", CModelA3M);
}
void Player::Update() {
	//�L�[���͂�����Ă���ꍇ�A
	//�܂��A��ʂ̌����Ă�����������
	//�L�����N�^�[���i�ޕ��������
	//�L�����N�^�[�̕�����ς��A�i�܂���A�A�j���[�V�����ύX������B
	if (Base* b = Base::FindObject(BaseType::eCamera)) {
		cam_ang = b->m_rot.y;
	}
	//�x�N�g��������
	key_dir = CVector3D(0, 0, 0);
	//�Q�[���p�b�h0�Ԃ������
	if (CInput::GetPadData(0)) {
		//���X�e�B�b�N�̏�Ԃ��擾
		CVector2D axis = CInput::GetLStick(0);
		//�L�����N�^�[�ړ��L�[�ɒ���
		key_dir = CVector3D(-axis.x, 0, axis.y);
	}
	//�L�[�{�[�h���͂̏ꍇ
	if (HOLD(CInput::eLeft)) {
		key_dir.x = 1;
	}
	if (HOLD(CInput::eRight)) {
		key_dir.x = -1;
	}
	if (HOLD(CInput::eUp)) {
		key_dir.z = 1;
	}
	if (HOLD(CInput::eDown)) {
		key_dir.z = -1;
	}
	//���͂������
	if (key_dir.LengthSq() > 0.1) {
		//�L�[�̕����x�N�g�����p�x�ɋt�Z
		key_ang = atan2(key_dir.x, key_dir.z);
		//�J�����̊p�x�{�L�[�̊p�x�ŃL�����N�^�[�̊p�x�����߂�
		m_rot.y = Utility::NormalizeAngle(cam_ang + key_ang);
		//�����x�N�g��
		m_dir = CVector3D(sin(m_rot.y), 0, cos(m_rot.y));
		//�ړ�
		m_pos += m_dir * move_speed;
		m_model.ChangeAnimation(1);
	}
	else {
		m_model.ChangeAnimation(0);
	}
	//�R���g���[���[�̏ꍇ
	//if()
}

void Player::Render() {
	m_model.UpdateAnimation();
	m_model.SetScale(0.01, 0.01, 0.01);
	m_model.SetPos(m_pos);
	m_model.SetRot(m_rot);
	m_model.Render();
}