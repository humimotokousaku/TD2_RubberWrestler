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
	WorldTransform UI_worldTransform_;

	// タイトルの文字
	Sprite* titleName_;
	// GamePadのAボタン
	std::unique_ptr<Sprite> guidePad_A_;
};