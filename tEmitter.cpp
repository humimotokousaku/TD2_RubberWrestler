#include "tEmitter.h"

void tEmitter::Initialize(const Vector3& position, Model* dustModel, uint32_t& dustTexture, Model* reFireModel, uint32_t& reFireTexture) {

	worldTransform_.Initialize();
	worldTransform_.translation_ = {};

	dustModel_ = dustModel;
	dustTextureHandle_ = dustTexture;

	reFireModel_ = reFireModel;
	reFireTextureHandle_ = reFireTexture;

	assert(dustModel_);
	assert(reFireModel_);

	flame = 0;
}

void tEmitter::Update() {
	//火花の更新
	for (std::list<std::unique_ptr<Dust>>::iterator dustIt = dusts_.begin(); dustIt != dusts_.end();) {
		Dust* dust = dustIt->get();

		if (dust->GetIsDead()) {
			dustIt = dusts_.erase(dustIt);
		}
		else {
			dust->Update();
			dustIt++;
		}
		
	}

	//残り火の更新
	for (std::list<std::unique_ptr<ReFire>>::iterator reFireIt = reFires_.begin(); reFireIt != reFires_.end();) {
		ReFire* reFire = reFireIt->get();

		if (reFire->GetIsDead()) {
			reFireIt = reFires_.erase(reFireIt);
		}
		else {
			reFire->Update();
			reFireIt++;
		}
	}

	worldTransform_.UpdateMatrix();
	worldTransform_.TransferMatrix();
}



void tEmitter::Draw(const ViewProjection& view) {
	//火花を描画
	for (std::list<std::unique_ptr<Dust>>::iterator dustIt = dusts_.begin(); dustIt != dusts_.end(); dustIt++) {
		Dust* dust = dustIt->get();
		if (dust->GetIsDelay()) {
			dustModel_->Draw(dust->GetWT(), view, dustTextureHandle_, kBlendModeNormal, dust->GetColor());
		}
	}

	//残り火を描画
	for (std::list<std::unique_ptr<ReFire>>::iterator reFireIt = reFires_.begin(); reFireIt != reFires_.end(); reFireIt++) {
		ReFire* reFire = reFireIt->get();
		reFireModel_->Draw(reFire->GetWT(), view, reFireTextureHandle_,kBlendModeNormal,reFire->GetColor());
	}
}

void tEmitter::OnCollision() {
	for (int i = 0; i < MAXDUST; i++) {
		SpawnDusts();
	}
	for (int i = 0; i < MAXReFire; i++) {
		SpawnReFire();
	}
}

void tEmitter::SpawnDusts() {
	std::unique_ptr<Dust> newDust = std::make_unique<Dust>();
	newDust->Initialize(worldTransform_.translation_);
	dusts_.push_back(std::move(newDust));
}

void tEmitter::SpawnReFire() {
	std::unique_ptr<ReFire> newReFire = std::make_unique<ReFire>();
	newReFire->Initialize(worldTransform_.translation_);
	reFires_.push_back(std::move(newReFire));
}