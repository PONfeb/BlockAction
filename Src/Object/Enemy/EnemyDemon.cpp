#include "EnemyDemon.h"

EnemyDemon::EnemyDemon(void)
{
}

EnemyDemon::~EnemyDemon(void)
{
}

void EnemyDemon::SetParam(void)
{
	hp_ = 2;
	scales_ = { 0.7f, 0.7f, 0.7f };
	speed_ = 5.0f;
	spawnRange_ = 1.0f;
	collisionRadius_ = 50.0f;
}