#include "Player.h"

Player::Player(const CVector3D& pos) :Base(BaseType::ePlayer)
{
	m_model = COPY_RESOURCE("Antman", CModelA3M);
	m_pos = pos;
}

void Player::Update()
{
	//移動速度
	float move_speed = 0.05f;
	//回転速度
	float rot_speed = DtoR(5.0f);
	//AorDキーを押した場合のキャラクターの回転の処理
	if (HOLD(CInput::eLeft)) {
		m_rot.y +=  rot_speed;
	}
	if (HOLD(CInput::eRight)) {
		m_rot.y -= rot_speed;
	}
	//wキーを押した場合の処理
	if (HOLD(CInput::eUp)) {
		CVector3D dir (sin(m_rot.y), 0, cos(m_rot.y));
		m_pos += dir * move_speed;
		m_model.ChangeAnimation(1);
	}
	else {
		m_model.ChangeAnimation(0);
	}
}

void Player::Render()
{
	m_model.UpdateAnimation();
	m_model.SetScale(0.01, 0.01, 0.01);
	m_model.SetPos(m_pos);
	m_model.SetRot(m_rot);
	m_model.Render();
}
