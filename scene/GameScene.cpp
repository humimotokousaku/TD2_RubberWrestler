#include "GameScene.h"
#include "../Manager/GameManager.h"
#include <iostream>
#include <chrono>
#include <thread>

void GameScene::Initialize() {
	// シーンの切り替え
	sceneNum = GAME_SCENE;
	// 基本機能の生成
	audio_ = Audio::GetInstance();
	input_ = Input::GetInstance();
	input_->Initialize();

	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	// 自機
	modelFighterBody_.reset(Model::CreateModelFromObj("resources/float_Body", "float_Body.obj"));
	modelFighterHead_.reset(Model::CreateModelFromObj("resources/float_Head", "float_Head.obj"));
	modelFighterL_arm_.reset(Model::CreateModelFromObj("resources/float_L_arm", "float_L_arm.obj"));
	modelFighterR_arm_.reset(Model::CreateModelFromObj("resources/float_R_arm", "float_R_arm.obj"));
	std::vector<Model*> playerModels = {
		modelFighterBody_.get(), modelFighterHead_.get(), modelFighterL_arm_.get(),
		modelFighterR_arm_.get()};
	// リングのマット
	modelRingMat_.reset(Model::CreateModelFromObj("resources/ring", "Ground.obj"));
	// 天球
	modelSkydome_.reset(Model::CreateModelFromObj("resources/skydome", "skydome.obj"));

	// カメラの位置と向き
	viewProjection_.translation_.y = 11;
	viewProjection_.translation_.z = -40;
	viewProjection_.rotation_.x = 3.14f / 10.0f;

	//敵の生成
	enemy_ = std::make_unique<Enemy>();
	enemy_->Initialize(playerModels);

	// 自機の生成
	player_ = std::make_unique<Player>();
	player_->Initialize(playerModels);
	player_->SetViewProjection(&viewProjection_);
	player_->SetEnemy(enemy_.get());
	//player_->SetEnemyPearent(&enemy_->GetWorldTransform());

	//ダストモデルの読み込み
	dustModel_.reset(Model::CreateModelFromObj("resources/particle/dust", "dust.obj"));
	//残り火モデルの読み込み
	reFireModel_.reset(Model::CreateModelFromObj("resources/particle/reFire", "reFire.obj"));
	//球モデルの読み込み
	//whiteSphereModel_.reset(Model::CreateModelFromObj("resources/particle/dust", "dust.obj"));

	dustTextureHandle_ = DUST;
	reFireTextureHandle_ = REFIRE;

	viewProjection_.translation_.z = -50.0f;

	tEmitter_ = std::make_unique<tEmitter>();
	tEmitter_->Initialize(player_->GetWorldPosition(), dustModel_.get(), dustTextureHandle_, reFireModel_.get(), reFireTextureHandle_);

	enemy_->SetPlayer(player_.get());

	// リングのマットの生成
	ringMat_ = std::make_unique<RingMat>();
	ringMat_->Initialize(modelRingMat_.get());

	// 天球
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(modelSkydome_.get());
}

void GameScene::Update() {
	viewProjection_.UpdateMatrix();

	// 自機
	player_->Update();

	Velocity_ = player_->GetThrowVelocity();

	enemy_->SetVelocity(Velocity_);

	//敵
	enemy_->Update();

	// リングのマット
	ringMat_->Update();

	// 天球
	skydome_->Update();

	/*//FPSを120に固定する処理///////////////////////////////////////////////////
	auto startTime = std::chrono::high_resolution_clock::now();

	auto endTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> frameTime = endTime - startTime;

	// もしフレーム時間が目標のフレーム時間よりも短い場合、スリープでウェイトを入れる
	if (frameTime.count() < targetFrameTime) {
		std::this_thread::sleep_for(std::chrono::duration<double>(targetFrameTime - frameTime.count()));
	}
	//////////////////////////////////////////////////////////////////////////*/

	if (input_->TriggerKey(DIK_SPACE)) {
		tEmitter_->OnCollision(player_->GetWorldPosition());
	}

	tEmitter_->Update();

	// シーンの切り替え
	if (input_->TriggerKey(DIK_RETURN)) {
		SceneTransition::sceneChangeType_ = FADE_IN;
	}

	if (SceneTransition::GetInstance()->GetSceneChangeSignal()) {
		sceneNum = GAMEOVER_SCENE;
	}
}

void GameScene::Draw() {
	player_->Draw(viewProjection_, WHITE);
	tEmitter_->Draw(viewProjection_);
	enemy_->Draw(viewProjection_, WHITE);
	ringMat_->Draw(viewProjection_, UVCHEKER);
	skydome_->Draw(viewProjection_, BACKGROUND);
}

void GameScene::Finalize() {
	player_->Finalize();
	viewProjection_.constBuff_.ReleaseAndGetAddressOf();
}