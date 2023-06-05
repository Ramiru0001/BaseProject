#pragma once
#include "../Base/Base.h"

class Enemy : public Base{
private:
	//モデルオブジェクト
	CModelA3M m_model;
	CVector3D m_rot_target;
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pos">出現位置</param>
	Enemy(const CVector3D& pos);
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();
	/// <summary>
	/// 描画処理
	/// </summary>
	void Render();
	/// <summary>
	/// 当たり判定検証
	/// </summary>
	/// <param name="b">対象物</param>
	void Collision(Base* b);
};