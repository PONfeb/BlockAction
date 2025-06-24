
#include "GameScene.h"
#include "../Object/Grid.h"
#include "../Object/Player.h"
#include "../Object/Enemy/EnemyManager.h"
#include "../Object/Stage/BlockManager.h"
#include <DxLib.h>

GameScene::GameScene(void)
{
}

GameScene::~GameScene(void)
{
}

void GameScene::Init(void)
{
	// グリッド線
	grid_ = new Grid();
	grid_->Init();

	// ステージ管理
	blockManager_ = new BlockManager();
	blockManager_->Init();

	// プレイヤー
	player_ = new Player();
	player_->Init();

	// 敵管理
	enemyManager_ = new EnemyManager(player_);
	enemyManager_->Init();
}

void GameScene::Update(void)
{
	// グリッド線
	grid_->Update();

	// ステージ管理
	blockManager_->Update();

	// プレイヤー
	player_->Update();

	// 敵
	enemyManager_->Update();

	// 衝突判定
	Collision();
}

void GameScene::Draw(void)
{
	// ステージ管理
	blockManager_->Draw();

	// プレイヤー
	player_->Draw();

	// 敵
	enemyManager_->Draw();

#ifdef _DEBUG

	// グリッド線
	grid_->Draw();

	DrawFormatString(0, 20, 0xfffff, "GameScene");

	// 地面との衝突用線分
	DrawSphere3D(lineTopPos_, 20.0f, 10, 0x00ff00, 0x00ff00, true);
	DrawSphere3D(lineDownPos_, 20.0f, 10, 0x00ff00, 0x00ff00, true);
	DrawLine3D(lineTopPos_, lineDownPos_, 0xff0000);


#endif // DEBUG

}

void GameScene::Release(void)
{
	// グリッド線
	grid_->Release();
	delete grid_;

	// ステージ管理
	blockManager_->Release();
	delete blockManager_;

	// プレイヤー
	player_->Release();
	delete player_;

	// 敵
	enemyManager_->Release();
	delete enemyManager_;
}

void GameScene::Collision(void)
{
	// ステージブロックとプレイヤーの衝突
	CollisionStage();

}

void GameScene::CollisionStage(void)
{
	// ステージブロックとプレイヤーの衝突
	VECTOR playerPos = player_->GetPos();

	// 線分の上座標
	VECTOR topPos = playerPos;
	topPos.y = playerPos.y + (Player::COLLISION_LEN * 2.0f);
	lineTopPos_ = topPos;

	// 線分の下座標
	VECTOR downPos = playerPos;
	downPos.y = playerPos.y - Player::COLLISION_LEN;
	lineDownPos_ = downPos;

	// 線分とブロックモデルの衝突判定
	MV1_COLL_RESULT_POLY result;
	if (blockManager_->IsCollisionLine(topPos, downPos, &result))
	{
		// プレイヤーに衝突座標を渡す
		player_->CollisionStage(result.HitPosition);
	}

}