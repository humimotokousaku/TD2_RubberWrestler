#pragma once
#include "IScene.h"
// object
#include "../object/Sprite.h"
// components
#include "../components/Input.h"
#include "../components/Audio.h"

class GameManager;
class GameClear : public IScene
{
public:
	// 初期化
	void Initialize()override;

	// 更新処理
	void Update()override;

	// 描画
	void Draw()override;

	void Finalize()override;
private:
	Input* input_;
	Audio* audio_;
	Microsoft::WRL::ComPtr<IXAudio2> xAudio2_;
	IXAudio2MasteringVoice* masterVoice_;
	SoundData soundData1_;

	WorldTransform worldTransform_;
	// UIの数
	const static int kMaxUI = 2;
	WorldTransform UI_worldTransform_[kMaxUI];

	Sprite* gameClear_;
	// GamePadのAボタン
	std::unique_ptr<Sprite> guidePad_A_;

	// GamePadのAボタン
	std::unique_ptr<Sprite> guideText_Press_;
};

