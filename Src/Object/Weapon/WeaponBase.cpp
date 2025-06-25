
#include "WeaponBase.h"

WeaponBase::WeaponBase(void)
{
}

WeaponBase::~WeaponBase(void)
{
}

void WeaponBase::Init(TYPE type)
{
	type_ = type;

	// �����͐������Ă��Ȃ�
	isAlive_ = false;

	// �摜�⃂�f���Ȃǂ̃��[�h
	Load();

	// �p�����[�^�ݒ�
	SetParam();

	// ���f���̑傫���ݒ�
	MV1SetScale(modelId_, scales_);

}

void WeaponBase::Update(void)
{
	if (!isAlive_)
	{
		return;
	}

	// �ړ�����
	Move();
}

void WeaponBase::Draw(void)
{
}

void WeaponBase::Release(void)
{
}

void WeaponBase::Use(VECTOR pos, VECTOR dir)
{
}

void WeaponBase::Load(void)
{
}

void WeaponBase::SetParam(void)
{
}

void WeaponBase::Move(void)
{
	// �ړ�����(�������)
	pos_ = VAdd(pos_, VScale(moveDir_, speed_));
}
