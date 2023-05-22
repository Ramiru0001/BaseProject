#include "Enemy.h"
Enemy::Enemy(CVector3D &pos):Base(BaseType::eEnemy) {
	//���f���ǂݍ���
	model = COPY_RESOURCE("Golem", CModelA3M);
	m_pos = pos;
	m_rot = CVector3D(0, 0, 0);
}
void Enemy::Render() {
	model.UpdateAnimation();
	model.SetScale(0.01, 0.01, 0.01);
	model.SetPos(m_pos);
	model.SetRot(m_rot);
	model.Render();
}
void Enemy::Update() {
	//�v���C���[��ǂ�������
	//�ҋ@�Ƒ���̃��[�V�����؂�ւ�
	if (Base* p = Base::FindObject(BaseType::ePlayer)) {
		p_pos = p->m_pos;
		//std::cout << "cam_rot y:" << cam_rot.y << std::endl;
	};
	//�v���C���[�܂ł̃x�N�g�������߂�
	m_vec = p_pos - m_pos;
	//�x�N�g�������]�l���t�Z
	m_rot.y = atan2(m_vec.x, m_vec.z);
	if (m_vec.Length() > 1.5) {
		//���K�������x�N�g���Ɉړ������������āA���W�ɑ���
		m_pos += m_vec.GetNormalize() * move_speed;
		model.ChangeAnimation(1);
	}
	else {
		model.ChangeAnimation(0);
	}
}
