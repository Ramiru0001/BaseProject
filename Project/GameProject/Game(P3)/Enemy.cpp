#include "Enemy.h"
Enemy::Enemy(CVector3D &pos):Base(BaseType::eEnemy) {
	//���f���ǂݍ���
	model = COPY_RESOURCE("Golem", CModelA3M);
	m_pos = pos;
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

}
