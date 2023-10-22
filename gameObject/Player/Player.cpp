#include "Player.h"
#include "../../Manager/ImGuiManager.h"
#include "../../math/Matrix4x4.h"
#include "../../components/Input.h"
#include <cassert>
#define _USE_MATH_DEFINES
#include "math.h"

#include "../Enemy/Enemy.h"

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
	SetParent(&GetWorldTransformBody());
	worldTransformBody_.parent_ = worldTransform_.parent_;

	// 腕の座標指定
	worldTransformL_arm_.translation_.x = 1.5f;
	worldTransformR_arm_.translation_.x = -1.5f;
	worldTransformL_arm_.translation_.y = 5.0f;
	worldTransformR_arm_.translation_.y = 5.0f;

	grab_.endFrame = 25;
	throw_.endFrame = 20;
	lariat_.endFrame = 60;

	// グループを追加
	globalVariables_ = GlobalVariables::GetInstance();
	GlobalVariables::GetInstance()->CreateGroup(groupName);
	globalVariables_->AddItem(groupName, "Grab_endFrame", grab_.endFrame);
	globalVariables_->AddItem(groupName, "Throw_endFrame", throw_.endFrame);
	globalVariables_->AddItem(groupName, "Lariat_endFrame", lariat_.endFrame);
}

// Updateの関数定義
void Player::Update() {
	// 基底クラスの更新処理
	ICharacter::Update();

	XINPUT_STATE joyState{};
	// ゲームパッド状態取得(接続されていないときは何もせずに抜ける)
	Input::GetInstance()->GetJoystickState(0, joyState);

	// 初期化
	if (behaviorRequest_) {
		//  振るまいを変更
		behavior_ = behaviorRequest_.value();
		switch (behavior_) {
			// 何もつかんでいないとき
		case Behavior::NONE:
			worldTransformL_arm_.rotation_ = { 0,0,0 };
			worldTransformR_arm_.rotation_ = { 0,0,0 };
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
			worldTransformBody_.rotation_ = tempBodyWorldTransform_.rotation_;
			wait_.frame = 0;
			wait_.endFrame = 120;
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
		if (wait_.frame > wait_.endFrame) {
			behaviorRequest_ = Behavior::LARIAT;
		}

		wait_.frame++;

		// playerの今の状態とinfo
		ImGui::Text("Behavior:WAITING\nWaitFrame:%d", wait_.frame);
		break;
		// ラリアット
	case Behavior::LARIAT:
		BehaviorLariatUpdate();
		break;
	}

	ImGui::DragFloat3("L_arm.rotate", &worldTransformL_arm_.rotation_.x, 0.01f, -6.28f, 6.28f);
	ImGui::DragFloat3("R_arm.rotate", &worldTransformR_arm_.rotation_.x, 0.01f, -6.28f, 6.28f);
	ImGui::DragFloat3("Body.rotate", &worldTransformBody_.rotation_.x, 0.01f, -6.28f, 6.28f);
	ImGui::Text("Buffer_Body.rotate%f  %f  %f", tempBodyWorldTransform_.rotation_.x, tempBodyWorldTransform_.rotation_.y, tempBodyWorldTransform_.rotation_.z);
	ImGui::Text("Body.translation%f  %f  %f", worldTransformBody_.translation_.x, worldTransformBody_.translation_.y, worldTransformBody_.translation_.z);
	ImGui::Text("Buffer.translation%f  %f  %f", worldTransformBuffer_.translation_.x, worldTransformBuffer_.translation_.y, worldTransformBuffer_.translation_.z);
	ImGui::Text("worldTransform.translation%f  %f  %f", enemy_->GetTranslation().x, enemy_->GetTranslation().y, enemy_->GetTranslation().z);
	ImGui::End();

	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();

	// 調整項目をまとめた関数
	ApplyGlobalVariables();
	// ボタンを押したらsave
	if (globalVariables_->GetInstance()->GetIsSave()) {
		globalVariables_->SaveFile(groupName);
	}
}

// Drawの関数定義
void Player::Draw(const ViewProjection& viewProjection, uint32_t textureHandle) {
	models_[kModelIndexBody]->Draw(worldTransformBody_, viewProjection, textureHandle, kBlendModeNone, { 1,1,1,1 });
	models_[kModelIndexHead]->Draw(worldTransformHead_, viewProjection, textureHandle, kBlendModeNone, { 1,1,1,1 });
	models_[kModelIndexL_arm]->Draw(worldTransformL_arm_, viewProjection, textureHandle, kBlendModeNone, { 1,1,1,1 });
	models_[kModelIndexR_arm]->Draw(worldTransformR_arm_, viewProjection, textureHandle, kBlendModeNone, { 1,1,1,1 });
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

			worldTransformBody_.translation_ = Add(worldTransformBody_.translation_, move);
			worldTransformBody_.translation_.y = 0;
			worldTransform_.translation_ = worldTransformBody_.translation_;

			// 目標角度の算出
			goalAngle_ = std::atan2(move.x, move.z);
		}
	}
	worldTransform_.translation_ = worldTransformBody_.translation_;
	// 最短角度補間
	worldTransformBody_.rotation_.y = LerpShortAngle(worldTransformBody_.rotation_.y, goalAngle_, 0.1f);
	worldTransform_.rotation_.y = worldTransformBody_.rotation_.y;

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

	Rotate();
}

/// 各ふるまいに応じた挙動と初期化ここから

// つかむ瞬間
void Player::BehaviorGrabInitialize() {
	grab_.frame = 0;
	grab_.endFrame = 25;
	worldTransformL_arm_.rotation_ = { 0.3f, 0.0f, 1.5f };
	worldTransformR_arm_.rotation_ = { 0.3f, 0.0f, -1.5f };
	worldTransformBody_.rotation_.x = -(float)M_PI / 8.0f;

	enemy_->SetParent(&GetWorldTransformBody());
}

void Player::BehaviorGrabUpdate() {

	if (grab_.frame < grab_.endFrame) {
		worldTransformL_arm_.rotation_.x = 0.3f + (-2.2f - 0.3f) * easeInBack((float)grab_.frame / grab_.endFrame);
		worldTransformR_arm_.rotation_.x = 0.3f + (-2.2f - 0.3f) * easeInBack((float)grab_.frame / grab_.endFrame);

		worldTransformL_arm_.rotation_.y = 0.0f + (-1.5f - 0.0f) * easeInBack((float)grab_.frame / grab_.endFrame);
		worldTransformR_arm_.rotation_.y = 0.0f + (1.0f - 0.0f) * easeInBack((float)grab_.frame / grab_.endFrame);

		worldTransformBody_.rotation_.x = -(float)M_PI / 8.0f + (0.8f - (-(float)M_PI / 8.0f)) * easeInBack((float)grab_.frame / grab_.endFrame);
		// 腕の挙動
		//worldTransformL_arm_.rotation_.x += 0.05f;
		//worldTransformR_arm_.rotation_.x += 0.05f;
	}
	else {
			// 振るまいリクエストをリセット
			behaviorRequest_ = Behavior::GRABING;
	}
	//else if (grabFrame_ > 10) {
	//	if (worldTransformL_arm_.rotation_.x >= -(float)M_PI / 2.0f) {
	//		// 腕の挙動
	//		worldTransformL_arm_.rotation_.x -= 0.2f;
	//		worldTransformR_arm_.rotation_.x -= 0.2f;
	//		worldTransformL_arm_.rotation_.y -= 0.1f;
	//		worldTransformR_arm_.rotation_.y += 0.1f;

	//		// 前景姿勢にする
	//		worldTransformBody_.rotation_.x += 0.1f;
	//	}

	//}
	grab_.frame++;

	// playerの今の状態とinfo
	ImGui::Text("Behavior:GRAB\nGrabFrame:%d", grab_.frame);
}

// つかんでいる間
void Player::BehaviorGrabingInitialize() {
	worldTransformL_arm_.rotation_ = { 0.3f, 0.0f, 1.5f };
	worldTransformR_arm_.rotation_ = { 0.0f,0.0f,0.0f };
	worldTransformBody_.rotation_.x = 0.05f;
}
void Player::BehaviorGrabingUpdate() {
	// 移動処理
	ProcessUserInput();
	tempBodyWorldTransform_ = worldTransformBody_;
	// playerの今の状態とinfo
	ImGui::Text("Behavior:GRABING\nPRESS SPACEorR_TRIGGER");
}

// 投げる
void Player::BehaviorThrowInitialize() {
	throw_.frame = 0;
	throw_.endFrame = 20;
	worldTransformBuffer_ = worldTransformBody_;

}
void Player::BehaviorThrowUpdate() {
	if (throw_.frame < throw_.endFrame) {
		// 身体の挙動
		worldTransformL_arm_.rotation_.x = 0.3f + (-2.0f - 0.3f) * easeInBack((float)throw_.frame / throw_.endFrame);
		worldTransformL_arm_.rotation_.y = 0.0f + (-2.5f - 0.0f) * easeInBack((float)throw_.frame / throw_.endFrame);

		worldTransformBody_.rotation_.y = tempBodyWorldTransform_.rotation_.y + (tempBodyWorldTransform_.rotation_.y - (6.28f * 3) - tempBodyWorldTransform_.rotation_.y) * easeInBack((float)throw_.frame / throw_.endFrame);
	
		ThrowEnemy();
	}
	else {
		enemy_->SetParent(nullptr);
		//enemy_->SetWorldTransform(worldTransformBuffer_);
		worldTransform_.UpdateMatrix();
		enemy_->SetTranslation(worldTransformBody_.translation_);
		
		// 振るまいリクエストをリセット
		behaviorRequest_ = Behavior::WAITING;
	}

	throw_.frame++;

	// playerの今の状態とinfo
	ImGui::Text("Behavior:THROW\nThrowFrame:%d", throw_.frame);
}

// ラリアット
void Player::BehaviorLariatInitialize() {
	lariat_.frame = 0;
	lariat_.endFrame = 60;
	worldTransformL_arm_.rotation_ = { 0.0f,0.0f,1.2f };
}

void Player::BehaviorLariatUpdate() {
	if (lariat_.frame > lariat_.endFrame) {
		behaviorRequest_ = Behavior::NONE;
	}

	lariat_.frame++;

	// playerの今の状態とinfo
	ImGui::Text("Behavior:LARIAT\nLariatFrame:%d", lariat_.frame);
}

/// 各ふるまいに応じた挙動と初期化ここまで

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

void Player::ApplyGlobalVariables() {
	grab_.endFrame = globalVariables_->GetIntValue(groupName, "Grab_endFrame");
	throw_.endFrame = globalVariables_->GetIntValue(groupName, "Throw_endFrame");
	lariat_.endFrame = globalVariables_->GetIntValue(groupName, "Lariat_endFrame");
}

#pragma endregion
#pragma endregion

void Player::ThrowEnemy() {
	// 投げ飛ばす速度
	const float kThrowSpeed = 1.0f;
	throwVelocity_ = { 0, 0, kThrowSpeed };

	//速度ベクトルを自機の向きに合わせて回転させる
	throwVelocity_ = TransformNormal(throwVelocity_, worldTransform_.matWorld_);
}

void Player::Rotate() {
	// 回転速さ[ラジアン/frame]
	const float kRotSpeed = 0.3f;

	// 押した方向で移動ベクトルを変更
	if (input_->PressKey(DIK_Q)) {
		worldTransformBody_.rotation_.y += kRotSpeed;
	}
	else if (input_->PressKey(DIK_E)) {
		worldTransformBody_.rotation_.y -= kRotSpeed;
	}
}