#pragma once
#include "IScene.h"
#include "../components/Input.h"
#include "../object/Sphere.h"
#include "../base/WorldTransform.h"
#include "../base/ViewProjection.h"
#include "../base/Model.h"
#include "../object/Cube.h"

class GameManager;

class TitleScene : public IScene
{
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize()override;
private:
	const static int kMaxCube = 2;
	Model* block_;
	Model* axis_;

	int textureNum_;
	Input* input_;
	Vector3 pos_;
	WorldTransform cubeWorldTransform_[kMaxCube];
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;
};