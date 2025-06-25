
#pragma once

#include <DxLib.h>

class WeaponBase
{
public:

	//������
	enum class TYPE
	{
		PUNCH,
		AXE,
		MAX
	};

	//�R���X�g���N�^
	WeaponBase(void);
	//�f�X�g���N�^
	~WeaponBase(void);

	void Init(TYPE type);
	virtual void Update(void);
	virtual void Draw(void) = 0;
	virtual void Release(void) = 0;

	// ���W�擾
	VECTOR GetPos(void) const { return pos_; }

	// �Փ˔���p���a
	float GetColRad(void) const { return collisionRadius_; }

	// ����̎g�p
	virtual void Use(VECTOR pos, VECTOR dir) = 0;

	// ��������
	bool IsAlive(void) { return isAlive_; }

	// ��������̐ݒ�
	void SetAlive(bool isAlive) { isAlive_ = isAlive; }

	// �����ʂ̎擾
	TYPE GetType(void) { return type_; }



protected:

	// �����ʏ��
	TYPE type_ = TYPE::AXE;

	int modelId_ = -1;
	VECTOR pos_;
	VECTOR angles_;
	VECTOR scales_;

	// �Փ˔���p���a
	float collisionRadius_ = 0.f;

	// �ړ��X�s�[�h
	float speed_ = 0;

	// �ړ�����
	VECTOR moveDir_ = { 0,0,0 };

	// ��������
	bool isAlive_ = false;

	// �g�p���̈ʒu����(Y)
	VECTOR localPos_ = { 0,0,0 };

	// �摜�⃂�f���Ȃǂ̃��[�h(�������z�֐�)
	virtual void Load(void) = 0;

	// �p�����[�^�ݒ�(�������z�֐�)
	virtual void SetParam(void) = 0;

	// �ړ�����
	virtual void Move(void);
};

