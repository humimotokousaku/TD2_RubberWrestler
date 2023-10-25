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
	WorldTransform worldTransform_;


	// タイトルの文字
	Sprite* titleName_;
	// GamePadのAボタン
	std::unique_ptr<Sprite> guidePad_A_;

	// GamePadのAボタン
	std::unique_ptr<Sprite> guideText_Press_;
	// UIの数
	const static int kMaxUI = 2;
	WorldTransform UI_worldTransform_[kMaxUI];
};