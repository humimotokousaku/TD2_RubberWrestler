#include "GameScene.h"
#include "../Manager/GameManager.h"

void GameScene::Initialize() {
	// シーンの切り替え
	sceneNum = GAME_SCENE;
	// 基本機能
	input_ = Input::GetInstance();
	input_->Initialize();

	viewProjection_.Initialize();

	//ダストモデルの読み込み
	dustModel_.reset(Model::CreateModelFromObj("resources/particle/dust", "dust.obj"));
	//残り火モデルの読み込み
	reFireModel_.reset(Model::CreateModelFromObj("resources/particle/reFire", "reFire.obj"));

	dustTextureHandle_ = DUST;
	reFireTextureHandle_ = REFIRE;

	tEmitter_ = std::make_unique<tEmitter>();
	tEmitter_->Initialize({ 0,0,0 }, dustModel_.get(), dustTextureHandle_, reFireModel_.get(), reFireTextureHandle_);
}

void GameScene::Update() {
	if (input_->TriggerKey(DIK_SPACE)) {
		tEmitter_->OnCollision();
	}
	
	tEmitter_->Update();

	viewProjection_.UpdateViewMatrix();
	viewProjection_.TransferMatrix();
}

void GameScene::Draw() {
	tEmitter_->Draw(viewProjection_);
}

void GameScene::Finalize() {

}