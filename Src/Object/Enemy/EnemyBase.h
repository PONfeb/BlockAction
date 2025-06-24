
#pragma once

#include <DxLib.h>
#include <vector>

class Player;
class AnimationController;
class ShotBase;

class EnemyBase
{
public:

	// �W���̎��Ȕ����F
	static constexpr COLOR_F COLOR_EMI_DEFAULT = { 0.6f, 0.6f, 0.6f, 1.0f };

	// �G�l�~�[���
	enum class TYPE
	{
		DEMON,
		WIZARD,
		GIANT,
		MAX,
	};

	// �A�j���[�V�������
	enum class ANIM_TYPE
	{
		ATTACK,
		DEATH,
		HIT_REACT,
		IDLE,
		JUMP,
		RUN,
		WALK,
		MAX,
	};

	// ���
	enum class STATE
	{
		NONE,
		STANDBY,
		ATTACK,
	};

	// �R���X�g���N�^
	EnemyBase(void);
	// �f�X�g���N�^
	virtual ~EnemyBase(void);

	// ��������
	void Init(TYPE type,
		int baseModelId, int baseAttackEffectModelId, Player* player);

	void Update(void);
	void Draw(void);
	void Release(void);

	// ��ԑJ��
	void ChangeState(STATE state);

	VECTOR GetPos_(void);

	// �Փ˔���p���a
	float GetCollisionRadius_(void);

	std::vector<ShotBase*> GetShots(void);

protected:

	// ���f���̃n���h��ID
	int modelId_;
	VECTOR pos_;
	VECTOR angles_;
	VECTOR scales_;

	// �e
	std::vector<ShotBase*> shots_;

	// �G�t�F�N�g�p���f���n���h��ID
	int baseAttackEffectModelId_;

	// �Փ˔���p���a
	float collisionRadius_;

	Player* player_; // �v���C���[�ւ̃|�C���^

	// �A�j���[�V��������
	AnimationController* animationController_;

	// ���
	TYPE type_;

	// ���
	STATE state_;

	// �ړ�����
	VECTOR moveDir_;

	// �ړ����x
	float speed_;

	// HP
	int hp_;

	// �o���͈�
	float spawnRange_;

	// �U���Ԋu�J�E���^
	int cntAttack_;

	// �v���C���[�̕���������
	void LookPlayer(void);

	// �ړ�����
	void Move(void);

	// �o�����W�̐ݒ�
	void SetSpawnPosition(void);

	// �p�����[�^�[�ݒ�i�������z�֐��j
	virtual void SetParam(void) = 0;

	// ��ԑJ��
	virtual void ChangeStandby(void);
	virtual void ChangeAttack(void);

	// ��ԕʍX�V
	virtual void UpdateStandby(void);
	virtual void UpdateAttack(void);

	// ��ԕʕ`��
	virtual void DrawStandby(void);
	virtual void DrawAttack(void);

	// �e�̍X�V
	void UpdateShot(void);

	// �e�̕`��
	void DrawShot(void);

	// �L���Ȓe���擾
	ShotBase * GetValidShot(void);
};

