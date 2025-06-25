#include "Player.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Utility/AsoUtility.h"
#include "../Application.h"
#include "Common/AnimationController.h"
#include "../Object/Weapon/WeaponPunch.h"
#include "../Object/Weapon/WeaponBase.h"
#include "../Object/Weapon/WeaponAxe.h"
#include <DxLib.h>

Player::Player(void)
{
}

Player::~Player(void)
{
}

void Player::Init(void)
{
	// �v���C���[�摜�̓ǂݍ���
	modelId_ = MV1LoadModel("Data/Model/Player/Player.mv1");

	//�U���摜
	punchImg_ = LoadGraph("Data/Image/Punch.png");
	axeImg_ = LoadGraph("Data/Image/Axe.png");


	// ���f���̎��Ȕ����F��ݒ�
	MV1SetMaterialEmiColor(modelId_, 0, COLOR_EMI_DEFAULT);

	// ���f���̑傫����ݒ�
	MV1SetScale(modelId_, SCALES);

	// ���f���̏����ʒu��ݒ�
	pos_ = DEFAULT_POS;
	MV1SetPosition(modelId_, pos_);

	// ���f���̏����p�x��ݒ�
	angles_ = { 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f };
	MV1SetRotationXYZ(modelId_, angles_);

	// ���f���A�j���[�V��������̏�����
	animationController_ = new AnimationController(modelId_);
	for (int i = 0; i < static_cast<int>(ANIM_TYPE::MAX); i++)
	{
		animationController_->AddInFbx(i, 30.0f, i);
	}
	// �����A�j���[�V�����Đ�
	animationController_->Play(static_cast<int>(ANIM_TYPE::IDLE));

	//�W�����v�̏�����
	jumpPow_ = 0.0f;

	//�W�����v����̏�����
	isJump_ = false;

	//����̏�����
	weaponPunch_ = new WeaponPunch();
	weaponPunch_->Init(WeaponBase::TYPE::PUNCH);

	weaponAxe_ = new WeaponAxe();
	weaponAxe_->Init(WeaponBase::TYPE::AXE);

	//�����U���̓p���`
	useWeapon_ = weaponAxe_;


	//�J�����Ɏ������g�ɓn��
	SceneManager::GetInstance().GetCamera()->SetFollow(this);

	//�������
	ChangeState(STATE::STANBY);

	//HP������
	hp_ = MAX_HP;

	//�t���O������
	isGameOver_ = false;
	isGameClear_ = false;
}

void Player::ChangeState(STATE state)
{
	state_ = state;

	//��Ԃɉ���������
	switch (state_)
	{
	case Player::STATE::STANBY:
		ChangeStandby();
		break;
	case Player::STATE::KNOCKBACK:
		ChangeKnockBack();
		break;
	case Player::STATE::DEAD:
		ChangeDeath();
		break;
	case Player::STATE::END:
		ChangeEnd();
		break;
	case Player::STATE::ATTACK:
		ChangeAttack();
		break;
	case Player::STATE::VICTORY:
		ChangeVictory();
	}


}

void Player::ChangeStandby(void)
{
	//���s�A�j���[�V�������Đ�
	animationController_->Play(static_cast<int>(ANIM_TYPE::WALK));

	//�g�U���F���f�t�H���g�ɖ߂�
	MV1SetMaterialEmiColor(modelId_, 0, COLOR_DIF_DEFALT);
}

void Player::ChangeKnockBack(void)
{
	//�W�����v����ɂ���
	isJump_ = true;

	//�m�b�N�o�b�N�J�E���^�����Z�b�g
	cntKnockBack_ = 0;

	//����t���O���I�t�ɂ���
	if (useWeapon_->IsAlive())
	{
		useWeapon_->SetAlive(false);
	}


}

void Player::ChangeDeath(void)
{
	{
		//���S�A�j���[�V�������Đ��i���[�v�Ȃ��j
		animationController_->Play(static_cast<int>(ANIM_TYPE::DEATH), false);
	}
}

void Player::ChangeEnd(void)
{

}

void Player::ChangeAttack(void)
{
	animationController_->Play(static_cast<int>(ANIM_TYPE::IDLE_ATTACK), false);
	useWeapon_->Use(pos_, moveDir_); // ��������n��

}

void Player::ChangeVictory(void)
{
	//�����A�j���[�V�������Đ�
	animationController_->Play(static_cast<int>(ANIM_TYPE::WAVE), true);
}

void Player::Update(void)
{

	//��ԕʍX�V
	switch (state_)
	{
	case Player::STATE::STANBY:
		UpdateStandby();
		break;
	case Player::STATE::KNOCKBACK:
		UpdateKnockBack();
		break;
	case Player::STATE::DEAD:
		UpdateDeath();
		break;
	case Player::STATE::END:
		UpdateEnd();
		break;
	case Player::STATE::ATTACK:
		UpdateAttack();
		break;
	case Player::STATE::VICTORY:
		UpdateVictory();
		break;
	}

	useWeapon_->Update();
	/*weaponAxe_->Update();
	weaponPunch_->Update();*/
	//animationController_->Update();

	//�A�j���[�V�����̍X�V
	animationController_->Update();
}


void Player::UpdateStandby(void)
{
	//�ړ�����
	ProcessMove();

	//�W�����v����
	ProcessJump();

	//���X�|�[��
	Respawn();

	//�U��
	ProcessAttack();

}

void Player::UpdateKnockBack(void)
{
	//���n������ʏ��Ԃ֖߂�
	if (!isJump_)
	{
		ChangeState(STATE::STANBY);
		//�ȍ~�̏������s��Ȃ�
		return;
	}

	cntKnockBack_++;

	//�W�����v����
	jumpPow_ -= GRAVITY_POW;
	pos_.y += jumpPow_;

	//�m�b�N�o�b�N�����Ɉړ�������
	pos_.x += knockBackDir_.x * SPEED_KNOCKBACK;
	pos_.z += knockBackDir_.z * SPEED_KNOCKBACK;

	MV1SetPosition(modelId_, pos_);

	//�X�|�[������
	Respawn();
}

void Player::UpdateDeath(void)
{
	if (animationController_->IsEnd())
	{
		ChangeState(STATE::END);
	}
}

void Player::UpdateEnd(void)
{
	isGameOver_ = true;
}

void Player::UpdateAttack(void)
{
	if (animationController_->IsEnd())
	{
		useWeapon_->SetAlive(false);
		ChangeState(STATE::STANBY);
	}

	//// �����˂̓��͔���i��: Z�L�[�œ�����j
	//if (CheckHitKey(KEY_INPUT_Z)) {
	//	if (weaponAxe_ && !weaponAxe_->IsAlive()) {
	//		weaponAxe_->Use(GetPos(), moveDir_);
	//	}
	//}
}

void Player::UpdateVictory(void)
{
	// �v���C���[���O�iZ+�����j������
	angles_.y = 6.28f;
	MV1SetRotationXYZ(modelId_, angles_);

	// �Q�[���N���A�t���O
	isGameClear_ = true;
}


void Player::Draw(void)
{

	switch (state_)
	{
	case Player::STATE::STANBY:
		DrawStandby();
		break;
	case Player::STATE::KNOCKBACK:
		DrawKnockBack();
		break;
	case Player::STATE::ATTACK:
		DrawAttack();
		break;
	case Player::STATE::DEAD:
		DrawDeath();
		break;
	case Player::STATE::END:
		DrawEnd();
		break;
	case Player::STATE::VICTORY:
		DrawVictory();
		break;
	}

	useWeapon_->Draw();


	auto& input = InputManager::GetInstance();


	if (useWeapon_ == weaponAxe_)
	{
		DrawGraph(100, 100, axeImg_, true);
	}
	else if (useWeapon_ == weaponPunch_)
	{
		DrawGraph(100, 100, punchImg_, true);
	}
}

void Player::DrawStandby(void)
{
	//���f���̕`��
	MV1DrawModel(modelId_);
	//���f���̈ʒu��ݒ�
	MV1SetPosition(modelId_, pos_);
}

void Player::DrawKnockBack(void)
{
	//�t���[����(cntKnockBack_�Ōv��)�ŐF��؂�ւ���
	if ((cntKnockBack_ / TERM_BLINK) % 2 == 0)
	{
		//�_��
		MV1SetMaterialEmiColor(modelId_, 0, COLOR_DIF_BLINK);
	}
	else
	{
		//�ʏ�
		MV1SetMaterialEmiColor(modelId_, 0, COLOR_DIF_DEFALT);
	}

	//���f���̕`��
	MV1DrawModel(modelId_);

	//�ʒu�̐ݒ�
	MV1SetPosition(modelId_, pos_);
}

void Player::DrawDeath(void)
{
	//���f���̕`��
	MV1DrawModel(modelId_);

	//�ʒu�̐ݒ�
	MV1SetPosition(modelId_, pos_);
}

void Player::DrawEnd(void)
{

}

void Player::DrawAttack(void)
{
	//���f���̕`��
	MV1DrawModel(modelId_);
	//�ʒu�̐ݒ�
	/*MV1SetPosition(modelId_, pos_);*/
}

void Player::DrawVictory(void)
{
	// �����A�j���[�V�����̕`��
	MV1DrawModel(modelId_);
	// �ʒu�̐ݒ�
	MV1SetPosition(modelId_, pos_);
}

void Player::Release(void)
{
	//���f���̍폜
	MV1DeleteModel(modelId_);
	DeleteGraph(axeImg_, punchImg_);

	//�A�j���[�V�����R���g���[���[�̍폜
	animationController_->Release();
	delete animationController_;

	useWeapon_->Release();
}

void Player::CollisionStage(VECTOR pos)
{
	// �Փ˔���Ɏw����W�ɉ����߂�
	pos_ = pos;
	jumpPow_ = 0.0f;
	isJump_ = false;
}

void Player::Respawn(void)
{

	if (pos_.y < DEATH_POS_X)
	{
		// �v���C���[�̈ʒu�����X�|�[���ʒu�ɖ߂�
		pos_.y = RESPAWN_POS_Y;
		pos_.x = STAGE_SIZE_X / 2.0f;
		pos_.z = STAGE_SIZE_Z / 2.0f;
		// �W�����v�͂�������
		jumpPow_ = 0.0f;
		// �W�����v�����������
		isJump_ = false;
	}
}

void Player::KnockBack(VECTOR dirXZ, float jumpPow)
{
	//�����������o�ϐ��Ɋi�[
	knockBackDir_ = dirXZ;
	jumpPow_ = jumpPow;

	//�m�b�N�o�b�N�ɏ�ԑJ��
	ChangeState(STATE::KNOCKBACK);
}

void Player::Damage(int damage)
{
	//���Z���
	hp_ -= damage;
	//����̐����t���O�����Z�b�g
	if (useWeapon_->IsAlive())
	{
		useWeapon_->SetAlive(false);
	}

	//HP��0��菬�����Ȃ����Ƃ�
	if (hp_ == 0)
	{
		hp_ = 0;
		//���S��ԂɑJ��
		ChangeState(STATE::DEAD);
	}
}

int Player::GetHp(void)
{
	return hp_;
}

bool Player::IsInvincible(void)
{
	//�m�b�N�o�b�N���A���S�A�I���������͖��G
	return state_ == STATE::KNOCKBACK
		|| state_ == STATE::DEAD
		|| state_ == STATE::END;
}

void Player::PlayVictoryAnimation(void)
{
	// �����A�j���[�V�������Đ�
	animationController_->Play(static_cast<int>(ANIM_TYPE::WAVE), false);
}

void Player::ProcessMove(void)
{
	if (state_ == STATE::ATTACK) return;
	// �ړ�����
	auto& input = InputManager::GetInstance();
	VECTOR moveDir = AsoUtility::VECTOR_ZERO;

	if (input.IsNew(KEY_INPUT_W)) {
		moveDir.z += 1.0f;
	}
	if (input.IsNew(KEY_INPUT_S)) {
		moveDir.z -= 1.0f;
	}
	if (input.IsNew(KEY_INPUT_A)) {
		moveDir.x -= 1.0f;
	}
	if (input.IsNew(KEY_INPUT_D)) {
		moveDir.x += 1.0f;
	}
	if (moveDir.x != 0.0f || moveDir.z != 0.0f)
	{
		float len = sqrtf(moveDir.x * moveDir.x + moveDir.z * moveDir.z);
		moveDir.x /= len;
		moveDir.z /= len;
		moveDir_ = moveDir; // �������ŕۑ�
		// ...�ړ�����...
	}

	// �ړ�������ꍇ�̂ݏ���
	if (moveDir.x != 0.0f || moveDir.z != 0.0f)
	{
		// ���K��
		float len = sqrtf(moveDir.x * moveDir.x + moveDir.z * moveDir.z);
		moveDir.x /= len;
		moveDir.z /= len;

		// �ʒu���X�V
		pos_.x += moveDir.x * 10.0f;
		pos_.z += moveDir.z * 10.0f;

		// �������X�V
		float angle = atan2f(moveDir.x, moveDir.z); // x,z�̏��ɒ���
		angles_.y = angle; // Y����]

		//���f���ʒu�̕␳
		angles_.y += AsoUtility::Deg2RadF(180.0f);

		//���f���̕`��ړ����Ă���̂ŕ����A�j���[�V�������Đ�
		animationController_->Play(static_cast<int>(ANIM_TYPE::WALK), true);

	}
	else
	{
		//���f���̕`��ړ����Ă��Ȃ��̂őҋ@�A�j���[�V�������Đ�
		animationController_->Play(static_cast<int>(ANIM_TYPE::IDLE));
	}


	//���f���̊p�x�E�ʒu��ݒ�
	MV1SetPosition(modelId_, pos_);
	MV1SetRotationXYZ(modelId_, angles_);
}

void Player::ProcessJump(void)
{
	if (state_ == STATE::ATTACK) return;
	InputManager& ins = InputManager::GetInstance();

	//�d��
	jumpPow_ -= GRAVITY_POW;

	//�W�����v����
	if (ins.IsTrgDown(KEY_INPUT_SPACE) && !isJump_)
	{
		//�W�����v�͂����Z
		jumpPow_ = JUMP_POW;
		isJump_ = true;

		//�A�j���[�V�����̍Đ�
		animationController_->Play(static_cast<int>(ANIM_TYPE::JUMP));
	}

	//�v���C���[�̍��W�Ɉړ��ʂ����Z����
	pos_.y += jumpPow_;

	//�n�ʂɒ�������
	MV1SetPosition(modelId_, pos_);
}

void Player::ProcessAttack(void)
{
	auto& input = InputManager::GetInstance();
	if (input.IsTrgDown(KEY_INPUT_K) && !isJump_ && !useWeapon_->IsAlive())
	{
		ChangeState(STATE::ATTACK);
	}

	//J�L�[�ŕ��ƃp���`��؂�ւ���
	if (input.IsTrgDown(KEY_INPUT_J))
	{
		if (useWeapon_ == weaponPunch_)
		{
			useWeapon_ = weaponAxe_;
		}
		else
		{
			useWeapon_ = weaponPunch_;
		}
	}
}