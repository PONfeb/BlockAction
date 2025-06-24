
#pragma once

#include "EnemyBase.h"

class EnemyDemon : public EnemyBase
{

public:
	// コンストラクタ
	EnemyDemon(void);
	// デストラクタ
	~EnemyDemon(void);

private:
	// パラメータ設定
	void SetParam(void) override;

};