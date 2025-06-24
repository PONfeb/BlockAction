
#pragma once

#include <vector>

class EnemyBase;

class Player;

class EnemyManager
{
public:

	// Demon�̏o���Ԋu
	static constexpr int SPAWN_INTERVAL_DEMON = 60;	
	
	// Wizard�̏o���Ԋu
	static constexpr int SPAWN_INTERVAL_WIZARD = 80;

	// �R���X�g���N�^
	EnemyManager(Player* player);
	// �f�X�g���N�^
	~EnemyManager(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

private:

	// �G�l�~�[
	std::vector<EnemyBase*> enemys_;

	// �G�l�~�[�p�̃��f���n���h��ID
	std::vector<int> enemyModelIds_;

	// �v���C���[�̃|�C���^
	Player* player_;

	// �o���Ԋu
	int cntSpawn_;
};