
#pragma once

#include <DxLib.h>
#include"SceneBase.h"

class BlockManager;

class Grid;

class Player;

class EnemyManager;

class GameScene : public SceneBase
{
public:

	// コンストラクタ
	GameScene(void);

	// デストラクタ
	~GameScene(void);

	// 初期化処理
	 void Init(void)override;

	// 更新ステップ
	 void Update(void) override;

	// 描画処理
	 void Draw(void)override;

	// 解放処理
	 void Release(void) override;
private:

	// ブロック管理（ステージ）
	 BlockManager* blockManager_;

	 // グリッド線
	Grid* grid_;

	// プレイヤー
	Player* player_;

	// 敵
	EnemyManager* enemyManager_;

	// 地面の衝突用線分
	VECTOR lineTopPos_;
	VECTOR lineDownPos_;

	// 衝突判定
	void Collision(void);

	// ステージブロックとプレイヤーの衝突
	void CollisionStage(void);
};