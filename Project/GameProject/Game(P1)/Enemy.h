#pragma once
#include "../Base/Base.h"

class Enemy : public Base{
private:
	//���f���I�u�W�F�N�g
	CModelA3M m_model;
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="pos">�o���ʒu</param>
	Enemy(const CVector3D& pos);
	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();
	/// <summary>
	/// �`�揈��
	/// </summary>
	void Render();
};