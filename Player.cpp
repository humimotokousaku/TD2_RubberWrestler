#include "Player.h"
#include "math/Matrix4x4.h"
#include "Manager/ImGuiManager.h"
#include <cassert>

Player::Player() {

}

Player::~Player() {
}

void Player::Initialize() {
	// 入力
	input_ = Input::GetInstance();
	input_->Initialize();

	// 登録したテクスチャの番号を代入
	textureHandle_ = UVCHEKER;
	// モデルの読み込み
	model_ = Model::CreateModelFromObj("resources", "block.obj");
	// モデルの読み込みが失敗なら止める
	assert(model_);

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
}

void Player::Update() {
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
			grabFrame_ = 4;
			break;
			// つかんでいるとき
		case Behavior::GRABING:
			break;
			// 投げた瞬間
		case Behavior::THROW:
			throwFrame_ = 10;
			break;
			// 待機中
		case Behavior::WAITING:
			break;
			// ラリアット
		case Behavior::LARIAT:
			lariatFrame_ = 60;
			break;
		}
		// 振るまいリクエストをリセット
		behaviorRequest_ = std::nullopt;
	}

	ImGui::Begin("Player");
	XINPUT_STATE joyState{};
	// ゲームパッド状態取得
	if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
		return;
	}

	// 更新処理
	switch (behavior_) {
		// 何もつかんでいないとき
	case Behavior::NONE:
		ProcessUserInput();
		// つかみ行動に移る
		if (input_->TriggerKey(DIK_SPACE) || joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
			behaviorRequest_ = Behavior::GRAB;
		}

		ImGui::Text("Behavior:NONE\nPRESS SPACE");
		break;
		// つかむ瞬間
	case Behavior::GRAB:
		if (grabFrame_ <= 0) {
			behaviorRequest_ = Behavior::GRABING;
		}
		grabFrame_--;

		ImGui::Text("Behavior:GRAB\nWaitFrame:%d", grabFrame_);
		break;
		// つかんでいるとき
	case Behavior::GRABING:
		// 投げ行動に移る
		if (input_->TriggerKey(DIK_SPACE) || joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
			behaviorRequest_ = Behavior::THROW;;
		}
		ImGui::Text("Behavior:GRABING\nPRESS SPACE");
		break;
		// 投げた瞬間
	case Behavior::THROW:
		// 投げのアニメーションが終わったら
		if (throwFrame_ <= 0) {
			behaviorRequest_ = Behavior::WAITING;
		}
		throwFrame_--;

		ImGui::Text("Behavior:THROW\nWaitFrame:%d",throwFrame_);
		break;
		// 待機中
	case Behavior::WAITING:
		if (input_->TriggerKey(DIK_SPACE) || joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
			behaviorRequest_ = Behavior::LARIAT;
		}

		ImGui::Text("Behavior:WAITING\nPRESS SPACE");
		break;
		// ラリアット
	case Behavior::LARIAT:
		if (lariatFrame_ <= 0) {
			behaviorRequest_ = Behavior::NONE;
		}
		lariatFrame_--;

		ImGui::Text("Behavior:LARIAT\nWaitFrame:%d", lariatFrame_);
		break;
	}

	// ワールドトランスフォームを更新
	worldTransform_.UpdateMatrix();


	ImGui::Text("BehaviorNum:%d", behaviorRequest_);
	ImGui::End();
}

void Player::Draw(ViewProjection viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_, kBlendModeNone);
}

void Player::Finalize() {
	worldTransform_.constBuff_.ReleaseAndGetAddressOf();
	delete model_;
}

void Player::ProcessUserInput() {
	/// 移動処理

	// キーボード
	if (input_->PressKey(DIK_A)) {
		worldTransform_.translation_.x -= 0.1f;
	}
	if (input_->PressKey(DIK_D)) {
		worldTransform_.translation_.x += 0.1f;
	}
	if (input_->PressKey(DIK_S)) {
		worldTransform_.translation_.z -= 0.1f;
	}
	if (input_->PressKey(DIK_W)) {
		worldTransform_.translation_.z += 0.1f;
	}

	// ゲームパッド
	XINPUT_STATE joyState;
	// ゲームパッド状態取得
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		// デッドゾーンの設定
		SHORT leftThumbX = Input::GetInstance()->ApplyDeadzone(joyState.Gamepad.sThumbLX);
		SHORT leftThumbZ = Input::GetInstance()->ApplyDeadzone(joyState.Gamepad.sThumbLY);
		// 速さ
		const float speed = 0.1f;
		// 移動量
		Vector3 move{
			(float)leftThumbX / SHRT_MAX, 0.0f,
			(float)leftThumbZ / SHRT_MAX
		};
		// 移動量の速さを反映
		move = Multiply(speed, Normalize(move));

		// 移動量
		worldTransform_.translation_ = Add(worldTransform_.translation_, move);
	}
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
	worldTransformHammer_.parent_ = parent;
}