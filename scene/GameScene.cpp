#include "GameScene.h"
#include "../Manager/GameManager.h"
#include <iostream>
#include <chrono>
#include <thread>
#define _USE_MATH_DEFINES
#include <math.h>

void GameScene::Initialize() {
	// シーンの切り替え
	sceneNum = GAME_SCENE;
	// 基本機能の生成
	audio_ = Audio::GetInstance();
	input_ = Input::GetInstance();
	input_->Initialize();

	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
	mainCamera_.Initialize();

	// 自機
	modelFighterBody_.reset(Model::CreateModelFromObj("resources/player/float_Body", "float_Body.obj"));
	modelFighterHead_.reset(Model::CreateModelFromObj("resources/player/float_Head", "float_Head.obj"));
	modelFighterL_arm_.reset(Model::CreateModelFromObj("resources/player/float_L_arm", "float_L_arm.obj"));
	modelFighterR_arm_.reset(Model::CreateModelFromObj("resources/player/float_R_arm", "float_R_arm.obj"));
	std::vector<Model*> playerModels = {
		modelFighterBody_.get(), modelFighterHead_.get(), modelFighterL_arm_.get(),
		modelFighterR_arm_.get()};

	modelEnemyBody_.reset(Model::CreateModelFromObj("resources/enemy/float_Body", "float_Body.obj"));
	modelEnemyHead_.reset(Model::CreateModelFromObj("resources/enemy/float_Head", "float_Head.obj"));
	modelEnemyL_arm_.reset(Model::CreateModelFromObj("resources/enemy/float_L_arm", "float_L_arm.obj"));
	modelEnemyR_arm_.reset(Model::CreateModelFromObj("resources/enemy/float_R_arm", "float_R_arm.obj"));
	std::vector<Model*> enemyModels = {
		modelEnemyBody_.get(), modelEnemyHead_.get(), modelEnemyL_arm_.get(),
		modelEnemyR_arm_.get() };

	// リングのマット
	modelRingMat_.reset(Model::CreateModelFromObj("resources/ring", "Ground.obj"));
	// 天球
	modelSkydome_.reset(Model::CreateModelFromObj("resources/skydome", "skydome.obj"));

	// カメラの位置と向き
	//viewProjection_.translation_.y = 11;
	//viewProjection_.translation_.z = -40;
	//viewProjection_.rotation_.x = 3.14f / 10.0f;

	mainCamera_.translation_.y = 11;
	mainCamera_.translation_.z = -40;
	mainCamera_.rotation_.x = 3.14f / 10.0f;



	//敵の生成
	enemy_ = std::make_unique<Enemy>();

	// 自機の生成
	player_ = std::make_unique<Player>();
	player_->Initialize(playerModels);
	player_->SetViewProjection(&viewProjection_);

	enemy_->SetPlayer(player_.get());
	enemy_->Initialize(enemyModels);

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

	// リングのマットの生成
	ringMat_ = std::make_unique<RingMat>();
	ringMat_->Initialize(modelRingMat_.get());

	// 天球
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(modelSkydome_.get());

	// 追従カメラの生成
	for (int i = 0; i < kMaxFollowCamera_; i++) {
		followCamera_[i] = std::make_unique<FollowCamera>();
		followCamera_[i]->SetTarget(&player_->GetWorldTransform());
	}

	// 1カメ
	followCamera_[0]->Initialize(Vector3({ 0.0f, 2.0f, -20.0f }), Vector3(0, -(float)M_PI / 3, 0));
	// 2カメ
	followCamera_[1]->Initialize(Vector3({ 0.0f, 2.0f, -30.0f }), Vector3((float)M_PI / 2, (float)M_PI / 2, 0));

	// 最初はmainカメラ
	viewProjection_ = mainCamera_;
	player_->SetViewProjection(&viewProjection_);
}

void GameScene::Update() {
	viewProjection_.UpdateMatrix();
	mainCamera_.UpdateMatrix();

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
		tEmitter_->OnCollision(enemy_->GetWorldPosition());
	}

	tEmitter_->Update();

	// シーンの切り替え
	if (input_->TriggerKey(DIK_RETURN)) {
		SceneTransition::sceneChangeType_ = FADE_IN;
	}

	if (SceneTransition::GetInstance()->GetSceneChangeSignal()) {
		sceneNum = GAMEOVER_SCENE;
	}

	// Activeになっているカメラを使う
	for (int i = 0; i < kMaxFollowCamera_; i++) {
		if (followCamera_[i]->GetActive()) {
			followCamera_[i]->Update();
			viewProjection_.matView = followCamera_[i]->GetViewProjection().matView;
			viewProjection_.matProjection = followCamera_[i]->GetViewProjection().matProjection;
			viewProjection_ = followCamera_[i]->GetViewProjection();
			viewProjection_.UpdateMatrix();
			viewProjection_.TransferMatrix();
		}
	}
	// どちらのカメラもActiveじゃなかったらメインカメラを使う
	//if (!followCamera_[CAMERA1]->GetActive() && !followCamera_[CAMERA2]->GetActive()) {
	//	viewProjection_ = mainCamera_;
	//}

	if (player_->GetCameraArr() == 0) {
		followCamera_[CAMERA1]->SetActive(false);
		followCamera_[CAMERA2]->SetActive(false);
		viewProjection_ = mainCamera_;
	}
	if (player_->GetCameraArr() == 1) {
		followCamera_[CAMERA2]->SetActive(false);
		followCamera_[CAMERA1]->SetActive(true);
	}
	if (player_->GetCameraArr() == 2) {
		followCamera_[CAMERA1]->SetActive(false);
		followCamera_[CAMERA2]->SetActive(true);
	}

	//// 片方がActiveならfalseにする
	//if (followCamera_[CAMERA1]->GetActive()) {
	//	followCamera_[CAMERA2]->SetActive(false);
	//}
	//else if (followCamera_[CAMERA2]->GetActive()) {
	//	followCamera_[CAMERA1]->SetActive(false);
	//}

}

void GameScene::Draw() {
	player_->Draw(viewProjection_, WHITE);
	tEmitter_->Draw(viewProjection_);
	enemy_->Draw(viewProjection_, WHITE);
	ringMat_->Draw(viewProjection_, UVCHEKER);
	skydome_->Draw(viewProjection_, BACKGROUND);

	ImGui::Begin("camera");
	ImGui::Checkbox("isActive0", &followCamera_[0]->isActive_);
	ImGui::Checkbox("isActive1", &followCamera_[1]->isActive_);
	ImGui::Text("translation0:%f  %f  %f", followCamera_[0]->GetViewProjection().translation_.x, followCamera_[0]->GetViewProjection().translation_.y, followCamera_[0]->GetViewProjection().translation_.z);
	ImGui::Text("translation1:%f  %f  %f", followCamera_[1]->GetViewProjection().translation_.x, followCamera_[1]->GetViewProjection().translation_.y, followCamera_[1]->GetViewProjection().translation_.z);
	ImGui::Text("view.translation0:%f  %f  %f", viewProjection_.translation_.x, viewProjection_.translation_.y, viewProjection_.translation_.z);
	ImGui::End();
}

void GameScene::Finalize() {
	player_->Finalize();
	viewProjection_.constBuff_.ReleaseAndGetAddressOf();
}