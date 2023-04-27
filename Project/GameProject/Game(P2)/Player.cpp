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
	if (Base* b = Base::FindObject(BaseType::eCamera)) {
		cam_ang = b->m_rot.y;
	}
	//ベクトルを期化
	key_dir = CVector3D(0, 0, 0);
	//ゲームパッド0番があれば
	if (CInput::GetPadData(0)) {
		//左スティックの状態を取得
		CVector2D axis = CInput::GetLStick(0);
		//キャラクター移動キーに調整
		key_dir = CVector3D(-axis.x, 0, axis.y);
	}
	//キーボード入力の場合
	if (HOLD(CInput::eLeft)) {
		key_dir.x = 1;
	}
	if (HOLD(CInput::eRight)) {
		key_dir.x = -1;
	}
	if (HOLD(CInput::eUp)) {
		key_dir.z = 1;
	}
	if (HOLD(CInput::eDown)) {
		key_dir.z = -1;
	}
	//入力があれば
	if (key_dir.LengthSq() > 0.1) {
		//キーの方向ベクトルを角度に逆算
		key_ang = atan2(key_dir.x, key_dir.z);
		//カメラの角度＋キーの角度でキャラクターの角度を決める
		m_rot.y = Utility::NormalizeAngle(cam_ang + key_ang);
		//方向ベクトル
		m_dir = CVector3D(sin(m_rot.y), 0, cos(m_rot.y));
		//移動
		m_pos += m_dir * move_speed;
		m_model.ChangeAnimation(1);
	}
	else {
		m_model.ChangeAnimation(0);
	}
	//コントローラーの場合
	//if()
}

void Player::Render() {
	m_model.UpdateAnimation();
	m_model.SetScale(0.01, 0.01, 0.01);
	m_model.SetPos(m_pos);
	m_model.SetRot(m_rot);
	m_model.Render();
}