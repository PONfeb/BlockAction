
#include "EnemyManager.h"
#include "EnemyBase.h"
#include "EnemyDemon.h"
#include "EnemyWizard.h"
#include "../Player.h" 
#include "../../Object/Shot/ShotBase.h"
#include "../../Application.h"

EnemyManager::EnemyManager(Player* player)
{
	// プレイヤーのポインタを設定
	player_ = player;
}

EnemyManager::~EnemyManager(void)
{
}

void EnemyManager::Init(void)
{
	// 出現間隔の初期化
	cntSpawn_ = 0.f;

	// エネミーのモデルIDを初期化
	enemyModelIds_.emplace_back(MV1LoadModel((Application::PATH_MODEL + "Enemy/Demon.mv1").c_str()));
	enemyModelIds_.emplace_back(MV1LoadModel((Application::PATH_MODEL + "Enemy/Wizard.mv1").c_str()));
	enemyModelIds_.emplace_back(MV1LoadModel((Application::PATH_MODEL + "Enemy/Giant.mv1").c_str()));

	// 攻撃エフェクト用のモデルのロード
	attackEffectModelIds_.emplace_back(
		MV1LoadModel(
			(Application::PATH_MODEL + "Effect/Fireball/Fireball.mv1").c_str()));
	attackEffectModelIds_.emplace_back(
		MV1LoadModel(
			(Application::PATH_MODEL + "Effect/Fireball/Rock.mv1").c_str()));

	//状態遷移初期化
	wave_ = EnemyManager::WAVE::WAVE1;
	waveFrame_ = 0;
	cntSpawn_ = 0.f;
	demonSpawn_ = 0;
	wizardSpawn_ = 0;
}

void EnemyManager::ChangeState(WAVE nextWave)
{
	wave_ = nextWave;
	waveFrame_ = 0;
	cntSpawn_ = 0;

	switch (wave_)
	{
	case EnemyManager::WAVE::WAVE1:
		break;
	case EnemyManager::WAVE::WAVE2:
		break;
	case EnemyManager::WAVE::WAVE3:
		break;

	}
}

void EnemyManager::ChangeWave1(void)
{
}

void EnemyManager::ChangeWave2(void)
{
}

void EnemyManager::ChangeWave3(void)
{
}

void EnemyManager::Update(void)
{
	switch (wave_)
	{
	case EnemyManager::WAVE::WAVE1:
		UpdateWave1();
		break;
	case EnemyManager::WAVE::WAVE2:
		UpdateWave2();
		break;
	case EnemyManager::WAVE::WAVE3:
		UpdateWave3();
		break;
	case EnemyManager::WAVE::END:
		UpdateWaveEnd();
		break; // 修正: break を追加
	}

	// 既存の敵を更新
	for (EnemyBase* enemy : enemys_)
	{
		if (enemy != nullptr) // nullptr チェック
		{
			enemy->Update();
		}
	}
}

void EnemyManager::UpdateWave1(void)
{
	waveFrame_++;

	if (demonSpawn_ < WAVE1_SPAWN_MAX && cntSpawn_ <= 0)
	{
		EnemyBase* enemy = new EnemyDemon();

		enemy->Init(EnemyBase::TYPE::DEMON,enemyModelIds_[static_cast<int>(EnemyBase::TYPE::DEMON)], -1, player_);
		enemys_.push_back(enemy);
		demonSpawn_++;

		//出現間隔を設定
		cntSpawn_ = static_cast<float>(WAVE1_SPAWN_INTERVAL);
	}
	else
	{
		//例外処理
		cntSpawn_--;
	}
	if (waveFrame_ > WAVE1_END_FRAME)
	{
		ChangeState(WAVE::WAVE2);
		waveFrame_ = 0;
		cntSpawn_ = 0;
	}
}

void EnemyManager::UpdateWave2(void)
{
	waveFrame_++;
	if (wizardSpawn_ < WAVE2_SPAWN_MAX && cntSpawn_ <= 0)
	{
		//敵情報を取得
		EnemyBase* enemy = new EnemyWizard();
		//敵の初期化(今回は球を打つので、第三引数はモデルハンドル)
		enemy->Init(EnemyBase::TYPE::WIZARD,
			enemyModelIds_[static_cast<int>(EnemyBase::TYPE::WIZARD)],
			attackEffectModelIds_[static_cast<int>(ShotBase::TYPE::STRAIGHT)],
			player_);

		//配列に登録
		enemys_.push_back(enemy);
		wizardSpawn_++;
		cntSpawn_ = 240;
	}
	else
	{
		cntSpawn_--;
	}

	//WAVE1,WAVE2で出現した敵がすべて撃破されているかを確認
	//WAVE2が500F経過しなければならない
	if (waveFrame_ > WAVE2_END_FRAME)
	{
		bool allDeath = true;
		for (auto& enemy : enemys_)
		{
			if (enemy && enemy->IsAlive())
			{
				allDeath = false;
				break;
			}
		}
		if (allDeath)
		{
			ChangeState(WAVE::WAVE3);
			waveFrame_ = 0;
			cntSpawn_ = 0;
			demonSpawn_ = 0;
			wizardSpawn_ = 0;
		}
	}

}

void EnemyManager::UpdateWave3(void)
{
	// Demon出現
	if (demonSpawn_ < WAVE3_DEMON_MAX && cntSpawn_ <= 0) {
		EnemyBase* enemy = new EnemyDemon();
		enemy->Init(EnemyBase::TYPE::DEMON,
			enemyModelIds_[static_cast<int>(EnemyBase::TYPE::DEMON)], -1, player_);
		enemys_.push_back(enemy);
		demonSpawn_++;
		cntSpawn_ = 10; // 出現間隔
		return;
	}
	// Wizard出現
	if (demonSpawn_ >= WAVE3_DEMON_MAX && wizardSpawn_ < WAVE3_WIZARD_MAX && cntSpawn_ <= 0) {
		EnemyBase* enemy = new EnemyWizard();
		enemy->Init(EnemyBase::TYPE::WIZARD,
			enemyModelIds_[static_cast<int>(EnemyBase::TYPE::WIZARD)],
			attackEffectModelIds_[static_cast<int>(ShotBase::TYPE::STRAIGHT)],
			player_);
		enemys_.push_back(enemy);
		wizardSpawn_++;
		cntSpawn_ = 10; // 出現間隔
		return;
	}

	// 出現間隔のカウントダウン
	if (cntSpawn_ > 0) {
		cntSpawn_--;
	}

	// WAVE3までで出現した敵がすべて倒されたらENDへ遷移
	int wave3EnemyCnt = 0;
	int wave3EnemyDead = 0;
	int totalWave3 = WAVE3_DEMON_MAX + WAVE3_WIZARD_MAX;
	int startIdx = static_cast<int>(enemys_.size()) - totalWave3; // インデックスを明示的にキャスト
	if (startIdx < 0) {
		startIdx = 0; // 負の値を防ぐ
	}

	for (size_t i = static_cast<size_t>(startIdx); i < enemys_.size(); ++i) { // size_tを使用
		if (i >= enemys_.size()) {
			break; // 範囲外アクセスを防ぐ
		}
		if (enemys_[i]) {
			wave3EnemyCnt++;
			if (!enemys_[i]->IsAlive()) wave3EnemyDead++;
		}
	}

	// すべて倒されたらENDへ
	if (wave3EnemyCnt == totalWave3 && wave3EnemyDead == totalWave3)
	{
		player_->ChangeState(Player::STATE::VICTORY);

		ChangeState(WAVE::END);
	}
}


void EnemyManager::UpdateWaveEnd(void)
{
}

void EnemyManager::Draw(void)
{
	switch (wave_)
	{
	case EnemyManager::WAVE::WAVE1:
		DrawWave1();
		break;
	case EnemyManager::WAVE::WAVE2:
		DrawWave2();
		break;
	case EnemyManager::WAVE::WAVE3:
		DrawWave3();
		break;
	}

	// 既存のエネミーを描画
	for (auto& enemy : enemys_)
	{
		if (enemy != nullptr)
		{
			enemy->Draw();
		}
	}

	// 残り秒数を描画
	if (wave_ == WAVE::WAVE1) {
		int remainSec = GetNextWaveTime();
		DrawFormatString(0, 560, 0xffffff, "次のWAVEまで: %d秒", remainSec);
	}
	else if (wave_ == WAVE::WAVE2)
	{
		DrawFormatString(0, 560, 0xffffff, "敵を倒したら次のWAVEだ！");
	}
	else if (wave_ == WAVE::WAVE3) {
		DrawFormatString(0, 560, 0xffffff, "すべての敵を倒せ！！");
	}

	switch (wave_)
	{
	case EnemyManager::WAVE::WAVE1:
		DrawFormatString(0, 540, 0xffffff, "WAVE1");
		break;
	case EnemyManager::WAVE::WAVE2:
		DrawFormatString(0, 540, 0xffffff, "WAVE2");
		break;
	case EnemyManager::WAVE::WAVE3:
		DrawFormatString(0, 540, 0xffffff, "WAVE3");
		break;
	}
}


void EnemyManager::DrawWave1(void)
{
}

void EnemyManager::DrawWave2(void)
{
}

void EnemyManager::DrawWave3(void)
{
}


void EnemyManager::Release(void)
{
	// Enemyクラスのメモリ解放
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

int EnemyManager::GetNextWaveTime() const
{
	int remainFrame = 0;
	switch (wave_) {
	case WAVE::WAVE1:
		remainFrame = WAVE1_END_FRAME - waveFrame_;
		break;
	case WAVE::WAVE2:
		remainFrame = WAVE2_END_FRAME - waveFrame_;
		break;
	default:
		remainFrame = 0;
		break;
	}
	if (remainFrame < 0) remainFrame = 0;
	return remainFrame / 60;
}