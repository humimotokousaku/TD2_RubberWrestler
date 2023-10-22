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
	// シーンの切り替え
	if (input_->TriggerKey(DIK_RETURN)) {
		SceneTransition::sceneChangeType_ = FADE_IN;
	}

	if (SceneTransition::GetInstance()->GetSceneChangeSignal()) {
		sceneNum = GAMECLEAR_SCENE;
	}

	worldTransform_.UpdateMatrix();
}

void GameOver::Draw() {
	gameOver_->Draw(worldTransform_,GAMEOVER,kBlendModeNone);
}

void GameOver::Finalize() {
	delete gameOver_;
	worldTransform_.constBuff_.ReleaseAndGetAddressOf();
}