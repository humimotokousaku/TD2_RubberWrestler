#pragma once
#include "../ICharacter.h"
#include "../../base/Model.h"
#include "../../base/WorldTransform.h"

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

	// パーツの親子関係
	void SetParent(const WorldTransform* parent);

	void SetVelocity(Vector3& velocity) { velocity_ = velocity; }

	Vector3 GetTranslation() { return worldTransform_.translation_; }

	void SetTranslation(Vector3 translation) {
		worldTransform_.translation_ = translation;
		worldTransform_.translation_.x += 3;
	worldTransform_.UpdateMatrix();
	}

	void SetWorldTransform(WorldTransform worldTransform);
private:
	// 浮遊ギミックの媒介変数
	float floatingParameter_;

	Vector3 velocity_;
};