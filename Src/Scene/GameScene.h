#pragma once  
#include "SceneBase.h"
#include "../UI/HpManager.h"
#include <DxLib.h>
class Camera;
//class Grid;
class BlockManager;
class Player;
class EnemyManager;
class ShotBase;
class HpManager;

class GameScene : public SceneBase
{
public:

	//ステージのサイズ
	static constexpr float STAGE_SIZE_X = 2000.0f; // X軸方向のサイズ
	static constexpr float STAGE_SIZE_Z = 2000.0f; // Z軸方向のサイズ

	// コンストラクタ  
	GameScene(void);

	// デストラクタ  
	~GameScene(void);

	//初期化
	void Init(void) override;

	//更新
	void Update(void) override;

	//描画
	void Draw(void) override;

	//解放
	void Release(void) override;

	//接触判定
	//引数：A座標(VECTOR型),Aの半径(float型),B座標(VECTOR型),B半径(float型)
	bool IsCollCheck(const VECTOR& aPos, float aRad, const VECTOR& bPos, float bRad);

	//2点の方向ベクトルを正規化
	//引数：いった元の座標(VECTOR型),行った先の座標(VECTOR型)
	VECTOR CalcKnockBackDirXZ(const VECTOR& from, const VECTOR& to);

	void ColWeapon();

	//プレイヤーの向きをカメラの向きに合わせる
	void SetPlayerDirToCamera(void);



private:

	//地面との衝突線分
	VECTOR lineTopPos_;
	VECTOR lineDownPos_;

	// カメラ  
	Camera* camera_;

	//グリッド線
	/*Grid* grid_;*/

	// ブロックマネージャー
	BlockManager* blockManager_;

	// プレイヤー
	Player* player_;

	// 敵
	EnemyManager* enemyManager_;

	//HP画像
	HpManager* hpManager_;

	// 衝突判定
	void Collision(void);

	void CollisionStage(void);

	//エネミーとプレイヤーの当たり判定
	void CollisionEnemy(void);


	// ゲームオーバー画像
	int imgGameOver_ = -1;
	// ゲームクリア画像
	int imgGameClear_ = -1;

	int backGroundImg_;

	//ゲームクリア判定
	bool isGameClear_ = false;



};


