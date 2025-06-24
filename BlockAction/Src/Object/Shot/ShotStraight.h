
#pragma once

#include "ShotBase.h"

class ShotStraight : public ShotBase
{
public:

	// コンストラクタ
	ShotStraight(TYPE type, int baseModelId);
	// デストラクタ
	~ShotStraight(void);

protected:

	// パラメーター設定
	void SetParam(void) override;

};

