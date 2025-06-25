
#pragma once

#include "WeaponBase.h"
#include <DxLib.h>

class WeaponAxe : public WeaponBase
{
public:
	
	// ���̃p�����[�^
	static constexpr float AXE_SPEED = 15.0f;
	static constexpr float AXE_COLLISION_RADIUS = 60.0f;
	static constexpr VECTOR AXE_SCALE = { 0.5f, 0.5f, 0.5f };
	static constexpr VECTOR AXE_RELATIVE_POS = { 0.0f, 90.0f, 0.0f };
	static constexpr float AXE_JUMP_POW = 30.0f;
	static constexpr float AXE_GRAVITY = 2.0f;

	// �R���X�g���N�^
	WeaponAxe(void);
	// �f�X�g���N�^
	~WeaponAxe(void);

	void Update() override;
	void Draw(void) override;
	void Release(void) override;

	// ����g�p
	void Use(VECTOR pos, VECTOR dir) override;

protected:

	// �摜�⃂�f���̃��[�h
	void Load(void) override;

	// �p�����[�^�ݒ�
	void SetParam(void) override;
	void UpdateParabolaAngle(VECTOR& pos, VECTOR& vel, float gravity, VECTOR& angles);

private:

	VECTOR velocity_; // ���̑��x�x�N�g��
	float gravity_;   // �d��
	float jumpPow_;   // �W�����v��

};


