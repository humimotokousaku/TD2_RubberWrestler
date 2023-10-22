#include "ICharacter.h"

void ICharacter::Initialize(const std::vector<Model*>& models) {
	models_ = models;
	worldTransform_.Initialize();
}

void ICharacter::Update() {
	worldTransform_.UpdateMatrix();
}

void ICharacter::Draw(const ViewProjection& viewProjection, uint32_t textureHandle) {
	for (Model* model : models_) {
		model->Draw(worldTransform_, viewProjection, textureHandle,kBlendModeNone, { 1,1,1,1 });
	}
}