
#include "BlockManager.h"
#include "Block.h"
#include "../../Application.h"
#include "../../Utility/AsoUtility.h"
#include <string>
#include<fstream>

BlockManager::BlockManager(void)
{
}

BlockManager::~BlockManager(void)
{
}

void BlockManager::Init(void)
{
	// �e��u���b�N���f���̃��[�h
	std::string PATH = Application::PATH_MODEL + "Blocks/";
	baseBlockModelIds_[0] = MV1LoadModel((PATH + "Block_Grass.mv1").c_str());
	baseBlockModelIds_[1] = MV1LoadModel((PATH + "Block_Metal.mv1").c_str());
	baseBlockModelIds_[2] = MV1LoadModel((PATH + "Block_Ice.mv1").c_str());

	// �O���t�@�C������}�b�v�f�[�^��ǂݍ���
	LoadMapCsvData();

}

void BlockManager::Update(void)
{
	// �u���b�N�̍X�V
	for (int z = 0; z < NUM_BLOCK_Z; z++)
	{
		for (int x = 0; x < NUM_BLOCK_X; x++)
		{
			blocks_[z][x]->Update();
		}
	}
}

void BlockManager::Draw(void)
{
	// �}�b�v�̕`��
	for (int z = 0; z < NUM_BLOCK_Z; z++)
	{
		for (int x = 0; x < NUM_BLOCK_X; x++)
		{
			blocks_[z][x]->Draw();
		}
	}
	
}

void BlockManager::Release(void)
{
	// �u���b�N�̃��������
	for (int z = 0; z < NUM_BLOCK_Z; z++)
	{
		for (int x = 0; x < NUM_BLOCK_X; x++)
		{
			blocks_[z][x]->Release();
			delete blocks_[z][x];
		}
	}

	// ���f���̃��������
	for (int i = 0; i < NUM_BLOCK_MODELS; i++)
	{
		MV1DeleteModel(baseBlockModelIds_[i]);
	}
}

bool BlockManager::IsCollisionLine(VECTOR topPos, VECTOR downPos, MV1_COLL_RESULT_POLY* result)
{
	bool ret = false;

	for (int z = 0; z < NUM_BLOCK_Z; z++)
	{
		for (int x = 0; x < NUM_BLOCK_X; x++)
		{
			// 2�����z�񂩂�Block�������o��
			Block* block = blocks_[z][x];

			// �����ƃ��f���̏Փ˔���
			MV1_COLL_RESULT_POLY res =
				MV1CollCheck_Line(block->GetModelId(), -1, topPos, downPos);
			
			if (res.HitFlag)
			{
				// ���ʂ�Ԃ�
				*result = res;
				return true;
			}
		}
	}
	return ret;
}


void BlockManager::LoadMapCsvData(void)
{
	// �t�@�C���̓Ǎ�
	std::ifstream ifs = std::ifstream(Application::PATH_MAP_DATA);
	if (!ifs)
	{
		// �G���[������
		return;
	}
	// �t�@�C�����P�s���ǂݍ���
	std::string line; // 1�s�̕������
	std::vector<std::string> strSplit; // 1�s��1�����̓��I�z��ɕ���
	int chipNo = 0;
	int z = 0;
	while (getline(ifs, line))
	{
		// �P�s���J���}��؂�ŕ���
		strSplit = AsoUtility::Split(line, ',');

		for (int x = 0; x < strSplit.size(); x++) 
		{
			// string����int�ɕϊ�
			chipNo = stoi(strSplit[x]);

			// int�^���L���X�g���� enum class�ɕϊ�
			Block::TYPE type = static_cast<Block::TYPE>(chipNo);

			// ��ɓǂݍ���ł������f���̃n���h��ID���擾����
			int baseModelId = baseBlockModelIds_[chipNo];

			// �u���b�N�𐶐�
			Block* block = new Block();
			block->Create(type, baseModelId, x, z);

			// �Q�����z��Ƀu���b�N�N���X�̃|�C���^���i�[
			blocks_[z][x] = block;
		}
		z++;
	}
}
