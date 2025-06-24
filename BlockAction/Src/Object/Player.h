
#pragma once

#include <DxLib.h>
#include "../Object/Stage/BlockManager.h"

class AnimationController;

class Player
{
public:

	// �A�j���[�V�������
	enum class ANIM_TYPE
	{
		DEATH,
		DUCK,
		HIT_REACT,
		IDLE,
		IDLE_ATTACK,
		IDLE_HOLD,
		JUMP,
		JUMP_IDLE,
		JUMP_LAND,
		NO,
		PUNCH,
		RUN,
		RUN_ATTACK,
		RUN_HOLD,
		WALK,
		WALK_HOLD,
		WAVE,
		YES,
		MAX,
	};


	// ���f���̑傫��
	static constexpr VECTOR SCALES = { 0.5f, 0.5f, 0.5f };
	
	// �����ʒu
	static constexpr VECTOR DEFAULT_POS = { 300.0f, 10.0f, 300.0f };
	
	// �W���̎��Ȕ����F
	static constexpr COLOR_F COLOR_EMI_DEFAULT = { 0.5f, 0.5f, 0.5f, 0.5f };

	// �ړ��X�s�[�h
	static constexpr float SPEED_MOVE = 10.0f;

	// �d��
	static constexpr float GRAVITY_POW = 0.8f;

	// �n�ʏՓ˗p�����̒���
	static constexpr float COLLISION_LEN = 10.0f;

	// �W�����v��
	static constexpr float JUMP_POW = 20.0f;

	// ���X�|�[���J�n�ʒu
	static constexpr float RESPAWN_Y = -1500.0f;

	// ���X�|�[���ʒu
	static constexpr VECTOR RESPAWN_POS =
	{ BlockManager::SIZE_BLOCK * 10, 800.0f, BlockManager::SIZE_BLOCK * 10 };

	// �R���X�g���N�^
	Player(void);
	// �f�X�g���N�^
	~Player(void);
	
	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	// ���W�̎擾
	VECTOR GetPos(void) const;

	// ���W�̐ݒ�
	void SetPos(VECTOR pos);

	// �Փ˔���i�����߂��j
	void CollisionStage(VECTOR pos);

private:
	
	// ���f���̃n���h��ID
	int modelId_;
	VECTOR pos_;
	VECTOR angles_;
	VECTOR scales_;

	// �W�����v��
	float jumpPow_;

	// �W�����v����
	bool isJump_;

	// �A�j���[�V��������
	AnimationController* animationController_;

	// �s������
	void ProcessMove(void);

	void ProcessJump(void);

	// ���X�|�[��
	void Respawn(void);
};