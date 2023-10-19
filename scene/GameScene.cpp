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
		bottomRope_[i]->Initialize(modelUvSphere_.get(), { -20.0f, 2.5f + 2.5f * i, -20.0f }, { 20.0f, 2.5f + 2.5f * i, -20.0f });
	}
	for (int i = 0; i < 3; i++) {
		topRope_[i] = std::make_unique<Rope>();
		topRope_[i]->Initialize(modelUvSphere_.get(), { -20.0f, 2.5f + 2.5f * i, 20.0f }, { 20.0f, 2.5f + 2.5f * i, 20.0f });
	}
	for (int i = 0; i < 3; i++) {
		rightRope_[i] = std::make_unique<Rope>();
		rightRope_[i]->Initialize(modelUvSphere_.get(), { 20.0f, 2.5f + 2.5f * i, -20.0f }, { 20.0f, 2.5f + 2.5f * i, 20.0f });
	}
	for (int i = 0; i < 3; i++) {
		leftRope_[i] = std::make_unique<Rope>();
		leftRope_[i]->Initialize(modelUvSphere_.get(), { -20.0f, 2.5f + 2.5f * i, -20.0f }, { -20.0f, 2.5f + 2.5f * i, 20.0f });
	}
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
	}
	for (int i = 0; i < 3; i++) {
		topRope_[i]->Update();
	}
	for (int i = 0; i < 3; i++) {
		rightRope_[i]->Update();
	}
	for (int i = 0; i < 3; i++) {
		leftRope_[i]->Update();
	}
	
	for (int i = 0; i < 3; i++) {
		if (input_->TriggerKey(DIK_LSHIFT)) {
			bottomRope_[i]->TestSpring();
		}
	}

	//ロープの当たり判定
	//下のロープ
	float ropeFactor = 25;
	float swingWidthSubtract = 0.9f;
	int ropeSize = 3;
	int enemyWidth = 1;
	for (int i = 0; i < 3; i++) {
		for (auto ropeNodeIt = bottomRope_[i]->GetListBeginSpring(); ropeNodeIt != bottomRope_[i]->GetListEndSpring(); ropeNodeIt++) {
			Rope::RopeNode* ropeNode = ropeNodeIt->get();

			if (enemy_->GetWorldPosition().z - enemyWidth <= bottomRope_[i]->GetWorldPos(*ropeNode).z
					&& bottomRope_[i]->GetWorldPos(*ropeNode).x - ropeSize < enemy_->GetWorldPosition().x
					&& enemy_->GetWorldPosition().x < bottomRope_[i]->GetWorldPos(*ropeNode).x + ropeSize
					&& !bottomRope_[i]->IsEdgeNode(*ropeNode) && !bottomRope_[i]->IsHitNode(ropeNode)) {
				bottomRope_[i]->SetIsHitNode(ropeNode, true);
			}
			if (bottomRope_[i]->IsHitNode(ropeNode)) {
				ropeNode->worldTransform.translation_.z = enemy_->GetWorldTransform().translation_.z - enemyWidth;
				enemy_->SetEnemySpeed({ enemy_->GetEnemySpeed().x, enemy_->GetEnemySpeed().y, enemy_->GetEnemySpeed().z + 0.005f });
				
				if (enemy_->GetEnemySpeed().z >= 0.0f) {
					bottomRope_[i]->SetIsHitNode(ropeNode, false);
					float differenceHittingPosition = enemy_->GetWorldPosition().z - enemy_->GetPrePosition().z;
					enemy_->SetEnemySpeed({ enemy_->GetEnemySpeed().x * swingWidthSubtract, 0, -differenceHittingPosition / ropeFactor});
				}
			}
		}
	}
	//上のロープ
	for (int i = 0; i < 3; i++) {
		for (auto ropeNodeIt = topRope_[i]->GetListBeginSpring(); ropeNodeIt != topRope_[i]->GetListEndSpring(); ropeNodeIt++) {
			Rope::RopeNode* ropeNode = ropeNodeIt->get();

			if (enemy_->GetWorldPosition().z + enemyWidth >= topRope_[i]->GetWorldPos(*ropeNode).z
					&& topRope_[i]->GetWorldPos(*ropeNode).x - ropeSize < enemy_->GetWorldPosition().x
					&& enemy_->GetWorldPosition().x < topRope_[i]->GetWorldPos(*ropeNode).x + ropeSize
					&& !topRope_[i]->IsEdgeNode(*ropeNode) && !topRope_[i]->IsHitNode(ropeNode)) {
				topRope_[i]->SetIsHitNode(ropeNode, true);
			}
			if (topRope_[i]->IsHitNode(ropeNode)) {
				ropeNode->worldTransform.translation_.z = enemy_->GetWorldTransform().translation_.z + enemyWidth;
				enemy_->SetEnemySpeed({ enemy_->GetEnemySpeed().x, 0, enemy_->GetEnemySpeed().z - 0.005f });

				if (enemy_->GetEnemySpeed().z <= 0.0f) {
					topRope_[i]->SetIsHitNode(ropeNode, false);
					float differenceHittingPosition = enemy_->GetWorldPosition().z - enemy_->GetPrePosition().z;
					enemy_->SetEnemySpeed({ enemy_->GetEnemySpeed().x * swingWidthSubtract, enemy_->GetEnemySpeed().y, -differenceHittingPosition / ropeFactor });
				}
			}
		}
	}
	//右のロープ
	for (int i = 0; i < 3; i++) {
		for (auto ropeNodeIt = rightRope_[i]->GetListBeginSpring(); ropeNodeIt != rightRope_[i]->GetListEndSpring(); ropeNodeIt++) {
			Rope::RopeNode* ropeNode = ropeNodeIt->get();

			if (enemy_->GetWorldPosition().x + enemyWidth >= rightRope_[i]->GetWorldPos(*ropeNode).x
					&& rightRope_[i]->GetWorldPos(*ropeNode).z - ropeSize < enemy_->GetWorldPosition().z
					&& enemy_->GetWorldPosition().z < rightRope_[i]->GetWorldPos(*ropeNode).z + ropeSize
					&& !rightRope_[i]->IsEdgeNode(*ropeNode) && !rightRope_[i]->IsHitNode(ropeNode)) {
				rightRope_[i]->SetIsHitNode(ropeNode, true);
			}
			if (rightRope_[i]->IsHitNode(ropeNode)) {
				ropeNode->worldTransform.translation_.x = enemy_->GetWorldTransform().translation_.x + enemyWidth;
				enemy_->SetEnemySpeed({ enemy_->GetEnemySpeed().x - 0.005f , enemy_->GetEnemySpeed().y, enemy_->GetEnemySpeed().z});

				if (enemy_->GetEnemySpeed().x <= 0.0f) {
					rightRope_[i]->SetIsHitNode(ropeNode, false);
					float differenceHittingPosition = enemy_->GetWorldPosition().x - enemy_->GetPrePosition().x;
					enemy_->SetEnemySpeed({ -differenceHittingPosition / ropeFactor , 0, enemy_->GetEnemySpeed().z * swingWidthSubtract });
				}
			}
		}
	}
	//左のロープ
	for (int i = 0; i < 3; i++) {
		for (auto ropeNodeIt = leftRope_[i]->GetListBeginSpring(); ropeNodeIt != leftRope_[i]->GetListEndSpring(); ropeNodeIt++) {
			Rope::RopeNode* ropeNode = ropeNodeIt->get();

			if (enemy_->GetWorldPosition().x - enemyWidth <= leftRope_[i]->GetWorldPos(*ropeNode).x
					&& leftRope_[i]->GetWorldPos(*ropeNode).z - ropeSize < enemy_->GetWorldPosition().z
					&& enemy_->GetWorldPosition().z < leftRope_[i]->GetWorldPos(*ropeNode).z + ropeSize
					&& !leftRope_[i]->IsEdgeNode(*ropeNode) && !leftRope_[i]->IsHitNode(ropeNode)) {
				leftRope_[i]->SetIsHitNode(ropeNode, true);
			}
			if (leftRope_[i]->IsHitNode(ropeNode)) {
				ropeNode->worldTransform.translation_.x = enemy_->GetWorldTransform().translation_.x - enemyWidth;
				enemy_->SetEnemySpeed({ enemy_->GetEnemySpeed().x + 0.005f , enemy_->GetEnemySpeed().y, enemy_->GetEnemySpeed().z});

				if (enemy_->GetEnemySpeed().x >= 0.0f) {
					leftRope_[i]->SetIsHitNode(ropeNode, false);
					float differenceHittingPosition = enemy_->GetWorldPosition().x - enemy_->GetPrePosition().x;
					enemy_->SetEnemySpeed({ -differenceHittingPosition / ropeFactor , 0, enemy_->GetEnemySpeed().z * swingWidthSubtract });
				}
			}
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