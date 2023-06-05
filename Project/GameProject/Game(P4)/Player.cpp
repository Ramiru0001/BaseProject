#include "Player.h"
Player::Player(const CVector3D &pos) :Base(BaseType::ePlayer) {
	m_model = COPY_RESOURCE("Girl", CModelA3M);
	m_pos = pos;
	m_rot = CVector3D(0, 0, 0);
	m_vec = CVector3D(0, 0, 0);
}
void Player::Update() {
	Base *Camera = Base::FindObject(BaseType::eCamera);
	cam_rot = Camera->m_rot;
	CVector3D key_dir = CVector3D(0, 0, 0);
	if (HOLD(CInput::eUp))key_dir.z = 1;
	if (HOLD(CInput::eDown))key_dir.z = -1;
	if (HOLD(CInput::eLeft))key_dir.x = 1;
	if (HOLD(CInput::eRight))key_dir.x = -1;
	if (key_dir.LengthSq() > 0) {
		CVector3D dir = CMatrix::MRotationY(cam_rot.y) * key_dir;
		m_pos += dir * move_speed;
		m_model.ChangeAnimation(1);
	}
	else {
		m_model.ChangeAnimation(0);
	}
}
void Player::Render() {
	m_model.UpdateAnimation();
	m_model.SetPos(m_pos);
	m_model.SetRot(0,cam_rot.y,0);
	m_model.SetScale(0.01f,0.01f,0.01f);
	m_model.Render();
}