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
	modelUvSphere_.reset(Model::CreateModelFromObj("resources/uvSphere", "uvSphere.obj"));
	modelCube_.reset(Model::CreateModelFromObj("resources/Rope", "Rope.obj"));
	std::vector<Model*> playerModels = {
		modelFighterBody_.get(), modelFighterHead_.get(), modelFighterL_arm_.get(),
		modelFighterR_arm_.get() };

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

	leftTopBox_ = std::make_unique<Box>();
	leftBotBox_ = std::make_unique<Box>();
	rightTopBox_ = std::make_unique<Box>();
	rightBotBox_ = std::make_unique<Box>();
	leftTopBox_->Initialize(modelCube_.get(), {-20, 0, 20}, {2, 10, 2});
	leftBotBox_->Initialize(modelCube_.get(), {-20, 0, -20}, {2, 10, 2});
	rightTopBox_->Initialize(modelCube_.get(), {20, 0, 20}, {2, 10, 2});
	rightBotBox_->Initialize(modelCube_.get(), {20, 0, -20}, {2, 10, 2});
	Box::SetPlayer(player_.get());
	Box::SetEnemy(enemy_.get());

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

	//ロープのテスト生成
	float ropeInterval = 5.0f;
	for (int i = 0; i < 3; i++) {
		bottomRope_[i] = std::make_unique<Rope>();
		switch (i)
		{
		case 0:
			bottomRope_[i]->Initialize(modelCube_.get(), { -20.0f, 5.0f, -20.0f }, { 20.0f, 5.0f, -20.0f });
			break;
		case 1:
			bottomRope_[i]->Initialize(modelCube_.get(), { 0, ropeInterval, 0 }, { 0, ropeInterval, 0 });
			break;
		case 2:
			bottomRope_[i]->Initialize(modelCube_.get(), { 0, -ropeInterval, 0 }, { 0, -ropeInterval, 0 });
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
			topRope_[i]->Initialize(modelCube_.get(), { -20.0f, 5.0f, 20.0f }, { 20.0f, 5.0f, 20.0f });
			break;
		case 1:
			topRope_[i]->Initialize(modelCube_.get(), { 0, ropeInterval, 0 }, { 0, ropeInterval, 0 });
			break;
		case 2:
			topRope_[i]->Initialize(modelCube_.get(), { 0, -ropeInterval, 0 }, { 0, -ropeInterval, 0 });
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
			rightRope_[i]->Initialize(modelCube_.get(), { 20.0f, 5.0f, -20.0f }, { 20.0f, 5.0f, 20.0f });
			break;
		case 1:
			rightRope_[i]->Initialize(modelCube_.get(), { 0, ropeInterval, 0 }, { 0, ropeInterval, 0 });
			break;
		case 2:
			rightRope_[i]->Initialize(modelCube_.get(), { 0, -ropeInterval, 0 }, { 0, -ropeInterval, 0 });
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
			leftRope_[i]->Initialize(modelCube_.get(), { -20.0f, 5.0f, -20.0f }, { -20.0f, 5.0f, 20.0f });
			break;
		case 1:
			leftRope_[i]->Initialize(modelCube_.get(), { 0, ropeInterval, 0 }, { 0, ropeInterval, 0 });
			break;
		case 2:
			leftRope_[i]->Initialize(modelCube_.get(), { 0, -ropeInterval, 0 }, { 0, -ropeInterval, 0 });
			break;
		default:
			break;
		}
	}
	leftRope_[1]->SetParent(leftRope_[0].get());
	leftRope_[2]->SetParent(leftRope_[0].get());
}

void GameScene::Update() {
	viewProjection_.UpdateMatrix();
	mainCamera_.UpdateMatrix();

	// 自機
	player_->Update();

	//敵
	enemy_->Update();

	//敵とプレイヤーの当たり判定
	if (Distance(player_->GetWorldPosition(), enemy_->GetWorldPosition()) <= 5.0f && enemy_->GetReboundCount() == 2 && player_->GetBehavior() == Player::Behavior::WAITING) {
		player_->SetBehaviorRequest(Player::Behavior::LARIAT);
		enemy_->SetSpeed({0, 0, 0});
	}

	// リングのマット
	ringMat_->Update();

	// 天球
	skydome_->Update();

	//障害物
	leftTopBox_->Update();
	leftBotBox_->Update();
	rightTopBox_->Update();
	rightBotBox_->Update();

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
	if (enemy_->GetPairingTimeCounter() <= 0) {
		enemy_->SetPairingTimeCounter(enemy_->GetPairingTimeCounter() + 1);
	}
	int cooldownFrame = -10;
	int powerRope = 100;
	int enemyReboundCount = 2;
	//下のロープ
	for (auto ropeNodeIt = bottomRope_[0]->GetListBeginRopeNode(); ropeNodeIt != bottomRope_[0]->GetListEndRopeNode(); ropeNodeIt++) {
		Rope::RopeNode* ropeNode = ropeNodeIt->get();

		if (IsHitEnemy(enemy_->GetWorldPosition(), ropeNode) && !enemy_->IsParent() && enemy_->GetPairingTimeCounter() >= 0) {
			Vector3 enemyPos;
			enemyPos.x = enemy_->GetWorldPosition().x - ropeNode->worldTransform.translation_.x;
			enemyPos.z = enemy_->GetWorldPosition().z - ropeNode->worldTransform.translation_.z;
			enemy_->SetParentPosition(enemyPos);
			bottomRope_[0]->SetSxternalForce(ropeNode, enemy_->GetSpeed() * powerRope);
			enemy_->SetParentRope(&ropeNode->worldTransform);
			enemy_->SetSpeed({ 0, 0, 0 });
			enemy_->SetEnemyPos({ enemyPos.x, -1.5f, enemyPos.z });
			enemy_->SetPairingTimeCounter(1);
			enemy_->UpDateMatrix();

			if (enemy_->GetReboundCount() == 0) {
				enemy_->SetReboundCount(1);
			}
			else if (enemy_->GetReboundCount() == 1) {
				enemy_->SetReboundCount(2);
			}
		}

		else if (IsHitEnemy(enemy_->GetWorldPosition(), ropeNode) && enemy_->IsParent()) {
			enemy_->SetPairingTimeCounter(enemy_->GetPairingTimeCounter() + 1);
			if (std::abs(enemy_->GetPrePosition().z - enemy_->GetWorldPosition().z) < 0.01f && enemy_->GetPairingTimeCounter() > 15) {
				Vector3 velocity;
				velocity.x = 0;
				velocity.y = 0;
				velocity.z = (float)ropeNode->externalForce_.z * 1.5f * -1 / powerRope;
				enemy_->SetParentRope(nullptr);
				enemy_->SetEnemyPos({ ropeNode->worldTransform.translation_.x - enemy_->GetParentPosition().x, 0, ropeNode->worldTransform.translation_.z - enemy_->GetParentPosition().z + 15 });
				bottomRope_[0]->SetSxternalForce(ropeNode, { 0, -0.1f, 0 });
				enemy_->SetPairingTimeCounter(cooldownFrame);
				if (enemy_->GetReboundCount() == enemyReboundCount) {
					Vector3 playerDirection = player_->GetWorldPosition() - enemy_->GetWorldPosition();
					velocity = Normalize(playerDirection);
					velocity *= 2;

					enemy_->SetSpeed(velocity);
				}
				else {
					enemy_->SetSpeed(velocity);
				}
			}
		}

	}
	//上のロープ
	for (auto ropeNodeIt = topRope_[0]->GetListBeginRopeNode(); ropeNodeIt != topRope_[0]->GetListEndRopeNode(); ropeNodeIt++) {
		Rope::RopeNode* ropeNode = ropeNodeIt->get();

		if (IsHitEnemy(enemy_->GetWorldPosition(), ropeNode) && !enemy_->IsParent() && enemy_->GetPairingTimeCounter() >= 0) {
			Vector3 enemyPos;
			enemyPos.x = enemy_->GetWorldPosition().x - ropeNode->worldTransform.translation_.x;
			enemyPos.z = enemy_->GetWorldPosition().z - ropeNode->worldTransform.translation_.z;
			enemy_->SetParentPosition(enemyPos);
			topRope_[0]->SetSxternalForce(ropeNode, enemy_->GetSpeed() * powerRope);
			enemy_->SetParentRope(&ropeNode->worldTransform);
			enemy_->SetSpeed({ 0, 0, 0 });
			enemy_->SetEnemyPos({ enemyPos.x, -1.5f, enemyPos.z });
			enemy_->SetPairingTimeCounter(1);
			enemy_->UpDateMatrix();

			if (enemy_->GetReboundCount() == 0) {
				enemy_->SetReboundCount(1);
			}
			else if (enemy_->GetReboundCount() == 1) {
				enemy_->SetReboundCount(2);
			}
		}
		else if (IsHitEnemy(enemy_->GetWorldPosition(), ropeNode) && enemy_->IsParent()) {
			enemy_->SetPairingTimeCounter(enemy_->GetPairingTimeCounter() + 1);

			if (std::abs(enemy_->GetPrePosition().z - enemy_->GetWorldPosition().z) < 0.01f && enemy_->GetPairingTimeCounter() > 15) {
				Vector3 velocity;
				velocity.x = 0;
				velocity.y = 0;
				velocity.z = (float)ropeNode->externalForce_.z * 1.5f * -1 / powerRope;
				enemy_->SetParentRope(nullptr);
				enemy_->SetEnemyPos({ ropeNode->worldTransform.translation_.x - enemy_->GetParentPosition().x, 0, ropeNode->worldTransform.translation_.z - enemy_->GetParentPosition().z - 15 });
				topRope_[0]->SetSxternalForce(ropeNode, { 0, -0.1f, 0 });
				enemy_->SetPairingTimeCounter(cooldownFrame);
				if (enemy_->GetReboundCount() == enemyReboundCount) {
					Vector3 playerDirection = player_->GetWorldPosition() - enemy_->GetWorldPosition();
					velocity = Normalize(playerDirection);
					velocity *= 2;

					enemy_->SetSpeed(velocity);
				}
				else {
					enemy_->SetSpeed(velocity);
				}
			}
		}

	}
	//左のロープ
	for (auto ropeNodeIt = leftRope_[0]->GetListBeginRopeNode(); ropeNodeIt != leftRope_[0]->GetListEndRopeNode(); ropeNodeIt++) {
		Rope::RopeNode* ropeNode = ropeNodeIt->get();

		if (IsHitEnemy(enemy_->GetWorldPosition(), ropeNode) && !enemy_->IsParent() && enemy_->GetPairingTimeCounter() >= 0) {
			Vector3 enemyPos;
			enemyPos.x = enemy_->GetWorldPosition().x - ropeNode->worldTransform.translation_.x;
			enemyPos.z = enemy_->GetWorldPosition().z - ropeNode->worldTransform.translation_.z;
			enemy_->SetParentPosition(enemyPos);
			leftRope_[0]->SetSxternalForce(ropeNode, enemy_->GetSpeed() * powerRope);
			enemy_->SetParentRope(&ropeNode->worldTransform);
			enemy_->SetSpeed({ 0, 0, 0 });
			enemy_->SetEnemyPos({ enemyPos.x, -1.5f, enemyPos.z });
			enemy_->SetPairingTimeCounter(1);
			enemy_->UpDateMatrix();

			if (enemy_->GetReboundCount() == 0) {
				enemy_->SetReboundCount(1);
			}
			else if (enemy_->GetReboundCount() == 1) {
				enemy_->SetReboundCount(2);
			}
		}
		else if (IsHitEnemy(enemy_->GetWorldPosition(), ropeNode) && enemy_->IsParent()) {
			enemy_->SetPairingTimeCounter(enemy_->GetPairingTimeCounter() + 1);

			if (std::abs(enemy_->GetPrePosition().z - enemy_->GetWorldPosition().z) < 0.01f && enemy_->GetPairingTimeCounter() > 15) {
				Vector3 velocity;
				velocity.x = (float)ropeNode->externalForce_.x * 1.5f * -1 / powerRope;
				velocity.y = 0;
				velocity.z = 0;
				enemy_->SetParentRope(nullptr);
				enemy_->SetEnemyPos({ ropeNode->worldTransform.translation_.x - enemy_->GetParentPosition().x + 15, 0, ropeNode->worldTransform.translation_.z - enemy_->GetParentPosition().z });
				leftRope_[0]->SetSxternalForce(ropeNode, { 0, -0.1f, 0 });
				enemy_->SetPairingTimeCounter(cooldownFrame);
				if (enemy_->GetReboundCount() == enemyReboundCount) {
					Vector3 playerDirection = player_->GetWorldPosition() - enemy_->GetWorldPosition();
					velocity = Normalize(playerDirection);
					velocity *= 2;

					enemy_->SetSpeed(velocity);
				}
				else {
					enemy_->SetSpeed(velocity);
				}
			}
		}
	}
	//右のロープ
	for (auto ropeNodeIt = rightRope_[0]->GetListBeginRopeNode(); ropeNodeIt != rightRope_[0]->GetListEndRopeNode(); ropeNodeIt++) {
		Rope::RopeNode* ropeNode = ropeNodeIt->get();

		if (IsHitEnemy(enemy_->GetWorldPosition(), ropeNode) && !enemy_->IsParent() && enemy_->GetPairingTimeCounter() >= 0) {
			Vector3 enemyPos;
			enemyPos.x = enemy_->GetWorldPosition().x - ropeNode->worldTransform.translation_.x;
			enemyPos.z = enemy_->GetWorldPosition().z - ropeNode->worldTransform.translation_.z;
			enemy_->SetParentPosition(enemyPos);
			rightRope_[0]->SetSxternalForce(ropeNode, enemy_->GetSpeed() * powerRope);
			enemy_->SetParentRope(&ropeNode->worldTransform);
			enemy_->SetSpeed({ 0, 0, 0 });
			enemy_->SetEnemyPos({ enemyPos.x, -1.5f, enemyPos.z });
			enemy_->SetPairingTimeCounter(1);
			enemy_->UpDateMatrix();

			if (enemy_->GetReboundCount() == 0) {
				enemy_->SetReboundCount(1);
			}
			else if (enemy_->GetReboundCount() == 1) {
				enemy_->SetReboundCount(2);
			}
		}
		else if (IsHitEnemy(enemy_->GetWorldPosition(), ropeNode) && enemy_->IsParent()) {
			enemy_->SetPairingTimeCounter(enemy_->GetPairingTimeCounter() + 1);

			if (std::abs(enemy_->GetPrePosition().z - enemy_->GetWorldPosition().z) < 0.01f && enemy_->GetPairingTimeCounter() > 15) {
				Vector3 velocity;
				velocity.x = (float)ropeNode->externalForce_.x * 1.5f * -1 / powerRope;
				velocity.y = 0;
				velocity.z = 0;
				enemy_->SetParentRope(nullptr);
				enemy_->SetEnemyPos({ ropeNode->worldTransform.translation_.x - enemy_->GetParentPosition().x - 15, 0, ropeNode->worldTransform.translation_.z - enemy_->GetParentPosition().z });
				rightRope_[0]->SetSxternalForce(ropeNode, { 0, -0.1f, 0 });
				enemy_->SetPairingTimeCounter(cooldownFrame);
				if (enemy_->GetReboundCount() == enemyReboundCount) {
					Vector3 playerDirection = player_->GetWorldPosition() - enemy_->GetWorldPosition();
					velocity = Normalize(playerDirection);
					velocity *= 2;

					enemy_->SetSpeed(velocity);
				}
				else {
					enemy_->SetSpeed(velocity);
				}
			}
		}

	}
}



void GameScene::Draw() {
	player_->Draw(viewProjection_, WHITE);
	tEmitter_->Draw(viewProjection_);
	enemy_->Draw(viewProjection_, WHITE);
	ringMat_->Draw(viewProjection_, UVCHEKER);
	skydome_->Draw(viewProjection_, BACKGROUND);

	leftTopBox_->Draw(viewProjection_, UVCHEKER);
	leftBotBox_->Draw(viewProjection_, UVCHEKER);
	rightTopBox_->Draw(viewProjection_, UVCHEKER);
	rightBotBox_->Draw(viewProjection_, UVCHEKER);

	for (int i = 0; i < 3; i++) {
		bottomRope_[i]->Draw(viewProjection_, ROPE);
	}
	for (int i = 0; i < 3; i++) {
		topRope_[i]->Draw(viewProjection_, ROPE);
	}
	for (int i = 0; i < 3; i++) {
		rightRope_[i]->Draw(viewProjection_, ROPE);
	}
	for (int i = 0; i < 3; i++) {
		leftRope_[i]->Draw(viewProjection_, ROPE);
	}

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
	enemy_->Finalize();
	viewProjection_.constBuff_.ReleaseAndGetAddressOf();
}