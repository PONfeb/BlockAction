
#include "WeaponPunch.h"
#include "../../Application.h"
#include "../Player.h"

WeaponPunch::WeaponPunch(void)
{
	player_ = new Player();
}
WeaponPunch::~WeaponPunch(void)
{
}

void WeaponPunch::Update(void)
{
	pos_.x += moveDir_.x * speed_;
	pos_.z += moveDir_.z * speed_;

}

void WeaponPunch::Draw(void)
{
	if (!isAlive_)
	{
		return;
	}

	bool isXRev = false;
	bool isYRev = false;
	int img = -1;

	//�p���`�̌������v�Z����
	float angle = atan2f(moveDir_.x, moveDir_.z);
	//�摜
	int imgXZ_ = imgsY_[cntAnimation_];
	//�p���`�G�t�F�N�g
	DrawBillboard3D(
		pos_, 0.5f, 0.5f, IMG_SCALE, -angle, imgXZ_, true, false, false);

	cntAnimation_++;
	if (cntAnimation_ >= NUM_SPRITE_ALL)
	{
		isAlive_ = false;
	}
}

void WeaponPunch::Release(void)
{
	// �G�t�F�N�g�摜�̃������̉��
	for (int i = 0; i < NUM_SPRITE_ALL; i++)
	{
		DeleteGraph(imgsX_[i]);
		DeleteGraph(imgsY_[i]);
	}
}

void WeaponPunch::Use(VECTOR pos, VECTOR dir)
{
	// ����̍�������
	pos_ = VAdd(pos, localPos_);
	moveDir_ = dir;
	isAlive_ = true;
	cntAnimation_ = 0;
}

void WeaponPunch::Load(void)
{
	// �G�t�F�N�g�摜(��)�̃��[�h
	LoadDivGraph(
		(Application::PATH_IMAGE + "DrillX.png").c_str(),
		NUM_SPRITE_ALL,
		NUM_SPRITE_X, NUM_SPRITE_Y,
		SIZE_XSPRITE_X, SIZE_XSPRITE_Y,
		imgsX_);

	// �G�t�F�N�g�摜(��)�̃��[�h
	LoadDivGraph(
		(Application::PATH_IMAGE + "DrillY.png").c_str(),
		NUM_SPRITE_ALL,
		NUM_SPRITE_X, NUM_SPRITE_Y,
		SIZE_YSPRITE_X, SIZE_YSPRITE_Y,
		imgsY_);
}

void WeaponPunch::SetParam(void)
{
	// ���f���̑傫��
	scales_ = { 1.0f, 1.0f, 1.0f };

	// �ړ��X�s�[�h
	speed_ = 10.0f;

	// �Փ˔���p���a
	collisionRadius_ = 100.0f;

	// �g�p���̈ʒu����
	localPos_ = { 0.0f, 90.0f, 0.0f };

	// �A�j���[�V���J�E���^������
	cntAnimation_ = 0;
}

