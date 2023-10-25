#pragma once
#include "IScene.h"
// components
#include "../components/Input.h"
// object
#include "../object/Sprite.h"
// base
#include "../base/WorldTransform.h"
#include "../base/ViewProjection.h"
#include "../base/Model.h"
#include "../object/Cube.h"

#include "../SceneTransition.h"

#include <memory>

class GameManager;

class TitleScene : public IScene
{
public:
	~TitleScene() = default;
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize()override;
private:
	Input* input_;
	// タイトル文字の位置
	WorldTransform worldTransform_;
	// 背景の位置
	WorldTransform backGroundWorldTransform_;
	// UIの数
	const static int kMaxUI = 4;
	WorldTransform UI_worldTransform_[kMaxUI];

	// タイトルの文字
	Sprite* titleName_;
	// 背景
	std::unique_ptr<Sprite> backGround_;
	// GamePadのAボタン
	std::unique_ptr<Sprite> guidePad_A_;

	// Press文字
	std::unique_ptr<Sprite> guideText_Press_;

	// チュートリアル
	std::unique_ptr<Sprite> guideText_Reflection_;
	std::unique_ptr<Sprite> guideText_Goal_;

	bool isTutorial_;
};