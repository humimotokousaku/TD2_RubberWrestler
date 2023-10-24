#include "GameClear.h"
#include "../Manager/GameManager.h"

void GameClear::Initialize() {
	// シーンの切り替え
	sceneNum = GAMECLEAR_SCENE;
	
	// 基本機能
	input_ = Input::GetInstance();
	input_->Initialize();
	HRESULT result;
	// Xaudio2エンジンのインスタンスを生成
	result = XAudio2Create(&xAudio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);
	// マスターボイスを生成
	result = xAudio2_->CreateMasteringVoice(&masterVoice_);
	// 音声読み込み
	soundData1_ = audio_->SoundLoadWave("resources/fanfare.wav");
	// 音声再生
	audio_->SoundPlayWave(xAudio2_.Get(), soundData1_);

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	// クリアの文字
	gameClear_ = new Sprite();
	gameClear_->Initialize(Vector3(0, 0, 0), Vector3(384 * 2, 128 * 2, 0), false);

	worldTransform_.translation_ = { 256,100,0 };

	guidePad_A_ = std::make_unique<Sprite>();
	guidePad_A_->Initialize(Vector3(0, 0, 0), Vector3(64, 64, 0), false);

	UI_worldTransform_.Initialize();
	UI_worldTransform_.translation_ = { 640,500,0 };
}

void GameClear::Update() {
	// シーンの切り替え
	if (input_->GamePadTrigger(XINPUT_GAMEPAD_A) || input_->TriggerKey(DIK_SPACE)) {
		SceneTransition::sceneChangeType_ = FADE_IN;
	}

	if (SceneTransition::GetInstance()->GetSceneChangeSignal()) {
		sceneNum = TITLE_SCENE;
	}

	worldTransform_.UpdateMatrix();
}

void GameClear::Draw() {
	// UI
	guidePad_A_->Draw(UI_worldTransform_, GUIDE_PAD_A, kBlendModeNone);
	// クリア文字
	gameClear_->Draw(worldTransform_,GAMECLEAR, kBlendModeNone);
}

void GameClear::Finalize() {
	delete gameClear_;
	worldTransform_.constBuff_.ReleaseAndGetAddressOf();
	xAudio2_.Reset();
	audio_->SoundUnload(&soundData1_);
}