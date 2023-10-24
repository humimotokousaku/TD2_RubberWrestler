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
	gameOver_->Initialize(Vector3(0, 0, 0), Vector3(384 * 2, 128 * 2, 0), false);

	worldTransform_.translation_ = { 256,100,0 };

	guidePad_A_ = std::make_unique<Sprite>();
	guidePad_A_->Initialize(Vector3(0, 0, 0), Vector3(64, 64, 0), false);

	UI_worldTransform_.Initialize();
	UI_worldTransform_.translation_ = { 640,500,0 };
}

void GameOver::Update() {
	// シーンの切り替え
	if (input_->GamePadTrigger(XINPUT_GAMEPAD_A) || input_->TriggerKey(DIK_SPACE)) {
		SceneTransition::sceneChangeType_ = FADE_IN;
	}

	if (SceneTransition::GetInstance()->GetSceneChangeSignal()) {
		sceneNum = GAMECLEAR_SCENE;
	}

	worldTransform_.UpdateMatrix();
}

void GameOver::Draw() {
	// UI
	guidePad_A_->Draw(UI_worldTransform_, GUIDE_PAD_A, kBlendModeNone);
	// ゲームオーバ
	gameOver_->Draw(worldTransform_,GAMEOVER,kBlendModeNone);
}

void GameOver::Finalize() {
	delete gameOver_;
	worldTransform_.constBuff_.ReleaseAndGetAddressOf();
}