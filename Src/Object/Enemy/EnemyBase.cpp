
#include "EnemyBase.h"
#include "../../Application.h"
#include "../../Utility/AsoUtility.h"
#include "../Stage/BlockManager.h"
#include "../Common/AnimationController.h"
#include "../Player.h"
#include "../Shot/ShotStraight.h"

EnemyBase::EnemyBase(void)
{
	int ebase = 0;
	ebase = 1;
}

EnemyBase::~EnemyBase(void)
{
	int ebaseD = 0;
	ebaseD = 1;
}

void EnemyBase::Init(TYPE type,
	int baseModelId, int baseAttackEffectModelId, Player* player)

{
	// エネミー種別
	type_ = type;

	// プレイヤーのポインタを格納
	player_ = player;

	// モデルの複製
	modelId_ = MV1DuplicateModel(baseModelId);

	// エフェクトモデルID
	baseAttackEffectModelId_ = baseAttackEffectModelId;

	// パラメータ設定
	SetParam();

	// 大きさ
	MV1SetScale(modelId_, scales_);

	// 出現座標の設定
	SetSpawnPosition();

	// プレイヤーの方向を向く
	LookPlayer();

	// 移動処理
	Move();

	// 色の調整
	MV1SetMaterialEmiColor(modelId_, 0, COLOR_EMI_DEFAULT);

	// モデルアニメーション制御の初期化
	animationController_ = new AnimationController(modelId_);
	for (int i = 0; i < static_cast<int>(ANIM_TYPE::MAX); i++)
	{
		animationController_->AddInFbx(i, 30.0f, i);
	}

	// 初期アニメーション再生
	animationController_->Play(static_cast<int>(ANIM_TYPE::WALK));

	// 初期状態
	ChangeState(STATE::STANDBY);
}

void EnemyBase::Update(void)
{
	switch (state_)
	{
	case EnemyBase::STATE::STANDBY:
		UpdateStandby();
		break;
	case EnemyBase::STATE::ATTACK:
		UpdateAttack();
		break;
	case EnemyBase::STATE::HIT_REACT:
		HitRectUpdate();
		break;
	case EnemyBase::STATE::DEAD_REACT:
		DeathUpdate();
		break;
	case EnemyBase::STATE::END:
		EndUpdate(); // 状態維持用。移動などしない。
		break;
	}

	UpdateShot();
	animationController_->Update();
}

void EnemyBase::HitRectUpdate(void)
{
	stateCnt_++;

	if (stateCnt_ > 20)
	{
		if (hp_ > 0)
		{
			ChangeState(STATE::STANDBY);
		}
		else
		{
			ChangeState(STATE::DEAD_REACT);
		}
	}
}

void EnemyBase::DeathUpdate(void)
{
	stateCnt_++;
	if (stateCnt_ > 30 && animationController_->IsEnd())
	{
		ChangeState(STATE::END);
	}
}

void EnemyBase::EndUpdate(void)
{
}

void EnemyBase::Draw(void)
{
	// DEAD_REACTやEND状態は描画しない、または透明化などする
	if (!isAlive_) return;

	switch (state_)
	{
	case EnemyBase::STATE::STANDBY:
		DrawStandby();
		break;
	case EnemyBase::STATE::ATTACK:
		DrawAttack();
		break;
	case EnemyBase::STATE::HIT_REACT:
		DrawHitReact();
		break;
	case EnemyBase::STATE::DEAD_REACT:
		DrawDeathReact();
		break;
	case EnemyBase::STATE::END:
		DrawEnd();
		break;
	}

	DrawShot();
}

void EnemyBase::Release(void)
{
	// モデルのメモリ解放
	MV1DeleteModel(modelId_);

	// アニメーションコントローラの解放
	animationController_->Release();
	delete animationController_;

	// 弾の解放
	for (ShotBase* shot : shots_)
	{
		shot->Release();
		delete shot;
	}
	shots_.clear();
}

void EnemyBase::Damage(int damage)
{
	if (!isAlive_) return; // すでに死んでいるなら無視

	hp_ -= damage;

	if (hp_ > 0)
	{
		ChangeState(STATE::HIT_REACT);
		isAlive_ = true;
	}
	else
	{
		ChangeState(STATE::DEAD_REACT);
		isAlive_ = false;
	}
}

void EnemyBase::ChangeState(STATE state)
{
	state_ = state;

	switch (state_)
	{
	case EnemyBase::STATE::STANDBY:
		ChangeStandby();
		break;
	case EnemyBase::STATE::ATTACK:
		ChangeAttack();
		break;
	}
}

VECTOR EnemyBase::GetPos_(void)
{
	return pos_;
}

float EnemyBase::GetCollisionRadius_(void)
{
	return collisionRadius_;
}

std::vector<ShotBase*> EnemyBase::GetShots(void)
{
	return shots_;
}

void EnemyBase::LookPlayer(void)
{
	// プレイヤーの座標を取得
	VECTOR playerPos = player_->GetPos();

	// 相手へのベクトルを計算
	VECTOR diff = VSub(playerPos, pos_);
	diff.y = 0.0f;

	// ベクトルの正規化で単位ベクトルを取得する
	moveDir_ = VNorm(diff);

	//方向から角度（ラジアン）に変換する
	angles_.y = atan2(moveDir_.x, moveDir_.z);

	// モデルの方向が正の負の方向を向いているので、補正する
	angles_.y += AsoUtility::Deg2RadF(180.0f);

	// 回転はY軸のみとする
	angles_.x = angles_.z = 0.0f;

	// モデルに角度を設定
	MV1SetRotationXYZ(modelId_, angles_);
}

void EnemyBase::Move(void)
{
	// 移動量
	VECTOR movePow = VScale(moveDir_, speed_);

	// 移動処理（座標＋移動量）
	pos_ = VAdd(pos_, movePow);

	// 移動制限処理
	if (pos_.x < 0.0f)
	{
		pos_.x = 0.0f;
	}

	if (pos_.x > BlockManager::WORLD_SIZE)
	{
		pos_.x = BlockManager::WORLD_SIZE;
	}

	if (pos_.z < 0.0f)
	{
		pos_.z = 0.0f;
	}

	if (pos_.z > BlockManager::WORLD_SIZE)
	{
		pos_.z = BlockManager::WORLD_SIZE;
	}

	// モデルに座標を設定する
	MV1SetPosition(modelId_, pos_);
}

void EnemyBase::SetSpawnPosition(void)
{
	// ステージの半分
	float radius = BlockManager::WORLD_SIZE / 2.0f;

	// ステージの中心座標
	VECTOR centerPos = { radius, 0.0f, radius };

	// 出現範囲の調整
	radius *= spawnRange_;

	// ランダムな角度を取得
	int degree = GetRand(360);
	float radian = static_cast<float>(degree) * DX_PI_F / 180.0f;

	// 角度から方向を決める
	float dirX = sinf(radian);
	float dirZ = cosf(radian);
	VECTOR dir = { dirX, 0.0f, dirZ };

	// 方向×長さ（長さが半径分のランダムなベクトル）
	VECTOR vec = VScale(dir, radius);

	// ステージの中心座標にベクトルを加筆して
	// 円周上の座標を計算する
	VECTOR pos = VAdd(centerPos, vec);

	pos_ = pos;
	MV1SetPosition(modelId_, pos_);
}

void EnemyBase::SetParam(void)
{
}

void EnemyBase::ChangeStandby(void)
{
	// 歩くアニメーション再生
	animationController_->Play(static_cast<int>(ANIM_TYPE::WALK));
}

void EnemyBase::ChangeAttack(void)
{
	// 攻撃アニメーション再生
	animationController_->Play(static_cast<int>(ANIM_TYPE::ATTACK), false);
	
	// 有効な弾を取得する
	ShotBase* shot = GetValidShot();

	// 弾を生成
	shot->CreateShot(pos_, moveDir_);

}

void EnemyBase::UpdateStandby(void)
{
	if (!isAlive_) return; // 死亡していたら動かない

	LookPlayer();
	Move();
}

void EnemyBase::UpdateAttack(void)
{
	// 攻撃アニメーションが終わったら通常状態に戻す
	if (animationController_->IsEnd())
	{
		ChangeState(STATE::STANDBY);
	}
}

void EnemyBase::DrawStandby(void)
{
	// モデルの描画
	MV1DrawModel(modelId_);
}

void EnemyBase::DrawAttack(void)
{
	// モデルの描画
	MV1DrawModel(modelId_);
}

void EnemyBase::DrawHitReact()
{
}

void EnemyBase::DrawDeathReact()
{
}

void EnemyBase::DrawEnd()
{
}

void EnemyBase::UpdateShot(void)
{	
	for (ShotBase* shot : shots_)
	{
		shot->Update();
	}
}

void EnemyBase::DrawShot(void)
{
	for (ShotBase* shot : shots_)
	{
		shot->Draw();
	}
}

ShotBase* EnemyBase::GetValidShot(void)
{
	size_t size = shots_.size();
	for (int i = 0; i < size; i++)
	{
		// 未使用(生存していない)
		if (!shots_[i]->IsAlive())
		{
			return shots_[i];
		}
	}

	// 新しい弾のインスタンスを生成する
	ShotBase* shot = new ShotStraight(
		ShotBase::TYPE::STRAIGHT, baseAttackEffectModelId_);
	
	// 可変長配列に追加
	shots_.push_back(shot);
	return shot;
}