#include "Dust.h"
#include <assert.h>


void Dust::Initialize(const Vector3& position) {
	//WorldTransformの初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	//速度をランダムの値で初期化
	velocity_ = { GetRandomVector3(-velocityValue_,velocityValue_) };

	//加速度をランダムの値で初期化
	acceleration_ = Multiply(velocity_, -0.01f);

	//質量をランダムの値で初期化
	mass_ = GetRandom(0.01f, 0.03f);

	//重力に質量を加えて初期化
	gravity_ = gravity_ * mass_;

	//デスフラグ
	isDead_ = false;

	//スケール
	worldTransform_.scale_ = { size_,size_,size_ };

	//消滅までの時間
	deathtimer_ = GetRandom(40, 80);

	//色の初期化(Vector4)
	color_ = { 0,0,0,1 };
}

void Dust::Update() {
	//生きている間
	if (!isDead_) {
		//速度に加速度を加算
		velocity_ = Add(velocity_, acceleration_);

		//重力を加算
		velocity_.y = velocity_.y - gravity_;

		//速度を加算
		worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);

		worldTransform_.UpdateMatrix();
	}
	// 時間経過でデス
	if (--deathtimer_ <= 0) {
		isDead_ = true;
	}
}
//void Dust::Spawn(Vector3& translation) {
//	std::unique_ptr<Dust> newDust = std::make_unique<Dust>();
//	newDust->Initialize(translation);
//	dusts_.push_back(std::move(newDust));
//}