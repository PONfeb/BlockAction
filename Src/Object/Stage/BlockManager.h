
#pragma once

#include <DxLib.h>

class Block;

class BlockManager
{
public:

	// �u���b�N�p�̃��f�����
	static constexpr int NUM_BLOCK_MODELS = 14;

	// �u���b�N�̐�(�}�b�v�̃T�C�Y)
	static constexpr int NUM_BLOCK_X = 20;
	static constexpr int NUM_BLOCK_Z = 20;

	// 1�u���b�N������̃T�C�Y
	static constexpr float SIZE_BLOCK = 100.0f;

	// �}�b�v�S�̂̃T�C�Y
	static constexpr float WORLD_SIZE =
		SIZE_BLOCK * static_cast<float>(NUM_BLOCK_X);
	
	// �R���X�g���N�^
	BlockManager(void);
	// �f�X�g���N�^
	~BlockManager(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	// �����ƃu���b�N�̏Փ˔���
	bool IsCollisionLine(VECTOR topPos, VECTOR downPos, MV1_COLL_RESULT_POLY* result);

private:

	// �u���b�N�p�̃��f���n���h��ID
	int baseBlockModelIds_[NUM_BLOCK_MODELS];

	// �}�b�v�f�[�^
	Block* blocks_[NUM_BLOCK_Z][NUM_BLOCK_X];

	// csv�t�@�C���̓ǂݍ���
	void LoadMapCsvData(void);

};

