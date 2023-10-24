#pragma once
#include "../ICharacter.h"
#include "../../base/Model.h"
#include "../../base/WorldTransform.h"
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

	// 浮遊ギミック初期化
	void InitializeFloatingGimmick();

	// 浮遊ギミック更新
	void UpdateFloatingGimmick();

	void SetPlayer(Player* player) { player_ = player; }

	// パーツの親子関係
	void SetParent(const WorldTransform* parent);

	void SetVelocity(Vector3& velocity) { velocity_ = velocity; }

	Vector3 GetTranslation() { return worldTransform_.translation_; }

	void SetThrowDir(Matrix4x4 throwDir) {
		throwDir_ = throwDir;
		worldTransform_.rotation_ = player_->GetPlayerBodyRotation();
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
private:
	// 浮遊ギミックの媒介変数
	float floatingParameter_;

	Vector3 velocity_;

	Player* player_;

	Matrix4x4 throwDir_;
};