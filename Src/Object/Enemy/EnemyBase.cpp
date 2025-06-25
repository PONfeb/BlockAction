
#include "EnemyBase.h"
#include "../../Application.h"
#include "../../Utility/AsoUtility.h"
#include "../Stage/BlockManager.h"
#include "../Common/AnimationController.h"
#include "../Player.h"
#include "../Shot/ShotStraight.h"

EnemyBase::EnemyBase(void)
{
	int ebase = 0;
	ebase = 1;
}

EnemyBase::~EnemyBase(void)
{
	int ebaseD = 0;
	ebaseD = 1;
}

void EnemyBase::Init(TYPE type,
	int baseModelId, int baseAttackEffectModelId, Player* player)

{
	// �G�l�~�[���
	type_ = type;

	// �v���C���[�̃|�C���^���i�[
	player_ = player;

	// ���f���̕���
	modelId_ = MV1DuplicateModel(baseModelId);

	// �G�t�F�N�g���f��ID
	baseAttackEffectModelId_ = baseAttackEffectModelId;

	// �p�����[�^�ݒ�
	SetParam();

	// �傫��
	MV1SetScale(modelId_, scales_);

	// �o�����W�̐ݒ�
	SetSpawnPosition();

	// �v���C���[�̕���������
	LookPlayer();

	// �ړ�����
	Move();

	// �F�̒���
	MV1SetMaterialEmiColor(modelId_, 0, COLOR_EMI_DEFAULT);

	// ���f���A�j���[�V��������̏�����
	animationController_ = new AnimationController(modelId_);
	for (int i = 0; i < static_cast<int>(ANIM_TYPE::MAX); i++)
	{
		animationController_->AddInFbx(i, 30.0f, i);
	}

	// �����A�j���[�V�����Đ�
	animationController_->Play(static_cast<int>(ANIM_TYPE::WALK));

	// �������
	ChangeState(STATE::STANDBY);
}

void EnemyBase::Update(void)
{
	switch (state_)
	{
	case EnemyBase::STATE::STANDBY:
		UpdateStandby();
		break;
	case EnemyBase::STATE::ATTACK:
		UpdateAttack();
		break;
	case EnemyBase::STATE::HIT_REACT:
		HitRectUpdate();
		break;
	case EnemyBase::STATE::DEAD_REACT:
		DeathUpdate();
		break;
	case EnemyBase::STATE::END:
		EndUpdate(); // ��Ԉێ��p�B�ړ��Ȃǂ��Ȃ��B
		break;
	}

	UpdateShot();
	animationController_->Update();
}

void EnemyBase::HitRectUpdate(void)
{
	stateCnt_++;

	if (stateCnt_ > 20)
	{
		if (hp_ > 0)
		{
			ChangeState(STATE::STANDBY);
		}
		else
		{
			ChangeState(STATE::DEAD_REACT);
		}
	}
}

void EnemyBase::DeathUpdate(void)
{
	stateCnt_++;
	if (stateCnt_ > 30 && animationController_->IsEnd())
	{
		ChangeState(STATE::END);
	}
}

void EnemyBase::EndUpdate(void)
{
}

void EnemyBase::Draw(void)
{
	// DEAD_REACT��END��Ԃ͕`�悵�Ȃ��A�܂��͓������Ȃǂ���
	if (!isAlive_) return;

	switch (state_)
	{
	case EnemyBase::STATE::STANDBY:
		DrawStandby();
		break;
	case EnemyBase::STATE::ATTACK:
		DrawAttack();
		break;
	case EnemyBase::STATE::HIT_REACT:
		DrawHitReact();
		break;
	case EnemyBase::STATE::DEAD_REACT:
		DrawDeathReact();
		break;
	case EnemyBase::STATE::END:
		DrawEnd();
		break;
	}

	DrawShot();
}

void EnemyBase::Release(void)
{
	// ���f���̃��������
	MV1DeleteModel(modelId_);

	// �A�j���[�V�����R���g���[���̉��
	animationController_->Release();
	delete animationController_;

	// �e�̉��
	for (ShotBase* shot : shots_)
	{
		shot->Release();
		delete shot;
	}
	shots_.clear();
}

void EnemyBase::Damage(int damage)
{
	if (!isAlive_) return; // ���łɎ���ł���Ȃ疳��

	hp_ -= damage;

	if (hp_ > 0)
	{
		ChangeState(STATE::HIT_REACT);
		isAlive_ = true;
	}
	else
	{
		ChangeState(STATE::DEAD_REACT);
		isAlive_ = false;
	}
}

void EnemyBase::ChangeState(STATE state)
{
	state_ = state;

	switch (state_)
	{
	case EnemyBase::STATE::STANDBY:
		ChangeStandby();
		break;
	case EnemyBase::STATE::ATTACK:
		ChangeAttack();
		break;
	}
}

VECTOR EnemyBase::GetPos_(void)
{
	return pos_;
}

float EnemyBase::GetCollisionRadius_(void)
{
	return collisionRadius_;
}

std::vector<ShotBase*> EnemyBase::GetShots(void)
{
	return shots_;
}

void EnemyBase::LookPlayer(void)
{
	// �v���C���[�̍��W���擾
	VECTOR playerPos = player_->GetPos();

	// ����ւ̃x�N�g�����v�Z
	VECTOR diff = VSub(playerPos, pos_);
	diff.y = 0.0f;

	// �x�N�g���̐��K���ŒP�ʃx�N�g�����擾����
	moveDir_ = VNorm(diff);

	//��������p�x�i���W�A���j�ɕϊ�����
	angles_.y = atan2(moveDir_.x, moveDir_.z);

	// ���f���̕��������̕��̕����������Ă���̂ŁA�␳����
	angles_.y += AsoUtility::Deg2RadF(180.0f);

	// ��]��Y���݂̂Ƃ���
	angles_.x = angles_.z = 0.0f;

	// ���f���Ɋp�x��ݒ�
	MV1SetRotationXYZ(modelId_, angles_);
}

void EnemyBase::Move(void)
{
	// �ړ���
	VECTOR movePow = VScale(moveDir_, speed_);

	// �ړ������i���W�{�ړ��ʁj
	pos_ = VAdd(pos_, movePow);

	// �ړ���������
	if (pos_.x < 0.0f)
	{
		pos_.x = 0.0f;
	}

	if (pos_.x > BlockManager::WORLD_SIZE)
	{
		pos_.x = BlockManager::WORLD_SIZE;
	}

	if (pos_.z < 0.0f)
	{
		pos_.z = 0.0f;
	}

	if (pos_.z > BlockManager::WORLD_SIZE)
	{
		pos_.z = BlockManager::WORLD_SIZE;
	}

	// ���f���ɍ��W��ݒ肷��
	MV1SetPosition(modelId_, pos_);
}

void EnemyBase::SetSpawnPosition(void)
{
	// �X�e�[�W�̔���
	float radius = BlockManager::WORLD_SIZE / 2.0f;

	// �X�e�[�W�̒��S���W
	VECTOR centerPos = { radius, 0.0f, radius };

	// �o���͈͂̒���
	radius *= spawnRange_;

	// �����_���Ȋp�x���擾
	int degree = GetRand(360);
	float radian = static_cast<float>(degree) * DX_PI_F / 180.0f;

	// �p�x������������߂�
	float dirX = sinf(radian);
	float dirZ = cosf(radian);
	VECTOR dir = { dirX, 0.0f, dirZ };

	// �����~�����i���������a���̃����_���ȃx�N�g���j
	VECTOR vec = VScale(dir, radius);

	// �X�e�[�W�̒��S���W�Ƀx�N�g�������M����
	// �~����̍��W���v�Z����
	VECTOR pos = VAdd(centerPos, vec);

	pos_ = pos;
	MV1SetPosition(modelId_, pos_);
}

void EnemyBase::SetParam(void)
{
}

void EnemyBase::ChangeStandby(void)
{
	// �����A�j���[�V�����Đ�
	animationController_->Play(static_cast<int>(ANIM_TYPE::WALK));
}

void EnemyBase::ChangeAttack(void)
{
	// �U���A�j���[�V�����Đ�
	animationController_->Play(static_cast<int>(ANIM_TYPE::ATTACK), false);
	
	// �L���Ȓe���擾����
	ShotBase* shot = GetValidShot();

	// �e�𐶐�
	shot->CreateShot(pos_, moveDir_);

}

void EnemyBase::UpdateStandby(void)
{
	if (!isAlive_) return; // ���S���Ă����瓮���Ȃ�

	LookPlayer();
	Move();
}

void EnemyBase::UpdateAttack(void)
{
	// �U���A�j���[�V�������I�������ʏ��Ԃɖ߂�
	if (animationController_->IsEnd())
	{
		ChangeState(STATE::STANDBY);
	}
}

void EnemyBase::DrawStandby(void)
{
	// ���f���̕`��
	MV1DrawModel(modelId_);
}

void EnemyBase::DrawAttack(void)
{
	// ���f���̕`��
	MV1DrawModel(modelId_);
}

void EnemyBase::DrawHitReact()
{
}

void EnemyBase::DrawDeathReact()
{
}

void EnemyBase::DrawEnd()
{
}

void EnemyBase::UpdateShot(void)
{	
	for (ShotBase* shot : shots_)
	{
		shot->Update();
	}
}

void EnemyBase::DrawShot(void)
{
	for (ShotBase* shot : shots_)
	{
		shot->Draw();
	}
}

ShotBase* EnemyBase::GetValidShot(void)
{
	size_t size = shots_.size();
	for (int i = 0; i < size; i++)
	{
		// ���g�p(�������Ă��Ȃ�)
		if (!shots_[i]->IsAlive())
		{
			return shots_[i];
		}
	}

	// �V�����e�̃C���X�^���X�𐶐�����
	ShotBase* shot = new ShotStraight(
		ShotBase::TYPE::STRAIGHT, baseAttackEffectModelId_);
	
	// �ϒ��z��ɒǉ�
	shots_.push_back(shot);
	return shot;
}