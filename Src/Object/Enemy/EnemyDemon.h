
#pragma once

#include "EnemyBase.h"

class EnemyDemon : public EnemyBase
{

public:
	// �R���X�g���N�^
	EnemyDemon(void);
	// �f�X�g���N�^
	~EnemyDemon(void);

private:
	// �p�����[�^�ݒ�
	void SetParam(void) override;

};