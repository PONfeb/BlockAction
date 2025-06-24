
#include "ShotBase.h"

ShotBase::ShotBase(TYPE type, int baseModelId)
{
	// �g�p�������e�ʂƓǂݍ��ݎ��Ԃ̍팸�̂���
	// ���f���f�[�^����������������ɑ��݂����Ȃ�
	// CreateShot�ŏ������Ă��܂��ƁA�ė��p����x�Ƀ��f������������A
	// Release������Ȃ��󋵂ɂȂ��Ă��܂�
	modelId_ = MV1DuplicateModel(baseModelId);
	type_ = type;
}

ShotBase::~ShotBase(void)
{
}

void ShotBase::CreateShot(VECTOR pos, VECTOR dir)
{
	// �e�̔��ˈʒu��ݒ�
	pos_ = pos;

	// �e�̔��˕����̐ݒ�
	dir_ = dir;

	// �p�����[�^�ݒ�
	SetParam();

	// �傫���̐ݒ�
	MV1SetScale(modelId_, scales_);

	// �ʒu�̐ݒ�
	MV1SetPosition(modelId_, pos_);
}

void ShotBase::Update(void)
{
	if (!IsAlive())
	{
		// �������Ă��Ȃ���Ώ������f
		return;
	}
	// �ړ�����
	pos_ = VAdd(pos_, VScale(dir_, speed_));
	// �ʒu�̐ݒ�
	MV1SetPosition(modelId_, pos_);
	// �����J�E���^�̌���
	ReduceCntAlive();
}

void ShotBase::Draw()
{
	if (!IsAlive())
	{
		// �������Ă��Ȃ���Ώ������f
		return;
	}
	// ���f���̕`��
	MV1DrawModel(modelId_);
#ifdef _DEBUG
	// �f�o�b�O�p�F�Փ˔���p����
	DrawSphere3D(pos_, collisionRadius_, 10, 0x0000ff, 0x0000ff, false);
#endif // _DEBUG
}

void ShotBase::Release(void)
{
	MV1DeleteModel(modelId_);
}

bool ShotBase::IsAlive(void)
{
	return isAlive_;
}

VECTOR ShotBase::GetPos(void)
{
	return pos_;
}

float ShotBase::GetCollisionRadius(void)
{
	return collisionRadius_;
}

ShotBase::TYPE ShotBase::GetType(void)
{
	return type_;
}

void ShotBase::ReduceCntAlive(void)
{
	cntAlive_--;
	if (cntAlive_ < 0)
	{
		isAlive_ = false;
	}
}