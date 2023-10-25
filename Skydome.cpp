#include "Skydome.h"

void Skydome::Initialize(Model* model) {
	assert(model);
	model_ = model;

	worldTransform_.Initialize();

	// 天球の大きさ
	worldTransform_.scale_ = Multiply(100.0f, worldTransform_.scale_);
}

void Skydome::Update() {
	worldTransform_.UpdateMatrix();
}

void Skydome::Draw(const ViewProjection& viewProjection, uint32_t textureHandle) {
	model_->Draw(worldTransform_, viewProjection, textureHandle, kBlendModeNone, { 1,0.1f,0,1 });
}