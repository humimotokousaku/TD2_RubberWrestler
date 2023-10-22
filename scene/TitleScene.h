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
};