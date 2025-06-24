
#pragma once

#include <DxLib.h>
#include "../Object/Stage/BlockManager.h"

class AnimationController;

class Player
{
public:

	// アニメーション種別
	enum class ANIM_TYPE
	{
		DEATH,
		DUCK,
		HIT_REACT,
		IDLE,
		IDLE_ATTACK,
		IDLE_HOLD,
		JUMP,
		JUMP_IDLE,
		JUMP_LAND,
		NO,
		PUNCH,
		RUN,
		RUN_ATTACK,
		RUN_HOLD,
		WALK,
		WALK_HOLD,
		WAVE,
		YES,
		MAX,
	};


	// モデルの大きさ
	static constexpr VECTOR SCALES = { 0.5f, 0.5f, 0.5f };
	
	// 初期位置
	static constexpr VECTOR DEFAULT_POS = { 300.0f, 10.0f, 300.0f };
	
	// 標準の自己発光色
	static constexpr COLOR_F COLOR_EMI_DEFAULT = { 0.5f, 0.5f, 0.5f, 0.5f };

	// 移動スピード
	static constexpr float SPEED_MOVE = 10.0f;

	// 重力
	static constexpr float GRAVITY_POW = 0.8f;

	// 地面衝突用線分の長さ
	static constexpr float COLLISION_LEN = 10.0f;

	// ジャンプ力
	static constexpr float JUMP_POW = 20.0f;

	// リスポーン開始位置
	static constexpr float RESPAWN_Y = -1500.0f;

	// リスポーン位置
	static constexpr VECTOR RESPAWN_POS =
	{ BlockManager::SIZE_BLOCK * 10, 800.0f, BlockManager::SIZE_BLOCK * 10 };

	// コンストラクタ
	Player(void);
	// デストラクタ
	~Player(void);
	
	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	// 座標の取得
	VECTOR GetPos(void) const;

	// 座標の設定
	void SetPos(VECTOR pos);

	// 衝突判定（押し戻し）
	void CollisionStage(VECTOR pos);

private:
	
	// モデルのハンドルID
	int modelId_;
	VECTOR pos_;
	VECTOR angles_;
	VECTOR scales_;

	// ジャンプ力
	float jumpPow_;

	// ジャンプ判定
	bool isJump_;

	// アニメーション制御
	AnimationController* animationController_;

	// 行動制御
	void ProcessMove(void);

	void ProcessJump(void);

	// リスポーン
	void Respawn(void);
};