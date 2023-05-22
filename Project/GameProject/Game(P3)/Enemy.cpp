#include "Enemy.h"
Enemy::Enemy(CVector3D &pos):Base(BaseType::eEnemy) {
	//モデル読み込み
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
	//プレイヤーを追いかける
	//待機と走るのモーション切り替え
	if (Base* p = Base::FindObject(BaseType::ePlayer)) {
		p_pos = p->m_pos;
		//std::cout << "cam_rot y:" << cam_rot.y << std::endl;
	};
	//プレイヤーまでのベクトルを求める
	m_vec = p_pos - m_pos;
	//ベクトルから回転値を逆算
	m_rot.y = atan2(m_vec.x, m_vec.z);
	if (m_vec.Length() > 1.5) {
		//正規化したベクトルに移動距離をかけて、座標に足す
		m_pos += m_vec.GetNormalize() * move_speed;
		model.ChangeAnimation(1);
	}
	else {
		model.ChangeAnimation(0);
	}
}
