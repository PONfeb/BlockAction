
#pragma once

#include "ShotBase.h"

class ShotStraight : public ShotBase
{
public:

	// �R���X�g���N�^
	ShotStraight(TYPE type, int baseModelId);
	// �f�X�g���N�^
	~ShotStraight(void);

protected:

	// �p�����[�^�[�ݒ�
	void SetParam(void) override;

};

