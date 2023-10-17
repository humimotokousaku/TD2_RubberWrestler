#include "GameScene.h"
#include "../Manager/GameManager.h"

void GameScene::Initialize() {
	// 基本機能
	input_ = Input::GetInstance();
	input_->Initialize();

	viewProjection_.Initialize();

	//ダストモデルの読み込み
	dustModel_.reset(Model::CreateModelFromObj("resources/particle/dust", "dust.obj"));
	//残り火モデルの読み込み
	reFireModel_.reset(Model::CreateModelFromObj("resources/particle/reFire", "reFire.obj"));

	tEmitter_ = std::make_unique<tEmitter>();
	tEmitter_->Initialize({ 0,0,0 }, dustModel_, dustTextureHandle_, reFireModel_, reFireTextureHandle_);
}

void GameScene::Update() {
	if (input_->TriggerKey(DIK_SPACE)) {
		tEmitter_->OnCollision();
	}

	viewProjection_.UpdateViewMatrix();
	viewProjection_.TransferMatrix();
}

void GameScene::Draw() {
	tEmitter_->Draw(viewProjection_);
}

void GameScene::Finalize() {

}