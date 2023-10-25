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
#include "../tEmitter.h"
#include "../gameObject/Rope.h"

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
	std::unique_ptr<Model> modelUvSphere_;
	std::unique_ptr<Model> modelCube_;

	// 敵の3Dモデル
	std::unique_ptr<Model> modelEnemyBody_;
	std::unique_ptr<Model> modelEnemyHead_;
	std::unique_ptr<Model> modelEnemyL_arm_;
	std::unique_ptr<Model> modelEnemyR_arm_;

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

	std::unique_ptr<Sprite> sprite_;

	WorldTransform worldTransform_[2];

	// 追従カメラ
	enum CameraType {
		CAMERA1,
		CAMERA2
	};

	const static int kMaxFollowCamera_ = 2;
	std::unique_ptr<FollowCamera> followCamera_[kMaxFollowCamera_];


	//ロープ
	std::unique_ptr<Rope> bottomRope_[3];
	std::unique_ptr<Rope> topRope_[3];
	std::unique_ptr<Rope> rightRope_[3];
	std::unique_ptr<Rope> leftRope_[3];

	// カメラ
	ViewProjection mainCamera_;
	ViewProjection viewProjection_;

	Vector3 Velocity_{};

	//ダストのモデル
	std::unique_ptr<Model> dustModel_;

	//ダストのテクスチャハンドル
	uint32_t dustTextureHandle_;

	//残り火のモデル
	std::unique_ptr<Model> reFireModel_;

	//残り火のテクスチャハンドル
	uint32_t reFireTextureHandle_;

	//球のモデル
	std::unique_ptr<Model> whiteSphereModel_;

	//球のテクスチャハンドル
	uint32_t whiteSphere_;

	std::unique_ptr<tEmitter> tEmitter_{};


	const double targetFPS = 120.0;
	const double targetFrameTime = 1.0 / targetFPS;

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
};

