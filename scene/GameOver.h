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

	WorldTransform worldTransform_;
	// UIの数
	const static int kMaxUI = 2;
	WorldTransform UI_worldTransform_[kMaxUI];
	// 背景の位置
	WorldTransform backGroundWorldTransform_;

	Sprite* gameOver_;
	// GamePadのAボタン
	std::unique_ptr<Sprite> guidePad_A_;
	// GamePadのAボタン
	std::unique_ptr<Sprite> guideText_Press_;
	// 背景
	std::unique_ptr<Sprite> backGround_;
};

