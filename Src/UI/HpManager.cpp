
#include "../Application.h"
#include "../Object/Player.h"
#include "HpManager.h"
#include <string>
#include <DxLib.h>
#include <functional>

HpManager::HpManager(Player* player)
{
	player_ = player;
}

HpManager::~HpManager(void)
{
}
void HpManager::Init(void)
{
	std::string path;

	// �Q�c�摜�̃��[�h
	path = Application::PATH_IMAGE + "Heart.png";
	imgHeart_ = LoadGraph(path.c_str());
	path = Application::PATH_IMAGE + "HeartLost.png";
	imgHeartLost_ = LoadGraph(path.c_str());

	//�摜�X�P�[��������
	for (int i = 0; i < Player::MAX_HP; ++i)
	{
		hearts_[i].scale = 1.0f;
	}
}

void HpManager::StartAnim()
{
}

void HpManager::Update(void)
{
	int hp = player_->GetHp();

	for (int i = 0; i < Player::MAX_HP; ++i)
	{
		if (i >= hp && hearts_[i].scale > 0.0f)
		{
			hearts_[i].scale -= 0.05f;
			if (hearts_[i].scale < 0.0f)
			{
				hearts_[i].scale = 0.0f;
			}
		}
	}
}

void HpManager::Draw(void)
{
	float heartSpacing = 80.0f; // �n�[�g�̊Ԋu
	float baseX = 50.0f;        // �`��J�nX���W
	float baseY = 50.0f;        // �`��J�nY���W

	for (int i = 0; i < Player::MAX_HP; ++i)
	{
		// �C�[�W���O�l���v�Z�iscale��1.0��0.0�Ɍ������Ƃ��Ƀo�E���X�j
		float t = 1.0f - hearts_[i].scale; // 0��1
		float eased = easeOutBounce(t);
		float drawScale = 1.0f - eased;    // 1��0

		// �n�[�g�摜�̒��S���W�擾
		float centerX, centerY;
		GetImageSize(imgHeart_, centerX, centerY);

		// �n�[�g�̈ʒu
		float x = baseX + i * heartSpacing;
		float y = baseY;

		// HP���c���Ă���ꍇ�͒ʏ�n�[�g�A�������ꍇ�̓��X�g�n�[�g
		int img = (hearts_[i].scale > 0.0f) ? imgHeart_ : imgHeartLost_;

		// �X�P�[����K�p���ĕ`��
		DrawRotaGraphF(x, y, drawScale, 0.0f, img, true, false);
	}

	// �v���C���[�̂g�o���
	//int max = Player::MAX_HP;
	//int playerHp = player_->GetHp();
	//for (int i = 0; i < max; i++)
	//{
	//	// �v���C���[�̂g�o�c�ʂɉ����āA�n�[�g�摜��؂�ւ���
	//	int img = -1;
	//
	//	if ((i + 1) > playerHp)
	//	{
	//		// �������n�[�g
	//		img = imgHeartLost_;
	//	}
	//	else
	//	{
	//		// �c���Ă���n�[�g
	//		img = imgHeart_;
	//	}
	//
	//	DrawRotaGraph(
	//		START_X + HP_ICON_WIDTH * i, START_Y, 1, 0, 0,  img, true);
	//}
}

void HpManager::Release(void)
{
	// �Q�c�摜�̃��������
	DeleteGraph(imgHeart_);
	DeleteGraph(imgHeartLost_);
}

void HpManager::GetImageSize(int img, float& centerX, float& centerY)
{
	int width = 0, height = 0;
	GetGraphSize(img, &width, &height);
	centerX = width / 2.0f;
	centerY = height / 2.0f;
}

float HpManager::easeOutBounce(float x)
{
	const float n1 = 7.5625f;
	const float d1 = 2.75f;

	if (x < 1.0f / d1) {
		return n1 * x * x;
	}
	else if (x < 2.0f / d1) {
		x -= 1.5f / d1;
		return n1 * x * x + 0.75f;
	}
	else if (x < 2.5f / d1) {
		x -= 2.25f / d1;
		return n1 * x * x + 0.9375f;
	}
	else {
		x -= 2.625f / d1;
		return n1 * x * x + 0.984375f;
	}
}