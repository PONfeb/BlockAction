
#pragma once

#include <vector>

class EnemyBase;

class Player;

class EnemyDemon;

class EnemyWizard;


class EnemyManager
{
public:

	//�G�̐����E�F�[�u
	enum class WAVE
	{
		WAVE1,
		WAVE2,
		WAVE3,
		END,
	};

	// �G�̏o���Ԋu
	static constexpr int SPAWN_INTERVAL_DEMON = 60;
	static constexpr int SPAWN_INTERVAL_WIZARD = 80;

	//��1�E�F�[�u
	//180F���Ƃ�Demon��`��(�ő吔2)
	static constexpr int WAVE1_SPAWN_INTERVAL_DEMON = 180;
	static constexpr int WAVE1_SPAWN_MAX = 2;
	//�E�F�[�u���Ƃ̃t���[����
	static constexpr int WAVE1_END_FRAME = 250;
	static constexpr int WAVE1_SPAWN_INTERVAL = 100;

	//��2�E�F�[�u
	//240F���Ƃ�Wizard��`��(�ő吔2)
	static constexpr int WAVE2_SPAWN_INTERVAL_WIZARD = 240;
	static constexpr int WAVE2_SPAWN_MAX = 2;
	//�E�F�[�u�I����FRAME��
	static constexpr int WAVE2_END_FRAME = 500;

	//��3�E�F�[�u
	static constexpr int WAVE3_DEMON_MAX = 5;
	static constexpr int WAVE3_WIZARD_MAX = 5;


	// �R���X�g���N�^
	EnemyManager(Player* player);

	// �f�X�g���N�^
	~EnemyManager(void);

	// ������
	void Init(void);



	//�X�e�[�g�Ǘ��p�֐�
	void ChangeState(WAVE nextWave);
	void ChangeWave1(void);
	void ChangeWave2(void);
	void ChangeWave3(void);

	// �X�V
	void Update(void);
	void UpdateWave1(void);
	void UpdateWave2(void);
	void UpdateWave3(void);
	void UpdateWaveEnd(void);

	// �`��
	void Draw(void);
	void DrawWave1(void);
	void DrawWave2(void);
	void DrawWave3(void);

	// ���
	void Release(void);


	//�G������
	std::vector<EnemyBase*> GetEnemys(void);

	//�X�e�[�g���擾
	WAVE GetWave(void) const { return wave_; }

	//�c�莞�Ԃ�`��
	int GetNextWaveTime()const;


private:

	//�E�F�[�u��ԑJ��
	WAVE wave_;

	// �G�l�~�[
	std::vector<EnemyBase*> enemys_;

	// �v���C���[�̃|�C���^
	Player* player_;

	// �o���Ԋu
	int cntSpawn_;

	//�G�l�~�[�p�̃��f���n���h��
	std::vector<int> enemyModelIds_;

	//�U���G�t�F�N�g�p�̃��f���n���h��ID
	std::vector<int> attackEffectModelIds_;

	// �G�l�~�[�̎��
	EnemyDemon* enemyDemon_;

	//�E�F�[�u�̃t���[����
	int waveFrame_ = 0;

	//WAVE1�ŏo������Demon�̐�
	int demonSpawn_ = 0;

	//WAVE2�ŏo������wizard�̐�
	int wizardSpawn_ = 0;

	//�X�|�[���J�E���^
	int enemyCnt_ = 0;
};