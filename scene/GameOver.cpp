#include "GameOver.h"
#include "../Manager/GameManager.h"

void GameOver::Initialize() {
	// シーンの切り替え
	sceneNum = GAMEOVER_SCENE;

	// 基本機能
	input_ = Input::GetInstance();
	input_->Initialize();

	worldTransform_.Initialize();

	// ゲームオーバの文字
	gameOver_ = new Sprite();
	gameOver_->Initialize(false);
}

void GameOver::Update() {
	if (input_->TriggerKey(DIK_RETURN)) {
		sceneNum = GAMECLEAR_SCENE;
	}

	worldTransform_.UpdateMatrix();
}

void GameOver::Draw() {
	gameOver_->Draw(worldTransform_,GAMEOVER,kBlendModeNone, { 1,1,1,1 });
}

void GameOver::Finalize() {
	delete gameOver_;
	worldTransform_.constBuff_.ReleaseAndGetAddressOf();
}