#pragma once
#include "../../base/Model.h"
#include "../../base/WorldTransform.h"
#include "../../components/Input.h"
#include "../ICharacter.h"

#include <optional>

class Player : public ICharacter{
public:
	Player();
	~Player();
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

	/// <summary>
	/// 解放処理
	/// </summary>
	void Finalize();

	// 自機のワールド座標
	Vector3 GetWorldPosition();

	const WorldTransform& GetWorldTransformBody() { return worldTransformBody_; }
	const WorldTransform& GetWorldTransformBase() { return worldTransform_; }

	void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
	}

	// パーツの親子関係
	void SetParent(const WorldTransform* parent);

	// 入力処理
	void ProcessUserInput();

private:
	// 入力
	Input* input_;

	// カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;
	// ワールド変換データ
	//WorldTransform worldTransformBase_;
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformHead_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;

	// 行動パターン
	enum class Behavior {
		NONE,	 // 何もつかんでいない
		GRAB,	 // つかんだ瞬間
		GRABING, // つかんでいるとき
		THROW,	 // 投げた瞬間
		WAITING, // ラリアットするまでの待ち時間
		LARIAT	 // ラリアット
	};
	Behavior behavior_ = Behavior::NONE;
	// 次の振るまいリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	// つかんだ瞬間のフレーム
	int grabFrame_;
	// 投げた時のフレーム
	int throwFrame_;
	// ラリアットした時のフレーム
	int lariatFrame_;
};
