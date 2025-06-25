
#include "Player.h"
#include "../Application.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "Common/AnimationController.h"

Player::Player(void)
{
}

Player::~Player(void)
{
}

void Player::Init(void)
{
	// ���f���̃��[�h
	modelId_ = MV1LoadModel((Application::PATH_MODEL + "Player/Player.mv1").c_str());

	// ���W�ݒ�
	pos_ = DEFAULT_POS;
	MV1SetPosition(modelId_, pos_);

	// �傫���ݒ�
	scales_ = SCALES;
	MV1SetScale(modelId_, scales_);

	// �p�x
	angles_ = { 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f };
	MV1SetRotationXYZ(modelId_, angles_);

	// �F�̒���(���Ȕ���)
	MV1SetMaterialEmiColor(modelId_, 0, COLOR_EMI_DEFAULT);

	// �W�����v�͂̏�����
	jumpPow_ = 0.0f;

	// HP�̏�����
	hp_ = MAX_HP;

	// ���f���A�j���[�V��������̏�����
	animationController_ = new AnimationController(modelId_);
	for (int i = 0; i < static_cast<int>(ANIM_TYPE::MAX); i++)
	{
		animationController_->AddInFbx(i, 30.0f, i);
	}
	
	// �����A�j���[�V�����Đ�
	animationController_->Play(static_cast<int>(ANIM_TYPE::IDLE));

	// �������
	state_ = STATE::STANDBY;

	// �J�����Ƀv���C���[��Ԃ�
	SceneManager::GetInstance().GetCamera()->SetFollow(this);
}

void Player::Update(void)
{

	switch (state_)
	{
	case Player::STATE::NONE:
		break;
	case Player::STATE::STANDBY:
		UpdateStandby();
		break;
	case Player::STATE::KNOCKBACK:
		UpdateKnockBack();
		break;
	case Player::STATE::ATTACK:
		UpdateAttack();
		break;
	case Player::STATE::DEAD:
		break;
	case Player::STATE::END:
		break;
	case Player::STATE::VICTORY:
		break;
	}
}

void Player::Draw(void)
{
	// ���f���̕`��
	MV1DrawModel(modelId_);

	switch (state_)
	{
	case Player::STATE::NONE:
		break;
	case Player::STATE::STANDBY:
		DrawStandby();
		break;
	case Player::STATE::KNOCKBACK:
		DrawKnockBack();
		break;
	case Player::STATE::ATTACK:
		DrawAttack();
		break;
	case Player::STATE::DEAD:
		DrawDead();
		break;
	case Player::STATE::END:
		DrawEnd();
		break;
	case Player::STATE::VICTORY:
		DrawVictory();
		break;
	}

#ifdef _DEBUG
	//DrawSphere3D(pos_, COLLISION_RADIUS, 10, 0x0000ff, 0x0000ff, false);
#endif // _DEBUG

}

void Player::Release(void)
{
	// ���f���̃��������
	MV1DeleteModel(modelId_);

	// �A�j���[�V�����R���g���[���̉��
	animationController_->Release();
	delete animationController_;
}

VECTOR Player::GetPos(void) const
{
	return pos_;
}

void Player::SetPos(VECTOR pos)
{
	pos_ = pos;
}

void Player::CollisionStage(VECTOR pos)
{
	// �Փ˂�����w����W�ɉ����߂�
	pos_ = pos;
	jumpPow_ = 0.0f;

	isJump_ = false;
}

void Player::KnockBack(VECTOR dirXZ, float jumpPow)
{
	// �����������o�ϐ��Ɋi�[
	knockBackDir_ = dirXZ;
	jumpPow_ = jumpPow;

	// �m�b�N�o�b�N�ɏ�ԑJ��
	ChangeState(STATE::KNOCKBACK);
}

void Player::Damage(int damage)
{
	hp_ -= damage;
	if (hp_ < 0)
	{
		hp_ = 0;
	}
}

int Player::GetHp(void)
{
	return hp_;
}


void Player::ProcessMove(void)
{
	InputManager& ins = InputManager::GetInstance();
	
	// �ړ����������߂�
	VECTOR moveDir = AsoUtility::VECTOR_ZERO;
	if (ins.IsNew(KEY_INPUT_W)) { moveDir = AsoUtility::DIR_F; }
	if (ins.IsNew(KEY_INPUT_S)) { moveDir = AsoUtility::DIR_B; }
	if (ins.IsNew(KEY_INPUT_A)) { moveDir = AsoUtility::DIR_L; }
	if (ins.IsNew(KEY_INPUT_D)) { moveDir = AsoUtility::DIR_R; }

	if (AsoUtility::EqualsVZero(moveDir))
	{
		// �ړ����Ă��Ȃ��̂�IDLE
		animationController_->Play(static_cast<int>(ANIM_TYPE::IDLE));
	}
	else
	{
		// �ړ��ʂ��v�Z����i�����~�X�s�[�h�j
		VECTOR movePow = VScale(moveDir, SPEED_MOVE);

		//�ړ������i���W�{�ړ��ʁj
		pos_ = VAdd(pos_, movePow);

		// ���f���ɍ��W��ݒ肷��
		MV1SetPosition(modelId_, pos_);

		//��������p�x�i���W�A���j�ɕϊ�����
		angles_.y = atan2(moveDir.x, moveDir.z);

		// ���f���̕��������̕��̕����������Ă���̂ŁA�␳����
		angles_.y += AsoUtility::Deg2RadF(180.0f);

		MV1SetRotationXYZ(modelId_, angles_);

		// �ړ����Ă���̂�WALK
		animationController_->Play(static_cast<int>(ANIM_TYPE::WALK));
	}
}

void Player::ProcessJump(void)
{
	InputManager& ins = InputManager::GetInstance();

	// �d�́i�����x�𑬓x�ɉ��Z���Ă����j
	jumpPow_ -= GRAVITY_POW;

	// �W�����v����
	if (ins.IsTrgDown(KEY_INPUT_SPACE) && !isJump_)
	{
		isJump_ = true;
		jumpPow_ = JUMP_POW;
		// �W�����v�A�j���[�V�����Đ�
		animationController_->Play(static_cast<int>(ANIM_TYPE::JUMP), false);
	}

	// �v���C���[�̍��W�Ɉړ��ʁi���x�A�W�����v�́j�����Z����
	pos_.y += jumpPow_;

	//// �Փ˔���O�̗�������
	//if (pos_.y < 0.0f)
	//{
	//	pos_.y = 0.0f;
	//	jumpPow_ = 0.0f;
	//}

	// ���f���ɍ��W��ݒ肷��
	MV1SetPosition(modelId_, pos_);
}

void Player::Respawn(void)
{
	if (pos_.y < RESPAWN_Y)
	{
		// ���X�|�[���ʒu��ݒ�
		pos_ = RESPAWN_POS;
		jumpPow_ = 0.0f;
	}

	// ���f���ɍ��W��ݒ肷��
	MV1SetPosition(modelId_, pos_);
}

void Player::ChangeState(STATE state)
{
	state_ = state;

	switch (state)
	{
	case Player::STATE::NONE:
		break;
	case Player::STATE::STANDBY:
		ChangeStandby();
		break;
	case Player::STATE::KNOCKBACK:
		ChangeKnockBack();
		break;
	case Player::STATE::ATTACK:
		ChangeAttack();
		break;
	case Player::STATE::DEAD:
		ChangeDead();
		break;
	case Player::STATE::END:
		ChangeEnd();
		break;
	case Player::STATE::VICTORY:
		ChangeVictory();
		break;
	}
}

void Player::ChangeStandby(void)
{
}

void Player::ChangeKnockBack(void)
{
	// �W�����v����ɂ���
	isJump_ = true;

	// �m�b�N�o�b�N�J�E���^���Z�b�g
	cntKnockBack_ = 0;
}

void Player::ChangeAttack(void)
{
}

void Player::ChangeDead(void)
{
}

void Player::ChangeEnd(void)
{
}

void Player::ChangeVictory(void)
{
}

void Player::UpdateStandby(void)
{
	// �ړ�����
	ProcessMove();

	// �W�����v
	ProcessJump();

	// �����ʒu���烊�X�|�[��
	Respawn();

	// �A�j���[�V�����̍X�V
	animationController_->Update();
}

void Player::UpdateKnockBack(void)
{
	// ���n������ʏ��Ԃɖ߂�
	if (!isJump_)
	{
		ChangeState(STATE::STANDBY);
		return;
	}
	cntKnockBack_++;

	// �W�����v����
	jumpPow_ -= GRAVITY_POW;
	pos_.y += jumpPow_;

	// �m�b�N�o�b�N�����Ɉړ�������
	VECTOR movePow = VScale(knockBackDir_, SPEED_KNOCKBACK);
	pos_ = VAdd(pos_, movePow);
	MV1SetPosition(modelId_, pos_);

	// ���X�|�[������
	Respawn();
}

void Player::UpdateAttack(void)
{
}

void Player::UpdateDead(void)
{
}

void Player::UpdateEnd(void)
{
}

void Player::UpdateVictory(void)
{
}

void Player::DrawStandby(void)
{
	// ���f���̕`��
	MV1DrawModel(modelId_);
}

void Player::DrawKnockBack(void)
{
	// ���f���̕`��
	MV1DrawModel(modelId_);
}

void Player::DrawAttack(void)
{
	// ���f���̕`��
	MV1DrawModel(modelId_);
}

void Player::DrawDead(void)
{
	// ���f���̕`��
	MV1DrawModel(modelId_);
}

void Player::DrawEnd(void)
{
	// ���f���̕`��
	MV1DrawModel(modelId_);
}

void Player::DrawVictory(void)
{
	// ���f���̕`��
	MV1DrawModel(modelId_);
}
