
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

	//パンチの向きを計算する
	float angle = atan2f(moveDir_.x, moveDir_.z);
	//画像
	int imgXZ_ = imgsY_[cntAnimation_];
	//パンチエフェクト
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
	// エフェクト画像のメモリの解放
	for (int i = 0; i < NUM_SPRITE_ALL; i++)
	{
		DeleteGraph(imgsX_[i]);
		DeleteGraph(imgsY_[i]);
	}
}

void WeaponPunch::Use(VECTOR pos, VECTOR dir)
{
	// 武器の高さ調整
	pos_ = VAdd(pos, localPos_);
	moveDir_ = dir;
	isAlive_ = true;
	cntAnimation_ = 0;
}

void WeaponPunch::Load(void)
{
	// エフェクト画像(横)のロード
	LoadDivGraph(
		(Application::PATH_IMAGE + "DrillX.png").c_str(),
		NUM_SPRITE_ALL,
		NUM_SPRITE_X, NUM_SPRITE_Y,
		SIZE_XSPRITE_X, SIZE_XSPRITE_Y,
		imgsX_);

	// エフェクト画像(横)のロード
	LoadDivGraph(
		(Application::PATH_IMAGE + "DrillY.png").c_str(),
		NUM_SPRITE_ALL,
		NUM_SPRITE_X, NUM_SPRITE_Y,
		SIZE_YSPRITE_X, SIZE_YSPRITE_Y,
		imgsY_);
}

void WeaponPunch::SetParam(void)
{
	// モデルの大きさ
	scales_ = { 1.0f, 1.0f, 1.0f };

	// 移動スピード
	speed_ = 10.0f;

	// 衝突判定用半径
	collisionRadius_ = 100.0f;

	// 使用時の位置調整
	localPos_ = { 0.0f, 90.0f, 0.0f };

	// アニメーショカウンタ初期化
	cntAnimation_ = 0;
}

