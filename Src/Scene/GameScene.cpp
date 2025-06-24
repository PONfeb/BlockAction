
#include "GameScene.h"
#include "../Object/Grid.h"
#include "../Object/Player.h"
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