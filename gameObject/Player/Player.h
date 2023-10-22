#pragma once
#include "../../base/Model.h"
#include "../../base/WorldTransform.h"
#include "../../components/Input.h"
#include "../../utility/GlobalVariables.h"
#include "../ICharacter.h"

#include <optional>

struct AnimationFrame {
	int frame = 0;
	int endFrame;
};

class Enemy;

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

	///
	/// User Method
	/// 

	// 入力処理
	void ProcessUserInput();

	// つかみ行動の初期化
	void BehaviorGrabInitialize();
	// つかみの挙動
	void BehaviorGrabUpdate();

	// つかんでいるときの行動初期化
	void BehaviorGrabingInitialize();
	// つかんでいるときの挙動
	void BehaviorGrabingUpdate();

	// 投げるときの行動初期化
	void BehaviorThrowInitialize();
	// 投げるときの挙動
	void BehaviorThrowUpdate();

	// ラリアットの行動初期化
	void BehaviorLariatInitialize();
	// ラリアットの挙動
	void BehaviorLariatUpdate();

	// 自機のワールド座標
	Vector3 GetWorldPosition();

	const WorldTransform& GetWorldTransformBody() { return worldTransformBody_; }
	const WorldTransform& GetWorldTransformBase() { return worldTransform_; }

	void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
	}

	// パーツの親子関係
	void SetParent(const WorldTransform* parent);

	// 調整項目
	void ApplyGlobalVariables();

	//プレイヤーの向いている方向に敵を飛ばす
	void ThrowEnemy();

	//キーボードで回転させる
	void Rotate();

	//投げる速度を取得する関数
	Vector3 GetThrowVelocity() { return throwVelocity_; }

	//void SetEnemyPearent(const WorldTransform* worldTransform) { enemy_->SetWorldTransform(*worldTransform); }

	void SetEnemy(Enemy* enemy) { enemy_ = enemy; }
private:
	// 入力
	Input* input_;

	// カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;
	// ワールド変換データ
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformHead_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;

	WorldTransform worldTransformBuffer_;

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
	AnimationFrame grab_;
	// 投げた時のフレーム
	AnimationFrame throw_;
	// 投げた後の待ち時間
	AnimationFrame wait_;
	// ラリアットした時のフレーム
	AnimationFrame lariat_;

	// 身体の向きを保存
	WorldTransform tempBodyWorldTransform_;

	// 目標角度
	float goalAngle_;

	GlobalVariables* globalVariables_;
	// ファイルの名前
	const char* groupName = "Player";

	//投げ飛ばす速度
	Vector3 throwVelocity_{};

	Enemy* enemy_;
};
