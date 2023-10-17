#include "ReFire.h"
#include <assert.h>

void ReFire::Initialize(const Vector3& position) {
	//WorldTransformの初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	//速度をランダムの値で初期化
	velocity_ = { GetRandomVector3(-velocityValue_,velocityValue_) };

	//加速度をランダムの値で初期化
	acceleration_ = Multiply(velocity_, GetRandom(-0.02f, -0.01f));

	//質量をランダムの値で初期化
	mass_ = GetRandom(0.001f, 0.003f);

	//重力に質量を加えて初期化
	gravity_ = gravity_ * mass_;

	//デスフラグ
	isDead_ = false;

	//スケール
	worldTransform_.scale_ = { 0.1f,0.1f,GetRandom(5.0f,10.0f) };

	//消滅までの時間
	deathtimer_ = GetRandom(20, 40);

	//色の初期化(Vector4)
	color_ = { 0,0,0,1 };
}

void ReFire::Update() {
	//生きている間
	if (!isDead_) {
		//減速
		velocity_ = Add(velocity_, acceleration_);

		//重力加算
		velocity_.y = velocity_.y - gravity_;

		//速度加算
		worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);

		// Y軸周り角度(0y)
		worldTransform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);

		velocityXZ_ = std::sqrt(velocity_.x * velocity_.x + velocity_.z * velocity_.z);
		// X軸周り角度(0x)
		worldTransform_.rotation_.x = std::atan2(-velocity_.y, velocityXZ_);

		worldTransform_.UpdateMatrix();
	}
	// 時間経過でデス
	if (--deathtimer_ <= 0) {
		isDead_ = true;
	}

	if (velocity_.x <= acceleration_.x && velocity_.y <= acceleration_.y && velocity_.z <= acceleration_.z) {
		isDead_ = true;
	}
}

//void Beam::Spawn() {
//	Beam* newBeam = new Beam();
//	newDust->Initialize(model_, tEmitter_->GetWorldPosition(), texturehandle_);
//	gameScene_->AddDust(newDust);
//	//Dust_.push_back(newDust);
//}