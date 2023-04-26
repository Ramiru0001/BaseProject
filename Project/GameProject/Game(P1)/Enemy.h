#pragma once
#include "../Base/Base.h"

class Enemy : public Base{
private:
	//モデルオブジェクト
	CModelA3M m_model;
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
};