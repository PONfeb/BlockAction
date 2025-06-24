
#include "Player.h"
#include "../Application.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "Common/AnimationController.h"

Player::Player(void)
{
}

Player::~Player(void)
{
}

void Player::Init(void)
{
	// モデルのロード
	modelId_ = MV1LoadModel((Application::PATH_MODEL + "Player/Player.mv1").c_str());

	// 座標設定
	pos_ = DEFAULT_POS;
	MV1SetPosition(modelId_, pos_);

	// 大きさ設定
	scales_ = SCALES;
	MV1SetScale(modelId_, scales_);

	// 角度
	angles_ = { 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f };
	MV1SetRotationXYZ(modelId_, angles_);

	// 色の調整(自己発光)
	MV1SetMaterialEmiColor(modelId_, 0, COLOR_EMI_DEFAULT);

	// ジャンプ力の初期化
	jumpPow_ = 0.0f;

	// モデルアニメーション制御の初期化
	animationController_ = new AnimationController(modelId_);
	for (int i = 0; i < static_cast<int>(ANIM_TYPE::MAX); i++)
	{
		animationController_->AddInFbx(i, 30.0f, i);
	}
	
	// 初期アニメーション再生
	animationController_->Play(static_cast<int>(ANIM_TYPE::IDLE));

	// カメラにプレイヤーを返す
	SceneManager::GetInstance().GetCamera()->SetFollow(this);
}

void Player::Update(void)
{
	// 移動操作
	ProcessMove();

	// ジャンプ
	ProcessJump();

	// 落下位置からリスポーン
	Respawn();

	// アニメーションの更新
	animationController_->Update();
}

void Player::Draw(void)
{
	// モデルの描画
	MV1DrawModel(modelId_);
}

void Player::Release(void)
{
	// モデルのメモリ解放
	MV1DeleteModel(modelId_);

	// アニメーションコントローラの解放
	animationController_->Release();
	delete animationController_;
}

VECTOR Player::GetPos(void) const
{
	return pos_;
}

void Player::SetPos(VECTOR pos)
{
	pos_ = pos;
}

void Player::CollisionStage(VECTOR pos)
{
	// 衝突したら指定座標に押し戻す
	pos_ = pos;
	jumpPow_ = 0.0f;

	isJump_ = false;
}


void Player::ProcessMove(void)
{
	InputManager& ins = InputManager::GetInstance();
	
	// 移動方向を決める
	VECTOR moveDir = AsoUtility::VECTOR_ZERO;
	if (ins.IsNew(KEY_INPUT_W)) { moveDir = AsoUtility::DIR_F; }
	if (ins.IsNew(KEY_INPUT_S)) { moveDir = AsoUtility::DIR_B; }
	if (ins.IsNew(KEY_INPUT_A)) { moveDir = AsoUtility::DIR_L; }
	if (ins.IsNew(KEY_INPUT_D)) { moveDir = AsoUtility::DIR_R; }

	if (AsoUtility::EqualsVZero(moveDir))
	{
		// 移動していないのでIDLE
		animationController_->Play(static_cast<int>(ANIM_TYPE::IDLE));
	}
	else
	{
		// 移動量を計算する（方向×スピード）
		VECTOR movePow = VScale(moveDir, SPEED_MOVE);

		//移動処理（座標＋移動量）
		pos_ = VAdd(pos_, movePow);

		// モデルに座標を設定する
		MV1SetPosition(modelId_, pos_);

		//方向から角度（ラジアン）に変換する
		angles_.y = atan2(moveDir.x, moveDir.z);

		// モデルの方向が正の負の方向を向いているので、補正する
		angles_.y += AsoUtility::Deg2RadF(180.0f);

		MV1SetRotationXYZ(modelId_, angles_);

		// 移動しているのでWALK
		animationController_->Play(static_cast<int>(ANIM_TYPE::WALK));
	}
}

void Player::ProcessJump(void)
{
	InputManager& ins = InputManager::GetInstance();

	// 重力（加速度を速度に加算していく）
	jumpPow_ -= GRAVITY_POW;

	// ジャンプ判定
	if (ins.IsTrgDown(KEY_INPUT_SPACE) && !isJump_)
	{
		isJump_ = true;
		jumpPow_ = JUMP_POW;
		// ジャンプアニメーション再生
		animationController_->Play(static_cast<int>(ANIM_TYPE::JUMP), false);
	}

	// プレイヤーの座標に移動量（速度、ジャンプ力）を加算する
	pos_.y += jumpPow_;

	//// 衝突判定前の落下制御
	//if (pos_.y < 0.0f)
	//{
	//	pos_.y = 0.0f;
	//	jumpPow_ = 0.0f;
	//}

	// モデルに座標を設定する
	MV1SetPosition(modelId_, pos_);
}

void Player::Respawn(void)
{
	if (pos_.y < RESPAWN_Y)
	{
		// リスポーン位置を設定
		pos_ = RESPAWN_POS;
		jumpPow_ = 0.0f;
	}

	// モデルに座標を設定する
	MV1SetPosition(modelId_, pos_);
}
