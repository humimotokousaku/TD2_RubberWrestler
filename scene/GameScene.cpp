#include "GameScene.h"
#include "../Manager/GameManager.h"
#include <iostream>
#include <chrono>
#include <thread>

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
	//球モデルの読み込み
	whiteSphereModel_.reset(Model::CreateModelFromObj("resources/particle/dust", "dust.obj"));

	dustTextureHandle_ = DUST;
	reFireTextureHandle_ = REFIRE;

	viewProjection_.translation_.z = -50.0f;

	tEmitter_ = std::make_unique<tEmitter>();
	tEmitter_->Initialize({ 0,0,0 }, dustModel_.get(), dustTextureHandle_, reFireModel_.get(), reFireTextureHandle_);

	
}

void GameScene::Update() {

	//FPSを120に固定する処理///////////////////////////////////////////////////
	//auto startTime = std::chrono::high_resolution_clock::now();

	//auto endTime = std::chrono::high_resolution_clock::now();
	//std::chrono::duration<double> frameTime = endTime - startTime;

	//// もしフレーム時間が目標のフレーム時間よりも短い場合、スリープでウェイトを入れる
	//if (frameTime.count() < targetFrameTime) {
	//	std::this_thread::sleep_for(std::chrono::duration<double>(targetFrameTime - frameTime.count()));
	//}
	//////////////////////////////////////////////////////////////////////////

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