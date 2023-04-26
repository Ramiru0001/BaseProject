#include "Enemy.h"

Enemy::Enemy(const CVector3D& pos) 
:Base(BaseType::eEnemy)
{
	m_model = COPY_RESOURCE("Golem", CModelA3M);
	m_pos = pos;
}

void Enemy::Update()
{
	const float move_speed = 0.03f;
	//リストからプレイヤーを探索
	if(Base* p = Base::FindObject(BaseType::ePlayer)){
		//プレイヤーへのベクトルを入手
		m_vec = p->m_pos - m_pos;
		//プレイヤーの方へ向く
		m_rot.y = atan2(m_vec.x, m_vec.z);
		//一定の距離より遠い場合
		if (m_vec.Length() > 1.5) {
			//プレイヤーを追いかける
			CVector3D dir(sin(m_rot.y), 0, cos(m_rot.y));
			m_pos += dir * move_speed;
			m_model.ChangeAnimation(1);
		}
		else {
			m_model.ChangeAnimation(0);
		}
	}
}
void Enemy::Render()
{
	m_model.UpdateAnimation();
	m_model.SetScale(0.01, 0.01, 0.01);
	m_model.SetPos(m_pos);
	m_model.SetRot(m_rot);
	m_model.Render();
}
