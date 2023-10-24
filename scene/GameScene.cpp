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
	modelCube_.reset(Model::CreateModelFromObj("resources/Rope", "Rope.obj"));
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
	if (enemy_->GetPairingTimeCounter() <= 0) {
		enemy_->SetPairingTimeCounter(enemy_->GetPairingTimeCounter() + 1);
	}
	int cooldownFrame = -10;
	//下のロープ
	for (auto ropeNodeIt = bottomRope_[0]->GetListBeginRopeNode(); ropeNodeIt != bottomRope_[0]->GetListEndRopeNode(); ropeNodeIt++) {
		Rope::RopeNode* ropeNode = ropeNodeIt->get();

		if (IsHitEnemy(enemy_->GetWorldPosition(), ropeNode) && !enemy_->IsParent() && enemy_->GetPairingTimeCounter() >= 0) {
			Vector3 enemyPos;
			enemyPos.x = enemy_->GetWorldPosition().x - ropeNode->worldTransform.translation_.x;
			enemyPos.z = enemy_->GetWorldPosition().z - ropeNode->worldTransform.translation_.z;
			enemy_->SetParentPosition(enemyPos);
			bottomRope_[0]->SetSxternalForce(ropeNode, enemy_->GetSpeed() * 30);
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
			else if (enemy_->GetReboundCount() == 2) {
				enemy_->SetReboundCount(3);
			}
		}

		else if (IsHitEnemy(enemy_->GetWorldPosition(), ropeNode) && enemy_->IsParent()) {
			enemy_->SetPairingTimeCounter(enemy_->GetPairingTimeCounter() + 1);
			if (std::abs(enemy_->GetPrePosition().z - enemy_->GetWorldPosition().z) < 0.01f && enemy_->GetPairingTimeCounter() > 15) {
				Vector3 velocity;
				velocity.x = (float)ropeNode->externalForce_.x * 1.0f * -1 / 30;
				velocity.y = 0;
				velocity.z = (float)ropeNode->externalForce_.z * 1.05f * -1 / 30;
				enemy_->SetParentRope(nullptr);
				enemy_->SetEnemyPos({ ropeNode->worldTransform.translation_.x - enemy_->GetParentPosition().x, 0, ropeNode->worldTransform.translation_.z - enemy_->GetParentPosition().z + 10 });
				bottomRope_[0]->SetSxternalForce(ropeNode, { 0, -0.1f, 0 });
				enemy_->SetPairingTimeCounter(cooldownFrame);
				if (enemy_->GetReboundCount() == 3) {
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
			topRope_[0]->SetSxternalForce(ropeNode, enemy_->GetSpeed() * 30);
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
			else if (enemy_->GetReboundCount() == 2) {
				enemy_->SetReboundCount(3);
			}
		}
		else if (IsHitEnemy(enemy_->GetWorldPosition(), ropeNode) && enemy_->IsParent()) {
			enemy_->SetPairingTimeCounter(enemy_->GetPairingTimeCounter() + 1);

			if (std::abs(enemy_->GetPrePosition().z - enemy_->GetWorldPosition().z) < 0.01f && enemy_->GetPairingTimeCounter() > 15) {
				Vector3 velocity;
				velocity.x = (float)ropeNode->externalForce_.x * 1.0f * -1 / 30;
				velocity.y = 0;
				velocity.z = (float)ropeNode->externalForce_.z * 1.05f * -1 / 30;
				enemy_->SetParentRope(nullptr);
				enemy_->SetEnemyPos({ ropeNode->worldTransform.translation_.x - enemy_->GetParentPosition().x, 0, ropeNode->worldTransform.translation_.z - enemy_->GetParentPosition().z - 10 });
				topRope_[0]->SetSxternalForce(ropeNode, { 0, -0.1f, 0 });
				enemy_->SetPairingTimeCounter(cooldownFrame);
				if (enemy_->GetReboundCount() == 3) {
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
			leftRope_[0]->SetSxternalForce(ropeNode, enemy_->GetSpeed() * 30);
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
			else if (enemy_->GetReboundCount() == 2) {
				enemy_->SetReboundCount(3);
			}
		}
		else if (IsHitEnemy(enemy_->GetWorldPosition(), ropeNode) && enemy_->IsParent()) {
			enemy_->SetPairingTimeCounter(enemy_->GetPairingTimeCounter() + 1);

			if (std::abs(enemy_->GetPrePosition().z - enemy_->GetWorldPosition().z) < 0.01f && enemy_->GetPairingTimeCounter() > 15) {
				Vector3 velocity;
				velocity.x = (float)ropeNode->externalForce_.x * 1.05f * -1 / 30;
				velocity.y = 0;
				velocity.z = (float)ropeNode->externalForce_.z * 1.0f * -1 / 30;
				enemy_->SetParentRope(nullptr);
				enemy_->SetEnemyPos({ ropeNode->worldTransform.translation_.x - enemy_->GetParentPosition().x + 10, 0, ropeNode->worldTransform.translation_.z - enemy_->GetParentPosition().z });
				leftRope_[0]->SetSxternalForce(ropeNode, { 0, -0.1f, 0 });
				enemy_->SetPairingTimeCounter(cooldownFrame);
				if (enemy_->GetReboundCount() == 3) {
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
			rightRope_[0]->SetSxternalForce(ropeNode, enemy_->GetSpeed() * 30);
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
			else if (enemy_->GetReboundCount() == 2) {
				enemy_->SetReboundCount(3);
			}
		}
		else if (IsHitEnemy(enemy_->GetWorldPosition(), ropeNode) && enemy_->IsParent()) {
			enemy_->SetPairingTimeCounter(enemy_->GetPairingTimeCounter() + 1);

			if (std::abs(enemy_->GetPrePosition().z - enemy_->GetWorldPosition().z) < 0.01f && enemy_->GetPairingTimeCounter() > 15) {
				Vector3 velocity;
				velocity.x = (float)ropeNode->externalForce_.x * 1.05f * -1 / 30;
				velocity.y = 0;
				velocity.z = (float)ropeNode->externalForce_.z * 1.0f * -1 / 30;
				enemy_->SetParentRope(nullptr);
				enemy_->SetEnemyPos({ ropeNode->worldTransform.translation_.x - enemy_->GetParentPosition().x - 10, 0, ropeNode->worldTransform.translation_.z - enemy_->GetParentPosition().z });
				rightRope_[0]->SetSxternalForce(ropeNode, { 0, -0.1f, 0 });
				enemy_->SetPairingTimeCounter(cooldownFrame);
				if (enemy_->GetReboundCount() == 3) {
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
	player_->Draw(viewProjection_, UVCHEKER);
	enemy_->Draw(viewProjection_, WHITE);
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
}

void GameScene::Finalize() {
	player_->Finalize();
	enemy_->Finalize();
	viewProjection_.constBuff_.ReleaseAndGetAddressOf();
}