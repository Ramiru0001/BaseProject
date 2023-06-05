#include "Enemy.h"

Enemy::Enemy(const CVector3D& pos) 
:Base(BaseType::eEnemy)
{
	m_model = COPY_RESOURCE("Golem", CModelA3M);
	m_pos = pos;
	m_rad = 0.8f;
}

void Enemy::Update()
{

	const float move_speed = 0.02f;
	//リストからプレイヤーを探索
	if(Base* p = Base::FindObject(BaseType::ePlayer)){
		//ターゲットへのベクトル
		CVector3D vec = p->m_pos - m_pos;
		//ベクトルから回転値を逆算
		m_rot_target.y = atan2(vec.x, vec.z);
		//回転値から方向ベクトルを計算
		CVector3D dir(CVector3D(sin(m_rot.y), 0, cos(m_rot.y)));
		//ターゲットへの距離が1.0fより遠ければ
		if (vec.Length() > 1.0f) {
			//前進
			m_pos += dir * move_speed;
			//前進アニメーション
			m_model.ChangeAnimation(1);
		} else {
			//待機アニメーション
			m_model.ChangeAnimation(0);
		}
	} else {
		//待機アニメーション
		m_model.ChangeAnimation(0);
	}
	//アニメーション更新
	m_model.UpdateAnimation();

	float a = Utility::NormalizeAngle(m_rot_target.y - m_rot.y);
	//割合による補間
	m_rot.y += a * 0.1f;
	m_rot.y = Utility::NormalizeAngle(m_rot.y);

	m_vec.y -= GRAVITY;
	m_pos.y += m_vec.y;

	//カプセル更新
	float height = 2.0f;
	m_lineS = m_pos + CVector3D(0, height - m_rad, 0);
	m_lineE = m_pos + CVector3D(0, m_rad, 0);
}

void Enemy::Render()
{
	m_model.SetPos(m_pos);
	m_model.SetRot(m_rot);
	m_model.SetScale(0.01f, 0.01f, 0.01f);
	m_model.Render();
}

void Enemy::Collision(Base* b)
{
	switch (b->GetType()) {
	case BaseType::eField:
	{
		CVector3D v(0, 0, 0);
		//カプセルとモデルの衝突
		auto tri = b->GetModel()->CollisionCupsel(m_lineS,m_lineE,m_rad);
		//接触した面の数繰り返す
		for (auto& t : tri) {
			if (t.m_normal.y < -0.5f) {
				//面が下向き→天井に当たった	
				//上昇速度を0に戻す
				if (m_vec.y > 0)
					m_vec.y = 0;
			}
			else if (t.m_normal.y > 0.5f) {
				//面が上向き→地面に当たった
				//重力落下速度を0に戻す
				if (m_vec.y < 0)
					m_vec.y = 0;
			}
			float max_y = max(t.m_vertex[0].y, max(t.m_vertex[1].y, t.m_vertex[2].y));
			//接触した面の方向へ、めり込んだ分押し戻す
			CVector3D nv = t.m_normal * (m_rad - t.m_dist);
			//最も大きな移動量を求める
			v.y = fabs(v.y) > fabs(nv.y) ? v.y : nv.y;
			v.x = fabs(v.x) > fabs(nv.x) ? v.x : nv.x;
			v.z = fabs(v.z) > fabs(nv.z) ? v.z : nv.z;
		}
		m_pos += v;
		m_lineS += v;
		m_lineE += v;
	}
	break;
	case BaseType::eEnemy:
	{
		//敵同士の判定
		CVector3D c1, dir1, c2, dir2;
		float dist;
		if (CCollision::CollisionCapsule(m_lineS, m_lineE, m_rad,
										b->m_lineS, b->m_lineE, b->m_rad,
										&dist, &c1, &dir1, &c2, &dir2)) {
			float s = (m_rad + b->m_rad) - dist;
			b->m_pos += dir1 * s * 0.5f;
			b->m_lineS += dir1 * s * 0.5f;
			b->m_lineE += dir1 * s * 0.5f;
			m_pos += dir2 * s * 0.5f;
			m_lineS += dir2 * s * 0.5f;
			m_lineE += dir2 * s * 0.5f;

		}
	}
	break;
	}
}
