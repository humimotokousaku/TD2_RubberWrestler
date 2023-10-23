#include "TitleScene.h"
#include "../Manager/ImGuiManager.h"

void TitleScene::Initialize() {
	// シーンの切り替え
	sceneNum = TITLE_SCENE;

	// 基本機能
	input_ = Input::GetInstance();
	input_->Initialize();

	worldTransform_.Initialize();

	// タイトル文字
	titleName_ = new Sprite();
	titleName_->Initialize(false);
}

void TitleScene::Update() {
	if (input_->TriggerKey(DIK_RETURN)) {
		SceneTransition::sceneChangeType_ = FADE_IN;
	}

	if (SceneTransition::GetInstance()->GetSceneChangeSignal()) {
		sceneNum = GAME_SCENE;
	}

	worldTransform_.UpdateMatrix();
}


//	//// 座標をコピーしてオフセット分ずらす
//	//viewProjection_.translation_ = Add(worldTransform_.translation_, offset);
//XINPUT_STATE joyState{};
//	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
//		//// デッドゾーンの設定
//		SHORT rightThumbX = Input::GetInstance()->ApplyDeadzone(joyState.Gamepad.sThumbRX);
//		const float kRadian = 0.02f;
//		viewProjection_.rotation_.y += (float)rightThumbX / SHRT_MAX * kRadian;
//	}
//	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
//		// デッドゾーンの設定
//		SHORT leftThumbX = Input::GetInstance()->ApplyDeadzone(joyState.Gamepad.sThumbLX);
//		SHORT leftThumbZ = Input::GetInstance()->ApplyDeadzone(joyState.Gamepad.sThumbLY);
//		//const float kRadian = 0.02f;
//		viewProjection_.translation_.x += (float)leftThumbX / SHRT_MAX * 0.5f;
//		viewProjection_.translation_.z += (float)leftThumbZ / SHRT_MAX * 0.5f;
//	}
//
//	// Keyboard
//	if (Input::GetInstance()->PressKey(DIK_LEFT)) {
//		const float speed = -0.1f;
//
//		Vector3 move = { speed,0,0 };
//
//		// 移動ベクトルをカメラの角度だけ回転
//		move = TransformNormal(move, viewProjection_.matView);
//
//		viewProjection_.translation_ = Add(viewProjection_.translation_, move);
//	}
//	if (Input::GetInstance()->PressKey(DIK_RIGHT)) {
//		const float speed = 0.1f;
//
//		Vector3 move = { speed,0,0 };
//
//		// 移動ベクトルをカメラの角度だけ回転
//		move = TransformNormal(move, viewProjection_.matView);
//
//		viewProjection_.translation_ = Add(viewProjection_.translation_, move);
//	}
//	if (Input::GetInstance()->PressKey(DIK_UP)) {
//		const float speed = 0.1f;
//
//		Vector3 move = { 0,0, speed };
//
//		// 移動ベクトルをカメラの角度だけ回転
//		move = TransformNormal(move, viewProjection_.matView);
//
//		viewProjection_.translation_ = Add(viewProjection_.translation_, move);
//	}
//	if (Input::GetInstance()->PressKey(DIK_DOWN)) {
//		const float speed = -0.1f;
//
//		Vector3 move = { 0,0, speed };
//
//		// 移動ベクトルをカメラの角度だけ回転
//		move = TransformNormal(move, viewProjection_.matView);
//
//		viewProjection_.translation_ = Add(viewProjection_.translation_, move);
//	}
//
//	// keyboard
//	if (Input::GetInstance()->PressKey(DIK_W)) {
//		viewProjection_.rotation_ = Add(viewProjection_.rotation_, { -0.01f,0,0 });
//	}
//	if (Input::GetInstance()->PressKey(DIK_A)) {
//		viewProjection_.rotation_ = Add(viewProjection_.rotation_, { 0,-0.01f,0 });
//	}
//	if (Input::GetInstance()->PressKey(DIK_S)) {
//		viewProjection_.rotation_ = Add(viewProjection_.rotation_, { 0.01f,0,0 });
//	}
//	if (Input::GetInstance()->PressKey(DIK_D)) {
//		viewProjection_.rotation_ = Add(viewProjection_.rotation_, { 0,0.01f,0 });
//	}
//
//	viewProjection_.UpdateViewMatrix();
//	viewProjection_.TransferMatrix();
//}

void TitleScene::Draw() {
	titleName_->Draw(worldTransform_, 0, kBlendModeNone,{1,1,1,1});
}

void TitleScene::Finalize() {
	delete titleName_;
	worldTransform_.constBuff_.ReleaseAndGetAddressOf();
}