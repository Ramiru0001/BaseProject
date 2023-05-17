#pragma once
#include "../Base/Base.h"

class Bullet : public Base{
private:
	//モデルオブジェクト
	//CModelObj m_model;
	CVector3D m_vec;

	int m_life;
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pos">発生位置</param>
	/// <param name="vec">移動ベクトル</param>
	Bullet(const CVector3D& pos, const CVector3D& vec);
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();
	/// <summary>
	/// 描画処理
	/// </summary>
	void Render();
};