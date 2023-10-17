#include "GameScene.h"
#include "../Manager/GameManager.h"

void GameScene::Initialize() {
	// 基本機能
	input_ = Input::GetInstance();
	input_->Initialize();

	viewProjection_.Initialize();

	//ダストモデルの読み込み
	dustModel_ = Model::CreateModelFromObj("../resources/Particle/Dust", "dust");
	//残り火モデルの読み込み
	reFireModel_ = Model::CreateModelFromObj("../resources/Particle/ReFire", "ReFire");
}

void GameScene::Update() {
	if (input_->TriggerKey(DIK_SPACE)) {
		temitter->OnCollision();
	}

	viewProjection_.UpdateViewMatrix();
	viewProjection_.TransferMatrix();
}

void GameScene::Draw() {
	temitter->Draw(viewProjection_);
}

void GameScene::Finalize() {

}