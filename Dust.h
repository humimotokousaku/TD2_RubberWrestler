#pragma once
#include "base/Model.h"
#include "base/ViewProjection.h"
#include "base/WorldTransform.h"
#include "math/Matrix4x4.h"
#include "math/vector3.h"

class Dust
{
public:
	//初期化
	void Initialize(const Vector3& position);

	//更新
	void Update();

	//生存フラグ
	bool GetIsDead() { return isDead_; }

	//発生処理
	//void Spawn(std::list<std::unique_ptr<Dust>> dusts,Vector3& translation);

	WorldTransform GetWT() { return worldTransform_; }

	Vector4 GetColor() { return color_; }
private:
	WorldTransform worldTransform_;

	Model* model_;
	uint32_t texturehandle_;

	//速度の値
	float velocityValue_ = 5;

	//加速度の値
	Vector3 accelerationValue_;

	//速度
	Vector3 velocity_;

	//加速度
	Vector3 acceleration_;

	//質量
	float mass_;

	//重力
	float gravity_ = 1.0f;

	//生存時間
	static const int32_t kLifeTime = 0;

	// デスタイマー
	int32_t deathtimer_ = 0;

	// デスフラグ
	bool isDead_ = false;

	//角度
	float velocityXZ_ = 0;

	//サイズ
	float size_ = 0.4f;

	//色
	Vector4 color_ = {};
};


