#include "Player.h"
Player::Player(const CVector3D pos) :Base(BaseType::ePlayer) {
	m_pos = pos;
	m_model = COPY_RESOURCE("Antman", CModelA3M);
}
void Player::Update() {
	//キー入力がされている場合、
	//まず、画面の向いている方向を入手
	//キャラクターが進む方向を入手
	//キャラクターの方向を変え、進ませる、アニメーション変更をする。
	if (HOLD(CInput::eLeft)) {

	}
}

void Player::Render() {
	m_model.SetScale(0.01, 0.01, 0.01);
	m_model.SetPos(m_pos);
	m_model.SetRot(m_rot);
	m_model.Render();
}