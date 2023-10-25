#include "TitleScene.h"
#include "../Manager/ImGuiManager.h"

void TitleScene::Initialize() {
	// シーンの切り替え
	sceneNum = TITLE_SCENE;

	// 基本機能
	input_ = Input::GetInstance();
	input_->Initialize();

	worldTransform_.Initialize();

	// タイトル文字
	titleName_ = new Sprite();
	titleName_->Initialize(Vector3(0, 0, 0), Vector3(384 * 2, 128 * 2, 0), false);

	worldTransform_.translation_ = { 256,100,0 };

	guidePad_A_ = std::make_unique<Sprite>();
	guidePad_A_->Initialize(Vector3(0, 0, 0), Vector3(64, 64, 0), false);

	guideText_Press_ = std::make_unique<Sprite>();
	guideText_Press_->Initialize(Vector3(0, 0, 0), Vector3(128, 64, 0), false);

	for (int i = 0; i < kMaxUI; i++) {
		UI_worldTransform_[i].Initialize();
	}

	UI_worldTransform_[0].translation_ = { 660,500,0 };
	UI_worldTransform_[1].translation_ = { 512,500,0 };
}

void TitleScene::Update() {
	if (input_->GamePadTrigger(XINPUT_GAMEPAD_A) || input_->TriggerKey(DIK_SPACE)) {
		SceneTransition::sceneChangeType_ = FADE_IN;
	}

	if (SceneTransition::GetInstance()->GetSceneChangeSignal()) {
		sceneNum = GAME_SCENE;
	}

	worldTransform_.UpdateMatrix();
}

void TitleScene::Draw() {
	// UI
	guidePad_A_->Draw(UI_worldTransform_[0], GUIDE_PAD_A, kBlendModeNone, {1,1,1,1});
	guideText_Press_->Draw(UI_worldTransform_[1], GUIDE_TEXT_PRESS, kBlendModeNone, { 1,1,1,1 });
	titleName_->Draw(worldTransform_, TITLE, kBlendModeNone,{1,1,1,1});
}

void TitleScene::Finalize() {
	delete titleName_;
	worldTransform_.constBuff_.ReleaseAndGetAddressOf();
}