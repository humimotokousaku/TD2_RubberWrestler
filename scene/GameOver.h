#pragma once
#include "IScene.h"
// object
#include "../object/Sprite.h"
// components
#include "../components/Input.h"

class GameManager;
class GameOver : public IScene
{
public:
	// 初期化
	void Initialize()override;

	// 更新処理
	void Update()override;

	// 描画
	void Draw()override;

	void Finalize()override;
private:
	Input* input_;
	Sprite* gameOver_;
};

