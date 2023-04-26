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
	if (HOLD(CInput::eLeft)) {

	}
}

void Player::Render() {
	m_model.SetScale(0.01, 0.01, 0.01);
	m_model.SetPos(m_pos);
	m_model.SetRot(m_rot);
	m_model.Render();
}