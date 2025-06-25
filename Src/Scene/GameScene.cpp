#include "GameScene.h"
#include "../Manager/Camera.h"
//#include "../Utility/Grid.h"
#include "../Object/Stage/BlockManager.h"
#include "../Object/Player.h"
#include "../Object/Enemy/EnemyManager.h"
#include "../Object/Enemy/EnemyBase.h"
#include "../Object/Shot/ShotBase.h"
#include "../UI/HpManager.h"
#include "../Application.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Object/Weapon/WeaponBase.h"
#include <ctime>
#include <cmath>


GameScene::GameScene(void)
{
}

GameScene::~GameScene(void)
{
}

void GameScene::Init(void)
{

	//�O���b�h��
	/*grid_ = new Grid();
	grid_->Init();*/

	// �J�����̏�����
	/*camera_ = new Camera();
	camera_->Init();*/

	//�u���b�N�}�l�[�W���[
	blockManager_ = new BlockManager();
	blockManager_->Init();

	// �v���C���[
	player_ = new Player();
	player_->Init();

	//�G�̏�����
	enemyManager_ = new EnemyManager(player_);
	enemyManager_->Init();

	//HP
	hpManager_ = new HpManager(player_);
	hpManager_->Init();

	//�Q�[���I�[�o�[�摜�̓ǂݍ���
	imgGameOver_ = LoadGraph("Data/Image/Gameover.png");
	//�Q�[���N���A�摜�̓ǂݍ���
	imgGameClear_ = LoadGraph((Application::PATH_IMAGE + "GameClear.png").c_str());

}

void GameScene::Update(void)
{
	// ���łɃQ�[���N���A�Ȃ�Player�̂�Update
	if (isGameClear_) {
		player_->Update();

		// �X�y�[�X�L�[�Ń^�C�g����
		if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_SPACE)) {
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
		}
		return;
	}

	// �ʏ�i�s
	/*grid_->Update();*/
	blockManager_->Update();
	player_->Update();
	enemyManager_->Update();
	hpManager_->Update();
	Collision();
	CollisionEnemy();
	ColWeapon();

	// �Q�[���I�[�o�[����
	bool gameOverFlag = player_->IsGameOver();
	if (gameOverFlag) {
		if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_SPACE)) {
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
		}
		return;
	}

	// �Q�[���N���A����
	bool gameClearFlag = player_->IsGameClear();
	if (gameClearFlag) {
		if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_SPACE)) {
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
		}
		return;
	}
}


void GameScene::Draw(void)
{

	//�u���b�N�}�l�[�W���[
	blockManager_->Draw();

	/*grid_->Draw();*/

	// �v���C���[�̕`��
	player_->Draw();

	// �G�̕`��
	enemyManager_->Draw();

	//HP�`��
	hpManager_->Draw();ssd

	//�Q�[���I�[�o�[�`��
	bool gameOverFlag = player_->IsGameOver();
	if (gameOverFlag == true)
	{
		//�Q�[���I�[�o�[
		int centerPosX = Application::SCREEN_SIZE_X / 2;
		int centerPosY = Application::SCREEN_SIZE_Y / 2;
		DrawRotaGraph(centerPosX, centerPosY, 1.f, 0, imgGameOver_, true, false, false);
	}

	// �Q�[���N���A�`��
	bool gameClearFlag = player_->IsGameClear();
	if (gameClearFlag) {
		int centerPosX = Application::SCREEN_SIZE_X / 2;
		int centerPosY = Application::SCREEN_SIZE_Y / 2;
		DrawRotaGraph(centerPosX, centerPosY, 1.f, 0, imgGameClear_, true, false, false);
	}
}

void GameScene::Collision(void)
{
	//�X�e�[�W�u���b�N�Ƃ̏Փ˔���
	CollisionStage();
}

void GameScene::CollisionStage(void)
{
	//�X�e�[�W�u���b�N�Ƃ̏Փ˔���
	VECTOR playerPos = player_->GetPos();

	//������̍��W���擾
	VECTOR topPos = playerPos;
	topPos.y += playerPos.y + (Player::COLLISION_LEN * 2.f);
	lineTopPos_ = topPos;

	//�������̍��W���擾
	VECTOR downPos = playerPos;
	downPos.y = playerPos.y - Player::COLLISION_LEN;
	lineDownPos_ = downPos;

	//�����ƃu���b�N�̏Փ˔���
	MV1_COLL_RESULT_POLY result;
	if (blockManager_->IsCollisonLine(topPos, downPos, &result))
	{
		//�v���C���[�ɏՓˍ��W��n��
		player_->CollisionStage(result.HitPosition);
	}

}

void GameScene::CollisionEnemy(void)
{

	if (player_->IsInvincible())
	{
		return;
	}

	//�v���C���[���W
	VECTOR playerPos = player_->GetPos();
	//�v���C���[�̓����蔼�a
	float playerRad = player_->COLLISION_RADIUS;

	//�G���X�g�̎擾
	std::vector<EnemyBase*> enemys = enemyManager_->GetEnemys();
	for (EnemyBase* enemy : enemys)
	{
		if (!enemy->IsAlive()) continue; // ���ǉ��F�������Ă��Ȃ��G�̓X�L�b�v

		if (player_->IsInvincible())
		{
			return;
		}

		//�G�̍��W���擾
		VECTOR enemyPos = enemy->GetPos_();
		//�G�̓����蔼�a���擾
		float enemyRad = enemy->GetCollisionRadius_();

		//�G�ƃv���C���[�̓����蔻��
		if (IsCollCheck(playerPos, playerRad, enemyPos, enemyRad))
		{
			//���K���x�N�g�����擾
			VECTOR dirXZ = CalcKnockBackDirXZ(enemyPos, playerPos);
			player_->KnockBack(dirXZ, 20.f);
			player_->Damage(1);
		}

		//�e���X�g���擾
		const std::vector<ShotBase*>& shots = enemy->GetShots();
		for (ShotBase* shot : shots)
		{
			if (player_->IsInvincible())
			{
				return;
			}

			//�e�̍��W���擾
			VECTOR shotPos = shot->GetPos();
			//�e�̓����蔼�a���擾
			float shotRad = shot->GetCollisionRadius();

			//�e�ƃv���C���[�̓����蔻��
			if (IsCollCheck(playerPos, playerRad, shotPos, shotRad))
			{
				//���K���x�N�g�����擾
				VECTOR dirXZ = CalcKnockBackDirXZ(playerPos, shotPos);
				player_->KnockBack(dirXZ, 20.f);
				player_->Damage(1);
			}
		}
	}
}

void GameScene::ColWeapon()
{

	//�v���C���[�����G���Ȃ珈�������Ȃ�
	if (player_->IsInvincible())
	{
		return;
	}

	//����̎�ނ��擾
	WeaponBase* weapon = player_->GetUseWeapon();
	//�G���X�g�̎擾
	std::vector<EnemyBase*> enemys = enemyManager_->GetEnemys();

	//�C���X�^���X���Ȃ��A������g�p���̓��^�[��
	if (!weapon || !weapon->IsAlive())
	{
		return;
	}

	//����̍��W���擾
	VECTOR weaponPos = weapon->GetPos();
	//�����蔼�a���擾
	float weapomRad = weapon->GetColRad();

	for (EnemyBase* enemy : enemys)
	{
		//���G���͏������Ȃ�
		if (player_->IsInvincible())
		{
			return;
		}

		//�G�̍��W���擾
		VECTOR enemyPos = enemy->GetPos_();
		//�G�̓����蔼�a���擾
		float enemyRad = enemy->GetCollisionRadius_();
		//�G�̐������Ă���Ƃ��̂ݏՓ˔�������{
		if (!enemy->IsAlive()) continue;
		//�G�ƕ���̓����蔻��
		if (weapon->IsAlive() && IsCollCheck(weaponPos, weapomRad, enemyPos, enemyRad))
		{
			enemy->Damage(1); // �_���[�W��1�^����
			weapon->SetAlive(false);
			enemy->SetAlive(false);
		}
	}
}


void GameScene::Release(void)
{
	// �J�����̉��
	/*camera_->Release();
	delete camera_;*/
	// �O���b�h���̉��
	/*grid_->Release();
	delete grid_;*/
	// �u���b�N�}�l�[�W���[�̉��
	blockManager_->Release();
	delete blockManager_;

	// �v���C���[�̉��
	player_->Release();
	delete player_;

	// �G�̉��
	enemyManager_->Release();
	delete enemyManager_;

	//HP���
	hpManager_->Release();
	delete hpManager_;

	//�摜�̉��
	if (imgGameOver_ != -1)
	{
		DeleteGraph(imgGameOver_);
	}

	//�摜�̉��
	if (imgGameClear_ != -1)
	{
		DeleteGraph(imgGameClear_);
	}
}

bool GameScene::IsCollCheck(const VECTOR& aPos, float aRad, const VECTOR& bPos, float bRad)
{
	// �������v�Z
	float dx = aPos.x - bPos.x;
	float dy = aPos.y - bPos.y;
	float dz = aPos.z - bPos.z;

	float distSquare = dx * dx + dy * dy + dz * dz; // �����̓��
	float radSum = aRad + bRad; // ���a�̘a
	return distSquare <= radSum * radSum; // �����̓�悪���a�̘a�̓��ȉ��Ȃ�Փ�
}


VECTOR GameScene::CalcKnockBackDirXZ(const VECTOR& from, const VECTOR& to)
{
	VECTOR dir;
	//�s�����悩�����������Ƃ�����
	dir.x = to.x - from.x;
	dir.y = 0.f;
	dir.z = to.z - from.z;

	//���K��
	float len = sqrtf(dir.x * dir.x + dir.z * dir.z);

	//�G���[�����0���Z�T������
	if (len != 0.f)
	{
		//���Z���
		dir.x /= len;
		dir.z /= len;
	}

	return dir;

}
