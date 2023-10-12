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
	modelUvSphere_.reset(Model::CreateModelFromObj("resources/uvSphere", "uvSphere.obj"));
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
	bottomRope_ = std::make_unique<Rope>();
	bottomRope_->Initialize(modelUvSphere_.get(), {-20.0f, 10.0f, -10.0f}, {20.0f, 10.0f, -10.0f});
}

void GameScene::Update() {
	// シーンの切り替え
	if (input_->TriggerKey(DIK_RETURN)) {
		sceneNum = GAMEOVER_SCENE;
	}
	viewProjection_.UpdateMatrix();

	// 自機
	player_->Update();

	//ロープ
	bottomRope_->Update();
	
	if (input_->TriggerKey(DIK_LSHIFT)) {
		bottomRope_->TestSpring();
	}

	//ロープと自機の当たり判定
	for (auto ropeNodeIt = bottomRope_->GetListBeginSpring(); ropeNodeIt != bottomRope_->GetListEndSpring(); ropeNodeIt++) {
		Rope::RopeNode* ropeNode = ropeNodeIt->get();

		if (player_->GetWorldPosition().z - 1 <= bottomRope_->GetWorldPos(*ropeNode).z && bottomRope_->GetWorldPos(*ropeNode).x - 3 < player_->GetWorldPosition().x && player_->GetWorldPosition().x < bottomRope_->GetWorldPos(*ropeNode).x + 3 && !bottomRope_->IsEdgeNode(*ropeNode)) {
			ropeNode->worldTransform.translation_.z = player_->GetWorldTransform().translation_.z - 1;
		}
	}


}

void GameScene::Draw() {
	player_->Draw(viewProjection_, UVCHEKER);
	bottomRope_->Draw(viewProjection_, UVCHEKER);
}

void GameScene::Finalize() {
	player_->Finalize();
	viewProjection_.constBuff_.ReleaseAndGetAddressOf();
}