#include "Player.h"
#include "../../Manager/ImGuiManager.h"
#include "../../math/Matrix4x4.h"
#include "../../components/Input.h"
#include <cassert>
#define _USE_MATH_DEFINES
#include "math.h"

#pragma region Default Method

Player::Player() {}
Player::~Player() {}

// Initializeの関数定義
void Player::Initialize(const std::vector<Model*>& models) {
	// 入力
	input_ = Input::GetInstance();
	input_->Initialize();

	// 基底クラスの初期化
	ICharacter::Initialize(models);

	//worldTransform_.Initialize();
	worldTransformBody_.Initialize();
	worldTransformHead_.Initialize();
	worldTransformL_arm_.Initialize();
	worldTransformR_arm_.Initialize();

	// 身体のパーツの親子関係を結ぶ
	worldTransformBody_.parent_ = &worldTransform_;
	SetParent(&GetWorldTransformBody());

	// 腕の座標指定
	worldTransformL_arm_.translation_.x = 1.5f;
	worldTransformR_arm_.translation_.x = -1.5f;
	worldTransformL_arm_.translation_.y = 5.0f;
	worldTransformR_arm_.translation_.y = 5.0f;

	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();
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
			break;
			// つかむ瞬間
		case Behavior::GRAB:
			BehaviorGrabInitialize();
			break;
			// つかんでいるとき
		case Behavior::GRABING:
			BehaviorGrabingInitialize();
			break;
			// 投げた瞬間
		case Behavior::THROW:
			BehaviorThrowInitialize();
			break;
			// 待機中
		case Behavior::WAITING:
			worldTransformBody_.rotation_.x = 0.0f;
			worldTransformL_arm_.rotation_ = { 0.0f, 0.0f, 0.0f };
			worldTransformR_arm_.rotation_ = { 0.0f, 0.0f, 0.0f };
			waitFrame_ = 120;
			break;
			// ラリアット
		case Behavior::LARIAT:
			BehaviorLariatInitialize();
			break;
		}
		// 振るまいリクエストをリセット
		behaviorRequest_ = std::nullopt;
	}

	ImGui::Begin("Player");

	// 更新処理
	switch (behavior_) {
		// 何もつかんでいないとき
	case Behavior::NONE:
		// 移動処理
		ProcessUserInput();
		// Rトリガーを押したらつかむ
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER || input_->TriggerKey(DIK_SPACE)) {
			behaviorRequest_ = Behavior::GRAB;
		}

		// playerの今の状態とinfo
		ImGui::Text("Behavior:NONE\nPRESS SPACEorR_TRIGGER");
		break;
		// つかむ瞬間
	case Behavior::GRAB:
		BehaviorGrabUpdate();
		break;
		// つかんでいるとき
	case Behavior::GRABING:
		BehaviorGrabingUpdate();
		// Rトリガーを押したら投げる
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER || input_->TriggerKey(DIK_SPACE)) {
			behaviorRequest_ = Behavior::THROW;
		}
		break;
		// 投げた瞬間
	case Behavior::THROW:
		BehaviorThrowUpdate();
		break;
		// 待機中
	case Behavior::WAITING:
		if (waitFrame_ <= 0) {
			behaviorRequest_ = Behavior::LARIAT;
		}

		waitFrame_--;

		// playerの今の状態とinfo
		ImGui::Text("Behavior:WAITING\nWaitFrame:%d", waitFrame_);
		break;
		// ラリアット
	case Behavior::LARIAT:
		BehaviorLariatUpdate();
		break;
	}

	ImGui::End();

	worldTransform_.UpdateMatrix();
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

#pragma endregion

#pragma region User Method

void Player::ProcessUserInput() {
	// ゲームパッド
	XINPUT_STATE joyState;
	// ゲームパッド状態取得
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		// デッドゾーンの設定
		SHORT leftThumbX = Input::GetInstance()->ApplyDeadzone(joyState.Gamepad.sThumbLX);
		SHORT leftThumbZ = Input::GetInstance()->ApplyDeadzone(joyState.Gamepad.sThumbLY);
		// 速さ
		const float speed = 0.3f;
		// しきい値
		const float threshold = 0.7f;
		bool isMoving = false;
		// 移動量
		Vector3 move{
			(float)joyState.Gamepad.sThumbLX / SHRT_MAX, 0.0f,
			(float)joyState.Gamepad.sThumbLY / SHRT_MAX };
		if (Length(move) > threshold) {
			isMoving = true;
		}

		if (isMoving) {
			// 移動量の速さを反映
			move = Multiply(speed, Normalize(move));

			// 回転行列
			Matrix4x4 rotateMatrix = MakeRotateMatrix(viewProjection_->rotation_);
			// 移動ベクトルをカメラの角度だけ回転
			move = TransformNormal(move, rotateMatrix);

			// 移動量
			worldTransform_.translation_.y = 0;
			worldTransform_.translation_ = Add(worldTransform_.translation_, move);
			worldTransformBody_.translation_ = worldTransform_.translation_;

			// 目標角度の算出
			goalAngle_ = std::atan2(move.x, move.z);
		}
	}
	// 最短角度補間
	worldTransform_.rotation_.y = LerpShortAngle(worldTransform_.rotation_.y, goalAngle_, 0.1f);
	worldTransformBody_.rotation_.y = worldTransform_.rotation_.y;

	worldTransform_.UpdateMatrix();

	if (input_->PressKey(DIK_W)) {
		// 速さ
		const float speed = 0.3f;
		// 移動量
		Vector3 move{
			0.0f, 0.0f,
			1.0f
		};

		// 移動量の速さを反映
		move = Multiply(speed, Normalize(move));

		// 移動量
		worldTransform_.translation_ = Add(worldTransform_.translation_, move);
		worldTransformBody_.translation_ = worldTransform_.translation_;
	}
	if (input_->PressKey(DIK_S)) {
		// 速さ
		const float speed = 0.3f;
		// 移動量
		Vector3 move{
			0.0f, 0.0f,
			-1.0f
		};

		// 移動量の速さを反映
		move = Multiply(speed, Normalize(move));

		// 移動量
		worldTransform_.translation_ = Add(worldTransform_.translation_, move);
		worldTransformBody_.translation_ = worldTransform_.translation_;
	}
	if (input_->PressKey(DIK_D)) {
		// 速さ
		const float speed = 0.3f;
		// 移動量
		Vector3 move{
			1.0f, 0.0f,
			0.0f
		};

		// 移動量の速さを反映
		move = Multiply(speed, Normalize(move));

		// 移動量
		worldTransform_.translation_ = Add(worldTransform_.translation_, move);
		worldTransformBody_.translation_ = worldTransform_.translation_;
	}
	if (input_->PressKey(DIK_A)) {
		// 速さ
		const float speed = 0.3f;
		// 移動量
		Vector3 move{
			-1.0f, 0.0f,
			0.0f
		};

		// 移動量の速さを反映
		move = Multiply(speed, Normalize(move));

		// 移動量
		worldTransform_.translation_ = Add(worldTransform_.translation_, move);
		worldTransformBody_.translation_ = worldTransform_.translation_;
	}
}

void Player::BehaviorGrabInitialize() {
	grabFrame_ = 0;
	worldTransformL_arm_.rotation_ = { 0.3f, 0.0f, 1.5f };
	worldTransformR_arm_.rotation_ = { 0.3f, 0.0f, -1.5f };
	worldTransformBody_.rotation_.x = -(float)M_PI / 8.0f;
}

void Player::BehaviorGrabUpdate() {
	if (grabFrame_ < 10) {
		// 腕の挙動
		worldTransformL_arm_.rotation_.x += 0.05f;
		worldTransformR_arm_.rotation_.x += 0.05f;
	}
	else if (grabFrame_ > 10) {
		if (worldTransformL_arm_.rotation_.x >= -(float)M_PI / 2.0f) {
			// 腕の挙動
			worldTransformL_arm_.rotation_.x -= 0.2f;
			worldTransformR_arm_.rotation_.x -= 0.2f;
			worldTransformL_arm_.rotation_.y -= 0.1f;
			worldTransformR_arm_.rotation_.y += 0.1f;

			// 前景姿勢にする
			worldTransformBody_.rotation_.x += 0.1f;
		}
		else {
			// 振るまいリクエストをリセット
			behaviorRequest_ = Behavior::GRABING;
		}
	}
	grabFrame_++;

	// playerの今の状態とinfo
	ImGui::Text("Behavior:GRAB\nGrabFrame:%d", grabFrame_);
}

void Player::BehaviorGrabingInitialize() {
	worldTransformL_arm_.rotation_ = { 0.3f, 0.0f, 1.5f };
	worldTransformR_arm_.rotation_ = { 0.0f,0.0f,0.0f };
	worldTransformBody_.rotation_.x = 0.05f;
}

void Player::BehaviorGrabingUpdate() {
	// 移動処理
	ProcessUserInput();

	// playerの今の状態とinfo
	ImGui::Text("Behavior:GRABING\nPRESS SPACEorR_TRIGGER");
}

void Player::BehaviorThrowInitialize() {
	throwFrame_ = 0;
}

void Player::BehaviorThrowUpdate() {
	if (throwFrame_ < 10) {
		// 身体の挙動
		worldTransformBody_.rotation_.y += 0.1f;
	}
	else if (throwFrame_ < 20) {
		// 腕の挙動
		worldTransformL_arm_.rotation_.y -= 0.2f;

		// 前景姿勢にする
		worldTransformBody_.rotation_.x += 0.15f;
		worldTransformBody_.rotation_.y -= 0.15f;
	}
	else if (worldTransformBody_.rotation_.y <= 0 && worldTransformBody_.rotation_.x >= 0) {
		worldTransformBody_.rotation_.x -= 0.15f;
		worldTransformBody_.rotation_.y += 0.15f;
	}
	else {
		// 振るまいリクエストをリセット
		behaviorRequest_ = Behavior::WAITING;
	}

	throwFrame_++;

	// playerの今の状態とinfo
	ImGui::Text("Behavior:THROW\nThrowFrame:%d", throwFrame_);
}

void Player::BehaviorLariatInitialize() {
	lariatFrame_ = 0;
}

void Player::BehaviorLariatUpdate() {
	if (lariatFrame_ >= 60) {
		behaviorRequest_ = Behavior::NONE;
	}

	lariatFrame_++;

	// playerの今の状態とinfo
	ImGui::Text("Behavior:LARIAT\nLariatFrame:%d", lariatFrame_);
}

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

#pragma endregion