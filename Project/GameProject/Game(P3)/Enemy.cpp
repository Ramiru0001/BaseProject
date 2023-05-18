#include "Enemy.h"
Enemy::Enemy(CVector3D &pos):Base(BaseType::eEnemy) {
	//モデル読み込み
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
	//プレイヤーを追いかける
	//待機と走るのモーション切り替え

}
