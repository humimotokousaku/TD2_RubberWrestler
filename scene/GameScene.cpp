#include "GameScene.h"
#include "../Manager/GameManager.h"

void GameScene::Initialize() {
	sceneNum = GAME_SCENE;
	// 基本機能の生成
	audio_ = Audio::GetInstance();
	input_ = Input::GetInstance();
	input_->Initialize();

	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	// 自機の生成
	player_ = std::make_unique<Player>();
	player_->Initialize();
}

void GameScene::Update() {
	viewProjection_.UpdateMatrix();

	// 自機
	player_->Update();

}

void GameScene::Draw() {
	player_->Draw(viewProjection_);
}

void GameScene::Finalize() {
	player_->Finalize();
	viewProjection_.constBuff_.ReleaseAndGetAddressOf();
}