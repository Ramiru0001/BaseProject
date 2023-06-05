#pragma once
#include "../Base/Base.h"

class Stage : public Base{
private:
	//ステージモデル
	CModelObj m_model;
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Stage();
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();
	/// <summary>
	/// 描画処理
	/// </summary>
	void Render();
	/// <summary>
	/// オブジェクトが持つモデルの取得
	/// </summary>
	/// <returns></returns>
	CModel* GetModel() {
		return &m_model;
	}
};