
#pragma once

#include "EnemyBase.h"

class EnemyWizard : public EnemyBase
{

public:

	// �U���Ԋu
	static constexpr int TEAM_ATTACK = 120;

	// �R���X�g���N�^
	EnemyWizard(void);
	// �f�X�g���N�^
	~EnemyWizard(void);

protected:

	// �p�����[�^�ݒ�
	void SetParam(void) override;

	// ��ԕʍX�V
	void UpdateStandby(void) override;
};