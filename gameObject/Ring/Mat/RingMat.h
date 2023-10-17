#pragma once
#include "../../../base/WorldTransform.h"
#include "../../../base/Model.h"
#include <memory>

class RingMat
{
public:
	///
	/// Default Method
	/// 
	RingMat() = default;
	~RingMat() = default;

	// 初期化
	void Initialize(Model* model);

	// 更新処理
	void Update();

	// 描画
	void Draw(const ViewProjection& viewProjection, uint32_t textureHandle);

	///
	/// User Method
	/// 
	

private:
	Model* model_;
	WorldTransform worldTransform_;
};