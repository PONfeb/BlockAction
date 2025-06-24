
#include "EnemyManager.h"
#include "../../Application.h"
#include "../../Object/Shot/ShotBase.h"
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

	// エネミーモデルのロード
	enemyModelIds_.emplace_back(
		MV1LoadModel((Application::PATH_MODEL + "Enemy/Demon.mv1").c_str()));
	enemyModelIds_.emplace_back(
		MV1LoadModel((Application::PATH_MODEL + "Enemy/Wizard.mv1").c_str()));
	enemyModelIds_.emplace_back(
		MV1LoadModel((Application::PATH_MODEL + "Enemy/Giant.mv1").c_str()));

	// 攻撃エフェクト用のモデルのロード
	attackEffectModelIds_.emplace_back(MV1LoadModel((Application::PATH_MODEL + "Effect/Fireball/Fireball.mv1").c_str()));
	attackEffectModelIds_.emplace_back(MV1LoadModel((Application::PATH_MODEL + "Effect/Rockfall/Rock.mv1").c_str()));
}

void EnemyManager::Update(void)
{

	// 出現カウンタ
	cntSpawn_++;

	// 一定間隔でDemonを出現させる
	if (cntSpawn_ % SPAWN_INTERVAL_DEMON == 0)
	{
		EnemyBase* enemy = new EnemyDemon();
		enemy->Init(
			EnemyBase::TYPE::DEMON,
			enemyModelIds_[static_cast<int>(EnemyBase::TYPE::DEMON)],
			-1, player_);


		enemys_.push_back(enemy);
	}

	// 一定間隔でWizardを出現させる
	if (cntSpawn_ % SPAWN_INTERVAL_WIZARD == 0)
	{
		EnemyBase* enemy = new EnemyWizard();
		enemy->Init(
			EnemyBase::TYPE::WIZARD,
			enemyModelIds_[static_cast<int>(EnemyBase::TYPE::WIZARD)],
			attackEffectModelIds_[static_cast<int>(ShotBase::TYPE::STRAIGHT)],
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

	// ベースとなるモデルのメモリ解放
	for (int id : enemyModelIds_)
	{
		MV1DeleteModel(id);
	}

}

std::vector<EnemyBase*> EnemyManager::GetEnemys(void)
{
	return enemys_;
}
