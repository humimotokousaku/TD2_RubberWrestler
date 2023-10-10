#pragma once
#include "IScene.h"
#include "../components/Input.h"
#include "../base/ViewProjection.h"
#include "../base/Model.h"
#include "../object/Sprite.h"

class GameManager;

class TitleScene : public IScene
{
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize()override;
private:
	Input* input_;
	ViewProjection viewProjection_;
};