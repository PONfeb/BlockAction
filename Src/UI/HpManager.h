
#pragma once

#include "../Object/Player.h"
#include <vector>

class Player;

class HpManager
{
public:

	//HPアイコンにスケール
	struct HeartIcon {
		float scale = 1.0f;
	};

	// ＨＰ描画の開始座標(X)
	static constexpr int START_X = 60;

	// ＨＰ描画の開始座標(Y)
	static constexpr int START_Y = 50;

	// ＨＰアイコンの描画幅
	static constexpr int HP_ICON_WIDTH = 100;
	
	// コンストラクタ
	HpManager(Player* player);
	// デストラクタ
	~HpManager(void);

	void Init(void);
	//アニメーション開始
	void StartAnim();
	void Update(void);
	void Draw(void);
	void Release(void);

	void GetImageSize(int img, float& centerX, float& centerY);

	float easeOutBounce(float x);
private:

	// プレイヤー情報
	Player* player_;

	HeartIcon hearts_[Player::MAX_HP];

	// ＨＰアイコン画像ハンドル
	int imgHeart_;

	// 失ったＨＰアイコン画像ハンドル
	int imgHeartLost_;
};