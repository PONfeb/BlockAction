
#pragma once

#include "../Object/Player.h"
#include <vector>

class Player;

class HpManager
{
public:

	//HP�A�C�R���ɃX�P�[��
	struct HeartIcon {
		float scale = 1.0f;
	};

	// �g�o�`��̊J�n���W(X)
	static constexpr int START_X = 60;

	// �g�o�`��̊J�n���W(Y)
	static constexpr int START_Y = 50;

	// �g�o�A�C�R���̕`�敝
	static constexpr int HP_ICON_WIDTH = 100;
	
	// �R���X�g���N�^
	HpManager(Player* player);
	// �f�X�g���N�^
	~HpManager(void);

	void Init(void);
	//�A�j���[�V�����J�n
	void StartAnim();
	void Update(void);
	void Draw(void);
	void Release(void);

	void GetImageSize(int img, float& centerX, float& centerY);

	float easeOutBounce(float x);
private:

	// �v���C���[���
	Player* player_;

	HeartIcon hearts_[Player::MAX_HP];

	// �g�o�A�C�R���摜�n���h��
	int imgHeart_;

	// �������g�o�A�C�R���摜�n���h��
	int imgHeartLost_;
};