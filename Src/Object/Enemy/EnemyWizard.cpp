#include "EnemyWizard.h"

EnemyWizard::EnemyWizard(void)
{
}

EnemyWizard::~EnemyWizard(void)
{
}

void EnemyWizard::SetParam(void)
{
	hp_ = 1;
	isAlive_ = true;
	scales_ = { 0.7f, 0.7f, 0.7f };
	speed_ = 0.0f;
	spawnRange_ = 0.8f;
	cntAttack_ = 0;
	collisionRadius_ = 40.0f;
}

void EnemyWizard::UpdateStandby(void)
{
	// プレイヤーの方向を向く
	LookPlayer();

	// 攻撃カウンタ
	cntAttack_++;

	// 一定間隔でDemonを出現させる
	if (cntAttack_ % TEAM_ATTACK == 0)
	{
		cntAttack_ = 0;
		ChangeState(STATE::ATTACK);
	}
}
