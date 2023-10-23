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
		modelFighterR_arm_.get() };

	// カメラの位置と向き
	viewProjection_.translation_.y = 11;
	viewProjection_.translation_.z = -30;
	viewProjection_.rotation_.x = 3.14f / 10.0f;

	// 自機の生成
	player_ = std::make_unique<Player>();
	player_->Initialize(playerModels);

	player_->SetViewProjection(&viewProjection_);

	//敵の生成
	enemy_ = std::make_unique<Enemy>();
	enemy_->Initialize(playerModels);

	//ロープのテスト生成
	for (int i = 0; i < 3; i++) {
		bottomRope_[i] = std::make_unique<Rope>();
		switch (i)
		{
		case 0:
			bottomRope_[i]->Initialize(modelUvSphere_.get(), { -20.0f, 5.0f, -20.0f }, { 20.0f, 5.0f, -20.0f });
			break;
		case 1:
			bottomRope_[i]->Initialize(modelUvSphere_.get(), { 0, 2.5f, 0 }, { 0, 2.5f, 0 });
			break;
		case 2:
			bottomRope_[i]->Initialize(modelUvSphere_.get(), { 0, -2.5f, 0 }, { 0, -2.5f, 0 });
			break;
		default:
			break;
		}
	}
	bottomRope_[1]->SetParent(bottomRope_[0].get());
	bottomRope_[2]->SetParent(bottomRope_[0].get());

	for (int i = 0; i < 3; i++) {
		topRope_[i] = std::make_unique<Rope>();
		switch (i)
		{
		case 0:
			topRope_[i]->Initialize(modelUvSphere_.get(), { -20.0f, 5.0f, 20.0f }, { 20.0f, 5.0f, 20.0f });
			break;
		case 1:
			topRope_[i]->Initialize(modelUvSphere_.get(), { 0, 2.5f, 0 }, { 0, 2.5f, 0 });
			break;
		case 2:
			topRope_[i]->Initialize(modelUvSphere_.get(), { 0, -2.5f, 0 }, { 0, -2.5f, 0 });
			break;
		default:
			break;
		}
	}
	topRope_[1]->SetParent(topRope_[0].get());
	topRope_[2]->SetParent(topRope_[0].get());

	for (int i = 0; i < 3; i++) {
		rightRope_[i] = std::make_unique<Rope>();
		switch (i)
		{
		case 0:
			rightRope_[i]->Initialize(modelUvSphere_.get(), { 20.0f, 5.0f, -20.0f }, { 20.0f, 5.0f, 20.0f });
			break;
		case 1:
			rightRope_[i]->Initialize(modelUvSphere_.get(), { 0, 2.5f, 0 }, { 0, 2.5f, 0 });
			break;
		case 2:
			rightRope_[i]->Initialize(modelUvSphere_.get(), { 0, -2.5f, 0 }, { 0, -2.5f, 0 });
			break;
		default:
			break;
		}
	}
	rightRope_[1]->SetParent(rightRope_[0].get());
	rightRope_[2]->SetParent(rightRope_[0].get());

	for (int i = 0; i < 3; i++) {
		leftRope_[i] = std::make_unique<Rope>();
		switch (i)
		{
		case 0:
			leftRope_[i]->Initialize(modelUvSphere_.get(), { -20.0f, 5.0f, -20.0f }, { -20.0f, 5.0f, 20.0f });
			break;
		case 1:
			leftRope_[i]->Initialize(modelUvSphere_.get(), { 0, 2.5f, 0 }, { 0, 2.5f, 0 });
			break;
		case 2:
			leftRope_[i]->Initialize(modelUvSphere_.get(), { 0, -2.5f, 0 }, { 0, -2.5f, 0 });
			break;
		default:
			break;
		}
	}
	leftRope_[1]->SetParent(leftRope_[0].get());
	leftRope_[2]->SetParent(leftRope_[0].get());
}

void GameScene::Update() {
	// シーンの切り替え
	if (input_->TriggerKey(DIK_RETURN)) {
		sceneNum = GAMEOVER_SCENE;
	}
	viewProjection_.UpdateMatrix();

	// 自機
	player_->Update();

	//敵
	enemy_->Update();

	//ロープ
	for (int i = 0; i < 3; i++) {
		bottomRope_[i]->Update();
		topRope_[i]->Update();
		rightRope_[i]->Update();
		leftRope_[i]->Update();
	}

	//ロープの一部に力を加えてテスト
	if (input_->TriggerKey(DIK_O)) {
		topRope_[0]->TestSpring();
	}

	//ロープの当たり判定
	//下のロープ
	for (auto ropeNodeIt = bottomRope_[0]->GetListBeginRopeNode(); ropeNodeIt != bottomRope_[0]->GetListEndRopeNode(); ropeNodeIt++) {
		Rope::RopeNode* ropeNode = ropeNodeIt->get();

		if (IsHitEnemy(enemy_->GetWorldPosition(), ropeNode, true)) {
			bottomRope_[0]->SetSxternalForce(enemy_->GetSpeed());
			enemy_->SetSpeed({ 0, 0, 0 });
			enemy_->SetEnemyPos({ 0, 0, 0 });
			enemy_->SetParent(&ropeNode->worldTransform);
			enemy_->UpDateMatrix();
		}

	}
	//上のロープ
	for (auto ropeNodeIt = topRope_[0]->GetListBeginRopeNode(); ropeNodeIt != topRope_[0]->GetListEndRopeNode(); ropeNodeIt++) {
		Rope::RopeNode* ropeNode = ropeNodeIt->get();

		if (IsHitEnemy(enemy_->GetWorldPosition(), ropeNode, true)) {
			topRope_[0]->SetSxternalForce(enemy_->GetSpeed());
			enemy_->SetSpeed({ 0, 0, 0 });
			enemy_->SetEnemyPos({ 0, 0, 0 });
			enemy_->SetParent(&ropeNode->worldTransform);
			enemy_->UpDateMatrix();
		}

	}
	//左のロープ
	for (auto ropeNodeIt = leftRope_[0]->GetListBeginRopeNode(); ropeNodeIt != leftRope_[0]->GetListEndRopeNode(); ropeNodeIt++) {
		Rope::RopeNode* ropeNode = ropeNodeIt->get();

		if (IsHitEnemy(enemy_->GetWorldPosition(), ropeNode, true)) {
			leftRope_[0]->SetSxternalForce(enemy_->GetSpeed());
			enemy_->SetSpeed({ 0, 0, 0 });
			enemy_->SetEnemyPos({ 0, 0, 0 });
			enemy_->SetParent(&ropeNode->worldTransform);
			enemy_->UpDateMatrix();
		}
	}
	//右のロープ
	for (auto ropeNodeIt = rightRope_[0]->GetListBeginRopeNode(); ropeNodeIt != rightRope_[0]->GetListEndRopeNode(); ropeNodeIt++) {
		Rope::RopeNode* ropeNode = ropeNodeIt->get();

		if (IsHitEnemy(enemy_->GetWorldPosition(), ropeNode, true)) {
			rightRope_[0]->SetSxternalForce(enemy_->GetSpeed());
			enemy_->SetSpeed({ 0, 0, 0 });
			enemy_->SetEnemyPos({ 0, 0, 0 });
			enemy_->SetParent(&ropeNode->worldTransform);
			enemy_->UpDateMatrix();
		}

	}
}

void GameScene::Draw() {
	player_->Draw(viewProjection_, UVCHEKER);
	enemy_->Draw(viewProjection_, WHITE);
	for (int i = 0; i < 3; i++) {
		bottomRope_[i]->Draw(viewProjection_, UVCHEKER);
	}
	for (int i = 0; i < 3; i++) {
		topRope_[i]->Draw(viewProjection_, UVCHEKER);
	}
	for (int i = 0; i < 3; i++) {
		rightRope_[i]->Draw(viewProjection_, UVCHEKER);
	}
	for (int i = 0; i < 3; i++) {
		leftRope_[i]->Draw(viewProjection_, UVCHEKER);
	}
}

void GameScene::Finalize() {
	player_->Finalize();
	enemy_->Finalize();
	viewProjection_.constBuff_.ReleaseAndGetAddressOf();
}