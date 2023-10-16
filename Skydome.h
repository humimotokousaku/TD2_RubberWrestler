#pragma once
#include "base/WorldTransform.h"
#include "base/Model.h"

class Skydome
{
public:
	/// 
	/// Default Method
	/// 
	Skydome() = default;
	~Skydome() = default;

	// 初期化
	void Initialize(Model* model);

	// 更新処理
	void Update();

	// 描画
	void Draw(const ViewProjection& viewProjection, uint32_t textureHandle);

	// 解放処理
	//void Finalize();

private:
	Model* model_;
	WorldTransform worldTransform_;
};

