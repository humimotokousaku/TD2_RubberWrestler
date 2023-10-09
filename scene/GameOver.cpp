#include "GameOver.h"
#include "../Manager/GameManager.h"

void GameOver::Initialize() {
	// シーンの切り替え
	sceneNum = GAMEOVER_SCENE;

	// 基本機能
	input_ = Input::GetInstance();
	input_->Initialize();

	// ゲームオーバの文字
	gameOver_ = new Sprite();
	gameOver_->Initialize(false);
}

void GameOver::Update() {
	if (input_->TriggerKey(DIK_SPACE)) {
		sceneNum = GAMECLEAR_SCENE;
	}
}

void GameOver::Draw() {
	gameOver_->Draw(GAMEOVER);
}

void GameOver::Finalize() {
	delete gameOver_;
}