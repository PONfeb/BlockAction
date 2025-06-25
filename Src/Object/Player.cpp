#include "Player.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Utility/AsoUtility.h"
#include "../Application.h"
#include "Common/AnimationController.h"
#include "../Object/Weapon/WeaponPunch.h"
#include "../Object/Weapon/WeaponBase.h"
#include "../Object/Weapon/WeaponAxe.h"
#include <DxLib.h>

Player::Player(void)
{
}

Player::~Player(void)
{
}

void Player::Init(void)
{
	// プレイヤー画像の読み込み
	modelId_ = MV1LoadModel("Data/Model/Player/Player.mv1");

	//攻撃画像
	punchImg_ = LoadGraph("Data/Image/Punch.png");
	axeImg_ = LoadGraph("Data/Image/Axe.png");


	// モデルの自己発光色を設定
	MV1SetMaterialEmiColor(modelId_, 0, COLOR_EMI_DEFAULT);

	// モデルの大きさを設定
	MV1SetScale(modelId_, SCALES);

	// モデルの初期位置を設定
	pos_ = DEFAULT_POS;
	MV1SetPosition(modelId_, pos_);

	// モデルの初期角度を設定
	angles_ = { 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f };
	MV1SetRotationXYZ(modelId_, angles_);

	// モデルアニメーション制御の初期化
	animationController_ = new AnimationController(modelId_);
	for (int i = 0; i < static_cast<int>(ANIM_TYPE::MAX); i++)
	{
		animationController_->AddInFbx(i, 30.0f, i);
	}
	// 初期アニメーション再生
	animationController_->Play(static_cast<int>(ANIM_TYPE::IDLE));

	//ジャンプの初期化
	jumpPow_ = 0.0f;

	//ジャンプ判定の初期化
	isJump_ = false;

	//武器の初期化
	weaponPunch_ = new WeaponPunch();
	weaponPunch_->Init(WeaponBase::TYPE::PUNCH);

	weaponAxe_ = new WeaponAxe();
	weaponAxe_->Init(WeaponBase::TYPE::AXE);

	//初期攻撃はパンチ
	useWeapon_ = weaponAxe_;


	//カメラに自分自身に渡す
	SceneManager::GetInstance().GetCamera()->SetFollow(this);

	//初期状態
	ChangeState(STATE::STANBY);

	//HP初期化
	hp_ = MAX_HP;

	//フラグ初期化
	isGameOver_ = false;
	isGameClear_ = false;
}

void Player::ChangeState(STATE state)
{
	state_ = state;

	//状態に応じた処理
	switch (state_)
	{
	case Player::STATE::STANBY:
		ChangeStandby();
		break;
	case Player::STATE::KNOCKBACK:
		ChangeKnockBack();
		break;
	case Player::STATE::DEAD:
		ChangeDeath();
		break;
	case Player::STATE::END:
		ChangeEnd();
		break;
	case Player::STATE::ATTACK:
		ChangeAttack();
		break;
	case Player::STATE::VICTORY:
		ChangeVictory();
	}


}

void Player::ChangeStandby(void)
{
	//歩行アニメーションを再生
	animationController_->Play(static_cast<int>(ANIM_TYPE::WALK));

	//拡散光色をデフォルトに戻す
	MV1SetMaterialEmiColor(modelId_, 0, COLOR_DIF_DEFALT);
}

void Player::ChangeKnockBack(void)
{
	//ジャンプ判定にする
	isJump_ = true;

	//ノックバックカウンタをリセット
	cntKnockBack_ = 0;

	//武器フラグをオフにする
	if (useWeapon_->IsAlive())
	{
		useWeapon_->SetAlive(false);
	}


}

void Player::ChangeDeath(void)
{
	{
		//死亡アニメーションを再生（ループなし）
		animationController_->Play(static_cast<int>(ANIM_TYPE::DEATH), false);
	}
}

void Player::ChangeEnd(void)
{

}

void Player::ChangeAttack(void)
{
	animationController_->Play(static_cast<int>(ANIM_TYPE::IDLE_ATTACK), false);
	useWeapon_->Use(pos_, moveDir_); // ←向きを渡す

}

void Player::ChangeVictory(void)
{
	//勝利アニメーションを再生
	animationController_->Play(static_cast<int>(ANIM_TYPE::WAVE), true);
}

void Player::Update(void)
{

	//状態別更新
	switch (state_)
	{
	case Player::STATE::STANBY:
		UpdateStandby();
		break;
	case Player::STATE::KNOCKBACK:
		UpdateKnockBack();
		break;
	case Player::STATE::DEAD:
		UpdateDeath();
		break;
	case Player::STATE::END:
		UpdateEnd();
		break;
	case Player::STATE::ATTACK:
		UpdateAttack();
		break;
	case Player::STATE::VICTORY:
		UpdateVictory();
		break;
	}

	useWeapon_->Update();
	/*weaponAxe_->Update();
	weaponPunch_->Update();*/
	//animationController_->Update();

	//アニメーションの更新
	animationController_->Update();
}


void Player::UpdateStandby(void)
{
	//移動操作
	ProcessMove();

	//ジャンプ操作
	ProcessJump();

	//リスポーン
	Respawn();

	//攻撃
	ProcessAttack();

}

void Player::UpdateKnockBack(void)
{
	//着地したら通常状態へ戻す
	if (!isJump_)
	{
		ChangeState(STATE::STANBY);
		//以降の処理を行わない
		return;
	}

	cntKnockBack_++;

	//ジャンプ処理
	jumpPow_ -= GRAVITY_POW;
	pos_.y += jumpPow_;

	//ノックバック方向に移動させる
	pos_.x += knockBackDir_.x * SPEED_KNOCKBACK;
	pos_.z += knockBackDir_.z * SPEED_KNOCKBACK;

	MV1SetPosition(modelId_, pos_);

	//スポーン判定
	Respawn();
}

void Player::UpdateDeath(void)
{
	if (animationController_->IsEnd())
	{
		ChangeState(STATE::END);
	}
}

void Player::UpdateEnd(void)
{
	isGameOver_ = true;
}

void Player::UpdateAttack(void)
{
	if (animationController_->IsEnd())
	{
		useWeapon_->SetAlive(false);
		ChangeState(STATE::STANBY);
	}

	//// 斧投射の入力判定（例: Zキーで投げる）
	//if (CheckHitKey(KEY_INPUT_Z)) {
	//	if (weaponAxe_ && !weaponAxe_->IsAlive()) {
	//		weaponAxe_->Use(GetPos(), moveDir_);
	//	}
	//}
}

void Player::UpdateVictory(void)
{
	// プレイヤーが前（Z+方向）を向く
	angles_.y = 6.28f;
	MV1SetRotationXYZ(modelId_, angles_);

	// ゲームクリアフラグ
	isGameClear_ = true;
}


void Player::Draw(void)
{

	switch (state_)
	{
	case Player::STATE::STANBY:
		DrawStandby();
		break;
	case Player::STATE::KNOCKBACK:
		DrawKnockBack();
		break;
	case Player::STATE::ATTACK:
		DrawAttack();
		break;
	case Player::STATE::DEAD:
		DrawDeath();
		break;
	case Player::STATE::END:
		DrawEnd();
		break;
	case Player::STATE::VICTORY:
		DrawVictory();
		break;
	}

	useWeapon_->Draw();


	auto& input = InputManager::GetInstance();


	if (useWeapon_ == weaponAxe_)
	{
		DrawGraph(100, 100, axeImg_, true);
	}
	else if (useWeapon_ == weaponPunch_)
	{
		DrawGraph(100, 100, punchImg_, true);
	}
}

void Player::DrawStandby(void)
{
	//モデルの描画
	MV1DrawModel(modelId_);
	//モデルの位置を設定
	MV1SetPosition(modelId_, pos_);
}

void Player::DrawKnockBack(void)
{
	//フレーム数(cntKnockBack_で計測)で色を切り替える
	if ((cntKnockBack_ / TERM_BLINK) % 2 == 0)
	{
		//点滅
		MV1SetMaterialEmiColor(modelId_, 0, COLOR_DIF_BLINK);
	}
	else
	{
		//通常
		MV1SetMaterialEmiColor(modelId_, 0, COLOR_DIF_DEFALT);
	}

	//モデルの描画
	MV1DrawModel(modelId_);

	//位置の設定
	MV1SetPosition(modelId_, pos_);
}

void Player::DrawDeath(void)
{
	//モデルの描画
	MV1DrawModel(modelId_);

	//位置の設定
	MV1SetPosition(modelId_, pos_);
}

void Player::DrawEnd(void)
{

}

void Player::DrawAttack(void)
{
	//モデルの描画
	MV1DrawModel(modelId_);
	//位置の設定
	/*MV1SetPosition(modelId_, pos_);*/
}

void Player::DrawVictory(void)
{
	// 勝利アニメーションの描画
	MV1DrawModel(modelId_);
	// 位置の設定
	MV1SetPosition(modelId_, pos_);
}

void Player::Release(void)
{
	//モデルの削除
	MV1DeleteModel(modelId_);
	DeleteGraph(axeImg_, punchImg_);

	//アニメーションコントローラーの削除
	animationController_->Release();
	delete animationController_;

	useWeapon_->Release();
}

void Player::CollisionStage(VECTOR pos)
{
	// 衝突判定に指定座標に押し戻す
	pos_ = pos;
	jumpPow_ = 0.0f;
	isJump_ = false;
}

void Player::Respawn(void)
{

	if (pos_.y < DEATH_POS_X)
	{
		// プレイヤーの位置をリスポーン位置に戻す
		pos_.y = RESPAWN_POS_Y;
		pos_.x = STAGE_SIZE_X / 2.0f;
		pos_.z = STAGE_SIZE_Z / 2.0f;
		// ジャンプ力を初期化
		jumpPow_ = 0.0f;
		// ジャンプ判定を初期化
		isJump_ = false;
	}
}

void Player::KnockBack(VECTOR dirXZ, float jumpPow)
{
	//引数をメンバ変数に格納
	knockBackDir_ = dirXZ;
	jumpPow_ = jumpPow;

	//ノックバックに状態遷移
	ChangeState(STATE::KNOCKBACK);
}

void Player::Damage(int damage)
{
	//減算代入
	hp_ -= damage;
	//武器の生存フラグをリセット
	if (useWeapon_->IsAlive())
	{
		useWeapon_->SetAlive(false);
	}

	//HPが0より小さくなったとき
	if (hp_ == 0)
	{
		hp_ = 0;
		//死亡状態に遷移
		ChangeState(STATE::DEAD);
	}
}

int Player::GetHp(void)
{
	return hp_;
}

bool Player::IsInvincible(void)
{
	//ノックバック中、死亡、終了処理中は無敵
	return state_ == STATE::KNOCKBACK
		|| state_ == STATE::DEAD
		|| state_ == STATE::END;
}

void Player::PlayVictoryAnimation(void)
{
	// 勝利アニメーションを再生
	animationController_->Play(static_cast<int>(ANIM_TYPE::WAVE), false);
}

void Player::ProcessMove(void)
{
	if (state_ == STATE::ATTACK) return;
	// 移動制御
	auto& input = InputManager::GetInstance();
	VECTOR moveDir = AsoUtility::VECTOR_ZERO;

	if (input.IsNew(KEY_INPUT_W)) {
		moveDir.z += 1.0f;
	}
	if (input.IsNew(KEY_INPUT_S)) {
		moveDir.z -= 1.0f;
	}
	if (input.IsNew(KEY_INPUT_A)) {
		moveDir.x -= 1.0f;
	}
	if (input.IsNew(KEY_INPUT_D)) {
		moveDir.x += 1.0f;
	}
	if (moveDir.x != 0.0f || moveDir.z != 0.0f)
	{
		float len = sqrtf(moveDir.x * moveDir.x + moveDir.z * moveDir.z);
		moveDir.x /= len;
		moveDir.z /= len;
		moveDir_ = moveDir; // ←ここで保存
		// ...移動処理...
	}

	// 移動がある場合のみ処理
	if (moveDir.x != 0.0f || moveDir.z != 0.0f)
	{
		// 正規化
		float len = sqrtf(moveDir.x * moveDir.x + moveDir.z * moveDir.z);
		moveDir.x /= len;
		moveDir.z /= len;

		// 位置を更新
		pos_.x += moveDir.x * 10.0f;
		pos_.z += moveDir.z * 10.0f;

		// 向きを更新
		float angle = atan2f(moveDir.x, moveDir.z); // x,zの順に注意
		angles_.y = angle; // Y軸回転

		//モデル位置の補正
		angles_.y += AsoUtility::Deg2RadF(180.0f);

		//モデルの描画移動しているので歩くアニメーションを再生
		animationController_->Play(static_cast<int>(ANIM_TYPE::WALK), true);

	}
	else
	{
		//モデルの描画移動していないので待機アニメーションを再生
		animationController_->Play(static_cast<int>(ANIM_TYPE::IDLE));
	}


	//モデルの角度・位置を設定
	MV1SetPosition(modelId_, pos_);
	MV1SetRotationXYZ(modelId_, angles_);
}

void Player::ProcessJump(void)
{
	if (state_ == STATE::ATTACK) return;
	InputManager& ins = InputManager::GetInstance();

	//重力
	jumpPow_ -= GRAVITY_POW;

	//ジャンプ判定
	if (ins.IsTrgDown(KEY_INPUT_SPACE) && !isJump_)
	{
		//ジャンプ力を加算
		jumpPow_ = JUMP_POW;
		isJump_ = true;

		//アニメーションの再生
		animationController_->Play(static_cast<int>(ANIM_TYPE::JUMP));
	}

	//プレイヤーの座標に移動量を加算する
	pos_.y += jumpPow_;

	//地面に着いたら
	MV1SetPosition(modelId_, pos_);
}

void Player::ProcessAttack(void)
{
	auto& input = InputManager::GetInstance();
	if (input.IsTrgDown(KEY_INPUT_K) && !isJump_ && !useWeapon_->IsAlive())
	{
		ChangeState(STATE::ATTACK);
	}

	//Jキーで斧とパンチを切り替える
	if (input.IsTrgDown(KEY_INPUT_J))
	{
		if (useWeapon_ == weaponPunch_)
		{
			useWeapon_ = weaponAxe_;
		}
		else
		{
			useWeapon_ = weaponPunch_;
		}
	}
}