
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

	// �G�l�~�[�̎擾
	std::vector<EnemyBase*> GetEnemys(void);

private:

	// �G�l�~�[�p�̃��f���n���h��ID
	std::vector<int> enemyModelIds_;

	// �U���G�t�F�N�g�p�̃��f���n���h��ID
	std::vector<int> attackEffectModelIds_;

	// �G�l�~�[
	std::vector<EnemyBase*> enemys_;

	// �v���C���[�̃|�C���^
	Player* player_;

	// �o���Ԋu
	int cntSpawn_;
};