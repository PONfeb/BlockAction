
#pragma once

#include<DxLib.h>

class Player;
class Camera
{
public:
	// �J�����̏����ʒu
	static constexpr VECTOR DEFAULT_POS = { 0.0f, 1000.0f, -1000.0f };
	
	// �J�����̏����p�x
	static constexpr VECTOR DEFAULT_ANGLES =
	{ 30.0f * DX_PI_F / 180.0f, 0.0f, 0.0f };
	
	// �J�����N���b�v�FNEAR
	static constexpr float CAMERA_NEAR = 3.0f;
	
	// �J�����N���b�v�FFAR
	static constexpr float CAMERA_FAR = 5000.0f;
	
	// �J�����X�s�[�h(�ړ�)
	static constexpr float SPEED_MOVE = 10.0f;
	
	// �J�����X�s�[�h(�x)
	static constexpr float SPEED_ANGLE_DEG = 1.0f;
	
	Camera(void);
	~Camera(void);

	// ������
	void Init(void);
	// �X�V
	void Update(void);
	// �`��O�̃J�����ݒ�
	void SetBeforeDraw(void);
	// �f�o�b�O�p�`��
	void DrawDebug(void);
	// ���
	void Release(void);

	// �Ǐ]�Ώۂ̐ݒ�
	void SetFollow(Player* player);

private:

	// �J�����̒Ǐ]�Ώ�
	Player* player_;

	// �J�����̈ʒu
	VECTOR pos_;

	// �J�����p�x(rad)
	VECTOR angles_;

};

