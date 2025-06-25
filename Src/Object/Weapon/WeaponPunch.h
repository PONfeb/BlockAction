
#pragma once

#include <DxLib.h>
#include "WeaponBase.h"

class Player;

class WeaponPunch : public WeaponBase
{
public:
	// �G�t�F�N�g�摜�̕�����
	static constexpr int NUM_SPRITE_X = 4;
	static constexpr int NUM_SPRITE_Y = 3;
	static constexpr int NUM_SPRITE_ALL = NUM_SPRITE_X * NUM_SPRITE_Y;

	// �G�t�F�N�g�摜�̕������ꂽ�傫��(X)
	static constexpr int SIZE_XSPRITE_X = 100;
	static constexpr int SIZE_XSPRITE_Y = 80;

	// �G�t�F�N�g�摜�̕������ꂽ�傫��(Y)
	static constexpr int SIZE_YSPRITE_X = 80;
	static constexpr int SIZE_YSPRITE_Y = 100;

	// �摜�̑傫���{��
	static constexpr float IMG_SCALE = 280.0f;

	// �R���X�g���N�^
	WeaponPunch(void);
	// �f�X�g���N�^
	~WeaponPunch(void);

	void Update(void);

	void Draw(void) override;

	void Release(void) override;

	// ������g�p����
	void Use(VECTOR pos, VECTOR dir) override;

protected:

	// �摜�⃂�f���Ȃǂ̃��[�h
	void Load(void) override;

	// �p�����[�^�ݒ�
	void SetParam(void) override;

private:

	// �A�j���[�V�����J�E���^
	int cntAnimation_;

	// �G�t�F�N�g�摜(��)
	int imgsX_[NUM_SPRITE_ALL];

	// �G�t�F�N�g�摜(�c)
	int imgsY_[NUM_SPRITE_ALL];

	//�v���C���[
	Player* player_;

};

