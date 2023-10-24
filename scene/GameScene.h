#pragma once
#include "IScene.h"
// base
#include "../base/ViewProjection.h"
#include "../base/Model.h"
// components
#include "../components/Audio.h"
#include "../components/Input.h"
// gameObject
#include "../gameObject/Player/Player.h"
#include "../gameObject/Enemy/Enemy.h"
#include "../gameObject/Ring/Mat/RingMat.h"
#include "../Skydome.h"
// object
#include "../object/Sprite.h"

#include "../FollowCamera.h"

#include <memory>


class GameManager;
class GameScene : public IScene
{
public:
	~GameScene() = default;
	// 初期化
	void Initialize()override;

	// 更新処理
	void Update()override;

	// 描画
	void Draw()override;

	void Finalize()override;
private:
	// 基本機能
	Audio* audio_;
	Input* input_;

	// 自機の3Dモデル
	std::unique_ptr<Model> modelFighterBody_;
	std::unique_ptr<Model> modelFighterHead_;
	std::unique_ptr<Model> modelFighterL_arm_;
	std::unique_ptr<Model> modelFighterR_arm_;
	// リングのマット
	std::unique_ptr<Model> modelRingMat_;
	// 天球
	std::unique_ptr<Model> modelSkydome_;

	// 自機
	std::unique_ptr<Player> player_;
	//敵
	std::unique_ptr<Enemy> enemy_;
	// リングのマット
	std::unique_ptr<RingMat> ringMat_;
	// 天球
	std::unique_ptr<Skydome> skydome_;

	// UIのテクスチャ

	// GamePadのAボタン
	std::unique_ptr<Sprite> guidePad_A_;
	// つかみの文字
	std::unique_ptr<Sprite> guideText_Grab_;
	// 投げの文字
	std::unique_ptr<Sprite> guideText_Throw_;
	// UIの数
	const static int kMaxUI = 3;
	WorldTransform UI_worldTransform_[kMaxUI];

	// 追従カメラ
	enum CameraType {
		CAMERA1,
		CAMERA2
	};

	const static int kMaxFollowCamera_ = 2;
	std::unique_ptr<FollowCamera> followCamera_[kMaxFollowCamera_];

	// カメラ
	ViewProjection mainCamera_;
	ViewProjection viewProjection_;

	Vector3 Velocity_{};
};

