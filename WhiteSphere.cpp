#include "WhiteSphere.h"

void WhiteSphere::Initialize(const Vector3& position) {
	//WorldTransformの初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	//速度を初期化
	velocity_ = { 1.0f,1.0f,1.0f };

	//デスフラグ
	isDead_ = false;

	//スケール
	worldTransform_.scale_ = { 0.1f,0.1f,1.0f };

	//消滅までの時間
	deathtimer_ = 50;

	//透明度
	alpha_ = 1;

	//色の初期化(Vector4)
	color_ = { 1,1,1,alpha_ };

	//フレーム数
	flame_ = 0;

	//t
	t_ = 0;
}

void WhiteSphere::Update() {
	//生きている間
	if (!isDead_) {
		t_ -= 0.02f;

		alpha_ -= t_;

		color_ = { 1,1,1,alpha_ };

		worldTransform_.UpdateMatrix();
	}
	// 時間経過でデス
	if (--deathtimer_ <= 0) {
		isDead_ = true;
	}
}
