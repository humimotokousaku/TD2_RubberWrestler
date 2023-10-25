#pragma once
#include "../ICharacter.h"
#include "../../base/Model.h"
#include "../../base/WorldTransform.h"
#include "../../components/Input.h"
#include "../Player/Player.h"

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

	void SetPlayer(Player* player) { player_ = player; }

	// パーツの親子関係
	void SetParent(const WorldTransform* parent);
	void SetBodyParent(const WorldTransform* parent);

	void SetVelocity(Vector3& velocity) { velocity_ = velocity; }

	const WorldTransform& GetWorldTransformBody() { return worldTransformBody_; }
	const WorldTransform& GetWorldTransformBase() { return worldTransform_; }

	Vector3 GetTranslation() { return worldTransform_.translation_; }

	void SetThrowDir(Matrix4x4 throwDir) {
		throwDir_ = throwDir;
		worldTransform_.rotation_ = player_->GetPlayerBodyRotation();
	}
	Matrix4x4 GetThrowDir() {
		return throwDir_;
	}

	void SetRotation(Vector3 rotation) { worldTransform_.rotation_ = rotation; }

	void SetTranslation(Vector3 translation) {
		worldTransform_.translation_ = translation;
		//worldTransform_.translation_.x = 0;
		worldTransform_.translation_.y = 0;
		worldTransform_.UpdateMatrix();
	}

	void SetWorldTransform(WorldTransform worldTransform);

	Vector3 GetWorldPosition();
public: 

	// 自機のワールド座標
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
	inline Vector3 GetReflectionSpeed() { return reflectionVelocity_; }
	inline void SetReflectionSpeed(Vector3 velocity) { reflectionVelocity_ = velocity; }
	inline bool IsParent() { return worldTransform_.parent_; }
	const WorldTransform& GetWorldTransform() { return worldTransform_; }
	void SetEnemyPos(Vector3 pos) {
		worldTransform_.translation_ = pos;
		worldTransform_.UpdateMatrix();
	}

	// パーツの親子関係
	void SetParentRope(const WorldTransform* parent) { worldTransform_.parent_ = parent; }
	void UpDateMatrix();
private:
	
	Input* input_;

	// 浮遊ギミックの媒介変数
	float floatingParameter_;

	//移動スピード
	Vector3 velocity_;
	Vector3 reflectionVelocity_;

	//ロープに当たる前の位置
	Vector3 prePos_;
	Vector3 parentPos_;
	int pairingTimeCounter_;
	int reboundCount_;

	Player* player_;

	Matrix4x4 throwDir_;

	//WorldTransform worldTransformBase_;
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformHead_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;
};