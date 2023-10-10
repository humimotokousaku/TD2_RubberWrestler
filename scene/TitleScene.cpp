#include "TitleScene.h"
#include "../Manager/ImGuiManager.h"

void TitleScene::Initialize() {
	sceneNum = TITLE_SCENE;
	// 基本機能
	input_ = Input::GetInstance();
	input_->Initialize();

	viewProjection_.Initialize();
}

void TitleScene::Update() {
	viewProjection_.UpdateViewMatrix();
	viewProjection_.TransferMatrix();
}

void TitleScene::Draw() {
}

void TitleScene::Finalize() {
	viewProjection_.constBuff_.ReleaseAndGetAddressOf();
}