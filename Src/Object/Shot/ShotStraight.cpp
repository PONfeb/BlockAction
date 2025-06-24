
#include "ShotStraight.h"

ShotStraight::ShotStraight(TYPE type, int baseModelId) : ShotBase(type, baseModelId)
{
}

ShotStraight::~ShotStraight(void)
{
}

void ShotStraight::SetParam(void)
{
	// ’e‚Ì‘å‚«‚³
	scales_ = { 0.4f, 0.4f, 0.4f };

	// ’e‚ÌˆÚ“®‘¬“x
	speed_ = 8.0f;

	// ’e‚Ì¶‘¶”»’è
	isAlive_ = true;

	// ’e‚Ì¶‘¶ŠúŠÔ
	cntAlive_  = 300;

	// ’e‚ÌÕ“Ë”»’è—p”¼Œa
	collisionRadius_ = 30.0f;
}
