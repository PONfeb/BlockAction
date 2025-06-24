
#include "../../Application.h"
#include "EnemyManager.h"
#include "EnemyDemon.h"
#include "EnemyWizard.h"
#include "EnemyBase.h"

EnemyManager::EnemyManager(Player* player)
{
	player_ = player;
}

EnemyManager::~EnemyManager(void)
{
}

void EnemyManager::Init(void)
{
	cntSpawn_ = 0;

	// �G�l�~�[���f���̃��[�h
	enemyModelIds_.emplace_back(
		MV1LoadModel((Application::PATH_MODEL + "Enemy/Demon.mv1").c_str()));
	enemyModelIds_.emplace_back(
		MV1LoadModel((Application::PATH_MODEL + "Enemy/Wizard.mv1").c_str()));
	enemyModelIds_.emplace_back(
		MV1LoadModel((Application::PATH_MODEL + "Enemy/Giant.mv1").c_str()));

}

void EnemyManager::Update(void)
{

	// �o���J�E���^
	cntSpawn_++;

	// ���Ԋu��Demon���o��������
	if (cntSpawn_ % SPAWN_INTERVAL_DEMON == 0)
	{
		EnemyBase* enemy = new EnemyDemon();
		enemy->Init(
			EnemyBase::TYPE::DEMON,
			enemyModelIds_[static_cast<int>(EnemyBase::TYPE::DEMON)],
			player_);


		enemys_.push_back(enemy);
	}

	// ���Ԋu��Wizard���o��������
	if (cntSpawn_ % SPAWN_INTERVAL_WIZARD == 0)
	{
		EnemyBase* enemy = new EnemyWizard();
		enemy->Init(
			EnemyBase::TYPE::WIZARD,
			enemyModelIds_[static_cast<int>(EnemyBase::TYPE::WIZARD)],
			player_);


		enemys_.emplace_back(enemy);
	}

	for (EnemyBase* enemy : enemys_)
	{
		enemy->Update();
	}
}

void EnemyManager::Draw(void)
{
	for (EnemyBase* enemy : enemys_)
	{
		enemy->Draw();
	}
}

void EnemyManager::Release(void)
{
	for (EnemyBase* enemy : enemys_)
	{
		enemy->Release();
		delete enemy;
	}

	// �x�[�X�ƂȂ郂�f���̃��������
	for (int id : enemyModelIds_)
	{
		MV1DeleteModel(id);
	}

}
