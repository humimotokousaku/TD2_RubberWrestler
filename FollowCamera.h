#pragma once
#include "base/ViewProjection.h"
#include "base/WorldTransform.h"

class FollowCamera {
public:
	void Initialize(const Vector3& pos , const Vector3& rotation);

	void Update();

	void SetTarget(const WorldTransform* target) { target_ = target; }

	/// <summary>
	/// ビュープロジェクションを取得
	/// </summary>
	/// <returns>ビュープロジェクション</returns>
	const ViewProjection& GetViewProjection() { return viewProjection_; }
	// ほかのカメラのviewProjectionの代入
	void SetViewProjection(ViewProjection viewProjection) { viewProjection_ = viewProjection; }
	// カメラの向きを指定

	// このカメラを使っているか
	bool GetActive() { return isActive_; }
	// このカメラを使う
	void SetActive(bool isActive) { isActive_ = isActive; }
	// このカメラを使っているか
	bool isActive_;
private:
	ViewProjection viewProjection_;
	const WorldTransform* target_ = nullptr;

	Vector3 offset_;

public:
	static int activeCameraNum_;
};
