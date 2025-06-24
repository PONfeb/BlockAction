
#pragma once

#include <DxLib.h>
#include"SceneBase.h"

class BlockManager;

class Grid;

class Player;

class EnemyManager;

class GameScene : public SceneBase
{
public:

	// �R���X�g���N�^
	GameScene(void);

	// �f�X�g���N�^
	~GameScene(void);

	// ����������
	 void Init(void)override;

	// �X�V�X�e�b�v
	 void Update(void) override;

	// �`�揈��
	 void Draw(void)override;

	// �������
	 void Release(void) override;
private:

	// �u���b�N�Ǘ��i�X�e�[�W�j
	 BlockManager* blockManager_;

	 // �O���b�h��
	Grid* grid_;

	// �v���C���[
	Player* player_;

	// �G
	EnemyManager* enemyManager_;

	// �n�ʂ̏Փ˗p����
	VECTOR lineTopPos_;
	VECTOR lineDownPos_;

	// �Փ˔���
	void Collision(void);

	// �X�e�[�W�u���b�N�ƃv���C���[�̏Փ�
	void CollisionStage(void);
};