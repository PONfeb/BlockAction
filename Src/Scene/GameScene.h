#pragma once  
#include "SceneBase.h"
#include "../UI/HpManager.h"
#include <DxLib.h>
class Camera;
//class Grid;
class BlockManager;
class Player;
class EnemyManager;
class ShotBase;
class HpManager;

class GameScene : public SceneBase
{
public:

	//�X�e�[�W�̃T�C�Y
	static constexpr float STAGE_SIZE_X = 2000.0f; // X�������̃T�C�Y
	static constexpr float STAGE_SIZE_Z = 2000.0f; // Z�������̃T�C�Y

	// �R���X�g���N�^  
	GameScene(void);

	// �f�X�g���N�^  
	~GameScene(void);

	//������
	void Init(void) override;

	//�X�V
	void Update(void) override;

	//�`��
	void Draw(void) override;

	//���
	void Release(void) override;

	//�ڐG����
	//�����FA���W(VECTOR�^),A�̔��a(float�^),B���W(VECTOR�^),B���a(float�^)
	bool IsCollCheck(const VECTOR& aPos, float aRad, const VECTOR& bPos, float bRad);

	//2�_�̕����x�N�g���𐳋K��
	//�����F���������̍��W(VECTOR�^),�s������̍��W(VECTOR�^)
	VECTOR CalcKnockBackDirXZ(const VECTOR& from, const VECTOR& to);

	void ColWeapon();

	//�v���C���[�̌������J�����̌����ɍ��킹��
	void SetPlayerDirToCamera(void);



private:

	//�n�ʂƂ̏Փː���
	VECTOR lineTopPos_;
	VECTOR lineDownPos_;

	// �J����  
	Camera* camera_;

	//�O���b�h��
	/*Grid* grid_;*/

	// �u���b�N�}�l�[�W���[
	BlockManager* blockManager_;

	// �v���C���[
	Player* player_;

	// �G
	EnemyManager* enemyManager_;

	//HP�摜
	HpManager* hpManager_;

	// �Փ˔���
	void Collision(void);

	void CollisionStage(void);

	//�G�l�~�[�ƃv���C���[�̓����蔻��
	void CollisionEnemy(void);


	// �Q�[���I�[�o�[�摜
	int imgGameOver_ = -1;
	// �Q�[���N���A�摜
	int imgGameClear_ = -1;

	int backGroundImg_;

	//�Q�[���N���A����
	bool isGameClear_ = false;



};


