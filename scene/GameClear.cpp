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
	gameClear_->Initialize(false);
}

void GameClear::Update() {
	if (input_->TriggerKey(DIK_RETURN)) {
		sceneNum = TITLE_SCENE;
	}

	worldTransform_.UpdateMatrix();
}

void GameClear::Draw() {
	// クリア文字
	gameClear_->Draw(worldTransform_,GAMECLEAR, kBlendModeNone, { 1,1,1,1 });
}

void GameClear::Finalize() {
	delete gameClear_;
	worldTransform_.constBuff_.ReleaseAndGetAddressOf();
	xAudio2_.Reset();
	audio_->SoundUnload(&soundData1_);
}