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

	// 自機
	std::unique_ptr<Player> player_;

	// カメラ
	ViewProjection viewProjection_;
};

