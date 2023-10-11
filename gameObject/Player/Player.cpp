#include "Player.h"
#include "../../Manager/ImGuiManager.h"
#include "../../math/Matrix4x4.h"
#include "../../components/Input.h"
#include <cassert>
#define _USE_MATH_DEFINES
#include "math.h"

Vector3 Player::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Player::SetParent(const WorldTransform* parent) {
	// 親子関係を結ぶ
	worldTransformHead_.parent_ = parent;
	worldTransformL_arm_.parent_ = parent;
	worldTransformR_arm_.parent_ = parent;
}

Player::Player() {}
Player::~Player() {}

// Initializeの関数定義
void Player::Initialize(const std::vector<Model*>& models) {
	// 入力
	input_ = Input::GetInstance();
	input_->Initialize();

	// 基底クラスの初期化
	ICharacter::Initialize(models);

	// 腕の座標指定
	worldTransformL_arm_.translation_.x = 1.5f;
	worldTransformR_arm_.translation_.x = -1.5f;
	worldTransformL_arm_.translation_.y = 5.0f;
	worldTransformR_arm_.translation_.y = 5.0f;

	// 身体のパーツの親子関係を結ぶ
	SetParent(&GetWorldTransformBody());
	worldTransformBody_.parent_ = worldTransform_.parent_;

	//worldTransform_.Initialize();
	worldTransformBody_.Initialize();
	worldTransformHead_.Initialize();
	worldTransformL_arm_.Initialize();
	worldTransformR_arm_.Initialize();
}

// Updateの関数定義
void Player::Update() {
	// 基底クラスの更新処理
	ICharacter::Update();

	XINPUT_STATE joyState;
	// ゲームパッド状態取得
	if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
		return;
	}

	// 初期化
	if (behaviorRequest_) {
		//  振るまいを変更
		behavior_ = behaviorRequest_.value();
		switch (behavior_) {
			// 何もつかんでいないとき
		case Behavior::NONE:
			// Rトリガーを押したら攻撃
			if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
				behaviorRequest_ = Behavior::GRAB;
			}
			break;
			// つかむ瞬間
		case Behavior::GRAB:
			break;
			// つかんでいるとき
		case Behavior::GRABING:
			break;
			// 投げた瞬間
		case Behavior::THROW:
			break;
			// 待機中
		case Behavior::WAITING:
			break;
			// ラリアット
		case Behavior::LARIAT:
			break;
		}
		// 振るまいリクエストをリセット
		behaviorRequest_ = std::nullopt;
	}

	// 更新処理
	switch (behavior_) {
		// 何もつかんでいないとき
	case Behavior::NONE:
		break;
		// つかむ瞬間
	case Behavior::GRAB:
		break;
		// つかんでいるとき
	case Behavior::GRABING:
		break;
		// 投げた瞬間
	case Behavior::THROW:
		break;
		// 待機中
	case Behavior::WAITING:
		break;
		// ラリアット
	case Behavior::LARIAT:
		break;
	}

	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();
}

// Drawの関数定義
void Player::Draw(const ViewProjection& viewProjection, uint32_t textureHandle) {
	models_[kModelIndexBody]->Draw(worldTransformBody_, viewProjection, textureHandle, kBlendModeNone);
	models_[kModelIndexHead]->Draw(worldTransformHead_, viewProjection, textureHandle, kBlendModeNone);
	models_[kModelIndexL_arm]->Draw(worldTransformL_arm_, viewProjection, textureHandle, kBlendModeNone);
	models_[kModelIndexR_arm]->Draw(worldTransformR_arm_, viewProjection, textureHandle, kBlendModeNone);
}

void Player::Finalize() {
	worldTransformBody_.constBuff_.ReleaseAndGetAddressOf();
	worldTransformHead_.constBuff_.ReleaseAndGetAddressOf();
	worldTransformL_arm_.constBuff_.ReleaseAndGetAddressOf();
	worldTransformR_arm_.constBuff_.ReleaseAndGetAddressOf();
}

void Player::ProcessUserInput() {
	XINPUT_STATE joyState;
	// ゲームパッド状態取得
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		// デッドゾーンの設定
		SHORT leftThumbX = Input::GetInstance()->ApplyDeadzone(joyState.Gamepad.sThumbLX);
		SHORT leftThumbZ = Input::GetInstance()->ApplyDeadzone(joyState.Gamepad.sThumbLY);
		// 速さ
		const float speed = 0.3f;
		// 移動量
		Vector3 move{
			(float)leftThumbX / SHRT_MAX, 0.0f,
			(float)leftThumbZ / SHRT_MAX
		};
		// 移動量の速さを反映
		move = Multiply(speed, Normalize(move));

		// 移動量
		worldTransform_.translation_ = Add(worldTransform_.translation_, move);
		worldTransformBody_.translation_ = worldTransform_.translation_;
	}
}
