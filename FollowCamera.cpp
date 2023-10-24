#include "FollowCamera.h"
#include "Manager/ImGuiManager.h"

void FollowCamera::Initialize(const Vector3& pos, const Vector3& rotation) {
	viewProjection_.Initialize();
	isActive_ = false;

	// カメラの初期位置を代入
	offset_ = pos;
	viewProjection_.rotation_.x = rotation.x;
	viewProjection_.rotation_.y = rotation.y;
}

void FollowCamera::Update() {


	if (target_) {
	// 追従対象からカメラまでのオフセット
	Vector3 offset = offset_;
		// カメラの角度から回転行列を計算
		Matrix4x4 rotateMatrix = MakeRotateMatrix(viewProjection_.rotation_);

		// オフセットをカメラの回転に合わせて回転
		offset = TransformNormal(offset, rotateMatrix);

		// 座標をコピーしてオフセット分ずらす
		viewProjection_.translation_ = Add(target_->translation_, offset);
	}

	viewProjection_.UpdateViewMatrix();
	viewProjection_.TransferMatrix();
}