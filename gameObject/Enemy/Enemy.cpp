#include "Enemy.h"
#include "../../math/Matrix4x4.h"
#define _USE_MATH_DEFINES
#include <math.h>

void Enemy::Initialize(const std::vector<Model*>& models) {
	// 入力
	input_ = Input::GetInstance();
	input_->Initialize();

	// 基底クラスの初期化
	ICharacter::Initialize(models);
	InitializeFloatingGimmick();

	//worldTransform_.Initialize();
	worldTransformBody_.Initialize();
	worldTransformHead_.Initialize();
	worldTransformL_arm_.Initialize();
	worldTransformR_arm_.Initialize();

	// 身体のパーツの親子関係を結ぶ
	SetBodyParent(&GetWorldTransformBody());
	worldTransformBody_.parent_ = &worldTransform_;

	// 腕の座標指定
	worldTransformL_arm_.translation_.x = 1.5f;
	worldTransformR_arm_.translation_.x = -1.5f;
	worldTransformL_arm_.translation_.y = 5.0f;
	worldTransformR_arm_.translation_.y = 5.0f;

	pairingTimeCounter_ = 0;
	reflectionVelocity_ = { 0, 0, 0 };

	worldTransform_.scale_ = { 0.9f,0.9f ,0.9f };
}

void Enemy::Update() {
	// 速さ
	prePos_ = GetWorldPosition();

	if (input_->PressKey(DIK_P)) {
		velocity_.z = 0.8f;
		velocity_.x = 1.1f;
	}
	if (input_->PressKey(DIK_R)) {
		worldTransform_.translation_.x = 0;
		worldTransform_.translation_.y = 0;
		worldTransform_.translation_.z = 0;
	}

	//移動処理
	/*worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);
	worldTransformBody_.translation_ = worldTransform_.translation_;*/
	// 移動量
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);
	//アップデート
	worldTransform_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();
	

	// 基底クラスの更新処理
	ICharacter::Update();
}

void Enemy::Draw(const ViewProjection& viewProjection, uint32_t textureHandle) {
	models_[kModelIndexBody]->Draw(worldTransformBody_, viewProjection, textureHandle, kBlendModeNone,{1,0,0,1});
	models_[kModelIndexHead]->Draw(worldTransformHead_, viewProjection, textureHandle, kBlendModeNone, { 1,0,0,1 });
	models_[kModelIndexL_arm]->Draw(worldTransformL_arm_, viewProjection, textureHandle, kBlendModeNone, { 1,0,0,1 });
	models_[kModelIndexR_arm]->Draw(worldTransformR_arm_, viewProjection, textureHandle, kBlendModeNone, { 1,0,0,1 });
}

void Enemy::UpDateMatrix() {
	//アップデート
	worldTransform_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();
	// 基底クラスの更新処理
	ICharacter::Update();
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
	worldTransform_.translation_.x = 6;
	worldTransform_.translation_.z = 0;
}

void Enemy::SetBodyParent(const WorldTransform* parent) {
	// 親子関係を結ぶ
	worldTransformHead_.parent_ = parent;
	worldTransformL_arm_.parent_ = parent;
	worldTransformR_arm_.parent_ = parent;
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

void Enemy::Finalize() {
	worldTransformBody_.constBuff_.ReleaseAndGetAddressOf();
	worldTransformHead_.constBuff_.ReleaseAndGetAddressOf();
	worldTransformL_arm_.constBuff_.ReleaseAndGetAddressOf();
	worldTransformR_arm_.constBuff_.ReleaseAndGetAddressOf();
}