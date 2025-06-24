
#include "GameScene.h"
#include "../Object/Grid.h"
#include "../Object/Player.h"
#include "../Utility/AsoUtility.h"
#include "../Object/Enemy/EnemyBase.h"
#include "../Object/Shot/ShotBase.h"
#include "../Object/Enemy/EnemyManager.h"
#include "../Object/Stage/BlockManager.h"
#include <DxLib.h>

GameScene::GameScene(void)
{
}

GameScene::~GameScene(void)
{
}

void GameScene::Init(void)
{
	// �O���b�h��
	grid_ = new Grid();
	grid_->Init();

	// �X�e�[�W�Ǘ�
	blockManager_ = new BlockManager();
	blockManager_->Init();

	// �v���C���[
	player_ = new Player();
	player_->Init();

	// �G�Ǘ�
	enemyManager_ = new EnemyManager(player_);
	enemyManager_->Init();
}

void GameScene::Update(void)
{
	// �O���b�h��
	grid_->Update();

	// �X�e�[�W�Ǘ�
	blockManager_->Update();

	// �v���C���[
	player_->Update();

	// �G
	enemyManager_->Update();

	// �Փ˔���
	Collision();
}

void GameScene::Draw(void)
{
	// �X�e�[�W�Ǘ�
	blockManager_->Draw();

	// �v���C���[
	player_->Draw();

	// �G
	enemyManager_->Draw();

#ifdef _DEBUG

	// �O���b�h��
	grid_->Draw();

	DrawFormatString(0, 20, 0xfffff, "GameScene");

	// �n�ʂƂ̏Փ˗p����
	DrawSphere3D(lineTopPos_, 20.0f, 10, 0x00ff00, 0x00ff00, true);
	DrawSphere3D(lineDownPos_, 20.0f, 10, 0x00ff00, 0x00ff00, true);
	DrawLine3D(lineTopPos_, lineDownPos_, 0xff0000);


#endif // DEBUG

}

void GameScene::Release(void)
{
	// �O���b�h��
	grid_->Release();
	delete grid_;

	// �X�e�[�W�Ǘ�
	blockManager_->Release();
	delete blockManager_;

	// �v���C���[
	player_->Release();
	delete player_;

	// �G
	enemyManager_->Release();
	delete enemyManager_;
}

void GameScene::Collision(void)
{
	// �X�e�[�W�u���b�N�ƃv���C���[�̏Փ�
	CollisionStage();

	// �G�ƃv���C���[�̏Փ�
	CollisionEnemy();
}

void GameScene::CollisionStage(void)
{
	// �X�e�[�W�u���b�N�ƃv���C���[�̏Փ�
	VECTOR playerPos = player_->GetPos();

	// �����̏���W
	VECTOR topPos = playerPos;
	topPos.y = playerPos.y + (Player::COLLISION_LEN * 2.0f);
	lineTopPos_ = topPos;

	// �����̉����W
	VECTOR downPos = playerPos;
	downPos.y = playerPos.y - Player::COLLISION_LEN;
	lineDownPos_ = downPos;

	// �����ƃu���b�N���f���̏Փ˔���
	MV1_COLL_RESULT_POLY result;
	if (blockManager_->IsCollisionLine(topPos, downPos, &result))
	{
		// �v���C���[�ɏՓˍ��W��n��
		player_->CollisionStage(result.HitPosition);
	}

}

void GameScene::CollisionEnemy(void)
{
	// �v���C���[�̍��W���擾
	VECTOR playerPos = player_->GetPos();

	// �G�̏����擾����
	std::vector<EnemyBase*> enemys = enemyManager_->GetEnemys();

	for (EnemyBase* enemy : enemys)
	{	

		// �G�̍��W���擾
		VECTOR enemyPos = enemy->GetPos_();

		// �G�̏Փ˔���p���a
		float enemyRadius = enemy->GetCollisionRadius_();

		if (AsoUtility::IsHitSpheres(playerPos, Player::COLLISION_RADIUS, enemyPos, enemyRadius))
		{
			int a = 0;
		}

		std::vector<ShotBase*> shots = enemy->GetShots();

		for (ShotBase* shot : shots)
		{

			// ���̍��W���擾
			VECTOR shotPos = shot->GetPos();

			// ���̔��a���擾
			float shotRadius = shot->GetCollisionRadius();

			if (AsoUtility::IsHitSpheres(playerPos, Player::COLLISION_RADIUS, shotPos, shotRadius))
			{
				int b = 0;
			}
		}
	}
}
