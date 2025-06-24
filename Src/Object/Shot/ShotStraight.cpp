
#include "ShotStraight.h"

ShotStraight::ShotStraight(TYPE type, int baseModelId) : ShotBase(type, baseModelId)
{
}

ShotStraight::~ShotStraight(void)
{
}

void ShotStraight::SetParam(void)
{
	// �e�̑傫��
	scales_ = { 0.4f, 0.4f, 0.4f };

	// �e�̈ړ����x
	speed_ = 8.0f;

	// �e�̐�������
	isAlive_ = true;

	// �e�̐�������
	cntAlive_  = 300;

	// �e�̏Փ˔���p���a
	collisionRadius_ = 30.0f;
}
