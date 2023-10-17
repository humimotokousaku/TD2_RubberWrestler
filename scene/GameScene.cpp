#include "GameScene.h"
#include "../Manager/GameManager.h"

void GameScene::Initialize() {
	// 基本機能
	input_ = Input::GetInstance();
	input_->Initialize();

	viewProjection_.Initialize();

	modelFighterBody_.reset(Model::CreateModelFromObj("resources/float_Body", "float_Body.obj"));
	//ダストモデルの読み込み
	dustModel_.reset(Model::CreateModelFromObj("resources/particle/dust", "dust.obj"));
	//残り火モデルの読み込み
	reFireModel_.reset(Model::CreateModelFromObj("resources/particle/reFire", "reFire.obj"));
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