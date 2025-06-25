#include "GameScene.h"
#include "../Manager/Camera.h"
//#include "../Utility/Grid.h"
#include "../Object/Stage/BlockManager.h"
#include "../Object/Player.h"
#include "../Object/Enemy/EnemyManager.h"
#include "../Object/Enemy/EnemyBase.h"
#include "../Object/Shot/ShotBase.h"
#include "../UI/HpManager.h"
#include "../Application.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Object/Weapon/WeaponBase.h"
#include <ctime>
#include <cmath>


GameScene::GameScene(void)
{
}

GameScene::~GameScene(void)
{
}

void GameScene::Init(void)
{

	//グリッド線
	/*grid_ = new Grid();
	grid_->Init();*/

	// カメラの初期化
	/*camera_ = new Camera();
	camera_->Init();*/

	//ブロックマネージャー
	blockManager_ = new BlockManager();
	blockManager_->Init();

	// プレイヤー
	player_ = new Player();
	player_->Init();

	//敵の初期化
	enemyManager_ = new EnemyManager(player_);
	enemyManager_->Init();

	//HP
	hpManager_ = new HpManager(player_);
	hpManager_->Init();

	//ゲームオーバー画像の読み込み
	imgGameOver_ = LoadGraph("Data/Image/Gameover.png");
	//ゲームクリア画像の読み込み
	imgGameClear_ = LoadGraph((Application::PATH_IMAGE + "GameClear.png").c_str());

}

void GameScene::Update(void)
{
	// すでにゲームクリアならPlayerのみUpdate
	if (isGameClear_) {
		player_->Update();

		// スペースキーでタイトルへ
		if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_SPACE)) {
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
		}
		return;
	}

	// 通常進行
	/*grid_->Update();*/
	blockManager_->Update();
	player_->Update();
	enemyManager_->Update();
	hpManager_->Update();
	Collision();
	CollisionEnemy();
	ColWeapon();

	// ゲームオーバー判定
	bool gameOverFlag = player_->IsGameOver();
	if (gameOverFlag) {
		if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_SPACE)) {
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
		}
		return;
	}

	// ゲームクリア判定
	bool gameClearFlag = player_->IsGameClear();
	if (gameClearFlag) {
		if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_SPACE)) {
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
		}
		return;
	}
}


void GameScene::Draw(void)
{

	//ブロックマネージャー
	blockManager_->Draw();

	/*grid_->Draw();*/

	// プレイヤーの描画
	player_->Draw();

	// 敵の描画
	enemyManager_->Draw();

	//HP描画
	hpManager_->Draw();ssd

	//ゲームオーバー描画
	bool gameOverFlag = player_->IsGameOver();
	if (gameOverFlag == true)
	{
		//ゲームオーバー
		int centerPosX = Application::SCREEN_SIZE_X / 2;
		int centerPosY = Application::SCREEN_SIZE_Y / 2;
		DrawRotaGraph(centerPosX, centerPosY, 1.f, 0, imgGameOver_, true, false, false);
	}

	// ゲームクリア描画
	bool gameClearFlag = player_->IsGameClear();
	if (gameClearFlag) {
		int centerPosX = Application::SCREEN_SIZE_X / 2;
		int centerPosY = Application::SCREEN_SIZE_Y / 2;
		DrawRotaGraph(centerPosX, centerPosY, 1.f, 0, imgGameClear_, true, false, false);
	}
}

void GameScene::Collision(void)
{
	//ステージブロックとの衝突判定
	CollisionStage();
}

void GameScene::CollisionStage(void)
{
	//ステージブロックとの衝突判定
	VECTOR playerPos = player_->GetPos();

	//線分上の座標を取得
	VECTOR topPos = playerPos;
	topPos.y += playerPos.y + (Player::COLLISION_LEN * 2.f);
	lineTopPos_ = topPos;

	//線分下の座標を取得
	VECTOR downPos = playerPos;
	downPos.y = playerPos.y - Player::COLLISION_LEN;
	lineDownPos_ = downPos;

	//線分とブロックの衝突判定
	MV1_COLL_RESULT_POLY result;
	if (blockManager_->IsCollisonLine(topPos, downPos, &result))
	{
		//プレイヤーに衝突座標を渡す
		player_->CollisionStage(result.HitPosition);
	}

}

void GameScene::CollisionEnemy(void)
{

	if (player_->IsInvincible())
	{
		return;
	}

	//プレイヤー座標
	VECTOR playerPos = player_->GetPos();
	//プレイヤーの当たり半径
	float playerRad = player_->COLLISION_RADIUS;

	//敵リストの取得
	std::vector<EnemyBase*> enemys = enemyManager_->GetEnemys();
	for (EnemyBase* enemy : enemys)
	{
		if (!enemy->IsAlive()) continue; // ★追加：生存していない敵はスキップ

		if (player_->IsInvincible())
		{
			return;
		}

		//敵の座標を取得
		VECTOR enemyPos = enemy->GetPos_();
		//敵の当たり半径を取得
		float enemyRad = enemy->GetCollisionRadius_();

		//敵とプレイヤーの当たり判定
		if (IsCollCheck(playerPos, playerRad, enemyPos, enemyRad))
		{
			//正規化ベクトルを取得
			VECTOR dirXZ = CalcKnockBackDirXZ(enemyPos, playerPos);
			player_->KnockBack(dirXZ, 20.f);
			player_->Damage(1);
		}

		//弾リストを取得
		const std::vector<ShotBase*>& shots = enemy->GetShots();
		for (ShotBase* shot : shots)
		{
			if (player_->IsInvincible())
			{
				return;
			}

			//弾の座標を取得
			VECTOR shotPos = shot->GetPos();
			//弾の当たり半径を取得
			float shotRad = shot->GetCollisionRadius();

			//弾とプレイヤーの当たり判定
			if (IsCollCheck(playerPos, playerRad, shotPos, shotRad))
			{
				//正規化ベクトルを取得
				VECTOR dirXZ = CalcKnockBackDirXZ(playerPos, shotPos);
				player_->KnockBack(dirXZ, 20.f);
				player_->Damage(1);
			}
		}
	}
}

void GameScene::ColWeapon()
{

	//プレイヤーが無敵中なら処理をしない
	if (player_->IsInvincible())
	{
		return;
	}

	//武器の種類を取得
	WeaponBase* weapon = player_->GetUseWeapon();
	//敵リストの取得
	std::vector<EnemyBase*> enemys = enemyManager_->GetEnemys();

	//インスタンスがない、かつ武器使用中はリターン
	if (!weapon || !weapon->IsAlive())
	{
		return;
	}

	//武器の座標を取得
	VECTOR weaponPos = weapon->GetPos();
	//当たり半径を取得
	float weapomRad = weapon->GetColRad();

	for (EnemyBase* enemy : enemys)
	{
		//無敵中は処理しない
		if (player_->IsInvincible())
		{
			return;
		}

		//敵の座標を取得
		VECTOR enemyPos = enemy->GetPos_();
		//敵の当たり半径を取得
		float enemyRad = enemy->GetCollisionRadius_();
		//敵の生存しているときのみ衝突判定を実施
		if (!enemy->IsAlive()) continue;
		//敵と武器の当たり判定
		if (weapon->IsAlive() && IsCollCheck(weaponPos, weapomRad, enemyPos, enemyRad))
		{
			enemy->Damage(1); // ダメージを1与える
			weapon->SetAlive(false);
			enemy->SetAlive(false);
		}
	}
}


void GameScene::Release(void)
{
	// カメラの解放
	/*camera_->Release();
	delete camera_;*/
	// グリッド線の解放
	/*grid_->Release();
	delete grid_;*/
	// ブロックマネージャーの解放
	blockManager_->Release();
	delete blockManager_;

	// プレイヤーの解放
	player_->Release();
	delete player_;

	// 敵の解放
	enemyManager_->Release();
	delete enemyManager_;

	//HP解放
	hpManager_->Release();
	delete hpManager_;

	//画像の解放
	if (imgGameOver_ != -1)
	{
		DeleteGraph(imgGameOver_);
	}

	//画像の解放
	if (imgGameClear_ != -1)
	{
		DeleteGraph(imgGameClear_);
	}
}

bool GameScene::IsCollCheck(const VECTOR& aPos, float aRad, const VECTOR& bPos, float bRad)
{
	// 距離を計算
	float dx = aPos.x - bPos.x;
	float dy = aPos.y - bPos.y;
	float dz = aPos.z - bPos.z;

	float distSquare = dx * dx + dy * dy + dz * dz; // 距離の二乗
	float radSum = aRad + bRad; // 半径の和
	return distSquare <= radSum * radSum; // 距離の二乗が半径の和の二乗以下なら衝突
}


VECTOR GameScene::CalcKnockBackDirXZ(const VECTOR& from, const VECTOR& to)
{
	VECTOR dir;
	//行った先かたいったもとを引く
	dir.x = to.x - from.x;
	dir.y = 0.f;
	dir.z = to.z - from.z;

	//正規化
	float len = sqrtf(dir.x * dir.x + dir.z * dir.z);

	//エラー回避で0除算控除する
	if (len != 0.f)
	{
		//除算代入
		dir.x /= len;
		dir.z /= len;
	}

	return dir;

}
