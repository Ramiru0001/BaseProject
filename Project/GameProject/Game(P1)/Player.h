#pragma once
#include "../Base/Base.h"
class Player : public Base {
private:
	//モデルオブジェクト
	CModelA3M m_model;
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pos">出現位置</param>
	Player(const CVector3D & pos);
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();
	/// <summary>
	/// 描画処理
	/// </summary>
	void Render();
};