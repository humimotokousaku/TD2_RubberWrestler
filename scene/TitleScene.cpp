#include "TitleScene.h"
#include "../Manager/ImGuiManager.h"

void TitleScene::Initialize() {
	// シーンの切り替え
	sceneNum = TITLE_SCENE;

	// 基本機能
	input_ = Input::GetInstance();
	input_->Initialize();

	// タイトル文字
	titleName_ = new Sprite();
	titleName_->Initialize(false);
}

void TitleScene::Update() {
	if (input_->TriggerKey(DIK_SPACE)) {
		sceneNum = GAME_SCENE;
	}
}

void TitleScene::Draw() {
	titleName_->Draw(TITLE);
}

void TitleScene::Finalize() {
	delete titleName_;
}