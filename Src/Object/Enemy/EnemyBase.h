
#pragma once

#include <DxLib.h>
#include <vector>

class Player;
class AnimationController;
class ShotBase;

class EnemyBase
{
public:

	// 標準の自己発光色
	static constexpr COLOR_F COLOR_EMI_DEFAULT = { 0.6f, 0.6f, 0.6f, 1.0f };

	// エネミー種別
	enum class TYPE
	{
		DEMON,
		WIZARD,
		GIANT,
		MAX,
	};

	// アニメーション種別
	enum class ANIM_TYPE
	{
		ATTACK,
		DEATH,
		HIT_REACT,
		IDLE,
		JUMP,
		RUN,
		WALK,
		MAX,
	};

	// 状態
	enum class STATE
	{
		NONE,
		STANDBY,
		ATTACK,
	};

	// コンストラクタ
	EnemyBase(void);
	// デストラクタ
	virtual ~EnemyBase(void);

	// 初期処理
	void Init(TYPE type,
		int baseModelId, int baseAttackEffectModelId, Player* player);

	void Update(void);
	void Draw(void);
	void Release(void);

	// 状態遷移
	void ChangeState(STATE state);

	VECTOR GetPos_(void);

	// 衝突判定用半径
	float GetCollisionRadius_(void);

	std::vector<ShotBase*> GetShots(void);

protected:

	// モデルのハンドルID
	int modelId_;
	VECTOR pos_;
	VECTOR angles_;
	VECTOR scales_;

	// 弾
	std::vector<ShotBase*> shots_;

	// エフェクト用モデルハンドルID
	int baseAttackEffectModelId_;

	// 衝突判定用半径
	float collisionRadius_;

	Player* player_; // プレイヤーへのポインタ

	// アニメーション制御
	AnimationController* animationController_;

	// 種別
	TYPE type_;

	// 状態
	STATE state_;

	// 移動方向
	VECTOR moveDir_;

	// 移動速度
	float speed_;

	// HP
	int hp_;

	// 出現範囲
	float spawnRange_;

	// 攻撃間隔カウンタ
	int cntAttack_;

	// プレイヤーの方向を向く
	void LookPlayer(void);

	// 移動処理
	void Move(void);

	// 出現座標の設定
	void SetSpawnPosition(void);

	// パラメーター設定（純粋仮想関数）
	virtual void SetParam(void) = 0;

	// 状態遷移
	virtual void ChangeStandby(void);
	virtual void ChangeAttack(void);

	// 状態別更新
	virtual void UpdateStandby(void);
	virtual void UpdateAttack(void);

	// 状態別描画
	virtual void DrawStandby(void);
	virtual void DrawAttack(void);

	// 弾の更新
	void UpdateShot(void);

	// 弾の描画
	void DrawShot(void);

	// 有効な弾を取得
	ShotBase * GetValidShot(void);
};

