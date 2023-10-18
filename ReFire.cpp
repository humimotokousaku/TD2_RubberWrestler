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
	mass_ = GetRandom(0.0001f, 0.001f);

	//重力に質量を加えて初期化
	gravity_ = gravity_ * mass_;

	//デスフラグ
	isDead_ = false;

	//スケール
	//worldTransform_.scale_ = { 1.0f,1.0f,1.0f };
	worldTransform_.scale_ = { 0.1f,0.1f,GetRandom(0.5f,1.0f) };

	//消滅までの時間
	deathtimer_ = 120;

	//色の初期化(Vector4)
	color_ = { 1,1,1,1 };

	//透明度
	alpha_ = 1;

	//フレーム数
	flame_ = 0;

	//t
	t_ = 0;
}

void ReFire::Update() {
	flame_++;
	//生きている間
	if (!isDead_) {
		//だんだん透明になる
		/*if (t_ <= 1) {
			t_ += 0.01f;
		}

		float easedT = easeInOutCubic(t_);

		point.end = (1.0f - easedT) * 300 + easedT * point.start;*/

		if (flame_ >= colorDelay_) {
			alpha_ -= GetRandom(0.02f,0.04f);
		}
		
		
		color_ = { 1,1,1,alpha_ };

		if (std::abs(velocity_.x) >= std::abs(acceleration_.x)) {
			//減速
			velocity_ = Add(velocity_, acceleration_);
		}

		//重力加算
		//velocity_.y = velocity_.y - gravity_;

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