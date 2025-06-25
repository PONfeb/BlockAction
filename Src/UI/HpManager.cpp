
#include <string>
#include <DxLib.h>
#include "../Application.h"
#include "../Object/Player.h"
#include "HpManager.h"

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
}

void HpManager::Update(void)
{
}

void HpManager::Draw(void)
{
	// �v���C���[�̂g�o���
	int max = Player::MAX_HP;
	int playerHp = player_->GetHp();
	for (int i = 0; i < max; i++)
	{
		// �v���C���[�̂g�o�c�ʂɉ����āA�n�[�g�摜��؂�ւ���
		int img = -1;

		if ((i + 1) > playerHp)
		{
			// �������n�[�g
			img = imgHeartLost_;
		}
		else
		{
			// �c���Ă���n�[�g
			img = imgHeart_;
		}

		DrawRotaGraph(
			START_X + HP_ICON_WIDTH * i, START_Y, 1, 0, 0,  img, true);
	}
}

void HpManager::Release(void)
{
	// �Q�c�摜�̃��������
	DeleteGraph(imgHeart_);
	DeleteGraph(imgHeartLost_);
}