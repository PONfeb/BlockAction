
#pragma once

#include "EnemyBase.h"

class EnemyWizard : public EnemyBase
{

public:

	// 攻撃間隔
	static constexpr int TEAM_ATTACK = 120;

	// コンストラクタ
	EnemyWizard(void);
	// デストラクタ
	~EnemyWizard(void);

protected:

	// パラメータ設定
	void SetParam(void) override;

	// 状態別更新
	void UpdateStandby(void) override;
};