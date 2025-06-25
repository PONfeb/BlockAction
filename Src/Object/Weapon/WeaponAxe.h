
#pragma once

#include "WeaponBase.h"
#include <DxLib.h>

class WeaponAxe : public WeaponBase
{
public:
	
	// 斧のパラメータ
	static constexpr float AXE_SPEED = 15.0f;
	static constexpr float AXE_COLLISION_RADIUS = 60.0f;
	static constexpr VECTOR AXE_SCALE = { 0.5f, 0.5f, 0.5f };
	static constexpr VECTOR AXE_RELATIVE_POS = { 0.0f, 90.0f, 0.0f };
	static constexpr float AXE_JUMP_POW = 30.0f;
	static constexpr float AXE_GRAVITY = 2.0f;

	// コンストラクタ
	WeaponAxe(void);
	// デストラクタ
	~WeaponAxe(void);

	void Update() override;
	void Draw(void) override;
	void Release(void) override;

	// 武器使用
	void Use(VECTOR pos, VECTOR dir) override;

protected:

	// 画像やモデルのロード
	void Load(void) override;

	// パラメータ設定
	void SetParam(void) override;
	void UpdateParabolaAngle(VECTOR& pos, VECTOR& vel, float gravity, VECTOR& angles);

private:

	VECTOR velocity_; // 斧の速度ベクトル
	float gravity_;   // 重力
	float jumpPow_;   // ジャンプ力

};


