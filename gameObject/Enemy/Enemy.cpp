#include "Enemy.h"
#include "../../math/Matrix4x4.h"
#define _USE_MATH_DEFINES
#include <math.h>

void Enemy::Initialize(const std::vector<Model*>& models) {
	// 基底クラスの初期化
	ICharacter::Initialize(models);
	InitializeFloatingGimmick();
}

void Enemy::Update() {
	// 速さ
	const float kSpeed = 0.3f;
	velocity_ = { 0.0f, 0.0f, kSpeed };

	// 移動ベクトルをカメラの角度だけ回転
	velocity_ = TransformNormal(velocity_, throwDir_);

	if (player_->GetIsThrow()) {
		// 移動量
		worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);
	}

	// 基底クラスの更新処理
	ICharacter::Update();
}

void Enemy::Draw(const ViewProjection& viewProjection, uint32_t textureHandle) {
	ICharacter::Draw(viewProjection, textureHandle);
}

void Enemy::InitializeFloatingGimmick() { floatingParameter_ = 0.0f; }

void Enemy::UpdateFloatingGimmick() {
	// 浮遊移動のサイクル<frame>
	uint16_t floatingCycle{};
	floatingCycle = 70;
	// 1フレームでのパラメータ加算値
	float step{};
	step = 2.0f * (float)M_PI / floatingCycle;
	// パラメータを1ステップ分加算
	floatingParameter_ += step;
	// 2πを超えたら0に戻す
	floatingParameter_ = (float)std::fmod(floatingParameter_, 2.0f * M_PI);

	// 浮遊の振幅<m>
	const float floatingAmplitude = 2.0f;
	// 浮遊を座標に反映
	worldTransform_.translation_.y = std::abs(std::sin(floatingParameter_)) * floatingAmplitude;
}

void Enemy::SetParent(const WorldTransform* parent) {
	// 親子関係を結ぶ
	worldTransform_.parent_ = parent;
	worldTransform_.rotation_ = player_->GetPlayerBodyRotation();
	worldTransform_.translation_.x = 3;
	worldTransform_.translation_.z = 0;
}

void Enemy::SetWorldTransform(WorldTransform worldTransform) {
	worldTransform_ = worldTransform;
	worldTransform_.translation_.x += 3;
}

Vector3 Enemy::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}