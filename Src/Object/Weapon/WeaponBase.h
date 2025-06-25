
#pragma once

#include <DxLib.h>

class WeaponBase
{
public:

	//武器種別
	enum class TYPE
	{
		PUNCH,
		AXE,
		MAX
	};

	//コンストラクタ
	WeaponBase(void);
	//デストラクタ
	~WeaponBase(void);

	void Init(TYPE type);
	virtual void Update(void);
	virtual void Draw(void) = 0;
	virtual void Release(void) = 0;

	// 座標取得
	VECTOR GetPos(void) const { return pos_; }

	// 衝突判定用半径
	float GetColRad(void) const { return collisionRadius_; }

	// 武器の使用
	virtual void Use(VECTOR pos, VECTOR dir) = 0;

	// 生存判定
	bool IsAlive(void) { return isAlive_; }

	// 生存判定の設定
	void SetAlive(bool isAlive) { isAlive_ = isAlive; }

	// 武器種別の取得
	TYPE GetType(void) { return type_; }



protected:

	// 武器種別状態
	TYPE type_ = TYPE::AXE;

	int modelId_ = -1;
	VECTOR pos_;
	VECTOR angles_;
	VECTOR scales_;

	// 衝突判定用半径
	float collisionRadius_ = 0.f;

	// 移動スピード
	float speed_ = 0;

	// 移動方向
	VECTOR moveDir_ = { 0,0,0 };

	// 生存判定
	bool isAlive_ = false;

	// 使用時の位置調整(Y)
	VECTOR localPos_ = { 0,0,0 };

	// 画像やモデルなどのロード(純粋仮想関数)
	virtual void Load(void) = 0;

	// パラメータ設定(純粋仮想関数)
	virtual void SetParam(void) = 0;

	// 移動処理
	virtual void Move(void);
};

