#include "Player.h"
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
	// 移動処理
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

	if (input_->TriggerKey(DIK_SPACE)) {

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

	// ワールドトランスフォームを更新
	worldTransform_.UpdateMatrix();
}

void Player::Draw(ViewProjection viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void Player::Finalize() {
	worldTransform_.constBuff_.ReleaseAndGetAddressOf();
	delete model_;
}