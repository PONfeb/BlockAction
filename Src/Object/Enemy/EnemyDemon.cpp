#include "EnemyDemon.h"

EnemyDemon::EnemyDemon(void)
{
}

EnemyDemon::~EnemyDemon(void)
{
}

void EnemyDemon::SetParam(void)
{
	hp_ = 1;
	isAlive_ = true;
	scales_ = { 0.7f, 0.7f, 0.7f };
	speed_ = 5.0f;
	spawnRange_ = 1.0f;
	collisionRadius_ = 50.0f;
}