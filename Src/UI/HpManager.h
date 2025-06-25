
#pragma once

class Player;

class HpManager
{
public:

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
	void Update(void);
	void Draw(void);
	void Release(void);
private:

	// �v���C���[���
	Player* player_;

	// �g�o�A�C�R���摜�n���h��
	int imgHeart_;

	// �������g�o�A�C�R���摜�n���h��
	int imgHeartLost_;
};