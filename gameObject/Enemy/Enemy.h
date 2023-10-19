#pragma once
#include "../ICharacter.h"
#include "../../base/Model.h"
#include "../../base/WorldTransform.h"
#include "../../components/Input.h"

class Enemy : public ICharacter {
public:
	/// <summary>
	/// 初期化
	/// <summary>
	void Initialize(const std::vector<Model*>& models) override;

	/// <summary>
	/// 更新
	/// <summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// <summary>
	void Draw(const ViewProjection& viewProjection, uint32_t textureHandle) override;

	void Finalize();

	// 浮遊ギミック初期化
	void InitializeFloatingGimmick();

	// 浮遊ギミック更新
	void UpdateFloatingGimmick();

public: 

	// 自機のワールド座標
	Vector3 GetWorldPosition();
	inline Vector3 GetPrePosition() { return prePos_; }
	inline Vector3 GetEnemySpeed() { return velocity_; }
	inline void SetEnemySpeed(Vector3 velocity) { velocity_ = velocity; }
	const WorldTransform& GetWorldTransform() { return worldTransform_; }
	const WorldTransform& GetWorldTransformBody() { return worldTransformBody_; }
	const WorldTransform& GetWorldTransformBase() { return worldTransform_; }

	// パーツの親子関係
	void SetParent(const WorldTransform* parent);

private:
	
	Input* input_;

	// 浮遊ギミックの媒介変数
	float floatingParameter_;

	//移動スピード
	Vector3 velocity_;

	//ロープに当たる前の位置
	Vector3 prePos_;
	
	//WorldTransform worldTransformBase_;
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformHead_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;
};