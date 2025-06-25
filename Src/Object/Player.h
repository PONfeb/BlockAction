#pragma once
#include <DxLib.h>
#include "Stage/BlockManager.h"
class WeaponBase;
class WeaponPunch;
class WeaponAxe;
class AnimationController;

class Player
{
public:

	//�v���C���[�̓����蔼�a
	static constexpr float COLLISION_RADIUS = 50.f;

	//�m�b�N�o�b�N�X�s�[�h
	static constexpr float SPEED_KNOCKBACK = 10.f;

	//�W���̊g�U���F
	static constexpr COLOR_F COLOR_DIF_DEFALT = { 1.f,1.f,1.f,1.f };

	//�_�Ŏ��̊g�U���F
	static constexpr COLOR_F COLOR_DIF_BLINK = { 1.0f,0.5f,0.5f,1.0f };

	//�_�ŊԊu
	static constexpr int TERM_BLINK = 8;

	//�v���C���[�̍ő�HP
	static constexpr int MAX_HP = 5;



	//�v���C���[�̌������X�e�[�g�Ǘ�����
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

	//�v���C���[�̏��
	enum class STATE
	{
		NONE,
		STANBY,
		KNOCKBACK,
		ATTACK,
		DEAD,
		END,
		VICTORY,
	};


	// ���f���̑傫��
	static constexpr VECTOR SCALES = { 0.5f, 0.5f, 0.5f };

	// �����ʒu
	static constexpr VECTOR DEFAULT_POS = { 300.0f, 10.0f, 300.0f };

	// �W���̎��Ȕ����F
	static constexpr COLOR_F COLOR_EMI_DEFAULT = { 0.5f, 0.5f, 0.5f, 0.5f };

	//�d��
	static constexpr float GRAVITY_POW = 0.8f;

	//�W�����v��
	static constexpr float JUMP_POW = 20.0f;

	//���X�|�[���ʒu
	static constexpr float RESPAWN_POS_Y = 800.f;

	//���S�ʒu
	static constexpr float DEATH_POS_X = -1500.f;

	//�n�ʏՓ˗p�����̒���
	static constexpr float COLLISION_LEN = 10.0f;

	//�X�e�[�W�T�C�Y
	static constexpr float STAGE_SIZE_X = 2000.0f;
	static constexpr float STAGE_SIZE_Z = 2000.0f;

	// �R���X�g���N�^
	Player(void);

	// �f�X�g���N�^
	~Player(void);

	//������
	void Init(void);

	//��ԑJ��
	void ChangeState(STATE state);
	//�ʏ���
	void ChangeStandby(void);
	//�m�b�N�o�b�N���
	void ChangeKnockBack(void);
	//���S���
	void ChangeDeath(void);
	//�I��
	void ChangeEnd(void);
	//�U��
	void ChangeAttack(void);
	//����
	void ChangeVictory(void);

	//�X�V
	void Update(void);

	//��ԕʍX�V
	void UpdateStandby(void);		//���s
	void UpdateKnockBack(void);		//�m�b�N�o�b�N
	void UpdateDeath(void);			//���S
	void UpdateEnd(void);			//�I��
	void UpdateAttack(void);		//�U��
	void UpdateVictory(void);		//����

	//�`��
	void Draw(void);

	//��ԕʕ`��
	void DrawStandby(void);			//���s
	void DrawKnockBack(void);		//�m�b�N�o�b�N
	void DrawDeath(void);			//���S
	void DrawEnd(void);				//�I��
	void DrawAttack(void);			//�U��
	void DrawVictory(void);		//����

	void Release(void);

	// �v���C���[�̈ʒu���擾
	// �ǂݎ���p
	//�O��������W���擾
	VECTOR GetPos(void) const { return pos_; }

	//�v���C���[���W�̍X�V
	// �ύX�\�ɂ��邽��const�͕s�v
	//�O��������W���󂯎��
	void SetPos(VECTOR pos) { pos_ = pos; }

	//�Փ˔���
	void CollisionStage(VECTOR pos);

	//���X�|�[��
	void Respawn(void);

	//�w������Ƀm�b�N�o�b�N������
	//������VECTOR�^��float�^
	void KnockBack(VECTOR dirXZ, float jumpPow);

	//�_���[�W��^����
	//����:int�^�̃_���[�W��
	void Damage(int damage);

	//�Q�b�^�[
	//HP�̎擾
	int GetHp(void);

	//���G
	bool IsInvincible(void);

	//�Q�b�^�[
	bool IsGameOver(void) const { return isGameOver_; }

	//�Q�[���N���A
	bool IsGameClear(void) const { return isGameClear_; }

	//����̎�ނ��擾
	WeaponBase* GetUseWeapon() { return useWeapon_; }

	//�v���C���[�̌�����ݒ�
	void SetDir(VECTOR dir) { moveDir_ = dir; }

	//�����A�j���[�V�������Đ�
	void PlayVictoryAnimation(void);
private:

	// ���f���̃n���h��ID
	int modelId_;
	int punchImg_;
	int axeImg_;
	VECTOR pos_;
	VECTOR angles_;
	VECTOR scales_;

	//�A�j���[�V�����R���g���[���[
	AnimationController* animationController_;

	//����
	WeaponBase* useWeapon_;
	WeaponPunch* weaponPunch_;
	WeaponAxe* weaponAxe_;

	//�W�����v��
	float jumpPow_;

	//�W�����v����
	bool isJump_;

	//�m�b�N�o�b�N����
	VECTOR knockBackDir_;

	//�ړ�����
	VECTOR moveDir_;

	//�m�b�N�o�b�N�J�E���^(�_��)
	int cntKnockBack_;

	//�v���C���[�̏��
	STATE state_;

	//HP
	int hp_;

	//�Q�[���I�[�o�[
	bool isGameOver_;

	//�Q�[���N���A
	bool isGameClear_;

	//�A�j���[�V�����̍X�V�֐�
	/*void UpdateAnimation(void);*/

	//�ړ�����
	void ProcessMove(void);
	void ProcessJump(void);
	void ProcessAttack(void);
};


