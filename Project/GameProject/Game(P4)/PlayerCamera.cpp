#include "PlayerCamera.h"
PlayerCamera::PlayerCamera():Base(BaseType::eCamera){
	m_rot = CVector3D(0, 0, 0);
	m_pos = CVector3D(0, 0, 0);
}
void PlayerCamera::Update() {
	//カメラの回転値を取得
	//cam_ang = m_rot.y;
	////方向キーのベクトル
	//CVector3D key_dir = CVector3D(0, 0, 0);
	//if (HOLD(CInput::eUp))key_dir.z = 1;
	//if (HOLD(CInput::eDown))key_dir.z = -1;
	//if (HOLD(CInput::eLeft))key_dir.x = 1;
	//if (HOLD(CInput::eRight))key_dir.x = -1;
	////入力があれば。
	//if (key_dir.LengthSq() > 0) {
	//	if (key_dir.LengthSq() > 0) {
	//		CVector3D dir = CMatrix::MRotationY(m_rot.y) * key_dir;
	//	}
	//}
	CVector2D mouse_vec = CInput::GetMouseVec();
	m_rot += CVector3D(mouse_vec.y, -mouse_vec.x, 0) * cam_speed;
	//肩越しカメラ
	//プレイヤーの肩の位置に。カメラの角度で配置
	{
		Base* Player = Base::FindObject(BaseType::ePlayer);
		p_pos = Player->m_pos;
		//キャラクターの右肩腰にカメラを設置する
		CMatrix cam_matrix = CMatrix::MTranselate(p_pos)
			* CMatrix::MTranselate(CVector3D(0, 1.5, 0))
			* CMatrix::MRotation(m_rot)
			* CMatrix::MTranselate(CVector3D(-0.5, 0.2, -2));
		//カメラ位置を行列で指定
		CCamera::GetCurrent()->SetTranseRot(cam_matrix);
	}
}