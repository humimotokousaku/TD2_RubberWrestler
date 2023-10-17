#pragma once
#include "IScene.h"
#include "../tEmitter.h"
#include "../components/Input.h"
#include "../base/ViewProjection.h"
#include "../base/Model.h"
#include "../object/Sprite.h"


class GameManager;
class GameScene : public IScene
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

	ViewProjection viewProjection_;

	//ダストのモデル
	std::unique_ptr<Model> dustModel_;

	//ダストのテクスチャハンドル
	uint32_t dustTextureHandle_;

	//残り火のモデル
	std::unique_ptr<Model> reFireModel_ = 0;

	//残り火のテクスチャハンドル
	uint32_t reFireTextureHandle_ = 0;

	std::unique_ptr<tEmitter> tEmitter_{};
};

