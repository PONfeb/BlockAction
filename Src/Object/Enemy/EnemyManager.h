
#pragma once

#include <vector>

class EnemyBase;

class Player;

class EnemyManager
{
public:

	// Demonの出現間隔
	static constexpr int SPAWN_INTERVAL_DEMON = 60;	
	
	// Wizardの出現間隔
	static constexpr int SPAWN_INTERVAL_WIZARD = 80;

	// コンストラクタ
	EnemyManager(Player* player);
	// デストラクタ
	~EnemyManager(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

private:

	// エネミー
	std::vector<EnemyBase*> enemys_;

	// エネミー用のモデルハンドルID
	std::vector<int> enemyModelIds_;

	// プレイヤーのポインタ
	Player* player_;

	// 出現間隔
	int cntSpawn_;
};