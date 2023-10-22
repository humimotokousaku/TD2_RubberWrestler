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

	bool GetIsDelay() { return isDelay_; }
private:
	WorldTransform worldTransform_;

	Model* model_;
	uint32_t texturehandle_;

	//速度の値
	float velocityValue_ = 1.5f;

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

	//フレーム数
	int flame_ = {};

	//拡散するフレーム数
	int diffusingCount_;

	//遅延
	const int delay_ = 13;

	//遅延フラグ
	bool isDelay_;

	//透明度
	float alpha_ = {};

	//透明度調整
	float alphaValue_ = {};

	//遅延
	const int colorDelay_ = 50;

	enum ParticleState {
		DIFFUSING,   // 拡散する
		STATIONARY,  // 留まる
		FALLING      // そのまま落ちる
	};

	//particleの状態
	int particleState_;
};


