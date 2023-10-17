#pragma once
#include "base/Model.h"
#include "base/ViewProjection.h"
#include "base/WorldTransform.h"
#include "math/Matrix4x4.h"
#include "math/vector3.h"
#include "Dust.h"
#include "ReFire.h"

#define MAXDUST 200
#define MAXReFire 100

class tEmitter
{
public:
	void Initialize(const Vector3& position, Model* dustModel, uint32_t& dustTexture, Model* reFireModel, uint32_t& reFireTexture);

	void Update();

	void Draw(const ViewProjection& view);

	void OnCollision();

	void SpawnDusts();

	void SpawnReFire();
private:
	WorldTransform worldTransform_;

	//ダストのリスト
	std::list<std::unique_ptr<Dust>> dusts_;

	//残り火のリスト
	std::list <std::unique_ptr<ReFire>> reFires_;

	//ダストのモデル
	Model* dustModel_ = 0;

	//残り火のモデル
	Model* reFireModel_ = 0;

	//ダストのテクスチャハンドル
	uint32_t dustTextureHandle_;

	//残り火のテクスチャハンドル
	uint32_t reFireTextureHandle_ = 0;
};

