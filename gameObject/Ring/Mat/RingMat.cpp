#include "RingMat.h"

void RingMat::Initialize(Model* model) {
	assert(model);
	model_ = model;

	worldTransform_.Initialize();

	worldTransform_.translation_.y = -2;
	worldTransform_.scale_ = Multiply(50.0f, worldTransform_.scale_);
}

void RingMat::Update() {
	worldTransform_.UpdateMatrix();
}

void RingMat::Draw(const ViewProjection& viewProjection, uint32_t textureHandle) {
	model_->Draw(worldTransform_, viewProjection, textureHandle, kBlendModeNone,{1,1,1,1});
}