#pragma once
// base
#include "base/WorldTransform.h"
#include "base/Model.h"
// components
#include "components/Input.h"

#include <optional>

class Player
{
public:
	///
	/// Default Method
	///

	// コンストラクタ
	Player();
	// デストラクタ
	~Player();

	// 初期化
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void Draw(ViewProjection viewProjection);

	// 解放処理
	void Finalize();

private:
	// 入力
	Input* input_;

	// ワールドトランスフォーム
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;
	// テクスチャ
	uint32_t textureHandle_ = 0u;

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

