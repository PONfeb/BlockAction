#pragma once
#include <DxLib.h>
#include "Stage/BlockManager.h"
class WeaponBase;
class WeaponPunch;
class WeaponAxe;
class AnimationController;

class Player
{
public:

	//プレイヤーの当たり半径
	static constexpr float COLLISION_RADIUS = 50.f;

	//ノックバックスピード
	static constexpr float SPEED_KNOCKBACK = 10.f;

	//標準の拡散光色
	static constexpr COLOR_F COLOR_DIF_DEFALT = { 1.f,1.f,1.f,1.f };

	//点滅時の拡散光色
	static constexpr COLOR_F COLOR_DIF_BLINK = { 1.0f,0.5f,0.5f,1.0f };

	//点滅間隔
	static constexpr int TERM_BLINK = 8;

	//プレイヤーの最大HP
	static constexpr int MAX_HP = 5;



	//プレイヤーの向きをステート管理する
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

	//プレイヤーの状態
	enum class STATE
	{
		NONE,
		STANBY,
		KNOCKBACK,
		ATTACK,
		DEAD,
		END,
		VICTORY,
	};


	// モデルの大きさ
	static constexpr VECTOR SCALES = { 0.5f, 0.5f, 0.5f };

	// 初期位置
	static constexpr VECTOR DEFAULT_POS = { 300.0f, 10.0f, 300.0f };

	// 標準の自己発光色
	static constexpr COLOR_F COLOR_EMI_DEFAULT = { 0.5f, 0.5f, 0.5f, 0.5f };

	//重力
	static constexpr float GRAVITY_POW = 0.8f;

	//ジャンプ力
	static constexpr float JUMP_POW = 20.0f;

	//リスポーン位置
	static constexpr float RESPAWN_POS_Y = 800.f;

	//死亡位置
	static constexpr float DEATH_POS_X = -1500.f;

	//地面衝突用線分の長さ
	static constexpr float COLLISION_LEN = 10.0f;

	//ステージサイズ
	static constexpr float STAGE_SIZE_X = 2000.0f;
	static constexpr float STAGE_SIZE_Z = 2000.0f;

	// コンストラクタ
	Player(void);

	// デストラクタ
	~Player(void);

	//初期化
	void Init(void);

	//状態遷移
	void ChangeState(STATE state);
	//通常状態
	void ChangeStandby(void);
	//ノックバック状態
	void ChangeKnockBack(void);
	//死亡状態
	void ChangeDeath(void);
	//終了
	void ChangeEnd(void);
	//攻撃
	void ChangeAttack(void);
	//勝利
	void ChangeVictory(void);

	//更新
	void Update(void);

	//状態別更新
	void UpdateStandby(void);		//歩行
	void UpdateKnockBack(void);		//ノックバック
	void UpdateDeath(void);			//死亡
	void UpdateEnd(void);			//終了
	void UpdateAttack(void);		//攻撃
	void UpdateVictory(void);		//勝利

	//描画
	void Draw(void);

	//状態別描画
	void DrawStandby(void);			//歩行
	void DrawKnockBack(void);		//ノックバック
	void DrawDeath(void);			//死亡
	void DrawEnd(void);				//終了
	void DrawAttack(void);			//攻撃
	void DrawVictory(void);		//勝利

	void Release(void);

	// プレイヤーの位置を取得
	// 読み取り専用
	//外部から座標を取得
	VECTOR GetPos(void) const { return pos_; }

	//プレイヤー座標の更新
	// 変更可能にするためconstは不要
	//外部から座標を受け取る
	void SetPos(VECTOR pos) { pos_ = pos; }

	//衝突判定
	void CollisionStage(VECTOR pos);

	//リスポーン
	void Respawn(void);

	//指定方向にノックバックさせる
	//引数はVECTOR型とfloat型
	void KnockBack(VECTOR dirXZ, float jumpPow);

	//ダメージを与える
	//引数:int型のダメージ数
	void Damage(int damage);

	//ゲッター
	//HPの取得
	int GetHp(void);

	//無敵
	bool IsInvincible(void);

	//ゲッター
	bool IsGameOver(void) const { return isGameOver_; }

	//ゲームクリア
	bool IsGameClear(void) const { return isGameClear_; }

	//武器の種類を取得
	WeaponBase* GetUseWeapon() { return useWeapon_; }

	//プレイヤーの向きを設定
	void SetDir(VECTOR dir) { moveDir_ = dir; }

	//勝利アニメーションを再生
	void PlayVictoryAnimation(void);
private:

	// モデルのハンドルID
	int modelId_;
	int punchImg_;
	int axeImg_;
	VECTOR pos_;
	VECTOR angles_;
	VECTOR scales_;

	//アニメーションコントローラー
	AnimationController* animationController_;

	//武器
	WeaponBase* useWeapon_;
	WeaponPunch* weaponPunch_;
	WeaponAxe* weaponAxe_;

	//ジャンプ力
	float jumpPow_;

	//ジャンプ判定
	bool isJump_;

	//ノックバック方向
	VECTOR knockBackDir_;

	//移動方向
	VECTOR moveDir_;

	//ノックバックカウンタ(点滅)
	int cntKnockBack_;

	//プレイヤーの状態
	STATE state_;

	//HP
	int hp_;

	//ゲームオーバー
	bool isGameOver_;

	//ゲームクリア
	bool isGameClear_;

	//アニメーションの更新関数
	/*void UpdateAnimation(void);*/

	//移動制御
	void ProcessMove(void);
	void ProcessJump(void);
	void ProcessAttack(void);
};


