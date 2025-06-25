
#pragma once

#include <vector>

class EnemyBase;

class Player;

class EnemyDemon;

class EnemyWizard;


class EnemyManager
{
public:

	//敵の生成ウェーブ
	enum class WAVE
	{
		WAVE1,
		WAVE2,
		WAVE3,
		END,
	};

	// 敵の出現間隔
	static constexpr int SPAWN_INTERVAL_DEMON = 60;
	static constexpr int SPAWN_INTERVAL_WIZARD = 80;

	//第1ウェーブ
	//180FごとにDemonを描画(最大数2)
	static constexpr int WAVE1_SPAWN_INTERVAL_DEMON = 180;
	static constexpr int WAVE1_SPAWN_MAX = 2;
	//ウェーブごとのフレーム数
	static constexpr int WAVE1_END_FRAME = 250;
	static constexpr int WAVE1_SPAWN_INTERVAL = 100;

	//第2ウェーブ
	//240FごとにWizardを描画(最大数2)
	static constexpr int WAVE2_SPAWN_INTERVAL_WIZARD = 240;
	static constexpr int WAVE2_SPAWN_MAX = 2;
	//ウェーブ終了のFRAME数
	static constexpr int WAVE2_END_FRAME = 500;

	//第3ウェーブ
	static constexpr int WAVE3_DEMON_MAX = 5;
	static constexpr int WAVE3_WIZARD_MAX = 5;


	// コンストラクタ
	EnemyManager(Player* player);

	// デストラクタ
	~EnemyManager(void);

	// 初期化
	void Init(void);



	//ステート管理用関数
	void ChangeState(WAVE nextWave);
	void ChangeWave1(void);
	void ChangeWave2(void);
	void ChangeWave3(void);

	// 更新
	void Update(void);
	void UpdateWave1(void);
	void UpdateWave2(void);
	void UpdateWave3(void);
	void UpdateWaveEnd(void);

	// 描画
	void Draw(void);
	void DrawWave1(void);
	void DrawWave2(void);
	void DrawWave3(void);

	// 解放
	void Release(void);


	//敵を識別
	std::vector<EnemyBase*> GetEnemys(void);

	//ステートを取得
	WAVE GetWave(void) const { return wave_; }

	//残り時間を描画
	int GetNextWaveTime()const;


private:

	//ウェーブ状態遷移
	WAVE wave_;

	// エネミー
	std::vector<EnemyBase*> enemys_;

	// プレイヤーのポインタ
	Player* player_;

	// 出現間隔
	int cntSpawn_;

	//エネミー用のモデルハンドル
	std::vector<int> enemyModelIds_;

	//攻撃エフェクト用のモデルハンドルID
	std::vector<int> attackEffectModelIds_;

	// エネミーの種類
	EnemyDemon* enemyDemon_;

	//ウェーブのフレーム数
	int waveFrame_ = 0;

	//WAVE1で出現するDemonの数
	int demonSpawn_ = 0;

	//WAVE2で出現したwizardの数
	int wizardSpawn_ = 0;

	//スポーンカウンタ
	int enemyCnt_ = 0;
};