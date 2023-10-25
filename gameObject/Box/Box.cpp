#include "Box.h"
#include "../Player/Player.h"
#include "../Enemy/Enemy.h"

Box::Box()
{
}

Box::~Box()
{
}

void Box::Initialize(Model* model, Vector3 pos, Vector3 size) {
	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.translation_ = pos;
	worldTransform_.scale_ = size;
}

void Box::Update() {

	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	//詳細
	float enemyWidth = 0.5f;
	
	Vector3 enemyPos = enemy_->GetWorldPosition();

	if (worldPos.x - worldTransform_.scale_.x <= enemyPos.x + enemyWidth && enemyPos.x - enemyWidth <= worldPos.x + worldTransform_.scale_.x) {
		if (worldPos.z - worldTransform_.scale_.z <= enemyPos.z + enemyWidth && enemyPos.z - enemyWidth <= worldPos.z + worldTransform_.scale_.z) {
			if (player_->GetBehavior() == Player::Behavior::WAITING) {
				player_->SetBehaviorRequest(Player::Behavior::NONE);
				enemy_->SetEnemyPos({0, 0, 0});
				enemy_->SetSpeed({ 0, 0, 0 });
			}
		}
	}

	worldTransform_.UpdateMatrix();
}

void Box::Draw(const ViewProjection& viewProjection, uint32_t textureHandle) {
	model_->Draw(worldTransform_, viewProjection, textureHandle, kBlendModeNone, { 1,1,1,1 });
}