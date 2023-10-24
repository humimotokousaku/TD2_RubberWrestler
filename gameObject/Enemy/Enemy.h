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
	inline int GetPairingTimeCounter() { return pairingTimeCounter_; }
	inline void SetPairingTimeCounter(int frame) { pairingTimeCounter_ = frame; }
	inline int GetReboundCount() { return reboundCount_; }
	inline void SetReboundCount(int reboundCount) { reboundCount_ = reboundCount; }
	inline Vector3 GetPrePosition() { return prePos_; }
	inline void SetPrePosition(Vector3 pos) { prePos_ = pos; }
	inline Vector3 GetParentPosition() { return parentPos_; }
	inline void SetParentPosition(Vector3 pos) { parentPos_ = pos; }
	inline Vector3 GetSpeed() { return velocity_; }
	inline void SetSpeed(Vector3 velocity) { velocity_ = velocity; }
	inline bool IsParent() { return worldTransform_.parent_; }
	const WorldTransform& GetWorldTransform() { return worldTransform_; }
	const WorldTransform& GetWorldTransformBody() { return worldTransformBody_; }
	void SetEnemyPos(Vector3 pos) {
		worldTransform_.translation_ = pos;
		worldTransform_.UpdateMatrix();
	}
	const WorldTransform& GetWorldTransformBase() { return worldTransform_; }

	// パーツの親子関係
	void SetParent(const WorldTransform* parent);
	void SetParentRope(const WorldTransform* parent) { worldTransform_.parent_ = parent; }
	void UpDateMatrix();
private:
	
	Input* input_;

	// 浮遊ギミックの媒介変数
	float floatingParameter_;

	//移動スピード
	Vector3 velocity_;

	//ロープに当たる前の位置
	Vector3 prePos_;
	Vector3 parentPos_;
	int pairingTimeCounter_;
	int reboundCount_;

	//WorldTransform worldTransformBase_;
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformHead_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;
};