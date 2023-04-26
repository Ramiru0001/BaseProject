#include "FollowCamera.h"
FollowCamera::FollowCamera() :Base(BaseType::eCamera) {
	//カメラの初期座標を設定
	m_pos = CVector3D(-10, 10, 10);
	m_rot = CVector3D(DtoR(30), DtoR(125), 0);
	CCamera::GetCurrent()->Perspective(fov, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, z_near, z_far);
}
void FollowCamera::Update() {
	if (Base* p = Base::FindObject(BaseType::ePlayer)) {
		//プレイヤーの座標を入手
		p_pos = p->m_pos;
	}
	//追従カメラ
	//マウスの移動量
	mouse_vec = CInput::GetMouseVec();
	//マウス移動からカメラを回転
	//マウス移動Y→X軸回転（上下）
	//マウス移動X→Y軸回転（左右）
	m_rot += CVector3D(mouse_vec.y, -mouse_vec.x, 0) * cam_speed;
	//上下角制限
	m_rot.x = min(DtoR(60), max(DtoR(-35), m_rot.x));
	//-180度~180度に正規化
	m_rot.y = Utility::NormalizeAngle(m_rot.y);
	//移動制限
	cam_dist = min(10.0f, max(2.0f, cam_dist + CInput::GetMouseWheel()));
	//注視点を設定
	cam_at = p_pos + CVector3D(0, 1.5, 0);
	//位置設定
	m_pos = cam_at + CMatrix::MRotation(m_rot).GetFront() * -cam_dist;
	//位置、注視点、上方向のベクトルを設定
	CCamera::GetCurrent()->LookAt(m_pos, cam_at, CVector3D(0, 1, 0));
	//位置と回転値
	CCamera::GetCurrent()->SetTranseRot(m_pos, m_rot);
}