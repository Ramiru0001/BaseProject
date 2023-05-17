#pragma once
#include "../Base/Base.h"

class Bullet : public Base{
private:
	//���f���I�u�W�F�N�g
	//CModelObj m_model;
	CVector3D m_vec;

	int m_life;
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="pos">�����ʒu</param>
	/// <param name="vec">�ړ��x�N�g��</param>
	Bullet(const CVector3D& pos, const CVector3D& vec);
	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();
	/// <summary>
	/// �`�揈��
	/// </summary>
	void Render();
};