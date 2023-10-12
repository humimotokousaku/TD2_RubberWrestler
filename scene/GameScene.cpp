#include "GameScene.h"
#include "../Manager/GameManager.h"

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

	// カメラの位置と向き
	viewProjection_.translation_.y = 11;
	viewProjection_.translation_.z = -30;
	viewProjection_.rotation_.x = 3.14f / 10.0f;

	// 自機の生成
	player_ = std::make_unique<Player>();
	player_->Initialize(playerModels);

	player_->SetViewProjection(&viewProjection_);

	//ロープのテスト生成
	rope_ = std::make_unique<Rope>();
	rope_->Initialize();
}

void GameScene::Update() {
	// シーンの切り替え
	if (input_->TriggerKey(DIK_RETURN)) {
		sceneNum = GAMEOVER_SCENE;
	}
	viewProjection_.UpdateMatrix();

	// 自機
	player_->Update();
}

void GameScene::Draw() {
	player_->Draw(viewProjection_, UVCHEKER);
}

void GameScene::Finalize() {
	player_->Finalize();
	viewProjection_.constBuff_.ReleaseAndGetAddressOf();
}