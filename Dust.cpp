#include "Dust.h"
#include <assert.h>


void Dust::Initialize(const Vector3& position) {
	//WorldTransformの初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	//速度をランダムの値で初期化
	velocity_ = { GetRandomVector3(-velocityValue_,velocityValue_) };

	//加速度をランダムの値で初期化
	acceleration_ = Multiply(velocity_, -0.02f);

	//質量をランダムの値で初期化
	mass_ = GetRandom(0.005f, 0.01f);

	//重力に質量を加えて初期化
	gravity_ = gravity_ * mass_;

	//デスフラグ
	isDead_ = false;

	//スケール
	worldTransform_.scale_ = { size_,size_,size_ };

	//消滅までの時間
	deathtimer_ = 120;

	//色の初期化(Vector4)
	color_ = { 1,0,0,1 };

	//フレーム数
	flame_ = 0;

	//拡散するフレーム数
	diffusingCount_ = 0;

	//遅延フラグ
	isDelay_ = false;

	//透明度
	alpha_ = 1;

	//透明度調整
	alphaValue_ = GetRandom(0.01f, 0.02f);

	//パーティクルの状態
	particleState_ = GetRandom(0, 2);
}

void Dust::Update() {
	flame_++;
	if (flame_ >= delay_) {
		isDelay_ = true;
	}

	

	//生きている間
	if (!isDead_) {
		alpha_ -= 0.01f;
		color_ = { 1,0,0,alpha_ };
		switch (particleState_)
		{
		case DIFFUSING:   // 拡散する
			diffusingCount_++;
			//重力を加算
			velocity_.y = velocity_.y - gravity_ * 2;

			//速度を加算
			worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);
			break;

		case STATIONARY:  // 留まる
			diffusingCount_++;
			if (diffusingCount_ >= 20) {
				velocity_ = Multiply(velocity_, 0.98f);
			}

			velocity_.y = velocity_.y - gravity_ * -0.02f;

			//速度を加算
			worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);
			break;

		case FALLING:     // 加速度が速度を上回ったら止める
			
			if (std::abs(velocity_.x) >= std::abs(acceleration_.x)) {
				//速度に加速度を加算
				velocity_ = Add(velocity_, acceleration_);
			}

			//重力を加算
			velocity_.y = velocity_.y - gravity_;

			//速度を加算
			worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);
			break;

		default:
			break;
		}
		worldTransform_.UpdateMatrix();
	}
	// 時間経過でデス
	if (--deathtimer_ <= 0) {
		isDead_ = true;
	}
}
