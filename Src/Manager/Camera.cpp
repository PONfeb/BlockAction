
#include "Camera.h"
#include "../Object/Player.h"
#include "InputManager.h"
#include "..//Utility/AsoUtility.h"
#include<DxLib.h>

Camera::Camera(void)
{
}

Camera::~Camera(void)
{
}

void Camera::Init(void)
{
	pos_ = DEFAULT_POS;
	angles_ = DEFAULT_ANGLES;
}

void Camera::Update(void)
{
	if (player_ != nullptr)
	{
		VECTOR playerPos = player_->GetPos();
		pos_.x = playerPos.x;
		pos_.z = playerPos.z - 1000.0f;
	}

	InputManager& ins = InputManager::GetInstance();

	// �ړ����������߂�
	VECTOR moveDir = AsoUtility::VECTOR_ZERO;
	if (ins.IsNew(KEY_INPUT_I)) { moveDir = AsoUtility::DIR_F; }
	if (ins.IsNew(KEY_INPUT_K)) { moveDir = AsoUtility::DIR_B; }
	if (ins.IsNew(KEY_INPUT_J)) { moveDir = AsoUtility::DIR_L; }
	if (ins.IsNew(KEY_INPUT_L)) { moveDir = AsoUtility::DIR_R; }

	VECTOR dir = VNorm(moveDir);

	// �ړ��ʂ��v�Z����(�����~�X�s�[�h)
	VECTOR movePow = VScale(moveDir, SPEED_MOVE);

	// �ړ�����(���W�{�ړ���)
	pos_ = VAdd(pos_, movePow);

	// X���̊p�x����
	float anglePowRad = AsoUtility::Deg2RadF(SPEED_ANGLE_DEG);
	if (ins.IsNew(KEY_INPUT_U)) { angles_.x += anglePowRad; }
	if (ins.IsNew(KEY_INPUT_O)) { angles_.x -= anglePowRad; }
}

void Camera::SetBeforeDraw(void)
{
	SetCameraNearFar(CAMERA_NEAR, CAMERA_FAR);
	SetCameraPositionAndAngle(
		pos_,
		angles_.x,
		angles_.y,
		angles_.z
		);
}

void Camera::DrawDebug(void)
{
#ifdef _DEBUG
	//DrawFormatString(0, 40, 0xffffff,
	//	"�J�������W:(%.2f,%.2f,%.2f)",
	//	pos_.x, pos_.y, pos_.z);
	//DrawFormatString(
	//	0, 60, 0xffffff,
	//	"�J�����p�x�F(%.2f, %.2f, %.2f)",
	//	AsoUtility::Deg2RadF(angles_.x),
	//	AsoUtility::Deg2RadF(angles_.y),
	//	AsoUtility::Deg2RadF(angles_.z)
	//);
#endif // DEBUG

	
}

void Camera::Release(void)
{
}

void Camera::SetFollow(Player* player)
{
	player_ = player;
}
