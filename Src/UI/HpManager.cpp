
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

	// ２Ｄ画像のロード
	path = Application::PATH_IMAGE + "Heart.png";
	imgHeart_ = LoadGraph(path.c_str());
	path = Application::PATH_IMAGE + "HeartLost.png";
	imgHeartLost_ = LoadGraph(path.c_str());

	//画像スケール初期化
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
	for (int i = 0; i < Player::MAX_HP; ++i)
	{
		// イージング値を計算（scaleが1.0→0.0に向かうときにバウンス）
		float t = 1.0f - hearts_[i].scale; // 0→1
		float eased = easeOutBounce(t);
		float drawScale = 1.0f - eased;    // 1→0

		// ハート画像の中心座標取得
		float centerX, centerY;
		GetImageSize(imgHeart_, centerX, centerY);

		// ハートの位置
		float x = HEART_POS_X + i * HEART_SPACE;
		float y = HEART_POS_Y;

		int img = (hearts_[i].scale > 0.0f) ? imgHeart_ : imgHeartLost_;

		DrawRotaGraphF(x, y, drawScale, 0.0f, img, true, false);
	}
}

void HpManager::Release(void)
{
	// ２Ｄ画像のメモリ解放
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