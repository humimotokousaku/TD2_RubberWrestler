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
	titleName_->Initialize(false);
}

void TitleScene::Update() {
	if (input_->TriggerKey(DIK_RETURN)) {
		sceneNum = GAME_SCENE;
	}

	worldTransform_.UpdateMatrix();
}

void TitleScene::Draw() {
	titleName_->Draw(worldTransform_, TITLE, kBlendModeNone);
}

void TitleScene::Finalize() {
	delete titleName_;
	worldTransform_.constBuff_.ReleaseAndGetAddressOf();
}