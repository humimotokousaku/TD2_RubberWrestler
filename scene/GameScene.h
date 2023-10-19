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
#include "../gameObject/Rope.h"
// object
#include "../object/Sprite.h"

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

	// 自機
	std::unique_ptr<Player> player_;

	//敵
	std::unique_ptr<Enemy> enemy_;

	//ロープ
	std::unique_ptr<Rope> bottomRope_[3];
	std::unique_ptr<Rope> topRope_[3];
	std::unique_ptr<Rope> rightRope_[3];
	std::unique_ptr<Rope> leftRope_[3];

	// カメラ
	ViewProjection viewProjection_;
};

