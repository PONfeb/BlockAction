
#pragma once

#include <DxLib.h>

class ShotBase
{
public:

	// �e�̎��
	enum class TYPE
	{
		STRAIGHT,
		ROCK
	};

	// �R���X�g���N�^(�e��ʁA���ƂȂ郂�f���̃n���h��ID)
	ShotBase(TYPE type, int baseModelId);
	// �f�X�g���N�^
	virtual ~ShotBase(void);

	// �e�̐���(�\���J�n���W�A�e�̐i�s����)
	virtual void CreateShot(VECTOR pos, VECTOR dir);

	// �X�V�X�e�b�v
	virtual void Update(void);
	// �`��
	void Draw(void);
	// �������
	void Release(void);

	// ��������
	bool IsAlive(void);

	// �e�̍��W
	VECTOR GetPos(void);

	// �e�̏Փ˔���p���a
	float GetCollisionRadius(void);

	// �e�̎��
	TYPE GetType(void);

protected:

	// �e�̎��
	TYPE type_;

	// �e�̃��f��ID
	int modelId_;

	// ����
	VECTOR dir_;

	// �e�̑傫��
	VECTOR scales_;

	// �e�̍��W
	VECTOR pos_;

	// �e�̈ړ����x
	float speed_;

	// �e�̐�������
	bool isAlive_;

	// �e�̐�������
	int cntAlive_;

	// �e�̏Փ˔���p���a
	float collisionRadius_;

	// �p�����[�^�ݒ�
	virtual void SetParam(void) = 0;

	// �e�̐������Ԃ̌���
	void ReduceCntAlive(void);
};